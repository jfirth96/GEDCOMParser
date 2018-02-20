#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

//Returns grade for successfully creating min valid GEDCOM
int TEST_CREATE_GEDCOM(char* GEDCOMfilename, int grade, char* testname);

//Returns grade for successfully printing GEDCOM
int TEST_PRINT_GEDCOM(char* GEDCOMfilename, int grade);

//Returns grade for successfully deleting GEDCOM
int TEST_DELETE_GEDCOM(char* GEDCOMfilename, int grade);

//Returns grade for successfully printing error
int TEST_PRINT_ERROR(char* GEDCOMfilename, int grade);

//Returns grade for successfully finding person
int TEST_FIND_PERSON(char* GEDCOMfilename, int grade);

//Returns grade for successfully getting descendants
int TEST_GET_DESCENDANTS(char* GEDCOMfilename, int grade);

//Returns grade for successful INV_FILE error
int TEST_INV_FILE(char* GEDCOMfilename, int grade);

//Returns grade for successful INV_GEDCOM error
int TEST_INV_GEDCOM(char* GEDCOMfilename, int grade);

//Returns grade for successful INV_HEADER error
int TEST_INV_HEADER(char* GEDCOMfilename, int grade);

//Returns grade for successful INV_RECORD error
int TEST_INV_RECORD(char* GEDCOMfilename, int grade);

#endif
