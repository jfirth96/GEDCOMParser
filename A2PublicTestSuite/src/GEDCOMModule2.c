#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"
#include <stdbool.h>
#include <strings.h>

//******************************** TEST DATA ********************************

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

static bool _tIsListEmpty(List l){
    if (l.head->data == NULL || l.tail->data == NULL){
        return true;
    }else{
        return false;
    }
}

static int _tListElemCount(List l){
    int count = 0;
    void* elem = NULL;
    
    ListIterator iter = createIterator(l);
    
    //Assume getLength() is broken/unimplemented
    while ((elem = nextElement(&iter)) != NULL){
        count += 1;
    }
    
    return count;
}


static GEDCOMobject* _tSimpleValid(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+1);
    strcpy(refSub->submitterName, "Submitter");
    strcpy(refSub->address, "");
    refSub->otherFields = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    
    refObj->submitter = refSub;
    
    //Initilize Header obj
    Header* refHead = malloc(sizeof(Header));
    
    strcpy(refHead->source, "PAF");
    refHead->gedcVersion = 5.5;
    refHead->encoding = ASCII;
    refHead->otherFields = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    refHead->submitter = refSub;
    
    refObj->header = refHead;
    
    //Initilize lists
    refObj->families = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    refObj->individuals = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    
    //Initialize Individuals and insert them into the list
    
    Individual* refInd1 = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd1->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd1);
    
    Individual* refInd2 = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd2->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd2);
    
    Individual* refInd3 = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd3->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd3);
    
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd1, refInd2);
    _tInsertBack(&refObj->families, refFam);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd3);
    
    _tInsertBack(&refInd1->families, refFam);
    _tInsertBack(&refInd2->families, refFam);
    _tInsertBack(&refInd3->families, refFam);
    
    return refObj;
}

//******************************** TEST CASES **********************************

//******************************** getDescendantListN ********************************

static SubTestRec _tGetDescSubTest1(int testNum, int subTest){
	char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tSimpleValid();
    
    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
  
    List descendants = getDescendantListN(obj, individual, 2);
   
    if (_tIsListEmpty(descendants)){
        sprintf(feedback, "Subtest %d.%d: Incorrect descendant list (# generations is 0, must be 1).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    if (_tListElemCount(descendants) != 1){
        sprintf(feedback, "Subtest %d.%d: Incorrect descendant list (# generations is > 1, must be 1).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
   
    List* generation = (List*)descendants.head->data;

    if (_tIsListEmpty(*generation)){
        sprintf(feedback, "Subtest %d.%d: Incorrect # of individuals in the 1st generation (# individuals = 0, must be 1).",testNum, subTest);
		result = createSubResult(FAIL, feedback);
		return result;
	}

    if (_tListElemCount(*generation) != 1){
        sprintf(feedback, "Subtest %d.%d: Incorrect # of individuals in the 1st generation (# individuals > 1, must be 1).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    
    Individual* indi = generation->head->data;
    
    if(strcmp(indi->givenName, "James") == 0 && strcmp(indi->surname, "Smith") == 0){
        sprintf(feedback, "Subtest %d.%d: Returned correct list of descendants (1 generation, 1 individual - James Smith).",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: Incorrect descendant in generation 1 of descendant list.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }

}


testRec* _tGetDescNTest(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getDescendantListN", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetDescSubTest1);
    return rec;
}

//******************************** getAncestorListN ********************************

static SubTestRec _tGetAnsSubTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    
    GEDCOMobject* obj = _tSimpleValid();
    
    Individual* refIndividual = _tCreateIndividual("James", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    List anscestors = getAncestorListN(obj, individual, 2);
    
    if (_tIsListEmpty(anscestors)){
        sprintf(feedback, "Subtest %d.%d: Incorrect ancestor list (# generations is 0, must be 1).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tListElemCount(anscestors) != 1){
        sprintf(feedback, "Subtest %d.%d: Incorrect ancestor list (# generations is > 1, must be 1).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    List* generation = (List*)anscestors.head->data;
    
    if (_tIsListEmpty(*generation)){
        sprintf(feedback, "Subtest %d.%d: Incorrect # of individuals in the 1st generation (# individuals = 0, must be 2).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tListElemCount(*generation) != 2){
        sprintf(feedback, "Subtest %d.%d: Incorrect # of individuals in the 1st generation (# individuals > 1, must be 2).",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    
    int indCount[2] = {0,0};
    
    void* elem = NULL;
    
    ListIterator iter = createIterator(*generation);
    
    while ((elem = nextElement(&iter)) != NULL){
        Individual* indi = (Individual*)elem;
        if(strcmp(indi->givenName, "John") == 0 && strcmp(indi->surname, "Smith") == 0){
            indCount[0] += 1;
        }
        
        if(strcmp(indi->givenName, "Elizabeth") == 0 && strcmp(indi->surname, "Stansfield") == 0){
            indCount[1] += 1;
        }
    }
    
    if (indCount[0] == 1 && indCount[1] == 1)
    {
        sprintf(feedback, "Subtest %d.%d: Returned correct list of ancestors (1 generation, 2 individuals - John Smith and Elizabeth Stansfield).",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: Incorrect ancestors in generation 1 of ancestor list.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
}


testRec* _tGetAnsNTest(int testNum){
    const int numSubs = 1;
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getAncestorListN", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetAnsSubTest1);
    return rec;
}

