#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "LinkedListAPI.h"
#include "GEDCOMparser.h"
//***************************************** GEDCOOM object functions *****************************************

/**
 * Function to create a GEDCOM object based on the contents of an GEDCOM file.
 *
 * @pre File name cannot be an empty string or NULL.  File name must have the .ged extension.
     File represented by this name must exist and must be readable.
 * @post Either:
 *   A valid GEDCOM has been created, its address was stored in the variable obj, and OK was returned
 *     or
 *   An error occurred, the GEDCOM was not created, all temporary memory was freed, obj was set to NULL, and the
 *   appropriate error code was returned
 * @return the error code indicating success or the error encountered when parsing the GEDCOM
 * @param fileName - a string containing the name of the GEDCOM file
 * @param a double pointer to a GEDCOMobject struct that needs to be allocated
 **/
GEDCOMerror createGEDCOM( char* fileName, GEDCOMobject** obj );


/**
 * Function to create a string representation of a GEDCOMobject.
 *
 * @pre GEDCOMobject object exists, is not null, and is valid
 * @post GEDCOMobject has not been modified in any way, and a string representing the GEDCOM contents has been created
 * @return a string contaning a humanly readable representation of a GEDCOMobject
 * @param obj - a pointer to a GEDCOMobject struct
 **/
char* printGEDCOM( const GEDCOMobject* obj );


/**
 * Function to delete all GEDCOM object content and free all the memory.
 *
 * @pre GEDCOM object exists, is not null, and has not been freed
 * @post GEDCOM object had been freed
 * @return none
 * @param obj - a pointer to a GEDCOMobject struct
 **/
void deleteGEDCOM( GEDCOMobject* obj );


/**
 * Function to "convert" the GEDCOMerror into a humanly redabale string.
 *
 * @return a string contaning a humanly readable representation of the error code
 * @param err - an error struct
 **/
char* printError( GEDCOMerror err );

/**
 * Function that searches for an individual in the list using a comparator function.
 * If an individual is found, a pointer to the Individual record
 * Returns NULL if the individual is not found.
 *
 * @pre GEDCOM object exists,is not NULL, and is valid.  Comparator function has been provided.
 * @post GEDCOM object remains unchanged.
 * @return The Individual record associated with the person that matches the search criteria.  If the Individual record is not found, return NULL.
 *   If multiple records match the search criteria, return the first one.
 * @param familyRecord - a pointer to a GEDCOMobject struct
 * @param compare - a pointer to comparator fuction for customizing the search
 * @param person - a pointer to search data, which contains seach criteria
 * Note: while the arguments of compare() and person are all void, it is assumed that records they point to are
 *   all of the same type - just like arguments to the compare() function in the List struct
 **/
Individual* findPerson( const GEDCOMobject* familyRecord, bool (*compare)( const void* first, const void* second ), const void* person );


/**
 * Function to return a list of all descendants of an individual in a GEDCOM
 *
 * @pre GEDCOM object exists, is not null, and is valid
 * @post GEDCOM object has not been modified in any way, and a list of descendants has been created
 * @return a list of descendants.  The list may be empty.  All list members must be of type Individual, and can appear in any order.
 * All list members must be COPIES of the Individual records in the GEDCOM file.  If the returned list is freed, the original GEDCOM
 * must remain unaffected.
 * @param familyRecord - a pointer to a GEDCOMobject struct
 * @param person - the Individual record whose descendants we want
 **/
List getDescendants( const GEDCOMobject* familyRecord, const Individual* person );


//************************************************************************************************************

//****************************************** List helper functions *******************************************
void deleteEvent(void* toBeDeleted);
int compareEvents(const void* first,const void* second);
char* printEvent(void* toBePrinted);

void deleteIndividual(void* toBeDeleted);
int compareIndividuals(const void* first,const void* second);
char* printIndividual( void* toBePrinted ) {
    if (toBePrinted == NULL) return NULL;

    Individual indi = *((Individual*)toBePrinted);
    char* string = NULL;
    if (indi.givenName != NULL) {
        string = realloc( string, sizeof( char ) * strlen( indi.givenName ) + 1 );
        strcpy( string, indi.givenName );
        strcat( string, " " );
    }
    if (indi.surname != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( indi.surname ) + 1) );
        strcat( string, indi.surname );
        strcat( string, "\n" );
    }
    return string;
}

void deleteFamily(void* toBeDeleted);
int compareFamilies(const void* first,const void* second);
char* printFamily( void* toBePrinted ) {
    if (toBePrinted == NULL) return NULL;

    Family fam = *((Family*)toBePrinted);
    char* string = NULL;
    if (fam.husband != NULL) {
        string = realloc( string, sizeof( char ) * 6);
        strcpy( string, "HUSB " );
        if (fam.husband->givenName != NULL) {
            string = realloc( string, sizeof( char ) * strlen( fam.husband->givenName ) + 1 );
            strcat( string, fam.husband->givenName );
        }
        if (fam.husband->surname != NULL) {
            string = realloc( string, sizeof( char ) * strlen( string ) + strlen( fam.husband->surname ) + 1 );
            strcat( string, " " );
            strcat( string, fam.husband->surname );
        }
        strcat( string, "\n" );
    }
    if (fam.wife != NULL) {
        string = realloc( string, sizeof( char ) * strlen( string ) + 6);
        strcpy( string, "WIFE " );
        if (fam.wife->givenName != NULL) {
            string = realloc( string, sizeof( char ) * strlen( fam.wife->givenName ) + 1 );
            strcat( string, fam.wife->givenName );
        }
        if (fam.wife->surname != NULL) {
            string = realloc( string, sizeof( char ) * strlen( string ) + strlen( fam.wife->surname ) + 1 );
            strcat( string, " " );
            strcat( string, fam.wife->surname );
        }
        strcat( string, "\n" );
    }
    if (getLength( fam.children) != 0) {
        ListIterator iter = createIterator( fam.children );
        void* element;
        while ((element = nextElement( &iter )) != NULL) {
            char* tmp = printIndividual( element );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( tmp ) + 1) );
            strcat( string, tmp );
            free( tmp );
        }
    }
    if (getLength( fam.otherFields ) != 0) {
        ListIterator iter = createIterator( fam.otherFields );
        void* element;
        while ((element = nextElement( &iter )) != NULL) {
            char* tmp = printField( element );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( tmp ) + 1) );
            strcat( string, tmp );
            free( tmp );
        }
    }
    return string;
}

void deleteField(void* toBeDeleted) {
    if (toBeDeleted == NULL) return;

    Field temp = *((Field*)toBeDeleted);
    if (temp.tag != NULL) {
        free( temp.tag );
        temp.tag = NULL;
    }
    if (temp.value != NULL) {
        free( temp.value );
        temp.value = NULL;
    }
}

int compareFields(const void* first, const void* second) {
    if (first == NULL || second == NULL) {
        return OTHER;
    }
    Field one = *((Field*)first);
    Field two = *((Field*)second);

    if (one.tag == NULL || one.value == NULL || two.tag == NULL || two.value == NULL) {
        return OTHER;
    }
    char* stringOne = malloc( sizeof( char ) * (strlen( one.tag ) + strlen( one.value ) + 2) );
    char* stringTwo = malloc( sizeof( char ) * (strlen( two.tag ) + strlen( two.value ) + 2) );

    strcpy( stringOne, one.tag );
    strcpy( stringTwo, two.tag );
    stringOne[strlen( stringOne ) - 1] = ' ';
    stringTwo[strlen( stringTwo ) - 1] = ' ';
    strcat( stringOne, one.value );
    strcat( stringTwo, two.value );

    if (strcmp( stringOne, stringTwo ) > 0) {
        return 1;
    } else if (strcmp( stringOne, stringTwo ) < 0) {
        return -1;
    } else {
        return 0;
    }
}

char* printField(void* toBePrinted) {
    if (toBePrinted == NULL) return NULL;
    Field temp = *((Field*)toBePrinted);

    if (temp.tag == NULL || temp.value == NULL) {
        return NULL;
    }

    char* stringOne = malloc( sizeof( char ) * (strlen( temp.tag ) + strlen( temp.value ) + 2) );

    strcpy( stringOne, temp.tag );
    stringOne[strlen( stringOne ) - 1] = '>';
    strcat( stringOne, temp.value );
    return stringOne;
}
//************************************************************************************************************
