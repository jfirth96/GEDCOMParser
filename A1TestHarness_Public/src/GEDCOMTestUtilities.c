//
//  GEDCOMTestUtilities.c
//  
//
//  Created by Denis Nikitenko on 2018-02-02.
//

#include <stdio.h>
#include "GEDCOMTestUtilities.h"

//******************************** HELPER FUNCTIONS ********************************

char * _stringConcatenation(char * destination, const char * const source, const char delimiter)
{
    if(source == NULL)
    {
        return destination;
    }

    if(destination == NULL)
    {
        destination = calloc(sizeof(char), (strlen(source) + 1));

        if(destination == NULL)
        {
            return NULL;
        }

        return strcpy(destination, source);
    }

    if(destination[0] == '\0')
    {
        free(destination);

        destination = calloc(sizeof(char), (strlen(source) + 1));

        if(destination == NULL)
        {
            return NULL;
        }

        return strcpy(destination, source);
    }

    size_t destinationLength = strlen(destination) + 1;
    size_t sourceLength = strlen(source) + 1;

    if(delimiter != '\0')
    {
        destination = realloc(destination, destinationLength + sourceLength + 1);

        if(destination == NULL)
        {
            return NULL;
        }

        destination[destinationLength - 1] = delimiter;
        destination[destinationLength] = '\0';

        return strncat(destination, source, sourceLength);
    }
    else
    {
        destination = realloc(destination, destinationLength + sourceLength);

        if(destination == NULL)
        {
            return NULL;
        }

        return strncat(destination, source, sourceLength);
    }
}

//******************************** LIST FUNCTIONS ********************************
void _tDummyDelete(void* toBeDeleted){}
char* _tDummyPrint(void* toBePrinted){ return NULL;}
int _tDummyCompare(const void* first,const void* second){return 0;}

List _tInitializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second)){
    List tmpList;
    
    tmpList.length = 0;
    tmpList.head = NULL;
    tmpList.tail = NULL;
    tmpList.deleteData = deleteFunction;
    tmpList.compare = compareFunction;
    tmpList.printData = printFunction;
    
    return tmpList;
}

Node* _tInitializeNode(void* data){
    Node* tmpNode;
    
    tmpNode = (Node*)malloc(sizeof(Node));
    
    if (tmpNode == NULL){
        return NULL;
    }
    
    tmpNode->data = data;
    tmpNode->previous = NULL;
    tmpNode->next = NULL;
    
    return tmpNode;
}

/**Inserts a Node at the front of a linked list.  List metadata is updated
 * so that head and tail pointers are correct.
 *@pre 'List' type must exist and be used in order to keep track of the linked list.
 *@param list pointer to the _tDummy head of the list
 *@param toBeAdded a pointer to data that is to be added to the linked list
 **/
void _tInsertBack(List* list, void* toBeAdded){
    if (list == NULL || toBeAdded == NULL){
        return;
    }
    
    Node* newNode = _tInitializeNode(toBeAdded);
    list->length += 1;
    if (list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
    }else{
        newNode->previous = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    
}

bool _tContains(List list, const Individual* ind, bool (*isEqual)(const void* test, const void* ref)){
    
    for (Node* ptr = list.head; ptr != NULL; ptr = ptr->next){
        if (isEqual(ind, ptr->data)){
            //           printf("Contains!\n");
            return true;
        }
    }
    return false;
}


bool _tListEqual(List testList, List refList, bool (*isEqual)(const void* test, const void* ref)){
    
    //For every reference object, see if the test list contains it
    //int i = 0;
    for (Node* ptr = refList.head; ptr != NULL; ptr = ptr->next){
        //    printf("i = %d\n", i);
        if (!_tContains(testList, ptr->data, isEqual)){
            return false;
        }
        //      printf("Found %dth element of test list\n", i++);
    }
    //    printf("Found references in test list!\n");
    
    
    //For every test object, see if the reference list contains it
    for (Node* ptr = testList.head; ptr != NULL; ptr = ptr->next){
        if (!_tContains(refList, ptr->data, isEqual)){
            return false;
        }
    }
    
    return true;
}

//******************************** STRUCT CREATION FUNCTIONS ********************************

void _tInitStr(char** str, const char* val){
    *str = malloc(strlen(val)+1);
    strcpy(*str, val);
}

Field* _tCreateField(char* tag, char* value){
    Field* field;
    
    field = malloc(sizeof(Field));
    if (tag != NULL){
        _tInitStr(&field->tag, tag);
    }
    
    if (value != NULL){
        _tInitStr(&field->value, value);
    }
    
    return field;
}

Event* _tCreateEvent(char* type, char* date, char* place){
    Event* evt;
    
    assert(strlen(type) < 5);
    
    evt = malloc(sizeof(Event));
    evt->otherFields = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    
    strcpy(evt->type, type);
    
    if (date != NULL){
        _tInitStr(&evt->date, date);
    }
    
    if (place != NULL){
        _tInitStr(&evt->place, place);
    }
    
    return evt;
}

Individual* _tCreateIndividual(char* givenName, char* surname){
    
    Individual* refInd = malloc(sizeof(Individual));
    
    if(givenName != NULL)
    {
        _tInitStr(&refInd->givenName, givenName);
    }
    else
    {
        refInd->givenName = calloc(sizeof(char), 1);
    }

    if(surname != NULL)
    {
        _tInitStr(&refInd->surname, surname);
    }
    else
    {
        refInd->surname = calloc(sizeof(char), 1);
    }
    
    refInd->events = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    refInd->families = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    refInd->otherFields = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    
    return refInd;
    
}

Family* _tCreateFamily(Individual* husband, Individual* wife){
    
    Family* refFam = malloc(sizeof(Family));
    refFam->wife = wife;
    refFam->husband = husband;
    
    refFam->events = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    refFam->children = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    refFam->otherFields = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);

    return refFam;
    
}


//******************************** COMPARISON FUNCTIONS ********************************

bool _tPtrCmp(const void* p1, const void* p2){
    if (p1 == p2){
        return true;
    }
    
    if (p1 == NULL && p2 != NULL){
        return false;
    }
    
    if (p1 != NULL && p2 == NULL){
        return false;
    }
    
    return true;
}

bool _tSubmEqual(const Submitter* testObj, const Submitter* refObj){
    
    if (testObj == refObj){
        return true;
    }
    
    if (testObj == NULL || refObj == NULL){
        return false;
    }
    
    if (strcmp(testObj->submitterName, refObj->submitterName ) != 0){
        return false;
    }
    
    if (strcmp(refObj->address, "") != 0) {
        if (strcmp(testObj->address, refObj->address ) != 0){
            return false;
        }
    }
    
    return true;
}

bool _tEventEqual(const void* testObj, const void* refObj){
    if (testObj == refObj){
        return true;
    }
    
    if (testObj == NULL || refObj == NULL){
        return false;
    }
    
    Event* testEvt = (Event*)testObj;
    Event* refEvt = (Event*)refObj;
    
    if (strcmp(testEvt->type, refEvt->type ) != 0){
        return false;
    }

//    if (refEvt->date != NULL && testEvt->date != NULL && (strcmp(testEvt->date, refEvt->date ) != 0)){
//        return false;
//    }

    if (refEvt->place != NULL && testEvt->place != NULL && (strcmp(testEvt->place, refEvt->place ) != 0)){
        return false;
    }
    
    //Compare otherFields
//    if (!_tListEqual(testEvt->otherFields, refEvt->otherFields, &_tFieldEqual)){
//        return false;
//    }
    
    return true;
}

bool _tHeadEqual(const Header* testObj, const Header* refObj){
    
    if (testObj == refObj){
        return true;
    }
    
    if (testObj == NULL || refObj == NULL){
        return false;
    }
    
    if (strcmp(testObj->source, refObj->source ) != 0){
        return false;
    }
    
    if (testObj->gedcVersion != refObj->gedcVersion ){
        return false;
    }
    
    if (testObj->encoding != refObj->encoding){
        return false;
    }
    
    if (!_tSubmEqual(testObj->submitter, refObj->submitter)){
        return false;
    }
    
    //Compare otherFields

    
    return true;
}

bool _tFieldEqual(const void* testObj, const void* refObj){
    
    if (testObj == refObj){
        return true;
    }
    
    if (testObj == NULL || refObj == NULL){
        return false;
    }
    
    Field* testField = (Field*)testObj;
    Field* refField = (Field*)refObj;
    
    //  printf("Comparing ");
    //   printf("%s %s to %s %s\n", refField->tag, refField->value, testField->tag, testField->value);
    
    if (refField->tag != NULL && testField->tag != NULL && (strcmp(testField->tag, refField->tag ) != 0)){
        return false;
    }
    
    if (refField->value != NULL && testField->value != NULL && (strcmp(testField->value, refField->value ) != 0)){
        return false;
    }
    // printf("Equal!\n\n");
    return true;
}


bool _tIndEqual(const void* testObj, const void* refObj){
    
    if (testObj == refObj){
        return true;
    }
    
    if (testObj == NULL || refObj == NULL){
        return false;
    }
    
    Individual* testInd = (Individual*)testObj;
    Individual* refInd = (Individual*)refObj;
    
    if (refInd->givenName != NULL && testInd->givenName != NULL && (strcmp(testInd->givenName, refInd->givenName ) != 0)){
        return false;
    }
    
    if (refInd->surname != NULL && testInd->surname != NULL && (strcmp(testInd->surname, refInd->surname ) != 0)){
        return false;
    }
    
    //Compare otherFields
//    if (!_tListEqual(testInd->otherFields, refInd->otherFields, &_tFieldEqual)){
//        return false;
//    }
    
    //Compare Events
    if (!_tListEqual(testInd->events, refInd->events, &_tEventEqual)){
        return false;
    }
    
    //Compare Families - shallow!
    if (!_tListEqual(testInd->families, refInd->families, &_tFamEqualShallow)){
        return false;
    }
    
    return true;
}

bool _tIndEqualShallow(const void* testObj, const void* refObj){
    
    if (testObj == refObj){
        return true;
    }
    
    if (testObj == NULL || refObj == NULL){
        return false;
    }
    
    Individual* testInd = (Individual*)testObj;
    Individual* refInd = (Individual*)refObj;
    
    if (refInd->givenName != NULL && testInd->givenName != NULL && (strcmp(testInd->givenName, refInd->givenName ) != 0)){
        return false;
    }
    
    if (refInd->surname != NULL && testInd->surname != NULL && (strcmp(testInd->surname, refInd->surname ) != 0)){
        return false;
    }
    
    //Compare otherFields
//    if (!_tListEqual(testInd->otherFields, refInd->otherFields, &_tFieldEqual)){
//        return false;
//    }
    
    //Compare Events
    if (!_tListEqual(testInd->events, refInd->events, &_tEventEqual)){
        return false;
    }
    
    return true;
}

bool _tFamEqual(const void* testObj, const void* refObj){
    
    if (testObj == refObj){
        return true;
    }
    
    if (testObj == NULL || refObj == NULL){
        return false;
    }
    
    Family* testFam = (Family*)testObj;
    Family* refFam = (Family*)refObj;
    
    if (!_tPtrCmp(testFam->husband, refFam->husband) && (!_tIndEqualShallow(testFam->husband, refFam->husband))){
        return false;
    }
   
    if (!_tPtrCmp(testFam->wife, refFam->wife) && (!_tIndEqualShallow(testFam->wife, refFam->wife))){
        return false;
    }
    
    //Compare otherFields
//    if (!_tListEqual(testFam->otherFields, refFam->otherFields, &_tFieldEqual)){
//        return false;
//    }
    
    //Compare Events
    if (!_tListEqual(testFam->events, refFam->events, &_tEventEqual)){
        return false;
    }
    
    //Compare children - shallow!
    if (!_tListEqual(testFam->children, refFam->children, &_tIndEqualShallow)){
        return false;
    }
    
    return true;
}

bool _tFamEqualShallow(const void* testObj, const void* refObj){
    
    if (testObj == refObj){
        return true;
    }
    
    if (testObj == NULL || refObj == NULL){
        return false;
    }
    
    Family* testFam = (Family*)testObj;
    Family* refFam = (Family*)refObj;
    
    if (!_tPtrCmp(testFam->husband, refFam->husband) && (!_tIndEqualShallow(testFam->husband, refFam->husband))){
        return false;
    }
    
    if (!_tPtrCmp(testFam->wife, refFam->wife) && (!_tIndEqualShallow(testFam->wife, refFam->wife))){
        return false;
    }
    
    //Compare otherFields
//    if (!_tListEqual(testFam->otherFields, refFam->otherFields, &_tFieldEqual)){
//        return false;
//    }
    
    //Compare Events
    if (!_tListEqual(testFam->events, refFam->events, &_tEventEqual)){
        return false;
    }
    
    return true;
}


bool _tObjEqual(const GEDCOMobject* testObj, const GEDCOMobject* refObj){
    
    if (testObj == refObj){
        return true;
    }
    
    if (testObj == NULL || refObj == NULL){
        return false;
    }
    
    if (!_tSubmEqual(testObj->submitter, refObj->submitter)){
        return false;
    }
    
    if (!_tHeadEqual(testObj->header, refObj->header)){
        return false;
    }
    
    //Compare individuals
    if (!_tListEqual(testObj->individuals, refObj->individuals, &_tIndEqual)){
        return false;
    }

    //Compare families
    if (!_tListEqual(testObj->families, refObj->families, &_tFamEqual)){
        return false;
    }
    
    return true;
}
