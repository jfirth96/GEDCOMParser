#ifndef GEDCOM_TEST_CASES_H
#define GEDCOM_TEST_CASES_H

#include "TestHarness.h"

testRec* _tValidFileTest1(int testNum);
testRec* _tValidFileTest2(int testNum);
testRec* _tValidFileTest3(int testNum);
testRec* _tValidFileTest4(int testNum);
testRec* _tValidFileTest5(int testNum);
testRec* _tValidFileTest6(int testNum);
testRec* _tValidFileTest7(int testNum);
testRec* _tValidFileTest8(int testNum);
testRec* _tValidFileTest9(int testNum);
testRec* _tValidFileTest10(int testNum);
testRec* _tValidFileTest11(int testNum);

testRec* _tinvalidFileTests1(int testNum);
testRec* _tinvalidFileTests2(int testNum);
testRec* _tinvalidFileTests3(int testNum);
testRec* _tinvalidFileTests4(int testNum);

testRec* _tDeleteGEDCOMTest1(int testNum);
testRec* _tDeleteGEDCOMTest2(int testNum);
testRec* _tDeleteGEDCOMTest3(int testNum);
testRec* _tDeleteGEDCOMTest4(int testNum);

testRec* _tPrintGEDCOMTest(int testNum);
testRec* _tPrintGEDCOMTestNULL(int testNum);

testRec* _tPrintGEDCOMerrorTest(int testNum);

testRec* _tFindPersonTest(int testNum);
testRec* _tFindPersonTestNull(int testNum);

testRec* _tGetDescendantsTest1(int testNum);
testRec* _tGetDescendantsTest2(int testNum);
testRec* _tGetDescendantsTest3(int testNum);
testRec* _tGetDescendantsTest4(int testNum);
testRec* _tGetDescendantsTest5(int testNum);


//A2

//Module 1
testRec* _tWriteTest(int testNum);
testRec* _tValidateTest(int testNum);

//Module 2
testRec* _tGetDescNTest(int testNum);
testRec* _tGetAnsNTest(int testNum);

//Module 3
testRec* _tTestIndToJSON(int testNum);
testRec* _tTestJSONtoInd(int testNum);

//******************************** indToJSON ********************************




#endif
