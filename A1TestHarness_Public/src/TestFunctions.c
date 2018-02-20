#include <stdbool.h>
#include <stdio.h>

#include "GEDCOMparser.h"
#include "GEDCOMTestUtilities.h"
#include "LinkedListAPI.h"
#include "TestFunctions.h"

int TEST_CREATE_GEDCOM(char* GEDCOMfilename, int grade, char* test_name)
	{
	GEDCOMobject * obj = NULL;
    GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);

	printf("%d", response.type);

	if(response.type == OK)
	{
		printf("Successful Creation of %s GEDCOM, %d/%d\n", test_name, grade, grade);
		return grade;		
		}
		else
		{
		printf("Unsuccessful Creation of %s GEDCOM, 0/%d\n", test_name, grade);
		return 0;
	}}


int TEST_PRINT_GEDCOM(char* GEDCOMfilename, int grade)
	{
	GEDCOMobject * obj = NULL;
	GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);

	if(response.type == OK)
	{
		char* print = printGEDCOM(obj);
		if(strstr(print, "Elizabeth") != NULL)
		{
			printf("Successful GEDCOM Print, %d/%d\n", grade, grade);
			return grade;
			}
			else
			{
			printf("Successful GEDCOM Print, 0/%d\n", grade);			
			return 0;
		}
	}
	else
	{
	printf("Error Creating GEDCOM. UnSuccessful GEDCOM print, 0/%d\n", grade);
	return 0;
	}}			
	

int TEST_DELETE_GEDCOM(char* GEDCOMfilename, int grade)
	{
	GEDCOMobject * obj = NULL;
	GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);

	if(response.type == OK)
	{
		deleteGEDCOM(obj);
		if(obj == NULL)
		{
			printf("Successful GEDCOM Deletion, %d/%d\n", grade, grade);
			return grade;
			}
			else
			{
			printf("UnSuccessful GEDCOM Deletion, 0/%d\n", grade);
			return 0;
		}
	}
	else
	{
	printf("Error Creating GEDCOM. UnSuccessful GEDCOM Deletion, 0/%d\n", grade);
	return 0;
	}}			


int TEST_PRINT_ERROR(char* GEDCOMfilename, int grade)
	{
	char* print;
	GEDCOMobject * obj = NULL;
	GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);
	print = printError(response);

	if(response.type != OK)
	{
		if(strcmp(print, "invalid record (line 16)") == 0)
		{
			printf("Successful Print Error Response, %d/%d\n", grade, grade);
			return grade;
			}
			else
			{
			printf("Unsuccessful Print Error Response, 0/%d\n", grade);
			return 0;
		}
	}
	else
	{
	printf("Error Creating GEDCOM. UnSuccessful GEDCOM Print Error, 0/%d\n", grade);		
	return 0;
	}}

/*
int TEST_FIND_PERSON(char* GEDCOMfilename, int grade)
	{
	GEDCOMobject * obj = NULL;
	GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);


	if(response.type == OK)
	{
		Individual * individual = findPerson(obj, compareIndividuals, getFromBack(obj->individuals)); 
		printf("Found individual: %d\n", individual == NULL);
		return grade;				
	}
	else
	{
	printf("Error Creating GEDCOM. UnSuccessful Find Person, 0/%d\n", grade);			
	return 0;
	}}
*/

int TEST_GET_DESCENDANTS(char* GEDCOMfilename, int grade)
	{
	GEDCOMobject * obj = NULL;
	GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);


	if(response.type == OK)
	{
		char * contents = NULL;
		void * elem = NULL;
		char * print = NULL;
        List descendants = getDescendants(obj, getFromFront(obj->individuals));

		ListIterator iter = createIterator(obj->individuals);
		while ((elem = nextElement(&iter)) != NULL){
		    print = (obj->individuals).printData((Field *) elem);
		    contents = _stringConcatenation(contents, "    ", '\0');
		    contents = _stringConcatenation(contents, print, '\0');
		    contents = _stringConcatenation(contents, "\n", '\0');
		}
		if(strstr(contents, "John") != NULL && 
			strstr(contents, "Elizabeth") != NULL &&
			strstr(contents, "James") != NULL)
		{
			printf("Successful Get Descendants, %d/%d\n", grade, grade);
    	    clearList(&descendants);		
			return grade;
		}
		else
		{
			printf("UnSuccessful Get Descendants, 0/%d\n", grade);			
			return 0;
		}
	}
	else
	{
	printf("Error Creating GEDCOM. UnSuccessful Get Descendants, 0/%d\n", grade);			
	return 0;
	}}
	


int TEST_INV_FILE(char* GEDCOMfilename, int grade)
	{
	GEDCOMobject * obj = NULL;
	GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);

	if(response.type == INV_FILE)
	{
		printf("Correct Handle of INV_FILE, %d/%d\n", grade, grade);
		return grade;
		}
		else
		{
		printf("Incorrect Handle of INV_FILE, 0/%d\n", grade);
		return 0;
	}}


int TEST_INV_GEDCOM(char* GEDCOMfilename, int grade)
	{
	GEDCOMobject * obj = NULL;
	GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);

	if(response.type == INV_GEDCOM)
	{
		printf("Correct Handle of INV_GEDCOM, %d/%d\n", grade, grade);
		return grade;
		}
		else
		{
		printf("Incorrect Handle of INV_GEDCOM, 0/%d\n", grade);
		return 0;
	}}


int TEST_INV_HEADER(char* GEDCOMfilename, int grade)
	{
	GEDCOMobject * obj = NULL;
	GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);

	if(response.type == INV_HEADER)
	{
		printf("Correct Handle of INV_HEADER, %d/%d\n", grade, grade);
		return grade;
		}
		else
		{
		printf("Incorrect Handle of INV_HEADER, 0/%d\n", grade);
		return 0;
	}}


int TEST_INV_RECORD(char* GEDCOMfilename, int grade)
	{
	GEDCOMobject * obj = NULL;
	GEDCOMerror response = createGEDCOM(GEDCOMfilename, &obj);

	if(response.type == INV_RECORD)
	{
		printf("Correct Handle of INV_RECORD, %d/%d\n", grade, grade);
		return grade;
		}
		else
		{
		printf("Incorrect Handle of INV_RECORD, 0/%d\n", grade);
		return 0;
	}}














	
