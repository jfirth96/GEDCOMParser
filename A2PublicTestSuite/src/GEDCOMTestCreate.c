#include "GEDCOMTestUtilities.h"

//******************************** TEST DATA ********************************

static GEDCOMobject* minValid(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize lists
    refObj->families = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    refObj->individuals = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    
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
    
    return refObj;
}

static GEDCOMobject* oneIndivNoFam(void){
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
    
    Individual* refInd = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd);
    
    return refObj;
}

static GEDCOMobject* individualsNoFam(void){
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
    
    Individual* refInd = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd);
    
    
    refInd = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd);
    
    refInd = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd->otherFields, refField);
    _tInsertBack(&refObj->individuals, refInd);
    
    
    return refObj;
}

static GEDCOMobject* oneFamNoIndiv(void){
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
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(NULL, NULL);
    _tInsertBack(&refObj->families, refFam);
    
    return refObj;
}


static GEDCOMobject* simpleValid(void){
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

static GEDCOMobject* nuclearFamily(void){
    //Initilize reference object
    GEDCOMobject* refObj = malloc(sizeof (GEDCOMobject));
    
    //Initilize Submitter obj
    Submitter* refSub = malloc(sizeof(Submitter)+1);
    strcpy(refSub->submitterName, "webTreePrint");
    strcpy(refSub->address, "");
    refSub->otherFields = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    
    refObj->submitter = refSub;
    
    //Initilize Header obj
    Header* refHead = malloc(sizeof(Header));
    
    strcpy(refHead->source, "webtreeprint.com");
    refHead->gedcVersion = 5.5;
    refHead->encoding = UTF8;
    refHead->otherFields = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    refHead->submitter = refSub;
    
    refObj->header = refHead;
    
    //Initilize lists
    refObj->families = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    refObj->individuals = _tInitializeList(&_tDummyPrint, &_tDummyDelete, &_tDummyCompare);
    
    //Initialize Individuals and insert them into the list
    Individual* refInd[5];
    
    
    //Ind 1
    refInd[0] = _tCreateIndividual("William", "Shakespeare");
    _tInsertBack(&refObj->individuals, refInd[0]);
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[0]->otherFields, refField);
    
    Event* refEvt = _tCreateEvent("BIRT", "BEF 23 APR 1564", "Stratford-upon-Avon");
    _tInsertBack(&refInd[0]->events, refEvt);
    
    refEvt = _tCreateEvent("CHR", "26 APR 1564", "Stratford-upon-Avon");
    _tInsertBack(&refInd[0]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "23 APR 1616", "Stratford-upon-Avon");
    _tInsertBack(&refInd[0]->events, refEvt);
    
    
    //Ind 2
    refInd[1] = _tCreateIndividual("Anne", "Hathaway");
    _tInsertBack(&refObj->individuals, refInd[1]);
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[1]->otherFields, refField);
    
    refEvt = _tCreateEvent("BIRT", "APR 1556", "Shottery, Warwickshire");
    _tInsertBack(&refInd[1]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "06 AUG 1623", "Stratford-upon-Avon");
    _tInsertBack(&refInd[1]->events, refEvt);
    
    
    //Ind 3
    refInd[2] = _tCreateIndividual("Susanna", "Shakespeare");
    _tInsertBack(&refObj->individuals, refInd[2]);
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[2]->otherFields, refField);
    
    refEvt = _tCreateEvent("BIRT", "MAY 1583", "Stratford-upon-Avon");
    _tInsertBack(&refInd[2]->events, refEvt);
    
    refEvt = _tCreateEvent("CHR", "26 MAY 1583", "Stratford-upon-Avon");
    _tInsertBack(&refInd[2]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "11 JUL 1649", "Stratford-upon-Avon");
    _tInsertBack(&refInd[2]->events, refEvt);
    
    
    //Ind 4
    refInd[3] = _tCreateIndividual("Hamnet", "Shakespeare");
    _tInsertBack(&refObj->individuals, refInd[3]);
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[3]->otherFields, refField);
    
    refEvt = _tCreateEvent("BIRT", "JAN 1585", "Stratford-upon-Avon");
    _tInsertBack(&refInd[3]->events, refEvt);
    
    refEvt = _tCreateEvent("CHR", "02 FEB 1585", "Stratford-upon-Avon");
    _tInsertBack(&refInd[3]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "AUG 1596", "Stratford-upon-Avon");
    _tInsertBack(&refInd[3]->events, refEvt);
    
    refEvt = _tCreateEvent("BURI", "11 AUG 1596", "Stratford-upon-Avon");
    _tInsertBack(&refInd[3]->events, refEvt);
    
    
    //Ind 5
    refInd[4] = _tCreateIndividual("Judith", "Shakespeare");
    _tInsertBack(&refObj->individuals, refInd[4]);
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[4]->otherFields, refField);
    
    refEvt = _tCreateEvent("BIRT", "JAN 1585", "Stratford-upon-Avon");
    _tInsertBack(&refInd[4]->events, refEvt);
    
    refEvt = _tCreateEvent("CHR", "02 FEB 1585", "Stratford-upon-Avon");
    _tInsertBack(&refInd[4]->events, refEvt);
    
    refEvt = _tCreateEvent("DEAT", "FEB 1662", "Stratford-upon-Avon");
    _tInsertBack(&refInd[4]->events, refEvt);
    
    refEvt = _tCreateEvent("BURI", "09 FEB 1662", "Stratford-upon-Avon");
    _tInsertBack(&refInd[4]->events, refEvt);
    
    
    //Initialize Families and insert them into the list
    
    //Fam 1
    Family* refFam = _tCreateFamily(refInd[0], refInd[1]);
    refEvt = _tCreateEvent("MARR", "NOV 1582", "");
    _tInsertBack(&refFam->events, refEvt);
    
    _tInsertBack(&refObj->families, refFam);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd[2]);
    _tInsertBack(&refFam->children, refInd[3]);
    _tInsertBack(&refFam->children, refInd[4]);
    
    _tInsertBack(&refInd[0]->families, refFam);
    _tInsertBack(&refInd[1]->families, refFam);
    _tInsertBack(&refInd[2]->families, refFam);
    _tInsertBack(&refInd[3]->families, refFam);
    _tInsertBack(&refInd[4]->families, refFam);
    
    
    return refObj;
}


static GEDCOMobject* simpleValid2Gen(void){
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
    Individual* refInd[8];
    
    refInd[0] = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[0]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[0]);
    
    refInd[1] = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[1]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[1]);
    
    
    refInd[2] = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[2]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[2]);
    
    
    refInd[3] = _tCreateIndividual("Jane", "Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[3]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[3]);
    
    
    refInd[4] = _tCreateIndividual("Jeff", "Smith-Doe");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[4]->otherFields, refField);

    _tInsertBack(&refObj->individuals, refInd[4]);


    refInd[5] = _tCreateIndividual("Jill", "Smith-Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[5]->otherFields, refField);

    _tInsertBack(&refObj->individuals, refInd[5]);


    refInd[6] = _tCreateIndividual("Jack", "Smith-Doe");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[6]->otherFields, refField);

    _tInsertBack(&refObj->individuals, refInd[6]);

    refInd[7] = _tCreateIndividual("June", "Smith-Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[7]->otherFields, refField);

    _tInsertBack(&refObj->individuals, refInd[7]);
    
    
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd[0], refInd[1]);
    _tInsertBack(&refObj->families, refFam);
    
    
    Family* refFam2 = _tCreateFamily(refInd[2], refInd[3]);
    _tInsertBack(&refObj->families, refFam2);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd[2]);
    
    _tInsertBack(&refFam2->children, refInd[4]);
    _tInsertBack(&refFam2->children, refInd[5]);
    _tInsertBack(&refFam2->children, refInd[6]);
    _tInsertBack(&refFam2->children, refInd[7]);

    _tInsertBack(&refInd[0]->families, refFam);
    _tInsertBack(&refInd[1]->families, refFam);
    _tInsertBack(&refInd[2]->families, refFam);
    
    _tInsertBack(&refInd[2]->families, refFam2);
    _tInsertBack(&refInd[3]->families, refFam2);
   
    _tInsertBack(&refInd[4]->families, refFam2);
    _tInsertBack(&refInd[5]->families, refFam2);
    _tInsertBack(&refInd[6]->families, refFam2);
    _tInsertBack(&refInd[7]->families, refFam2);
    
    
    return refObj;
}


static GEDCOMobject* simpleValid2GenE1(void){
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
    Individual* refInd[8];
    
    refInd[0] = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[0]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[0]);
    
    refInd[1] = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[1]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[1]);
    
    
    refInd[2] = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[2]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[2]);
    
    
    refInd[4] = _tCreateIndividual("Jeff", "Smith-Doe");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[4]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[4]);
    
    
    refInd[5] = _tCreateIndividual("Jill", "Smith-Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[5]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[5]);
    
    
    refInd[6] = _tCreateIndividual("Jack", "Smith-Doe");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[6]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[6]);
    
    refInd[7] = _tCreateIndividual("June", "Smith-Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[7]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[7]);
    
    
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd[0], refInd[1]);
    _tInsertBack(&refObj->families, refFam);
    
    
    Family* refFam2 = _tCreateFamily(refInd[2], NULL);
    _tInsertBack(&refObj->families, refFam2);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd[2]);
    
    _tInsertBack(&refFam2->children, refInd[4]);
    _tInsertBack(&refFam2->children, refInd[5]);
    _tInsertBack(&refFam2->children, refInd[6]);
    _tInsertBack(&refFam2->children, refInd[7]);
    
    _tInsertBack(&refInd[0]->families, refFam);
    _tInsertBack(&refInd[1]->families, refFam);
    _tInsertBack(&refInd[2]->families, refFam);
    
    _tInsertBack(&refInd[2]->families, refFam2);
    _tInsertBack(&refInd[4]->families, refFam2);
    _tInsertBack(&refInd[5]->families, refFam2);
    _tInsertBack(&refInd[6]->families, refFam2);
    _tInsertBack(&refInd[7]->families, refFam2);
    
    
    return refObj;
}

static GEDCOMobject* simpleValid2GenE2(void){
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
    Individual* refInd[4];
    
    refInd[0] = _tCreateIndividual("John", "Smith");
    Field* refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[0]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[0]);
    
    refInd[1] = _tCreateIndividual("Elizabeth", "Stansfield");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[1]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[1]);
    
    
    refInd[2] = _tCreateIndividual("James", "Smith");
    refField = _tCreateField("SEX", "M");
    _tInsertBack(&refInd[2]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[2]);
    
    
    refInd[3] = _tCreateIndividual("Jane", "Doe");
    refField = _tCreateField("SEX", "F");
    _tInsertBack(&refInd[3]->otherFields, refField);
    
    _tInsertBack(&refObj->individuals, refInd[3]);
    
    //Initialize Families and insert them into the list
    Family* refFam = _tCreateFamily(refInd[0], refInd[1]);
    _tInsertBack(&refObj->families, refFam);
    
    
    Family* refFam2 = _tCreateFamily(refInd[2], refInd[3]);
    _tInsertBack(&refObj->families, refFam2);
    
    //Set up mutual family and individual references
    
    _tInsertBack(&refFam->children, refInd[2]);
    
    _tInsertBack(&refInd[0]->families, refFam);
    _tInsertBack(&refInd[1]->families, refFam);
    _tInsertBack(&refInd[2]->families, refFam);
    
    _tInsertBack(&refInd[2]->families, refFam2);
    _tInsertBack(&refInd[3]->families, refFam2);
    
    
    return refObj;
}

static bool _tBardObjectValid(GEDCOMobject* obj){
    //31 individuals
    //11 families
    if (getLength(obj->families) != 11){
        return false;
    }
    
    if (getLength(obj->individuals) != 31){
        return false;
    }
    
    //Add other tests
    
    return true;
}

//******************************** TEST CASES ********************************

//******************************** GEDCOM creation ********************************

//******************************** Test 1 ********************************
SubTestRec createGEDTest1(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* refObj = minValid();
    GEDCOMobject* testObj;
    
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest1(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/minValid.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest1);
    return rec;
}

//******************************** Test 2 ********************************
SubTestRec createGEDTest2(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* refObj = oneIndivNoFam();
    GEDCOMobject* testObj;
    
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest2(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/oneIndivNoFam.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest2);
    return rec;
}

//******************************** Test 3 ********************************
SubTestRec createGEDTest3(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* refObj = individualsNoFam();
    GEDCOMobject* testObj;
    
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        printf("%s\n",printGEDCOM(testObj)) ;
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest3(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/individualsNoFam.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest3);
    return rec;
}

//******************************** Test 4 ********************************
SubTestRec createGEDTest4(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* refObj = oneFamNoIndiv();
    GEDCOMobject* testObj;
    
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        printf("%s\n",printGEDCOM(testObj)) ;
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest4(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/oneFamNoIndiv.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest4);
    return rec;
}

//******************************** Test 5 ********************************
SubTestRec createGEDTest5(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* refObj = simpleValid();
    GEDCOMobject* testObj;
    
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        printf("%s\n",printGEDCOM(testObj)) ;
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest5(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/simpleValid1N.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest5);
    return rec;
}

//******************************** Test 6 ********************************
SubTestRec createGEDTest6(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* refObj = nuclearFamily();
    GEDCOMobject* testObj;
    
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest6(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/nuclearFamily.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest6);
    return rec;
}

//******************************** Test 7 ********************************
SubTestRec createGEDTest7(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* refObj = simpleValid2Gen();
    GEDCOMobject* testObj;
    
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest7(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/simpleValid2Gen.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest7);
    return rec;
}

//******************************** Test 8 ********************************
SubTestRec createGEDTest8(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* refObj = simpleValid2GenE1();
    GEDCOMobject* testObj;
    
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest8(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/simpleValid2GenE1.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest8);
    return rec;
}


//******************************** Test 9 ********************************
SubTestRec createGEDTest9(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* refObj = simpleValid2GenE2();
    GEDCOMobject* testObj;
    
    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tObjEqual(testObj, refObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest9(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/simpleValid2GenE2.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest9);
    return rec;
}

//******************************** Test 10 ********************************
testRec* _tValidFileTest10(int testNum){
    const int numSubs = 3;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName1[] = "testFiles/valid/simpleValid1R.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM objects from files with line endings other than LF (\\n)", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    runSubTestWithFile(testNum, subTest, rec, fileName1, &createGEDTest5);
    subTest++;
    
    char fileName2[] = "testFiles/valid/simpleValid1R1N.ged";
    //sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName2);
    runSubTestWithFile(testNum, subTest, rec, fileName2, &createGEDTest5);
    subTest++;
    
    char fileName3[] = "testFiles/valid/simpleValid1N1R.ged";
    //sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName3);
    runSubTestWithFile(testNum, subTest, rec, fileName3, &createGEDTest5);
    return rec;
}

//******************************** Test 11 ********************************
SubTestRec createGEDTest11(int testNum, int subTest, char* fileName){
    SubTestRec result;
    char feedback[300];
    
    GEDCOMobject* testObj;

    GEDCOMerror err = createGEDCOM(fileName, &testObj);
    
    if (err.type != OK){
        sprintf(feedback, "Subtest %d.%d: Did not return OK (returned %d) when parsing a valid file (%s).",testNum,subTest, err.type, fileName);
        result = createSubResult(FAIL, feedback);
        return result;
    }
    
    if (_tBardObjectValid(testObj)){
        sprintf(feedback, "Subtest %d.%d: file %s parsed correctly",testNum,subTest, fileName);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: did not correctly parse a valid file (%s)",testNum,subTest, fileName);
        
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* _tValidFileTest11(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    char fileName[] = "testFiles/valid/shakespeare.ged";
    sprintf(feedback, "Test %d: Creating GEDCOM object from file (%s)", testNum, fileName);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTestWithFile(testNum, subTest, rec, fileName, &createGEDTest11);
    return rec;
}
