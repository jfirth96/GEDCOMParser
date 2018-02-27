#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"
#include <stdbool.h>
#include <strings.h>

//******************************** indToJSON ********************************

static SubTestRec _tIndToJSON1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* refObj = _tCreateIndividual("William","Shakespeare");
    
    char* testJSON = indToJSON(refObj);
    
    if (strcmp(testJSON, "{\"givenName\":\"William\",\"surname\":\"Shakespeare\"}") == 0){
        sprintf(feedback, "Subtest %d.%d: Correctly created JSON from Individual struct.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create JSON from Individual struct.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestIndToJSON(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: indToJSON", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tIndToJSON1);
    return rec;
}

//******************************** JSONtoInd ********************************

static SubTestRec _tJSONtoInd1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    Individual* refObj = _tCreateIndividual("William","Shakespeare");
    Individual* testObj;
    
    char testData[] = "{\"givenName\":\"William\",\"surname\":\"Shakespeare\"}";
    char* testJSON = malloc(sizeof(char)*(strlen(testData)+1));
    strcpy(testJSON, testData);
    
    testObj = JSONtoInd(testJSON);
    
    if (_tIndEqualShallow(refObj, testObj)){
        sprintf(feedback, "Subtest %d.%d: Correctly created Individual from JSON.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to create Individual from JSON.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tTestJSONtoInd(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: JSONtoInd", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tJSONtoInd1);
    return rec;
}


