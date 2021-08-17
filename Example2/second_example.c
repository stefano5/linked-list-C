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
#include <string.h>
#include <linked_list.c>


/*
 * Define new type.
 * You can make any compound type.
*/
typedef struct {
    int count;
    char a;
    float b;
    char str[32];
} ComplexType;


//Prototype
void insertComplexType(LinkedList *list);
void printComplexType(basic_type_list itemList, void * param);
void simpleReplace(basic_type_list* list, void* newItem);
int isEquals(basic_type_list item, void* toSearch);
int replaceFunction(basic_type_list *itemToReplace, void* paramToSearch, char keyToSearch, void* newItem, char keyToReplace);

//Main
int main(int argc, char **argv) {
    //Declaration and Initializing
    LinkedList list;
    makeNullList(&list);

    //Insert ComplexType
    for (int i=0; i<5; i++)
        insertComplexType(&list);

    //Just print the pointer because now you have structure then you have to write a new function for your structure
    printf("Print all pointer:\n");
    printList_simply(list);

    //Give you 'printComplexType' at function library 'managementLinkedList'. Now we don't care about the second parameter, it's just an auxiliary parameter for the passed function (here the passed function is printComplexType')
    managementLinkedList(list, NULL, printComplexType);

    //Replace one item only     [1]     complexity: low 
    ComplexType newItem;
    newItem.count  =   999;
    newItem.a      =   'j';   
    newItem.b      =   2.7;
    for (int i=0; i<32; i++) newItem.str[i] = '\0'; //initialize string
    sprintf(newItem.str, "MAIN WAS HERE");
    getElementLinkedList(&list, (void*)&newItem, simpleReplace);

    //Print list
    printf("List after replace: \n");
    managementLinkedList(list, NULL, printComplexType);

    //Remove one item
    int removeThisItem = 2;
    removeGenericElem(&list, (void*)&removeThisItem, isEquals);

    printf("List after remove one item (remove for count = %d): \n", removeThisItem);
    managementLinkedList(list, NULL, printComplexType);

    //Replace one item only     [2]     complexity: medium
    //we would replace the first item with 'newItem' like before
    int replaceThisItem = 0;
    char keyToSearch = 'A';     
    char keyToReplace = 'B';            //we need two differt key because we have to change different type (ComplexType will be int)
    int newValue = 1234;
    modifyElement(&list, (void*)&replaceThisItem, keyToSearch, (void*)&newValue, keyToReplace, replaceFunction);

    printf("List after replace one item (remove for count = %d): \n", replaceThisItem);
    managementLinkedList(list, NULL, printComplexType);

    //Delete all item, cleare the allocated memory
    deleteList(&list);
    exit(EXIT_SUCCESS);
}

/*
 *  This function get the pointer of the item so it can modify that value, TODO
 *
*/
int replaceFunction(basic_type_list *itemToReplace, void* paramToSearch, char keyToSearch, void* newItem, char keyToReplace) {
    ComplexType *elem       =   itemToReplace->data;
    switch (keyToSearch) {
        case 'A': // we search for int
            if (*(int*)paramToSearch == elem->count) {
                if (keyToReplace == 'A') { //modify with same type (like first).    NB: newItem have to be: ComplexType

                    ComplexType *newElem    =   (ComplexType*)newItem;
                    elem->count     =   newElem->count;
                    elem->a         =   newElem->a;
                    elem->b         =   newElem->b;
                    for (int i=0; i<32; i++) elem->str[i] = '\0';
                    strcpy(elem->str, newElem->str);
                } else { //replace with new type    NB: newItem have to be: int

                    free(elem);
                    printf("Replace ");
                    int *value = (int*)malloc(sizeof(int));
                    *value = *(int*)newItem;
                    itemToReplace->data = value;
                    itemToReplace->type = INT;
                    printf("done\n");
                }
                return TRUE;    //NEVER FORGET IT
            }
            break;
        case 'B': // we search for other... (float, char, char* ...)

            break;
    }
    return FALSE;
}

/*
 *  Compare the i_th item with one parameter 
 *
*/
int isEquals(basic_type_list item, void* toSearch) {
    ComplexType *elem       =   (ComplexType*)item.data;
    int conditionToClearItem=   *(int*)toSearch;
    return elem->count == conditionToClearItem;
}


/*
 * Suppose we would replace one item, where [ComplexType].count is 1
 */
void simpleReplace(basic_type_list* item, void* newItem) {
    ComplexType *elem       =   item->data;
    ComplexType *newElem    =   (ComplexType*)newItem;

    if (elem->count == 1) {
        printf("Replace..");
        elem->count     =   newElem->count;
        elem->a         =   newElem->a;
        elem->b         =   newElem->b;
        for (int i=0; i<32; i++) elem->str[i] = '\0';
        strcpy(elem->str, newElem->str);
        printf(" done\n\n");
    }
}


/*
 * If the type is VOID_GP you would print this structure:
 *  typedef struct {
 *      int count;
 *      char a;
 *      float b;
 *      char str[32];
 *   } ComplexType;
 *
 *   Then:
 */ 
void printComplexType(basic_type_list itemList, void *genericParam) {
    if ((int)itemList.type == VOID_GP) {
        ComplexType *elem = (ComplexType*)itemList.data;
        printf("#####################################\n");
        printf("\tvar count = %d\n",    elem->count);
        printf("\tvar   a   = %c\n",    elem->a);
        printf("\tvar   b   = %.2f\n",  elem->b);
        printf("\tvar  str  = %s\n",    elem->str);
        printf("\n");
    } else {
        printf("Default type:\n");
        printBasicType(itemList);
    }
}


int count;
void insertComplexType(LinkedList *list) {
    basic_type_list param;

    //Memory  allocation
    ComplexType *newParam = (ComplexType*)malloc(sizeof(ComplexType));

    //Initialize string
    for (int i=0; i<32; i++) newParam->str[i] = '\0';

    //creates random data
    newParam->count = count;
    newParam->a = 'a' + count;
    newParam->b = 3.14 + count;
    sprintf(newParam->str, "random str is \"%c%c%c\"", 'A' + count, 'Z' - count, 'H' + count);
    count++;

    //choose the datatype now (for other see linked_list.c source file)
    param.type = VOID_GP;    //VOID_G[ENERAL]P[URPOSE]
    param.data = (void*)newParam;


    //enter the data in the list
    Cons(list, param);
}

