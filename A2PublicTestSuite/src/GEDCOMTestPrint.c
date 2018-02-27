#include "GEDCOMTestUtilities.h"



//******************************** TEST CASES **********************************

//******************************** Print GEDCOM ********************************

//******************************** Test 1 **************************************

static SubTestRec _tPrintGEDCOMTest1(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test printGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
	char* print = printGEDCOM(obj);


	if(print != NULL && strlen(print) > 4)
    {
        sprintf(feedback, "Subtest %d.%d: Printed GEDCOM based on file (%s).",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: printGEDCOM did not handle non-empty GEDCOMobject correctly",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
}

static SubTestRec _tPrintGEDCOMTest2(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/nuclearFamily.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test printGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    
	char* print = printGEDCOM(obj);


    if(print != NULL && strlen(print) > 4)
    {
        sprintf(feedback, "Subtest %d.%d: Printed GEDCOM based on file (%s).",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: printGEDCOM did not handle non-empty GEDCOMobject correctly",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tPrintGEDCOMTest3(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/individualsNoFam.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test printGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    
	char* print = printGEDCOM(obj);


    if(print != NULL && strlen(print) > 4)
    {
        sprintf(feedback, "Subtest %d.%d: Printed GEDCOM based on file (%s).",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: printGEDCOM did not handle non-empty GEDCOMobject correctly",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tPrintGEDCOMTest4(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid2GenE2.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test printGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    
	char* print = printGEDCOM(obj);


    if(print != NULL && strlen(print) > 4)
    {
        sprintf(feedback, "Subtest %d.%d: Printed GEDCOM based on file (%s).",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: printGEDCOM did not handle non-empty GEDCOMobject correctly",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tPrintGEDCOMTest5(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
   
    printGEDCOM(NULL);
    
    sprintf(feedback, "Subtest %d.%d: handled NULL argument without crashing",testNum,subTest);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* _tPrintGEDCOMTest(int testNum){
    const int numSubs = 4;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];

    sprintf(feedback, "Test %d: Printing GEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);

    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMTest4);
    return rec;
}

testRec* _tPrintGEDCOMTestNULL(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: Printing GEDCOM with NULL argument", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMTest5);
    return rec;
}
