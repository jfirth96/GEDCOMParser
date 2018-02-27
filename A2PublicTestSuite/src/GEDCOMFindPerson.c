#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"
#include <stdbool.h>
#include <strings.h>

//******************************** TEST CASES **********************************

//******************************** Find Person ********************************

//******************************** Test 1 **************************************
static bool compareIndividualsBool(const void* first,const void* second){
    if(first == second){
        return true;
    }
    
    if(first == NULL || second == NULL)
    {
        return false;
    }

	bool boolresult;
	int intresult;

    char* firstName = _tStrCatTst(NULL, ((Individual *) first)->givenName, '\0');
    firstName = _tStrCatTst(firstName, ",", '\0');
    firstName = _tStrCatTst(firstName, ((Individual *) first)->surname, '\0');

    char* secondName = _tStrCatTst(NULL, ((Individual *) second)->givenName, '\0');
    secondName = _tStrCatTst(secondName, ",", '\0');
    secondName = _tStrCatTst(secondName, ((Individual *) second)->surname, '\0');

	intresult = strcmp(firstName, secondName);
	if(intresult == 0)
	{
		boolresult = true;
	}
	else
	{
		boolresult = false;
	}

    free(firstName);
    free(secondName);

    return boolresult;
}

static SubTestRec _tFindPersonSubTest1(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test findPerson due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

	Individual* individual = findPerson(NULL, NULL, NULL);

	if (individual == NULL){
		sprintf(feedback, "Subtest %d.%d: Successfully handled NULL inputs into findPerson %s.",testNum, subTest, fileName);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}
	else
	{
		sprintf(feedback, "Subtest %d.%d: Failed to handle NULL inputs into findPerson %s.",testNum, subTest, fileName);
		result = createSubResult(FAIL, feedback);
		return result;
	}
}

static SubTestRec _tFindPersonSubTest2(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/minValid.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test findPerson due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

	Individual* refIndividual = _tCreateIndividual("John", "Smith");
	Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
	
	if(individual == NULL)
	{
		sprintf(feedback, "Subtest %d.%d: Successfully returned NULL for individual non-existent in file %s.",testNum, subTest, fileName);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}		
	else
	{
		sprintf(feedback, "Subtest %d.%d: Failed to return NULL for individual non-existent in file %s.",testNum, subTest, fileName);
		result = createSubResult(FAIL, feedback);
		return result;
	}
}

static SubTestRec _tFindPersonSubTest3(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test findPerson due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

	Individual* refIndividual = _tCreateIndividual("John", "Smith");
	Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
	
	if(strcmp(individual->surname, refIndividual->surname) == 0 &&
       strcmp(individual->givenName, refIndividual->givenName) == 0)
	{
		sprintf(feedback, "Subtest %d.%d: Successfully found %s %s in file %s.",testNum, subTest, refIndividual->givenName, refIndividual->surname, fileName);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}		
	else
	{
		sprintf(feedback, "Subtest %d.%d: Failed to find %s %s in file %s.",testNum, subTest, refIndividual->givenName, refIndividual->surname, fileName);
		result = createSubResult(FAIL, feedback);
		return result;
	}
}

static SubTestRec _tFindPersonSubTest4(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/nuclearFamily.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test findPerson due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

	Individual* refIndividual = _tCreateIndividual("Susanna", "Shakespeare");
	Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
	
	if(strcmp(individual->surname, refIndividual->surname) == 0 && 
				strcmp(individual->givenName, refIndividual->givenName) == 0)
	{
		sprintf(feedback, "Subtest %d.%d: Successfully found %s %s in file %s.",testNum, subTest, refIndividual->givenName, individual->surname, fileName);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}		
	else
	{
		sprintf(feedback, "Subtest %d.%d: Failed to find %s %s in file %s.",testNum, subTest, refIndividual->givenName, refIndividual->surname, fileName);
		result = createSubResult(FAIL, feedback);
		return result;
	}
}

static SubTestRec _tFindPersonSubTest5(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test findPerson due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

	Individual* refIndividual = _tCreateIndividual("No", "Person");
	Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
	
	if(individual == NULL)
	{
		sprintf(feedback, "Subtest %d.%d: Successfully did not find %s %s in file %s.",testNum, subTest, refIndividual->givenName, refIndividual->surname, fileName);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}		
	else
	{
		sprintf(feedback, "Subtest %d.%d: Failed and found %s %s in file %s.",testNum, subTest, refIndividual->givenName, refIndividual->surname, fileName);
		result = createSubResult(FAIL, feedback);
		return result;
	}
}

static SubTestRec _tFindPersonSubTest6(int testNum, int subTest){
	char feedback[300];
    char fileName[] = "testFiles/valid/nuclearFamily.ged";
    SubTestRec result;
	GEDCOMobject * obj = NULL;
    
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test findPerson due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

	Individual* refIndividual = _tCreateIndividual("No", "Person");
	Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
	
	if(individual == NULL)
	{
		sprintf(feedback, "Subtest %d.%d: Successfully did not find %s %s in file %s.",testNum, subTest, refIndividual->givenName, refIndividual->surname, fileName);
		result = createSubResult(SUCCESS, feedback);
		return result;
	}		
	else
	{
		sprintf(feedback, "Subtest %d.%d: Failed and found %s %s in file %s.",testNum, subTest, refIndividual->givenName, refIndividual->surname, fileName);
		result = createSubResult(FAIL, feedback);
		return result;
	}
}

testRec* _tFindPersonTest(int testNum){
    const int numSubs = 5;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];

    sprintf(feedback, "Test %d: find Person", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);

	runSubTest(testNum, subTest, rec, &_tFindPersonSubTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tFindPersonSubTest3);
    subTest++;
	runSubTest(testNum, subTest, rec, &_tFindPersonSubTest4);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tFindPersonSubTest5);
    subTest++;
	runSubTest(testNum, subTest, rec, &_tFindPersonSubTest6);
    return rec;
}

testRec* _tFindPersonTestNull(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: find Person", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tFindPersonSubTest1);
    return rec;
}
