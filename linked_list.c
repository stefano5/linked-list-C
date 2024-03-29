#ifndef _LIST_C
#define _LIST_C

#include <stdio.h>
#include <stdlib.h>


#ifndef _QUEUE_C

#define CHAR        0
#define INT         1
#define FLOAT       2
#define DOUBLE      3
#define STRING      4
#define VOID_GP     5

#define TRUE        1
#define FALSE       0

#endif

#define MODIFY_FUNCTION_LIST                int(*modifyFunction)    (basic_type_list*, void*, char, void*, char)
#define REMOVE_FUNCTION_LIST                int(*removeFunction)    (basic_type_list, void*)
#define GET_ELEMENT_LIST_FUNCTION           void(*genericFunction)  (basic_type_list, void*)
#define GET_POINTER_LIST_FUNCTION           void(*genericFunction)  (basic_type_list*, void*)

#ifndef MEX_EMPTY_LIST
    #define MEX_EMPTY_LIST              "Empty list\n"
#endif

#define CHECK_IF_IS_EMPTY           if (l == NULL) { printf(MEX_EMPTY_LIST); return; }
#define CHECK_IF_IS_EMPTY_int       if (l == NULL) { printf(MEX_EMPTY_LIST); return -1; }

typedef struct {
    void *data;
    int type;
} basic_type_list;

typedef struct nodeList {
    basic_type_list info;
    struct nodeList* next;
} *LinkedList;

void makeNullList(LinkedList *lis) {
    *lis = NULL;
}

int emptyList(LinkedList l) {
    return l == NULL;
}

LinkedList Cons(LinkedList *lis, basic_type_list elem) {
    LinkedList temp;
    temp = (struct nodeList*)malloc(sizeof(struct nodeList));
    if (temp != NULL) {
        temp->info = elem;
        temp->next = *lis;
        *lis = temp;
    }
    return temp;
}

/**
 *  Elimina SOLO il primo elemento della lista SENZA rimuovere eventuali strutture puntate
 *
 * */
int Cdr(LinkedList *lis) {
    LinkedList temp;
    if (emptyList(*lis)) return FALSE;
    temp = *lis;
    *lis = (*lis)->next;
    free(temp);
    return TRUE;
}

/**
 * Cancella elemento in testa dalla lista rimuovendo anche il parametro puntato come 'info', se presente
 *
 * è safety con la struttura 'basic_type_list', testata con valgrind
 * */
int Cdr_pointerData(LinkedList *lis) {
    LinkedList temp;
    if (emptyList(*lis)) return FALSE;
    temp = *lis;
    if (temp->info.data != NULL)
        free(temp->info.data);
    *lis = (*lis)->next;
    free(temp);
    return TRUE;
}

/**
 * Ritorna il primo elemento della lista, se esiste
 *
 * */
basic_type_list* Car(LinkedList l) {
    if (!emptyList(l)) return &(l->info);
    return NULL;
}

int isEmptyList(LinkedList l) {
    if (emptyList(l)) {
        printf(MEX_EMPTY_LIST);
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * Stampa una istanza della struttura 'basic_type_list' rendendo trasparente il tipo 
 * Questo se elem.data è un tipo primitivo, altrimenti stampa solo il puntatore
 *
 * */
void printBasicType(basic_type_list elem) {
    switch (elem.type) {
        case CHAR:
            printf("%c\n", *((char*)elem.data));
            break;
        case INT:
            printf("%d\n", *((int*)elem.data));
            break;
        case FLOAT:
            printf("%.2f\n", *(float*)elem.data);
            break;
        case DOUBLE:
            printf("%f\n", *(double*)elem.data);
            break;
        case STRING:
            printf("%s\n", (char*)elem.data);
            break;
        case VOID_GP:
        default:
            printf("%p\n", elem.data);
            break;
    }
}

/**
 * Supponendo che in lista ci siano puntatori a degli specifici tipi primitivi la funzione sotto li stampa semplimente
 * Se non si ha un tipo primitivo come puntatore si può usare la funzione 'printLinkedList_g'
 *
 * */
void printList_simply(LinkedList l) {
    CHECK_IF_IS_EMPTY; 
    while (!emptyList(l)) {
        printBasicType(*Car(l));
        l = l->next;
    }
}

/**
 *  Elimina completamente una lista ed elimina il dato puntato dal puntatore 'info'
 *
 *  é safety, testata con valgrind
 *
 * */
void deleteList(LinkedList *lis) {
    while (Cdr_pointerData(lis));
}

int equals(basic_type_list ele1, basic_type_list ele2) {
    if (ele1.type == ele2.type) {
        switch (ele1.type) {
            case CHAR:
                return  *((char*)ele1.data) ==  *((char*)ele2.data);
            case INT:
                return  *((int*)ele1.data)  ==  *((int*)ele2.data);
            case FLOAT:
                return  *(float*)ele1.data  ==  *(float*)ele2.data;
            case DOUBLE:
                return  *(double*)ele1.data ==  *(double*)ele2.data;
            case STRING:
                return  !strcmp((char*)ele1.data, (char*)ele2.data);
            case VOID_GP:
            default:
                return  ele1.data == ele2.data;
        }
    } else return FALSE;

}


/** removeGenericElem(&MyLinkedList, &genericParamToDelete, routine);
 *
 * Rimuove un elemento dalla lista dato un certo parametro usato come chiave di ricerca. La ricerca avviene tramite una funzione scritta ad hoc.
 * Tale funzione deve ritornare il valore 'TRUE' se 'elementToRemove' combacia con quello in lista. 
 * Un esempio di tale funzione è:
 *
 *       int isEquals(basic_type_list param, void* identifier) {
 *           int id_toRemove = *(int*)identifier;
 *           int id_toSearch = ((Process*)param.data)->id;          //Cast su un generico tipo 'Process'
 *           return id_toRemove == id_toSearch;
 *       }
 * 
 * Il main chiamerà queste due funzioni così:
 *
 *      removeGenericElem(&process, (void*)&res, isEquals);
 *
 * safety, testata con valgrind
 * */
void removeGenericElem(LinkedList *l, void* elementToRemove, REMOVE_FUNCTION_LIST) {
    CHECK_IF_IS_EMPTY;                         // ^^ spetta alla funzione 'removeFunction' decidere il suo tipo 
    LinkedList paux;
    if (*l != NULL) {
        if (removeFunction((*l)->info, elementToRemove) == TRUE) {
            paux = *l;
            if (paux->info.data != NULL)
                free(paux->info.data);
            *l = (*l)->next;
            free(paux);
        } else removeGenericElem(&((*l)->next), elementToRemove, removeFunction);
    }
}


/** addParam_Data(&MyLinkedList, &modify, routine[search & modify]
 *
 *  modifica un elemento
 *
 * safety, testata con valgrind
 *
 * */
int modifyElement(LinkedList *l, void *paramToSearch, char keyToSerach, void *paramToAdd, char keyToAdd, MODIFY_FUNCTION_LIST) {
    CHECK_IF_IS_EMPTY_int;
    LinkedList paux = *l;
    while (!emptyList(*l)) {
        if (modifyFunction(Car(*l), paramToSearch, keyToSerach, paramToAdd, keyToAdd)) {
            *l = paux;
            return TRUE;
        } 
        *l = (*l)->next;
    }
    *l = paux;
    return FALSE;
}

/*
 *
 * scorri la lista e dai alla funzione 'func' l'elemento in lettura (NO modifica)
 *
 */
void managementLinkedList(LinkedList l, void *param, GET_ELEMENT_LIST_FUNCTION) {
    CHECK_IF_IS_EMPTY; 
    while (!emptyList(l)) {
        genericFunction(*Car(l), param);
        l = l->next;
    }
}

/**
 * Da' alla funzione 'genericFunction' il puntatore all'elemento in testa permettendo la modifica
 *
 * get[POINTER]ElementLinkedList
 *
 * */
void getElementLinkedList(LinkedList *l, void *param, GET_POINTER_LIST_FUNCTION) {
    CHECK_IF_IS_EMPTY; 
    LinkedList paux = *l;
    while (!emptyList(*l)) {
        genericFunction(Car(*l), param);
        *l = (*l)->next;
    }
    *l = paux;
}

#endif
