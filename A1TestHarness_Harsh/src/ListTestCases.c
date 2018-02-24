#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

#include "LinkedListAPI.h"
#include "TestHarness.h"
//#include "testcases.h"


//data management functions for string data

char * _stringCopy(const char * const str)
{
    if(str == NULL)
    {
        return NULL;
    }

    size_t strSize = strlen(str) + 1;
    char * copyStr = malloc(sizeof(char) * strSize);

    return strncpy(copyStr, str, strSize);
}

void  testDestroy(void *data){
    free(data);
}
char * testPrint(void *toBePrinted){
    if(toBePrinted!=NULL){
        return _stringCopy((char *)toBePrinted);
    }
    return NULL;
}
int testCompare(const void * one, const void * two)
{
    return strcmp((char*)one, (char*)two);
}

char * createData(char * input)
{
    char * dup = malloc(sizeof(char)*(strlen(input))+1);
    strcpy(dup,input);
    return dup;
}

//manual list creation to test insertions separately

List * _emptyList()
{
    List * list = malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->compare = &testCompare;
    list->deleteData = &testDestroy;
    list->printData = &testPrint;
    return list;
}

List * twoList()
{
    List * list = _emptyList();
    Node * n = malloc(sizeof(Node));
    n->data = createData("kilroy");
    n->previous = NULL;
    n->next = NULL;
    list->head = n;
    n=malloc(sizeof(Node));
    n->data = createData("zappa");
    n->next = NULL;
    list->head->next = n;
    n->previous = list->head;
    list->tail = n;
    list->length = 2;
    return list;
}

List * threeList()
{
    List * list = _emptyList();
    Node * n1 = malloc(sizeof(Node));
    n1->data = createData("kilroy");
    
    Node * n2 = malloc(sizeof(Node));
    n2->data = createData("leeroy");
    
    Node * n3 =malloc(sizeof(Node));
    n3->data = createData("zappa");
    
    list->head = n1;
    list->head->next = n2;
    list->head->previous = NULL;
    
    n2->next = n3;
    n2->previous = n1;
    
    list->tail = n3;
    list->tail->previous = n2;
    list->tail->next = NULL;
    
    list->length = 3;
    
    return list;
}


/*---------------------------------
 List creation
 --------------------------------*/

/*--------
 subtest 1: Initial values of List Variables
 ---------*/
SubTestRec initListTest1(int testNum, int subTest){
    List test;
    SubTestRec result;
    char feedback[300];
    test = initializeList(testPrint, testDestroy, testCompare);
    
    // reusing test variable from previous subtest
    if(test.head == NULL){
        if(test.tail == NULL){
            sprintf(feedback, "Subtest %d.%d: head and tail both NULL",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }else{
            sprintf(feedback, "Subtest %d.%d: List tail is not NULL on initialization.",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }else if (test.tail != NULL){
        sprintf(feedback, "Subtest %d.%d: List head and tail are not NULL on initialization.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: List head is not NULL on initialization.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 2: Testing empy list length
 ---------*/


SubTestRec initListTest2(int testNum, int subTest){
    List test;
    SubTestRec result;
    char feedback[300];
    test = initializeList(testPrint, testDestroy, testCompare);
    
    // reusing test variable from previous subtest
    if(test.length == 0){
        sprintf(feedback, "Subtest %d.%d: emply list has length 0",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: emply list does not have length 0 .",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 3: Function pointer initialization
 ---------*/

SubTestRec initListTest3(int testNum, int subTest){
    List test;
    SubTestRec result;
    char feedback[300];
    test = initializeList(testPrint, testDestroy, testCompare);
    
    if(test.deleteData != testDestroy  || test.compare !=testCompare || test.printData !=testPrint){
        sprintf(feedback, "Subtest %d.%d: At least one function pointer is incorrectly assigned.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: Function pointers are correctly assigned.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

testRec* initListTest(int testNum){
    const int numSubs = 2;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: List Initialization Test", testNum);
    testRec* rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &initListTest1);
    subTest++;
//    runSubTest(testNum, subTest, rec, &initListTest2);
//    subTest++;
    runSubTest(testNum, subTest, rec, &initListTest3);
    return rec;
    
}

/******************************************************************/

/*---------------------------------
 Node creation
 --------------------------------*/

/*--------
 subtest 1: initializeNode
 ---------*/
SubTestRec initNodeTest1(int testNum, int subTest){
    char * data = createData("test1");
    Node* n = initializeNode(data);
    char feedback[300];
    SubTestRec result;
    
    if(n == NULL){
        sprintf(feedback, "Subtest %d.%d: initializeNode() returned null.  Cannot continue node creation tests.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: initializeNode() was not null.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

/*--------
 subtest 2: check node data
 ---------*/
SubTestRec initNodeTest2(int testNum, int subTest){
    char * data = createData("test1");
    Node* n = initializeNode(data);
    char feedback[300];
    SubTestRec result;
    
    if(n->data != NULL)
    {
        if(testCompare((char *)(n->data),data)==0)
        {
            sprintf(feedback, "Subtest %d.%d: Node has correct data.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: Node data does not match test data",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Node data is NULL",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* initNodeTest(int testNum){
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: Node Initialization Test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &initNodeTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &initNodeTest2);
    return rec;
}
/******************************************************************/

/************************* getLength() tests *************************/

/*--------
 subtest 1: initial values of List Variables
 ---------*/
SubTestRec getLengthTest1(int testNum, int subTest){
    List* test;
    SubTestRec result;
    char feedback[300];
    test = threeList();
    
    // reusing test variable from previous subtest
    if(getLength(*test) == 3){
        sprintf(feedback, "Subtest %d.%d: getLength returns length corectly.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: getLength does not return length corectly.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* getLengthTest(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: List Initialization Test", testNum);
    testRec* rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &getLengthTest1);
    return rec;
    
}

/********************* Insert tests **********************/

/*--------
 subtest 1:  insert Front empty list
 ---------*/
SubTestRec insertTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = _emptyList();
    
    char* data = createData("sally");
    insertFront(test, data);
    if(test->head->data != NULL &&
       testCompare(test->head->data,data) == 0 &&
       testCompare(test->tail->data,data) == 0 &&
       test->head == test->tail &&
       test->head->previous == NULL &&
       test->head->next == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: correctly inserted front on empty list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly inserted at front of list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 2:  insert Front empty list - length check
 ---------*/
SubTestRec insertTest2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = _emptyList();
    
    char* data = createData("sally");
    insertFront(test, data);
    if(test->length == 1)
    {
        sprintf(feedback, "Subtest %d.%d: list length correctly updated on insert into empty list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: list length incorrectly updated on insert into empty list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 3:  insert Front existing List
 ---------*/
SubTestRec insertTest3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    List* test = twoList();
    Node* oldHead = test->head;
    char* data = createData("sally");
    insertFront(test, data);
    if(test->head->data != NULL && testCompare(test->head->data,data)==0)
    {
        if(testCompare(test->head->next->data,"kilroy")==0 &&
           testCompare(oldHead->previous->data, data) == 0)
        {
            sprintf(feedback, "Subtest %d.%d: correctly inserted front on populated list.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly inserted at front of populated list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 4:  insert Front existing List - length test
 ---------*/
SubTestRec insertTest4(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;

    List* test = twoList();
    char* data = createData("sally");
    insertFront(test, data);
    
    if(test->length == 3)
    {
        sprintf(feedback, "Subtest %d.%d: list length correctly updated on insert into populated list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: list length incorrectly updated on insert into populated list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 5:  insert Back empty list
 ---------*/
SubTestRec insertTest5(int testNum, int subTest){
    char* data;
    char feedback[300];
    SubTestRec result;
    
    List* test = _emptyList();
    data = createData("sally");
    insertBack(test, data);
    if(test->head->data!=NULL && testCompare(test->tail->data,data)==0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly inserted back on empty list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly inserted at back of list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 6:  insert Back existing List
 ---------*/
SubTestRec insertTest6(int testNum, int subTest){
    char* data;
    char feedback[300];
    SubTestRec result;
    
    List* test = twoList();
    Node* oldTail = test->tail;
    data = createData("sally");
    insertBack(test, data);
    if(test->tail->data!=NULL && testCompare(test->tail->data,data) == 0)
    {
        if(testCompare(test->tail->previous->data,"zappa") == 0 &&
           testCompare(oldTail->next->data, data) == 0)
        {
            sprintf(feedback, "Subtest %d.%d: correctly inserted back on populated list.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert back",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly inserted at back of populated list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 7:  insert Back NULL list
 ---------*/
SubTestRec insertTest7(int testNum, int subTest){
    char* data;
    char feedback[300];
    SubTestRec result;
    
    List* test = NULL;
    data = createData("sally");
    insertBack(test, data);
    //if it doesn't segfault
    sprintf(feedback, "Subtest %d.%d: insertBack handled NULL list.",testNum,subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

/*--------
 subtest 8:  insert Front NULL list
 ---------*/
SubTestRec insertTest8(int testNum, int subTest){
    char* data;
    char feedback[300];
    SubTestRec result;
    
    List* test = NULL;
    data = createData("sally");
    insertFront(test, data);
    //if it doesn't segfault
    sprintf(feedback, "Subtest %d.%d: insertFront handled NULL list.",testNum,subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* insertTest(int testNum){
    const int numSubs = 8;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: insertFront() and insertBack() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &insertTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertTest4);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertTest5);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertTest6);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertTest7);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertTest8);
    return rec;
}

/********************** getFront and getBack **********************/

/*--------
 subtest 1:  getFront populated list
 ---------*/
SubTestRec getTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = twoList();
    
    char* retrieved = (char *)getFromFront(*test);
    if(testCompare(retrieved, "kilroy")==0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly retrieved data from populated list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly retrieve from populated list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    return result;
}

/*--------
 subtest 2:  getBack populated list
 ---------*/
SubTestRec getTest2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = twoList();
    
    char* retrieved = (char *)getFromBack(*test);
    if(testCompare(retrieved, "zappa")==0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly retrieved data from populated list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly retrieve from populated list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}
/*--------
 subtest 3:  getFront empty
 ---------*/
SubTestRec getTest3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = _emptyList();
    
    char* retrieved = (char *)getFromFront(*test);
    if(retrieved == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: getFront correctly handled empty list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: getFront did not handle empty list correctly",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
}

/*--------
 subtest 4:  getBack emptylist
 ---------*/
SubTestRec getTest4(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = _emptyList();
    
    char * ret = (char *)getFromBack(*test);
    if(ret == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: getBack correctly handled empty list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: getBack did not handle empty list correctly",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* getTest(int testNum){
    const int numSubs = 4;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getFront() and getBack() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &getTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &getTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &getTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &getTest4);
    return rec;
}
/******************************************************************/

/************************* insert Sorted **************************/

/*--------
 subtest 1:  insert to middle of sorted list.
 ---------*/
SubTestRec insertSTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = twoList();
    
    char * data = createData("ming");
    insertSorted(test, data);
    
    if(testCompare(test->head->next->data, data) == 0)
    {
        if (testCompare(test->tail->previous->data, data) == 0 &&
            test->head->next->previous == test->head && test->tail->previous->next == test->tail){
            sprintf(feedback, "Subtest %d.%d: inserted to middle of sorted list.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }else{
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Did not insert sorted to middle of list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 2:  insert to back of sorted list
 ---------*/
SubTestRec insertSTest2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = twoList();
    
    char* data = createData("zyrg");
    
    Node* oldTail = test->tail;
    
    insertSorted(test, data);
    
    if(testCompare(test->tail->data, data) == 0)
    {
        if (testCompare(oldTail->next->data, data) == 0 &&
            testCompare(test->tail->previous->data, oldTail->data) == 0 &&
            test->tail->next == NULL){
            sprintf(feedback, "Subtest %d.%d: inserted to back of sorted list.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }else{
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
        
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Did not insert sorted to back of list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 3:  insert to front of sorted list
 ---------*/
SubTestRec insertSTest3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = twoList();
    Node* oldHead = test->head;
    char* data = createData("abbott");
    insertSorted(test, data);
    
    if(testCompare((char*)(test->head->data), data) == 0)
    {
        if (testCompare(oldHead->previous->data, data) == 0 &&
            testCompare(test->head->next->data, oldHead->data) == 0 &&
            test->head->previous == NULL){
            //printf("%s %s\n", (char*)data, (char*)oldHead->previous->data);
            sprintf(feedback, "Subtest %d.%d: inserted to front of sorted list.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }else{
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Did not insert sorted to front of list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 4:  insert sorted with empty list
 ---------*/
SubTestRec insertSTest4(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    List* test = _emptyList();
    char* data = createData("abbott");
    insertSorted(test, data);
    
    if(testCompare(test->head->data, data) == 0 &&
       testCompare(test->tail->data, data) == 0 &&
       test->head->previous == NULL && test->tail->next == NULL &&
       test->head == test->tail
       )
    {
        sprintf(feedback, "Subtest %d.%d: inserted single element to sorted list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: insert sorted failed on empty list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 5:  insertSorted NULL list
 ---------*/
SubTestRec insertSTest5(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    List* test = NULL;
    char* data = createData("sally");
    insertSorted(test, data);
    //if it doesn't segfault
    sprintf(feedback, "Subtest %d.%d: insertSorted handled NULL list.",testNum,subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec * insertSortedTest(int testNum)
{
    const int numSubs = 5;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: insertSorted() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &insertSTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertSTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertSTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertSTest4);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertSTest5);
    
    return rec;
}

/******************************************************************/

/**************************** Delete ******************************/

/**************************** Delete ******************************/

/*--------
 subtest 1:  delete existing data test (front)
 ---------*/
SubTestRec deleteTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = threeList();
    
    char * retrieved = (char *)deleteDataFromList(test, "kilroy");
    if(testCompare(retrieved, "kilroy")==0)
    {
        if(testCompare(test->head->data, "leeroy") == 0 &&
           test->head->previous == NULL)
        {
            sprintf(feedback, "Subtest %d.%d: correctly retrieved data from the front of a populated list.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: did not remove element from list",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not returned after being removed from list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 2:  delete existing data test (back)
 ---------*/
SubTestRec deleteTest2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    List* test = threeList();
    char* retrieved = (char *)deleteDataFromList(test, "zappa");
    if(testCompare(retrieved, "zappa")==0)
    {
        if(testCompare(test->tail->data, "leeroy") == 0 &&
           test->tail->next == NULL)
        {
            sprintf(feedback, "Subtest %d.%d: correctly retrieved data from the back of a populated list.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: did not remove element from list",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not returned after being removed from list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}


/*--------
 subtest 3:  delete existing data test (middle)
 ---------*/
SubTestRec deleteTest3(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    List* test = threeList();
    char* retrieved = (char *)deleteDataFromList(test, "leeroy");
    if(testCompare(retrieved, "leeroy")==0)
    {
        if(testCompare(test->tail->data, "zappa") == 0 &&
           testCompare(test->head->data, "kilroy") == 0 &&
           testCompare(test->head->next->data,"zappa") == 0 &&
           testCompare(test->tail->previous->data, "kilroy") == 0)
        {
            sprintf(feedback, "Subtest %d.%d: correctly retrieved data from the middle of a populated list.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: did not remove element from list",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not returned after being removed from list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}


/*--------
 subtest 4:  delete data that doesn't exist
 ---------*/
SubTestRec deleteTest4(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test;
    char* retrieved;
    
    test = twoList();
    retrieved = (char *)deleteDataFromList(test, "beth");
    
    if(retrieved == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: correctly handled invalid data.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed on invalid data",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 5:  delete null data
 ---------*/
SubTestRec deleteTest5(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test;
    //char* retrieved;
    
    test = twoList();
    deleteDataFromList(test, NULL);
    //if did not seg fault
    sprintf(feedback, "Subtest %d.%d: correctly handled NULL parameter.",testNum,subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

/*--------
 subtest 6:  delete populated list
 ---------*/
SubTestRec deleteTest6(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test;
    
    test = twoList();
    clearList(test);
    if(test->head == NULL && test->tail == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: clearList functioned correctly.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: did not correctly clear List",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 7:  delete empty list
 ---------*/
SubTestRec deleteTest7(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test;
    
    test = _emptyList();
    clearList(test);
    if(test->head == NULL && test->tail == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: correctly handled empty list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed on empty list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 8:  delete NULL list
 ---------*/
SubTestRec deleteTest8(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test;
    
    test = NULL;
    clearList(test);
    if(test == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: correctly handled NULL list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed on NULL list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 9:  delete existing data test (front)
 ---------*/
SubTestRec deleteTest9(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = threeList();
    
    deleteDataFromList(test, "kilroy");
    if(test->length == 2)
    {
        sprintf(feedback, "Subtest %d.%d: list length correctly updated on deletion from list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: list length incorrectly updated on deletion from list.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* deleteTest(int testNum)
{
    const int numSubs = 9;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: clearList() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &deleteTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &deleteTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &deleteTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &deleteTest4);
    subTest++;
    runSubTest(testNum, subTest, rec, &deleteTest5);
    subTest++;
    runSubTest(testNum, subTest, rec, &deleteTest6);
    subTest++;
    runSubTest(testNum, subTest, rec, &deleteTest7);
    subTest++;
    runSubTest(testNum, subTest, rec, &deleteTest8);
    subTest++;
    runSubTest(testNum, subTest, rec, &deleteTest9);
    
    return rec;
}

/******************************************************************/

/***************************** Print ******************************/


/*--------
 subtest 1:  print forwards
 ---------*/
SubTestRec printTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char * one = NULL;
    char * two = NULL;
    char * printed;
    List * test = twoList();
    
    printed =  toString(*test);
    one = strstr(printed, "k");
    two = strstr(printed, "z");
    
    if(one != NULL && two != NULL)
    {
        if((int)(one-printed) < (int)(two-printed))
        {
            sprintf(feedback, "Subtest %d.%d: List printed forwards in correct order.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: List did not print in correct order.",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
        }
        
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: List did not print correctly",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        
    }
    free(printed);
    return result;
}

/*--------
 subtest 2:  print empty List
 ---------*/
SubTestRec printTest2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    char* printed;
    
    List* test = _emptyList();
    printed = feedback; //to get a value other than NULL
    printed = toString(*test);
    if(printed == NULL || printed != NULL )
    {
        sprintf(feedback, "Subtest %d.%d: Empty list handled correctly.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: List did not handle empty list correctly",testNum,subTest);
        result = createSubResult(FAIL, feedback);
    }
    return result;
}

testRec * printTest(int testNum)
{
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: print test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &printTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &printTest2);
    
    return rec;
}

/******************************************************************/

/********************* Find tests **********************/

bool searchFunc(const void* el1, const void* el2){
    char* str1 = (char*)el1;
    char* str2 = (char*)el2;
    
    if (strcmp(str1, str2) == 0){
        return true;
    }else{
        return false;
    }
}

/*--------
 subtest 1:  find in empty list
 ---------*/
SubTestRec searchTest1(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "sally";
    SubTestRec result;
    List* test = _emptyList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(data == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: searching empty list correctly returns NULL.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: searching empty list does not return NULL",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 2:  find in list
 ---------*/
SubTestRec searchTest2(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "zappa";
    SubTestRec result;
    List* test = threeList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(strcmp((char*)data, "zappa") == 0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly found an existing element (last) in the list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to find an existing element in the list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 3:  find in list
 ---------*/
SubTestRec searchTest3(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "kilroy";
    SubTestRec result;
    List* test = threeList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(strcmp((char*)data, "kilroy") == 0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly found an existing element (first) in the list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to find an existing element in the list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 4:  find in list
 ---------*/
SubTestRec searchTest4(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "leeroy";
    SubTestRec result;
    List* test = threeList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(strcmp((char*)data, "leeroy") == 0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly found an existing element in the list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to find an existing element in the list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 5:  find in list
 ---------*/
SubTestRec searchTest5(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "booger";
    SubTestRec result;
    List* test = threeList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(data == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: correctly returned NULL after searching for non-existent element in the list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to return NULL after searching for non-existent element in the list.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* searchTest(int testNum){
    const int numSubs = 5;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: findElement() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &searchTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &searchTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &searchTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &searchTest4);
    subTest++;
    runSubTest(testNum, subTest, rec, &searchTest5);
    return rec;
}

