/*
 *
 *      Author: stefano
 *
 *      NB:
 *      All these functions require prior knowledge of the callbacks function. 
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <linked_list.c>

/*
 * In this example we will see how to create a linked list with a default type (int, char, int and char, and so on...)
 * We will use this function:
 * 
 *  -   LinkedList Cons(LinkedList *lis, basicType elem);
 *  -   void printList_simply(LinkedList l);
 *          This function print on stdout default type like int, char...
 *  -   void deleteList(LinkedList *lis);
 *          Clear all allocated memory
 *  -   int modifyElement(LinkedList *l, void *paramToSearch, char keyToSerach, void *paramToAdd, char keyToAdd, MODIFY_FUNCTION);
 *          where MODIFY_FUNCTION is:        int(*modifyFunc)(basicType*, void*, char, void*, char));
 *          this is a function that:
 *              -   return a int (TRUE or FALSE) 
 *              -   receve, one by one, all itmes in the first parameter (*basicType)
 *              -   wants:      a key (it's up to you to choose it unique) related to the type and the parameter to look for in the list.
 *                              a key always related to the type and the new parameter to be inserted 
 *                       Example, you have in list:     int x   end you want replace it with char x you can associate  key = 'A' for int and key = 'B' for char and so discriminate against them.
 *
 * If you want get one by one the item you can use:
 *  -   void getElementLinkedList(LinkedList *l, void *param, void(*func)(basicType*, void*));
 *              -   Note, with this function you have all the pointer then you can modify them.
 *
 *
 * How you can see you have to do 3 functions only. Of course, initializing first with the 'LinkedList' type.
 *
 *  1/2) insert function. You have to:
 *      -   Choose your data type, write it in (infoParam.type)     where infoParam is a basicType variable
 *      -   Allocate the memory for your value  (you can deallocate it with 'deleteList' function)
 *      -   Insert in list your value with 'Cons' function
 *  
 *  3) isEquals function. If you need to compare two items having same type.
 *
 *  4) modifyPar function. This is a call back function then you need to call it inside the parameter of 'modifyElement' function.
 *      This function receve, one by one, all items and can replace it
 *      Have two 'key', in order to can replace one type whit other one. So, you can associate the char 'A' at the type 'int'.
 *
 *  
 *
*/
void insertInt(LinkedList *list);
void insertChar(LinkedList *list);
int isEquals(basicType param, void* id);
int modifyPar(basicType *param, void* paramToSearch, char keyToSearch, void *paramToAdd, char keyToAdd);
void simpleReplace(basicType* list, void* newItem);


int main(int argc, char **argv) {
    //Declaration and Initializing
    LinkedList list;
    makeNullList(&list);

    //Insert of two different types in the list
    for (int i=0; i<5; i++) {
        insertInt(&list);
        insertChar(&list);
    }

    //Print all the items in the list. You don't care say here the datatype of items
    printList_simply(list);
    
    //Remove a specific item from the list
    char charToRemove = 'b';
    printf("\nDeleting of the '%c' character\n", charToRemove);
    removeGenericElem(&list, (void*)&charToRemove, isEquals);
    printList_simply(list);

    
    //Multiply all int for 100      [1]
    int multiplier = 100;
    printf("\nMultiplier all integer for %d\n", multiplier);
    getElementLinkedList(&list, (void*)&multiplier, simpleReplace);
    printList_simply(list);

    //Replace a specific item from the list. Replace 'charToRemove' with 'newElementInt'    [2]
    int newElementInt = 10;
    charToRemove = 'd';
    printf("\nCharacter replacement. %c -> %d. Note the change of the datatype\n", charToRemove, newElementInt);
    modifyElement(&list, (void*)&charToRemove, 'B', (void*)&newElementInt, 'A', modifyPar);
    printList_simply(list);

    //Delete the entire list cleaning the memory
    deleteList(&list);

	exit(EXIT_SUCCESS);
}

/*
 *  The change in this case was a simple increase 
*/
void simpleReplace(basicType* item, void* multiplier) {
    if (item->type == INT) {
        *((int*)item->data) *= *(int*)multiplier;
    }
}

int count=0;
void insertInt(LinkedList *list) {
    //Declaration
    basicType infoParam;

    //Memory allocation
    int *value = (int*)malloc(sizeof(int));

    //Creates random data
    *value = count;
    count++;

    //Choose the datatype now (for other see linked_list.c source file)
    infoParam.type=INT;
    infoParam.data=(void*)value;

    //Insert the data in the list
    Cons(list, infoParam);
}


void insertChar(LinkedList *list) {
    //Declaration
    basicType infoParam;

    //Memory allocation
    char *value = (char*)malloc(sizeof(char));

    //Creates random data
    *value = 'a' + count;
    count++;

    //Choose the datatype now (for other see linked_list.c source file)
    infoParam.type = CHAR;
    infoParam.data = (void*)value;

    //Insert the data in the list
    Cons(list, infoParam);
}

int isEquals(basicType param, void* id) {
    char toRemove = *(char*)id;
    char toSearch = *((char*)param.data);
    return toRemove == toSearch;
}

int modifyPar(basicType *param, void* paramToSearch, char keyToSearch, void *paramToAdd, char keyToAdd) {
    switch (keyToSearch) {
        case 'A':   //This means that 'paramToSearch' is: int
            if (*((int*)param->data) == *(int*)paramToSearch) { //if is equal
                if (param->type == INT) {
                    *((int*)param->data) = *(int*)paramToAdd;
                    return TRUE;
                }
            }
            break;
        case 'B':   //This means that 'paramToSearch' is: char
            if (*((char*)param->data) == *(char*)paramToSearch) {  //if is equal
                if (keyToAdd == 'A') {   //check the 'key' 
                    //paramToAdd is: int
                    param->type = INT;
                    *((int*)param->data) = *(int*)paramToAdd;
                    return TRUE;
                } else if (keyToAdd == 'B') {
                    //paramToAdd is: char
                    param->type = CHAR;
                    *((char*)param->data) = *(char*)paramToAdd;
                    return TRUE;
                }
            }
            break;
    }
    return FALSE;
}

