#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "LinkedListAPI.h"
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"
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
 * Function to "convert" the GEDCOMerror into a humanly readable string.
 *
 * @return a string contaning a humanly readable representation of the error code
 * @param err - an error struct
 **/
char* printError( GEDCOMerror err ) {
    char* string = NULL;
    switch (err.type) {
        case OK:
            string = malloc( sizeof( char ) * (strlen( "OK - no error\n" ) + 1) );
            strcpy( string, "OK - no error\n" );
            break;
        case INV_FILE:
            string = malloc( sizeof( char ) * (strlen( "Invalid file\n" ) + 1) );
            strcpy( string, "Invalid file\n" );
            break;
        case INV_GEDCOM:
            string = malloc( sizeof( char ) * (strlen( "Invalid GEDCOM\n" ) + 1) );
            strcpy( string, "Invalid GEDCOM\n" );
            break;
        case INV_HEADER:
            string = malloc( sizeof( char ) * (strlen( "Invalid header\n" ) + 1) );
            strcpy( string, "Invalid header\n" );
            break;
        case INV_RECORD:
            string = malloc( sizeof( char ) * (strlen( "Invalid record\n" ) + 1) );
            strcpy( string, "Invalid record\n" );
            break;
        case OTHER:
            string = malloc( sizeof( char ) * (strlen( "Non-GEDCOM related error\n" ) + 1) );
            strcpy( string, "Non-GEDCOM related error\n" );
            break;
        default:
            return NULL;
    }
    char num[5];
    sprintf( num, "%d", err.line );
    string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( num ) + 2) );
    strcat( string, num );
    strcat( string, "\n" );
    return string;
}


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
void deleteEvent(void* toBeDeleted) {
    if (toBeDeleted == NULL) return;

    Event *event = ((Event*)toBeDeleted);
    if (event->date != NULL)
        free( event->date );
    if (event->place != NULL)
        free( event->place );
    if (getLength( event->otherFields ) != 0)
        clearList( &(event->otherFields) );

    free( event );
}
int compareEvents(const void* first,const void* second) {
    if (first == NULL || second == NULL) return OTHER;

    Event event1 = *((Event*)first);
    Event event2 = *((Event*)second);
    char* date1 = NULL;
    char* date2 = NULL;

    if (event1.date != NULL) {
        date1 = convertDate( event1.date );
    }
    if (event2.date != NULL) {
        date2 = convertDate( event2.date );
    }
    if (date1 == NULL && date2 != NULL) {
        free( date2 );
        return -1;
    } else if (date1 != NULL && date2 == NULL) {
        free( date1 );
        return 1;
    } else if (date1 == NULL && date2 == NULL) {
        return 0;
    }
    int compareVal = strcmp( date1, date2 );
    free( date1 );
    free( date2 );
    if (compareVal > 0) {
        return 1;
    } else if (compareVal < 0) {
        return -1;
    } else return 0;
}
char* printEvent(void* toBePrinted) {
    if (toBePrinted == NULL) return NULL;

    Event event = *((Event*)toBePrinted);
    char* string = NULL;
    if (event.type != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( event.type ) + 3) );
        strcpy( string, event.type );
        strcat( string, "\n\t" );
    }
    if (event.date != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( event.date ) + 3) );
        strcat( string, event.date );
        strcat( string, "\n\t" );
    }
    if (event.place != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( event.place ) + 3) );
        strcat( string, event.place );
        strcat( string, "\n\t" );
    }
    if( getLength( event.otherFields ) != 0) {
        ListIterator iter = createIterator( event.otherFields );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            char* temp = printField( elem );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
            strcat( string, temp );
            strcat( string, "\n\t" );
        }
    }
    if (string != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + 2) );
        strcat( string, "\n" );
    }
    return string;
}

void deleteIndividual(void* toBeDeleted) {
    if (toBeDeleted == NULL) return;

    Individual* indi = ((Individual*)toBeDeleted);

    if (indi->givenName != NULL)
        free( indi->givenName );
    if (indi->surname != NULL)
        free( indi->surname );
    if (getLength( indi->events ) != 0)
        clearList( &(indi->events) );
    //if (getLength( indi->families ) != 0)
    //    clearList( &(indi->families) );
    if (getLength( indi->otherFields ) != 0)
        clearList( &(indi->otherFields) );

    free( indi );
}
int compareIndividuals(const void* first,const void* second) {
    if (first == NULL || second == NULL) return OTHER;

    Individual indiv1 = *((Individual*)first);
    Individual indiv2 = *((Individual*)second);
    char* string1 = NULL;
    char* string2 = NULL;

    if (indiv1.givenName != NULL) {
        string1 = realloc( string1, sizeof( char ) * strlen( indiv1.givenName ) + 2 );
        strcpy( string1, indiv1.givenName );
        strcat( string1, "," );
    }
    if (indiv1.surname != NULL) {
        string1 = realloc( string1, sizeof( char ) * (strlen( string1 ) + strlen( indiv1.surname ) + 1 ) );
        strcat( string1, indiv1.surname );
    }
    if (indiv2.givenName != NULL) {
        string2 = realloc( string2, sizeof( char ) * strlen( indiv2.givenName ) + 2 );
        strcpy( string2, indiv2.givenName );
        strcat( string2, "," );
    }
    if (indiv2.surname != NULL) {
        string2 = realloc( string2, sizeof( char ) * (strlen( string2 ) + strlen( indiv2.surname ) + 1 ) );
        strcat( string2, indiv2.surname );
    }
    int compareVal = strcmp( string1, string2 );
    free( string1 );
    free( string2 );
    if (compareVal > 0) {
        return 1;
    } else if (compareVal < 0) {
        return -1;
    } else return 0;
}
char* printIndividual( void* toBePrinted ) {
    if (toBePrinted == NULL) return NULL;

    Individual indi = *((Individual*)toBePrinted);
    char* string = NULL;
    if (indi.givenName != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( indi.givenName ) + 2) );
        strcpy( string, indi.givenName );
        strcat( string, " " );
    }
    if (indi.surname != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( indi.surname ) + 3) );
        strcat( string, indi.surname );
        strcat( string, "\n" );
    }
    if (getLength( indi.events ) != 0) {
        ListIterator iter = createIterator( indi.events );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            char* temp = printEvent( elem );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
            strcat( string, temp );
            strcat( string, "\n" );
            free( temp );
        }
    }
    if (getLength( indi.otherFields ) != 0) {
        ListIterator iter = createIterator( indi.otherFields );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            char* temp = printField( elem );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
            strcat( string, temp );
            strcat( string, "\n" );
            free( temp );
        }
    }
    return string;
}

void deleteFamily(void* toBeDeleted) {
    if (toBeDeleted == NULL) return;

    Family *fam = ((Family*)toBeDeleted);
    //if (getLength( fam->children ) != 0)
        //clearList( &(fam->children) );
    if (getLength( fam->otherFields ) != 0)
        clearList( &(fam->otherFields) );

    free( fam );
}
int compareFamilies( const void* first, const void* second ) {
    if (first == NULL || second == NULL) return OTHER;

    Family fam1 = *((Family*)first);
    Family fam2 = *((Family*)second);

    int members1 = familyMemberCount( (void*)&fam1 );
    int members2 = familyMemberCount( (void*)&fam2 );

    if (members1 > members2) {
        return 1;
    } else if (members1 < members2) {
        return -1;
    } else {
        return 0;
    }
}
char* printFamily( void* toBePrinted ) {
    if (toBePrinted == NULL) return NULL;

    Family fam = *((Family*)toBePrinted);
    char* string = NULL;
    if (fam.husband != NULL) {
        string = realloc( string, sizeof( char ) * 6);
        strcpy( string, "HUSB " );
        if (fam.husband->givenName != NULL) {
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( fam.husband->givenName ) + 1) );
            strcat( string, fam.husband->givenName );
        }
        if (fam.husband->surname != NULL) {
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( fam.husband->surname ) + 3) );
            strcat( string, " " );
            strcat( string, fam.husband->surname );
        }
        strcat( string, "\n" );
    }
    if (fam.wife != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + 6));
        strcat( string, "WIFE " );
        if (fam.wife->givenName != NULL) {
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( fam.wife->givenName ) + 1) );
            strcat( string, fam.wife->givenName );
        }
        if (fam.wife->surname != NULL) {
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( fam.wife->surname ) + 3) );
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
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( tmp ) + 7) );
            strcat( string, "CHIL " );
            strcat( string, tmp );
            strcat( string, "\n" );
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
    if (string != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + 2) );
        strcat( string, "\n" );
    }
    return string;
}

void deleteField(void* toBeDeleted) {
    if (toBeDeleted == NULL) return;

    Field* temp = ((Field*)toBeDeleted);
    if (temp->tag != NULL) {
        free( temp->tag );
    }
    if (temp->value != NULL) {
        free( temp->value );
    }
    free( temp );
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
        free( stringOne );
        free( stringTwo );
        return 1;
    } else if (strcmp( stringOne, stringTwo ) < 0) {
        free( stringOne );
        free( stringTwo );
        return -1;
    } else {
        free( stringOne );
        free( stringTwo );
        return 0;
    }
}

char* printField(void* toBePrinted) {
    if (toBePrinted == NULL) return NULL;
    Field temp = *((Field*)toBePrinted);

    if (temp.tag == NULL || temp.value == NULL) {
        return NULL;
    }

    char* stringOne = malloc( sizeof( char ) * (strlen( temp.tag ) + strlen( temp.value ) + 3) );

    strcpy( stringOne, temp.tag );
    strcat( stringOne, " " );
    strcat( stringOne, temp.value );
    return stringOne;
}
//************************************************************************************************************
