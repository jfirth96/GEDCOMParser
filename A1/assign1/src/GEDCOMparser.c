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
GEDCOMerror createGEDCOM( char* fileName, GEDCOMobject** obj ) {
    if (fileName == NULL)
        return createError( INV_FILE, -1 );
    /*else {
        char* temp;
        temp = strtok( fileName, "." );
        if (temp == NULL) {
            return createError( INV_FILE, -1 );
        }
        temp = strtok( NULL, "\0" );
        if (temp == NULL) {
            return createError( INV_FILE, -1 );
        }
        if (strcmp( temp, "ged" ) != 0) {
            //return createError( INV_FILE, -1 );
        }
    }*/

    (*obj)->header = NULL;
    (*obj)->submitter = NULL;

    FILE* file = fopen( fileName, "r" );
    if (file == NULL) {
        return createError( INV_FILE, -1 );
    }

    (*obj)->individuals = initializeList( &printIndividual, &deleteIndividual, &compareIndividuals );
    (*obj)->families = initializeList( &printFamily, &deleteFamily, &compareFamilies );

    GEDCOMline** record = NULL;
    int count = 0, lineNumber = 0;
    char temp[255], *p;
    bool hasHead, hasSubmit, hasTrailer, hasRecord;
    hasHead = hasSubmit = hasTrailer = hasRecord = false;
    p = robust_fgets( temp, 254, file );
    while (1) {
        count = 0;
        record = realloc( record, sizeof( GEDCOMline* ) * (count + 1) );
        record[count] = createGEDCOMline( temp );
        record[count]->lineNumber = lineNumber + 1;
        count++;
        lineNumber++;
        if ((p = robust_fgets( temp, 254, file )) == NULL) {
            if (strcmp( record[count - 1]->tag, "TRLR" ) != 0) {
                // free everything, return INV_GEDCOM
            } else {
                hasTrailer = true;
            }
            for (int i = 0; i < count; i++) {
                deleteGEDCOMline( record[i] );
            }
            free( record );
            break;
        }

        do {
            if (temp[0] == '0') {
                break;
            }
            record = realloc( record, sizeof( GEDCOMline* ) * (count + 1) );
            record[count] = createGEDCOMline( temp );
            record[count]->lineNumber = lineNumber + 1;
            count++;
            lineNumber++;
            p = robust_fgets( temp, 254, file);
        } while (1);


        if (record[0]->level == 0) {
            // validate 0-level tag
            if (strcmp( record[0]->tag, "HEAD" ) == 0) {
                // create header, pass array of lines and parse in the function
                (*obj)->header = createHeader( record, count );
                if ((*obj)->header == NULL) {
                    // free everything
                    // return INV_HEADER;
                    //GEDCOMerror err = createError( INV_HEADER, record[count - 1]->lineNumber );
                    for (int i = 0; i < count; i++) {
                        deleteGEDCOMline( record[i] );
                    }
                    free( record );
                    //return err;
                }
            } else if (strcmp( record[0]->tag, "SUBM" ) == 0) {
                // create submitter, pass array of lines and parse in the function
                (*obj)->submitter = createSubmitter( record, count );
            } else if (strcmp( record[0]->tag, "INDI" ) == 0) {
                // create individual, pass array of lines and parse in the function
                Individual* indi = createIndividual( record, count );
                insertSorted( &((*obj)->individuals), (void*)indi );
            } else if (strcmp( record[0]->tag, "FAM" ) == 0) {
                // create family, pass array of lines and parse in the function
                Family* fam = createFamily( record, count );
                insertSorted( &((*obj)->families), (void*)fam );
            } else {
                //break;
            }
        } else if (record[0]->level == 1) {
            // validate 1-level tag
            if (strcmp( record[0]->tag, "HEAD") == 0) {
                // throw INV_HEADER
            } else {

            }
        } else if (record[0]->level == 2) {
            // must have a value or CONT|CONC as the tag
        }
        //}
        for (int i = 0; i < count; i++) {
            deleteGEDCOMline( record[i] );
        }
        free( record );
        record = NULL;
    }
    //free( temp );
    //free( temp2 );
    fclose( file );
    return createError( OK, 0 );
}


/**
 * Function to create a string representation of a GEDCOMobject.
 *
 * @pre GEDCOMobject object exists, is not null, and is valid
 * @post GEDCOMobject has not been modified in any way, and a string representing the GEDCOM contents has been created
 * @return a string contaning a humanly readable representation of a GEDCOMobject
 * @param obj - a pointer to a GEDCOMobject struct
 **/
char* printGEDCOM( const GEDCOMobject* obj ) {
    char* buffer = NULL;
    char* temp = NULL;

    buffer = malloc( sizeof( char ) * (strlen( "-- Header --\n" ) + 2) );
    strcpy( buffer, "-- Header --\n" );
    temp = printHeader( obj->header );
    buffer = realloc( buffer, sizeof( char ) * (strlen( buffer ) + strlen( temp ) + 2) );
    strcat( buffer, temp );
    strcat( buffer, "\n" );
    free( temp );

    buffer = realloc( buffer, sizeof( char ) * (strlen( buffer ) + strlen( "-- Submitter --\n" ) + 1) );
    strcat( buffer, "-- Submitter --\n" );
    temp = printSubmitter( obj->submitter );
    buffer = realloc( buffer, sizeof( char ) * (strlen( buffer ) + strlen( temp ) + 2) );
    strcat( buffer, temp );
    strcat( buffer, "\n" );
    free( temp );

    buffer = realloc( buffer, sizeof( char ) * (strlen( buffer ) + strlen( "-- Individuals --\n" ) + 1) );
    strcat( buffer, "-- Individuals --\n" );
    ListIterator indivIter = createIterator( obj->individuals );
    void* elem;
    int i = 0;
    while ((elem = nextElement( &indivIter )) != NULL) {
        temp = printIndividual( elem );
        buffer = realloc( buffer, sizeof( char ) * (strlen( buffer ) + strlen( temp ) + 2) );
        strcat( buffer, temp );
        strcat( buffer, "\n" );
        free( temp );
    }

    buffer = realloc( buffer, sizeof( char ) * (strlen( buffer ) + strlen( "-- Families --\n" ) + 1) );
    strcat( buffer, "-- Families --\n" );
    ListIterator famIter = createIterator( obj->families );
    while ((elem = nextElement( &famIter )) != NULL) {
        temp = malloc( sizeof( char ) * (strlen( "-fam##-\n" ) + 1) );
        sprintf( temp, "-fam%d-\n", ++i );
        buffer = realloc( buffer, sizeof( char ) * (strlen( buffer ) + strlen( temp ) + 1) );
        strcat( buffer, temp );
        free( temp );
        temp = printFamily( elem );
        buffer = realloc( buffer, sizeof( char ) * (strlen( buffer ) + strlen( temp ) + 3) );
        strcat( buffer, temp );
        strcat( buffer, "\n" );
        free( temp );
    }
    return buffer;
}


/**
 * Function to delete all GEDCOM object content and free all the memory.
 *
 * @pre GEDCOM object exists, is not null, and has not been freed
 * @post GEDCOM object had been freed
 * @return none
 * @param obj - a pointer to a GEDCOMobject struct
 **/
void deleteGEDCOM( GEDCOMobject* obj ) {
    if (obj->header != NULL) {
        deleteHeader( obj->header );
    }
    if (obj->submitter != NULL) {
        deleteSubmitter( obj->submitter );
    }
    if (getLength( obj->individuals ) != 0) {
        clearList( &(obj->individuals) );
    }
    if (getLength( obj->families ) != 0) {
        clearList( &(obj->families) );
    }
    free( obj );
}


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
            string = malloc( sizeof( char ) * (strlen( "OK - no error " ) + 1) );
            strcpy( string, "OK - no error " );
            break;
        case INV_FILE:
            string = malloc( sizeof( char ) * (strlen( "Invalid file " ) + 1) );
            strcpy( string, "Invalid file " );
            break;
        case INV_GEDCOM:
            string = malloc( sizeof( char ) * (strlen( "Invalid GEDCOM " ) + 1) );
            strcpy( string, "Invalid GEDCOM " );
            break;
        case INV_HEADER:
            string = malloc( sizeof( char ) * (strlen( "Invalid header " ) + 1) );
            strcpy( string, "Invalid header " );
            break;
        case INV_RECORD:
            string = malloc( sizeof( char ) * (strlen( "Invalid record " ) + 1) );
            strcpy( string, "Invalid record " );
            break;
        case OTHER:
            string = malloc( sizeof( char ) * (strlen( "Non-GEDCOM related error " ) + 1) );
            strcpy( string, "Non-GEDCOM related error " );
            break;
        default:
            return NULL;
    }
    char num[15];
    sprintf( num, "(line %d)", err.line );
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
        strcat( string, "  " );
    }
    if (event.date != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( event.date ) + 3) );
        strcat( string, event.date );
        strcat( string, "  " );
    }
    if (event.place != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( event.place ) + 3) );
        strcat( string, event.place );
        strcat( string, "  " );
    }
    if (getLength( event.otherFields ) != 0) {
        ListIterator iter = createIterator( event.otherFields );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            char* temp = printField( elem );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
            strcat( string, temp );
            strcat( string, "  " );
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

    if (indi->givenName != NULL) {
        free( indi->givenName );
        indi->givenName = NULL;
    }
    if (indi->surname != NULL) {
        free( indi->surname );
        indi->surname = NULL;
    }
    if (getLength( indi->events ) != 0)
        clearList( &(indi->events) );
    if (getLength( indi->families ) != 0)
        clearList( &(indi->families) );
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
    char* string = calloc( sizeof( char ), 1 );
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
            //strcat( string, "\n" );
            free( temp );
        }
    }
    /*if (getLength( indi.otherFields ) != 0) {
        ListIterator iter = createIterator( indi.otherFields );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            char* temp = printField( elem );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
            strcat( string, temp );
            strcat( string, "\n" );
            free( temp );
        }
    }*/
    return string;
}

void deleteFamily(void* toBeDeleted) {
    if (toBeDeleted == NULL) return;

    Family *fam = ((Family*)toBeDeleted);
    fam->husband = NULL;
    fam->wife = NULL;
    if (getLength( fam->children ) != 0)
        clearList( &(fam->children) );
    if (getLength( fam->otherFields ) != 0)
        clearList( &(fam->otherFields) );
    if (getLength( fam->events ) != 0)
        clearList( &(fam->events) );

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
    char* string = calloc( sizeof( char ), 1 );
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
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( tmp ) + 10) );
            strcat( string, "CHIL " );
            strcat( string, tmp );
            strcat( string, "\n" );
            free( tmp );
        }
    }
    if (getLength( fam.events ) != 0) {
        ListIterator iter = createIterator( fam.events );
        void* element;
        while ((element = nextElement( &iter )) != NULL) {
            char* tmp = printEvent( element );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( tmp ) + 3) );
            strcat( string, tmp );
            //strcat( string, "\n" );
            free( tmp );
        }
    }
    /*if (getLength( fam.otherFields ) != 0) {
        ListIterator iter = createIterator( fam.otherFields );
        void* element;
        while ((element = nextElement( &iter )) != NULL) {
            char* tmp = printField( element );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( tmp ) + 3) );
            strcat( string, tmp );
            strcat( string, "\n" );
            free( tmp );
        }
    }*/
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
