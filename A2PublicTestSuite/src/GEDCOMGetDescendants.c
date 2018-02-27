#include "GEDCOMTestUtilities.h"
#include "GEDCOMparser.h"
#include <stdbool.h>
#include <strings.h>

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

    char * firstName = _tStrCatTst(NULL, ((Individual *) first)->givenName, '\0');
    firstName = _tStrCatTst(firstName, ",", '\0');
    firstName = _tStrCatTst(firstName, ((Individual *) first)->surname, '\0');

    char * secondName = _tStrCatTst(NULL, ((Individual *) second)->givenName, '\0');
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

//******************************** TEST CASES **********************************

//******************************** Get Descendants ********************************

static SubTestRec _tGetDescendantsSubTest1(int testNum, int subTest){
    char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
    void * elem = NULL;
    int counter = 0;

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test findPerson due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    List descendants = getDescendants(NULL, NULL);

    ListIterator iter = createIterator(descendants);
    while ((elem = nextElement(&iter)) != NULL){
        counter++;
       }

    if(counter == 0){
        sprintf(feedback, "Subtest %d.%d: Successfully handled NULL inputs to _tGetDescendants.",testNum, subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Failed to handle NULL inputs into _tGetDescendants.",testNum, subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

static SubTestRec _tGetDescendantsSubTest2(int testNum, int subTest){
    char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
    void * elem = NULL;
    int count = 0;

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test getDescendants due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);

    List descendants = getDescendants(obj, individual);

    ListIterator iter = createIterator(descendants);
    
    //Assume getLength() is broken/unimplemented
    while ((elem = nextElement(&iter)) != NULL){
        if(strcmp(((Individual*)elem)->givenName, "James") == 0 && strcmp(((Individual*)elem)->surname, "Smith") == 0) {
            count += 1;
        }
    }

    if( descendants.head == descendants.tail && count == 1 ){
        sprintf(feedback, "Subtest %d.%d: correctly returned non-empty descendant list for an individual in file %s.",testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: incorrect descendant list for an individual in file %s.",testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

}

static SubTestRec _tGetDescendantsSubTest3(int testNum, int subTest){
    char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
        
    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test getDescendants due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    
    Individual* refIndividual = _tCreateIndividual("James", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    List descendants = getDescendants(obj, individual);
    
    //Assume getLength() is broken/unimplemented
    if( descendants.head == NULL && descendants.head == descendants.tail ){
        sprintf(feedback, "Subtest %d.%d: returned an empty list for an individual with no descendants in file %s.",testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: incorrect descendant list for an individual in file %s.",testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
}

static SubTestRec _tGetDescendantsSubTest4(int testNum, int subTest){
    char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
    
    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test getDescendants due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    
    Individual* refIndividual = _tCreateIndividual("Some", "Dude");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    List descendants = getDescendants(obj, individual);
    
    //Assume getLength() is broken/unimplemented
    if( descendants.head == NULL && descendants.head == descendants.tail ){
        sprintf(feedback, "Subtest %d.%d: returned an empty list for an individual absent in file %s.",testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: incorrect descendant list for an individual in file %s.",testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}


static SubTestRec _tGetDescendantsSubTest5(int testNum, int subTest){
    char feedback[300];
    char fileName[] = "testFiles/valid/simpleValid2Gen.ged";
    void * elem = NULL;
    int count[5] = {0};
    

    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test getDescendants due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

    Individual* refIndividual = _tCreateIndividual("John", "Smith");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);

    List descendants = getDescendants(obj, individual);

    ListIterator iter = createIterator(descendants);

    
    //Assume getLength() is broken/unimplemented
    while ((elem = nextElement(&iter)) != NULL){
        if(strcmp(((Individual*)elem)->givenName, "James") == 0 && 
			strcmp(((Individual*)elem)->surname, "Smith") == 0) {
            count[0] += 1;
        }
        if(strcmp(((Individual*)elem)->givenName, "Jeff") == 0 && 
			strcmp(((Individual*)elem)->surname, "Smith-Doe") == 0) {
            count[1] += 1;
        }
        if(strcmp(((Individual*)elem)->givenName, "Jill") == 0 && 
			strcmp(((Individual*)elem)->surname, "Smith-Doe") == 0) {
            count[2] += 1;
        }
        if(strcmp(((Individual*)elem)->givenName, "Jack") == 0 && 
			strcmp(((Individual*)elem)->surname, "Smith-Doe") == 0) {
            count[3] += 1;
        }
        if(strcmp(((Individual*)elem)->givenName, "June") == 0 && 
			strcmp(((Individual*)elem)->surname, "Smith-Doe") == 0) {
            count[4] += 1;
        }
       }

    /*
    printf("list length: %d\n", getLength(descendants));
    for (int i = 0; i < 5; i++){
        printf("count[%d]=%d\n", i, count[i]);
    }
    */
    
    if(count[0] == 1 && count[1] == 1 && count[2] == 1 && count[3] == 1 && count[4] == 1 ){
        sprintf(feedback, "Subtest %d.%d: returned a correct list of descendants for an individual in file %s.",testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: incorrect descendant list for an individual in file %s.",testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }

}
//
//static SubTestRec _tGetDescendantsSubTest5(int testNum, int subTest){
//    char feedback[300];
//    char fileName[] = "testFiles/valid/nuclearFamily.ged";
//    void * elem = NULL;
//    char * print = NULL;
//    int Susannacorrectcounter = 0;
//    int Hamnetcorrectcounter = 0;
//    int Judithcorrectcounter = 0;
//
//    SubTestRec result;
//    GEDCOMobject * obj = NULL;
//    GEDCOMerror response = createGEDCOM(fileName, &obj);
//
//    if (response.type != OK){
//        sprintf(feedback, "Subtest %d.%d: Unable to test getDescendants due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
//        result = createSubResult(FAIL, feedback);
//        return result;
//    }
//
//    Individual* refIndividual = _tCreateIndividual("Anne", "Hathaway");
//    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
//
//    List descendants = getDescendants(obj, individual);
//
//    ListIterator iter = createIterator(descendants);
//    while ((elem = nextElement(&iter)) != NULL){
//        if(strcmp(((Individual*)elem)->givenName, "Susanna") == 0 &&
//            strcmp(((Individual*)elem)->surname, "Shakespeare") == 0) {
//            Susannacorrectcounter = 1;
//        }
//        if(strcmp(((Individual*)elem)->givenName, "Hamnet") == 0 &&
//            strcmp(((Individual*)elem)->surname, "Shakespeare") == 0) {
//            Hamnetcorrectcounter = 1;
//        }
//        if(strcmp(((Individual*)elem)->givenName, "Judith") == 0 &&
//            strcmp(((Individual*)elem)->surname, "Shakespeare") == 0) {
//            Judithcorrectcounter = 1;
//        }
//        free(print);
//       }
//
//   if(Susannacorrectcounter == 1 && Hamnetcorrectcounter == 1 && Judithcorrectcounter == 1){
//        sprintf(feedback, "Subtest %d.%d: Successfully handled _tGetDescendants searching for persons with descendants, without descendants, and absent name in filename %s.",testNum, subTest, fileName);
//        result = createSubResult(SUCCESS, feedback);
//        return result;
//    }
//    else
//    {
//        sprintf(feedback, "Subtest %d.%d: Failed to handle _tGetDescendants searching for persons with descendants, without descendants, and absent name in filename %s.",testNum, subTest, fileName);
//        result = createSubResult(FAIL, feedback);
//        return result;
//    }
//
//}

static SubTestRec _tGetDescendantsSubTest6(int testNum, int subTest){
    char feedback[300];
    char fileName[] = "testFiles/valid/shakespeare.ged";
    int counter = 0;
    void * elem = NULL;
    
    SubTestRec result;
    GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(fileName, &obj);
    
    if (response.type != OK){
        sprintf(feedback, "Subtest %d.%d: Unable to test getDescendants due to failure of createGEDCOM to open a valid file (%s).",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    
    Individual* refIndividual = _tCreateIndividual("John", "Shakespeare");
    Individual* individual = findPerson(obj, compareIndividualsBool, refIndividual);
    
    List descendants = getDescendants(obj, individual);
    ListIterator iter = createIterator(descendants);

    //Assume getLength() is broken/unimplemented
    while ((elem = nextElement(&iter)) != NULL){
        counter++;
    }
    
    if( counter == 15 ){
        sprintf(feedback, "Subtest %d.%d: returned a correct list of descendants for an individual in file %s.",testNum, subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: incorrect descendant list for an individual in file %s.",testNum, subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}


testRec* _tGetDescendantsTest1(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];

    sprintf(feedback, "Test %d: getDescendants - NULL args", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetDescendantsSubTest1);

    return rec;
}

testRec* _tGetDescendantsTest2(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getDescendants returning one child", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetDescendantsSubTest2);
    return rec;
}

testRec* _tGetDescendantsTest3(int testNum){
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getDescendants returnning empty lists", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetDescendantsSubTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &_tGetDescendantsSubTest4);
    //    subTest++;
    //    runSubTest(testNum, subTest, rec, &_tGetDescendantsTest5);
    return rec;
}


testRec* _tGetDescendantsTest4(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getDescendants returnning empty lists", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetDescendantsSubTest5);
    return rec;
}

testRec* _tGetDescendantsTest5(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: getDescendants returnning empty lists", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &_tGetDescendantsSubTest6);
    return rec;
}




