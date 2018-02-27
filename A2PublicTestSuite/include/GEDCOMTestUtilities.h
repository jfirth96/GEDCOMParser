#ifndef GEDCOM_TEST_UTILITIES_H
#define GEDCOM_TEST_UTILITIES_H

#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

#include "LinkedListAPI.h"
#include "GEDCOMparser.h"
#include "TestHarness.h"

//******************************** HELPER FUNCTIONS ********************************
char * _tStrCatTst(char * destination, const char * const source, const char delimiter);

//******************************** LIST FUNCTIONS ********************************
void _tDummyDelete(void* toBeDeleted);
char* _tDummyPrint(void* toBePrinted);
int _tDummyCompare(const void* first,const void* second);

List _tInitializeList(char* (*printFunction)(void* toBePrinted),void (*deleteFunction)(void* toBeDeleted),int (*compareFunction)(const void* first,const void* second));
Node* _tInitializeNode(void* data);
void _tInsertBack(List* list, void* toBeAdded);

bool _tContains(List list, const Individual* ind, bool (*isEqual)(const void* test, const void* ref));
bool _tListEqual(List testList, List refList, bool (*isEqual)(const void* test, const void* ref));

//******************************** CONSTRUCTOR FUNCTIONS ********************************

void _tInitStr(char** str, const char* val);
Field* _tCreateField(char* tag, char* value);
Event* _tCreateEvent(char* tag, char* date, char* place);
Individual* _tCreateIndividual(char* givenName, char* surname);
Family* _tCreateFamily(Individual* husband, Individual* wife);

//******************************** COMPARISON FUNCTIONS ********************************

bool _tPtrCmp(const void* str1, const void* str2);
bool _tSubmEqual(const Submitter* testObj, const Submitter* refObj);
bool _tHeadEqual(const Header* testObj, const Header* refObj);
bool _tFieldEqual(const void* testObj, const void* refObj);
bool _tEventEqual(const void* testObj, const void* refObj);
bool _tIndEqual(const void* testObj, const void* refObj);
bool _tIndEqualShallow(const void* testObj, const void* refObj);
bool _tFamEqual(const void* testObj, const void* refObj);
bool _tFamEqualShallow(const void* testObj, const void* refObj);
bool _tObjEqual(const GEDCOMobject* testObj, const GEDCOMobject* refObj);

#endif
