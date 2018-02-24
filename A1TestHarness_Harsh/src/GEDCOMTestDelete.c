#include "GEDCOMTestUtilities.h"



//******************************** TEST CASES **********************************

//******************************** Delete GEDCOM ********************************

//******************************** Test 1 **************************************

static SubTestRec _tDeleteGEDCOMSubTest1(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test deleteGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
	deleteGEDCOM(obj);
	sprintf(feedback, "Subtest %d.%d: GEDCOM object based on file (%s) deleted with no crashes.",testNum, subTest, fileName);
	result = createSubResult(SUCCESS, feedback);
	return result;
}

static SubTestRec _tDeleteGEDCOMSubTest2(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/minValid.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);

    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test deleteGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
	deleteGEDCOM(obj);
	sprintf(feedback, "Subtest %d.%d: GEDCOM object based on file (%s) deleted with no crashes.",testNum, subTest, fileName);
	result = createSubResult(SUCCESS, feedback);
	return result;
}

static SubTestRec _tDeleteGEDCOMSubTest3(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/nuclearFamily.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test deleteGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    deleteGEDCOM(obj);
	sprintf(feedback, "Subtest %d.%d: GEDCOM object based on file (%s) deleted with no crashes.",testNum, subTest, fileName);
	result = createSubResult(SUCCESS, feedback);
	return result;
}

static SubTestRec _tDeleteGEDCOMSubTest4(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/individualsNoFam.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);

    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test deleteGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
	deleteGEDCOM(obj);
	sprintf(feedback, "Subtest %d.%d: GEDCOM object based on file (%s) deleted with no crashes.",testNum, subTest, fileName);
	result = createSubResult(SUCCESS, feedback);
	return result;
}

static SubTestRec _tDeleteGEDCOMSubTest5(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid2Gen.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);

    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test deleteGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
	deleteGEDCOM(obj);
	sprintf(feedback, "Subtest %d.%d: GEDCOM object based on file (%s) deleted with no crashes.",testNum, subTest, fileName);
	result = createSubResult(SUCCESS, feedback);
	return result;
}

static SubTestRec _tDeleteGEDCOMSubTest6(int testNum, int subTest){
    char feedback[300];
    char fileName[] = "testFiles/valid/shakespeare.ged";
    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test deleteGEDCOM due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    deleteGEDCOM(obj);
    sprintf(feedback, "Subtest %d.%d: GEDCOM object based on file (%s) deleted with no crashes.",testNum, subTest, fileName);
    result = createSubResult(SUCCESS, feedback);
    return result;
}

testRec* _tDeleteGEDCOMTest1(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];

    sprintf(feedback, "Test %d: Deleting GEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);

    runSubTest(testNum, subTest, rec, &_tDeleteGEDCOMSubTest1);
    return rec;
}

testRec* _tDeleteGEDCOMTest2(int testNum){
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: Deleting GEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tDeleteGEDCOMSubTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tDeleteGEDCOMSubTest4);
    return rec;
}

testRec* _tDeleteGEDCOMTest3(int testNum){
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: Deleting GEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tDeleteGEDCOMSubTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tDeleteGEDCOMSubTest5);
    return rec;
}

testRec* _tDeleteGEDCOMTest4(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: Deleting GEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tDeleteGEDCOMSubTest6);
    return rec;
}
