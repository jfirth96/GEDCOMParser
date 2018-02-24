#include <stdbool.h>
#include <strings.h>

#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"

//******************************** TEST CASES **********************************

//******************************** INV_FILE tests ********************************

//******************************** Test 1 **************************************

static SubTestRec _tinvalidFileTest1(int testNum, int subTest){
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(NULL, &obj);
    
    if (response.type != INV_FILE || response.line != -1 || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get correct error response (INV_FILE, -1, NULL) with NULL filename for createGEDCOM.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled NULL filename into createGEDCOM.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest2(int testNum, int subTest){
    char fileName[] = "nonExistingFile.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_FILE || response.line != -1 || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get correct error response (INV_FILE, -1, NULL) with non-existing filename [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled non-existing filename [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest3(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/simpleValid.ge";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_FILE || response.line != -1 || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get correct error response (INV_FILE, -1, NULL) with non-.ged filename [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled non-.ged filename [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

testRec* _tinvalidFileTests1(int testNum){
    const int numSubs = 3;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];

    sprintf(feedback, "Test %d: INV_FILE", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest3);

    return rec;
}

//******************************** TEST CASES **********************************

//******************************** INV_GEDCOM tests ********************************

//******************************** Test 2 **************************************

static SubTestRec _tinvalidFileTest4(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/missingHeader.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_GEDCOM || response.line != -1 || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get correct error response (INV_GEDCOM, -1, NULL) with missing header in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled missing header in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest5(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/missingSubmitter.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_GEDCOM || response.line != -1 || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get correct error response (INV_GEDCOM, -1, NULL) with missing submitter in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled missing submitter in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest6(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/missingTrailer.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_GEDCOM || response.line != -1 || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get INV_GEDCOM with missing trailer in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled missing trailer in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

testRec* _tinvalidFileTests2(int testNum){
    const int numSubs = 3;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];

    sprintf(feedback, "Test %d: INV_GEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest4);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest5);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest6);

    return rec;
}

//******************************** TEST CASES **********************************

//******************************** INV_HEADER tests ********************************

//******************************** Test 3 **************************************

static SubTestRec _tinvalidFileTest7(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/nonZeroHeader.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_HEADER || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get INV_HEADER with non-zero level in header record in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled non-zero level in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest8(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/missingHeaderSource.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_HEADER || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get INV_HEADER with missing header in header record in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled missing header source in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest9(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/missingVersionHeader.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_HEADER || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get INV_HEADER with missing version in header record in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled missing version in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest10(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/missingEncodingHeader.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_HEADER || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get INV_HEADER with missing encoding in header record in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled missing encoding in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest11(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/missingSubmitterHeader.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != INV_HEADER || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get INV_HEADER with missing Submitter reference Header in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled missing Submitter reference in Header in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest14(int testNum, int subTest){
    char fileName[] = "testFiles/valid/nonASCIIEncoded.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK || (obj->header)->encoding != ANSEL){
        sprintf(feedback, "Subtest %d.%d: Failed to get accept ANSEL Character Encoding instead of ASCII in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled ANSEL Character Encoding [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

testRec* _tinvalidFileTests3(int testNum){
    const int numSubs = 6;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];

    sprintf(feedback, "Test %d: INV_HEADER", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest7);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest8);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest9);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest10);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest11);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest14);

    return rec;
}

//******************************** TEST CASES **********************************

//******************************** INV_RECORD tests ********************************

//******************************** Test 4 **************************************

static SubTestRec _tinvalidFileTest12(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/255line.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if ((response.type != INV_RECORD && response.type != INV_HEADER)
        || response.line != 3 || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get error with > 255 line in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled > 255 line in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

static SubTestRec _tinvalidFileTest13(int testNum, int subTest){
    char fileName[] = "testFiles/invalid/wrongLevel.ged";
    char feedback[300];

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);

    if ((response.type != INV_RECORD && response.type != INV_HEADER)
        || response.line != 7 || obj != NULL){
        sprintf(feedback, "Subtest %d.%d: Failed to get INV_RECORD with wrong level increment in file [%s] for createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    else{
        sprintf(feedback, "Subtest %d.%d: Successfully handled wrong level increment in file [%s] into createGEDCOM.", testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
}

//Add two more test cases for > 255 and wrongLevel

testRec* _tinvalidFileTests4(int testNum){
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];

    sprintf(feedback, "Test %d: INV_RECORD", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest12);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tinvalidFileTest13);

    return rec;
}
