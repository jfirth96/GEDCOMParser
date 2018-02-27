#ifndef LIST_TEST_CASES_H
#define LIST_TEST_CASES_H

#include "TestHarness.h"
/*-----------------
test cases
--------------------*/
testRec* initListTest(int testNum);
testRec* initNodeTest(int testNum);
testRec* getLengthTest(int testNum);
testRec* getTest(int testNum);
testRec* insertTest(int testNum);
testRec * insertSortedTest(int testNum);
testRec* deleteTest(int testNum);
testRec* searchTest(int testNum);
testRec * printTest(int testNum);

SubTestRec getSubDataFromChild(int* pipefd);
void sendSubDataToParent(int* pipefd, SubTestRec* tmpRec);
void runSubTest(int testNum, int subTest, testRec* rec, SubTestRec (*testFunc)(int, int));

#endif
