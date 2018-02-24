#include "GEDCOMTestUtilities.h"

//******************************** TEST CASES **********************************

//******************************** printError ********************************

//******************************** Test 1 **************************************

static SubTestRec _tPrintGEDCOMerrorTest1(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
    
    GEDCOMerror response;
    response.type = OK;
    response.line = -1;
	
    char* print = printError(response);

	if(print != NULL && strlen(print) > 1)
	{
		sprintf(feedback, "Subtest %d.%d: Printed error message based on code %d.",testNum, subTest, response.type);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}
	else
	{
		sprintf(feedback, "Subtest %d.%d: failed to print error message based on code %d.",testNum, subTest, response.type);
		result = createSubResult(FAIL, feedback);
		return result;
	}
    
}

static SubTestRec _tPrintGEDCOMerrorTest2(int testNum, int subTest){
	char feedback[300];
     SubTestRec result;
    
    GEDCOMerror response;
    response.type = INV_FILE;
    response.line = 10;
    
    char* print = printError(response);
    
    if(print != NULL && strlen(print) > 1)
    {
        sprintf(feedback, "Subtest %d.%d: Printed error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to print error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
}


static SubTestRec _tPrintGEDCOMerrorTest3(int testNum, int subTest){
	char feedback[300];
     SubTestRec result;
    
    GEDCOMerror response;
    response.type = INV_GEDCOM;
    response.line = 10;
    
    char* print = printError(response);
    
    if(print != NULL && strlen(print) > 1)
    {
        sprintf(feedback, "Subtest %d.%d: Printed error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to print error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
}


static SubTestRec _tPrintGEDCOMerrorTest4(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMerror response;
    response.type = INV_HEADER;
    response.line = 10;
    
    char* print = printError(response);
    
    if(print != NULL && strlen(print) > 1)
    {
        sprintf(feedback, "Subtest %d.%d: Printed error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to print error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tPrintGEDCOMerrorTest5(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMerror response;
    response.type = INV_RECORD;
    response.line = 10;
    
    char* print = printError(response);
    
    if(print != NULL && strlen(print) > 1)
    {
        sprintf(feedback, "Subtest %d.%d: Printed error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to print error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
}

static SubTestRec _tPrintGEDCOMerrorTest6(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMerror response;
    response.type = OTHER_ERROR;
    response.line = 10;
    
    char* print = printError(response);
    
    if(print != NULL && strlen(print) > 1)
    {
        sprintf(feedback, "Subtest %d.%d: Printed error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to print error message based on code %d.",testNum, subTest, response.type);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
}


testRec* _tPrintGEDCOMerrorTest(int testNum){
    const int numSubs = 6;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];

    sprintf(feedback, "Test %d: Printing GEDCOM", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);

    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMerrorTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMerrorTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMerrorTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMerrorTest4);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMerrorTest5);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tPrintGEDCOMerrorTest6);
    return rec;
}
