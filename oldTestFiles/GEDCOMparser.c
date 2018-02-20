/**
 * @file GEDCOMparser.c
 * @author Jackson Firth 0880887
 * @version CIS2750 A1
 * @date February 2nd 2018
 *
 * @brief This file contains the implementation of the GEDCOMparser module
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"
#include "LinkedListAPI.h"

#define MAX_LINE 300

GEDCOMerror createGEDCOM( char* fileName, GEDCOMobject** obj ) {
    refCount = 0;
    if (fileName == NULL || strlen( fileName ) == 0) {
        *obj = NULL;
        obj = NULL;
        return createError( INV_FILE, -1 );
    } else {
        // Validate filename has .ged extension
        char* temp;
        char* copy = malloc( sizeof( char ) * (strlen( fileName ) + 1) );
        strcpy( copy, fileName );
        temp = strtok( fileName, "." );
        if (temp == NULL) {
            *obj = NULL;
            obj = NULL;
            free( copy );
            return createError( INV_FILE, -1 );
        }
        temp = strtok( NULL, "" );
        if (temp == NULL) {
            *obj = NULL;
            obj = NULL;
            free( copy );
            return createError( INV_FILE, -1 );
        }
        if (strcmp( temp, "ged" ) != 0) {
            *obj = NULL;
            obj = NULL;
            free( copy );
            return createError( INV_FILE, -1 );
        }
        strcpy( fileName, copy );
        free( copy );
    }
    FILE* file = fopen( fileName, "r" );
    if (file == NULL) {
        *obj = NULL;
        obj = NULL;
        return createError( INV_FILE, -1 );
    }

    // Initialize object
    *obj = calloc( sizeof( GEDCOMobject ), 1 );
    (*obj)->header = NULL;
    (*obj)->submitter = NULL;
    (*obj)->individuals = initializeList( &printIndividual, &deleteIndividual, &compareIndividuals );
    (*obj)->families = initializeList( &printFamily, &deleteFamily, &compareFamilies );

    GEDCOMline** record = NULL;
    int count = 0, lineNumber = 0;
    char temp[MAX_LINE], *p;
    bool hasHead, hasSubmit, hasTrailer, hasRecord;
    hasHead = hasSubmit = hasTrailer = hasRecord = false;
    p = robust_fgets( temp, 299, file );
    while( temp[0] == '\n' ) {
        p = robust_fgets( temp, 299, file );
    }
    while (1) {
        count = 0;
        if (strlen( temp ) > 255) {
            GEDCOMerror err = createError( INV_RECORD, lineNumber + 1);
            for (int i = 0; i < count; i++) {
                deleteGEDCOMline( record[i] );
            }
            for (int i = 0; i < refCount; i++) {
                free( referenceArray[i]->extRefID );
                free( referenceArray[i] );
            }
            free( referenceArray );
            free( record );
            fclose( file );
            deleteGEDCOM( *obj );
            *obj = NULL;
            obj = NULL;
            return err;
        }
        record = realloc( record, sizeof( GEDCOMline* ) * (count + 1) );
        record[count] = createGEDCOMline( temp );
        if (lineNumber == 0 && strcmp( record[count]->tag, "HEAD" ) != 0) {
            GEDCOMerror err = createError( INV_GEDCOM, -1 );
            for (int i = 0; i < count; i++) {
                deleteGEDCOMline( record[i] );
            }
            for (int i = 0; i < refCount; i++) {
                free( referenceArray[i]->extRefID );
                free( referenceArray[i] );
            }
            free( referenceArray );
            free( record );
            fclose( file );
            deleteGEDCOM( *obj );
            *obj = NULL;
            obj = NULL;
            return err;
        }
        if (strcmp( record[count]->tag, "CONC" ) == 0) {
            modifyGEDCOMline( record[count - 1], record[count]->lineValue );
        } else if (strcmp( record[count]->tag, "CONT" ) == 0) {
            char* temp = calloc( sizeof( char ), strlen( record[count]->lineValue ) + 2 );
            strcpy( temp, "\n" );
            strcat( temp, record[count]->lineValue );
            modifyGEDCOMline( record[count - 1], temp );
            free( temp );
        }
        if (record[count] == NULL) {
            // free everything return NULL
            GEDCOMerror err = createError( INV_RECORD, record[count - 1]->lineNumber );
            for (int i = 0; i < count; i++) {
                deleteGEDCOMline( record[i] );
            }
            for (int i = 0; i < refCount; i++) {
                free( referenceArray[i]->extRefID );
                free( referenceArray[i] );
            }
            free( referenceArray );
            free( record );
            fclose( file );
            deleteGEDCOM( *obj );
            *obj = NULL;
            obj = NULL;
            return err;
        }
        record[count]->lineNumber = lineNumber + 1;
        count++;
        lineNumber++;
        if ((p = robust_fgets( temp, 299, file )) == NULL) {
            if (strcmp( record[count - 1]->tag, "TRLR" ) != 0) {
                // free everything, return INV_GEDCOM
                GEDCOMerror err = createError( INV_GEDCOM, -1 );
                for (int i = 0; i < count; i++) {
                    deleteGEDCOMline( record[i] );
                }
                if (hasHead) {
                    deleteHeader( (*obj)->header );
                }
                if (hasSubmit) {
                    deleteSubmitter( (*obj)->submitter );
                }
                if (getLength( (*obj)->individuals ) != 0) {
                    clearList( &((*obj)->individuals) );
                }
                if (getLength( (*obj)->families ) != 0) {
                    clearList( &((*obj)->families) );
                }
                for (int i = 0; i < refCount; i++) {
                    free( referenceArray[i]->extRefID );
                    free( referenceArray[i] );
                }
                free( referenceArray );
                free( record );
                fclose( file );
                free( *obj );
                *obj = NULL;
                obj = NULL;
                return err;
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
            if (strlen( temp ) > 255) {
                GEDCOMerror err = createError( INV_RECORD, lineNumber + 1 );
                for (int i = 0; i < count; i++) {
                    deleteGEDCOMline( record[i] );
                }
                for (int i = 0; i < refCount; i++) {
                    free( referenceArray[i]->extRefID );
                    free( referenceArray[i] );
                }
                free( referenceArray );
                free( record );
                fclose( file );
                deleteGEDCOM( *obj );
                *obj = NULL;
                obj = NULL;
                return err;
            }
            record = realloc( record, sizeof( GEDCOMline* ) * (count + 1) );
            record[count] = createGEDCOMline( temp );
            if (record[count] == NULL) {
                // free everything return NULL
                GEDCOMerror err = createError( INV_RECORD, record[count - 1]->lineNumber );
                for (int i = 0; i < count; i++) {
                    deleteGEDCOMline( record[i] );
                }
                for (int i = 0; i < refCount; i++) {
                    free( referenceArray[i]->extRefID );
                    free( referenceArray[i] );
                }
                free( referenceArray );
                free( record );
                fclose( file );
                deleteGEDCOM( *obj );
                *obj = NULL;
                obj = NULL;
                return err;
            }
            if (strcmp( record[count]->tag, "CONC" ) == 0) {
                modifyGEDCOMline( record[count - 1], record[count]->lineValue );
            } else if (strcmp( record[count]->tag, "CONT" ) == 0) {
                char* temp = calloc( sizeof( char ), strlen( record[count]->lineValue ) + 2 );
                strcpy( temp, "\n" );
                strcat( temp, record[count]->lineValue );
                modifyGEDCOMline( record[count - 1], temp );
                free( temp );
            }
            record[count]->lineNumber = lineNumber + 1;
            count++;
            lineNumber++;
            p = robust_fgets( temp, 299, file);
            while( p != NULL && temp[0] == '\n' ) {
                p = robust_fgets( temp, 299, file );
            }
        } while (1);

        for (int i = 1; i < count; i++) {
            if ((record[i]->level > record[i - 1]->level + 1)
                || (record[i]->level < record[i - 1]->level - 1 )) {
                for (int i = 0; i < count; i++) {
                    deleteGEDCOMline( record[i] );
                }
                for (int i = 0; i < refCount; i++) {
                    free( referenceArray[i]->extRefID );
                    free( referenceArray[i] );
                }
                free( referenceArray );
                free( record );
                fclose( file );
                deleteGEDCOM( *obj );
                *obj = NULL;
                obj = NULL;
                return createError( INV_RECORD, record[i]->lineNumber );
            }
        }
        if (strcmp( record[0]->tag, "HEAD" ) == 0) {
            // create header, pass array of lines and parse in the function
            (*obj)->header = createHeader( record, count );
            if ((*obj)->header == NULL) {
                GEDCOMerror err = createError( INV_HEADER, record[count - 1]->lineNumber );
                for (int i = 0; i < count; i++) {
                    deleteGEDCOMline( record[i] );
                }
                for (int i = 0; i < refCount; i++) {
                    free( referenceArray[i]->extRefID );
                    free( referenceArray[i] );
                }
                free( referenceArray );
                free( record );
                fclose( file );
                free( *obj );
                *obj = NULL;
                obj = NULL;
                return err;
            } else {
                hasHead = true;
            }
        } else if (strcmp( record[0]->tag, "SUBM" ) == 0) {
            // create submitter, pass array of lines and parse in the function
            printf( "in code block for 0 SUBM @S001@ record which DNE\n" );
            printf( "%d", record[0]->level );
            if (record[0]->extRefID != NULL) {
                printf( " XR_%s", record[0]->extRefID );
            }
            printf( " T_%s", record[0]->tag );
            if (record[0]->lineValue != NULL) {
                printf( " LV_%s", record[0]->extRefID );
            }
            printf( "\n" );
            bool subFlag = false;
            if (record[0]->level == 0) {
                (*obj)->submitter = createSubmitter( record, count );
            } else {
                (*obj)->submitter = NULL;
                subFlag = true;
            }
            if ((*obj)->submitter == NULL) {
                // free everything
                // return INV_RECORD; ????
                printf( "submit is NULL\n" );
                GEDCOMerror err;
                if (subFlag) {
                    err = createError( INV_GEDCOM, -1 );
                } else {
                    err = createError( INV_RECORD, record[count - 1]->lineNumber );
                }
                for (int i = 0; i < count; i++) {
                    deleteGEDCOMline( record[i] );
                }
                if (hasHead) {
                    deleteHeader( (*obj)->header );
                }
                if (getLength( (*obj)->individuals ) != 0) {
                    clearList( &((*obj)->individuals) );
                }
                if (getLength( (*obj)->families ) != 0) {
                    clearList( &((*obj)->families) );
                }
                for (int i = 0; i < refCount; i++) {
                    free( referenceArray[i]->extRefID );
                    free( referenceArray[i] );
                }
                free( referenceArray );
                free( record );
                fclose( file );
                free( *obj );
                free( obj );
                obj = NULL;
                return err;
            } else {
                hasSubmit = true;
                hasRecord = true;
                (*obj)->header->submitter = (*obj)->submitter;
            }
        } else if (strcmp( record[0]->tag, "INDI" ) == 0) {
            // create individual, pass array of lines and parse in the function
            Individual* indi = createIndividual( record, count );
            if (indi == NULL) {
                // free everything
                GEDCOMerror err = createError( INV_RECORD, record[count - 1]->lineNumber );
                for (int i = 0; i < count; i++) {
                    deleteGEDCOMline( record[i] );
                }
                for (int i = 0; i < refCount; i++) {
                    free( referenceArray[i]->extRefID );
                    free( referenceArray[i] );
                }
                free( referenceArray );
                deleteGEDCOM( *obj );
                free( record );
                fclose( file );
                free( *obj );
                *obj = NULL;
                free( obj );
                obj = NULL;
                return err;
            } else {
                if (record[0]->extRefID != NULL) {
                    referenceArray = realloc( referenceArray, sizeof( RefPair* ) * (refCount + 1) );
                    referenceArray[refCount] = calloc( sizeof( RefPair ), 1 );
                    referenceArray[refCount]->extRefID = malloc( sizeof( char ) * (strlen( record[0]->extRefID ) + 1) );
                    strcpy( referenceArray[refCount]->extRefID, record[0]->extRefID );
                    referenceArray[refCount]->indi = indi;
                    refCount++;
                }
            }
            insertBack( &((*obj)->individuals), (void*)indi );
        } else if (strcmp( record[0]->tag, "FAM" ) == 0) {
            // create family, pass array of lines and parse in the function
            Family* fam = createFamily( record, count );
            if (fam == NULL) {
                // free everything
                GEDCOMerror err = createError( INV_RECORD, record[count - 1]->lineNumber );
                for (int i = 0; i < count; i++) {
                    deleteGEDCOMline( record[i] );
                }
                for (int i = 0; i < refCount; i++) {
                    free( referenceArray[i]->extRefID );
                    free( referenceArray[i] );
                }
                free( referenceArray );
                deleteGEDCOM( *obj );
                free( record );
                fclose( file );
                free( *obj );
                *obj = NULL;
                free( obj );
                obj = NULL;
                return err;
            } else {
                insertSorted( &((*obj)->families), (void*)fam );
            }
        }
        for (int i = 0; i < count; i++) {
            deleteGEDCOMline( record[i] );
        }
        free( record );
        record = NULL;
    }
    fclose( file );
    for (int i = 0; i < refCount; i++) {
        free( referenceArray[i]->extRefID );
        free( referenceArray[i] );
    }
    free( referenceArray );
    if (hasHead && hasSubmit && hasTrailer && hasRecord ) {
        return createError( OK, -1 );
    } else {
        deleteGEDCOM( *obj );
        *obj = NULL;
        return createError( INV_GEDCOM, -1 );
    }
}

char* printGEDCOM( const GEDCOMobject* obj ) {
    char* buffer = NULL;
    char* temp = NULL;

    if (obj == NULL) {
        return NULL;
    }

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
        case OTHER_ERROR:
            string = malloc( sizeof( char ) * (strlen( "Non-GEDCOM related error " ) + 1) );
            strcpy( string, "Non-GEDCOM related error " );
            break;
        default:
            return NULL;
    }
    char num[15];
    sprintf( num, "(line %d)", err.line );
    string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( num ) + 3) );
    strcat( string, num );
    strcat( string, "\n" );
    return string;
}

Individual* findPerson( const GEDCOMobject* familyRecord, bool (*compare)( const void* first, const void* second ), const void* person ) {
    if (person == NULL) {
        return NULL;
    }

    if (getLength( familyRecord->individuals ) != 0) {
        ListIterator iter =  createIterator( familyRecord->individuals );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            if (compare( elem, person )) {
                return (Individual*)elem;
            }
        }
    }
    return NULL;
}

List getDescendants( const GEDCOMobject* familyRecord, const Individual* person ) {
    List toReturn = initializeList( &printIndividualNames, &dummyDelete, &compareIndividuals );

    if (familyRecord == NULL || person == NULL) {
        return toReturn;
    }

    recursiveGetDescendants( &toReturn, person );
    return toReturn;
}


//****************************************** List helper functions *******************************************
void deleteEvent(void* toBeDeleted) {
    if (toBeDeleted == NULL) {
        return;
    }

    Event *event = ((Event*)toBeDeleted);
    if (event->date != NULL) {
        free( event->date );
    }
    if (event->place != NULL) {
        free( event->place );
    }
    if (getLength( event->otherFields ) != 0) {
        clearList( &(event->otherFields) );
    }
    free( event );
}
int compareEvents(const void* first,const void* second) {
    if (first == NULL || second == NULL) {
        return OTHER_ERROR;
    }

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
    if (toBePrinted == NULL) {
        return NULL;
    }

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
    if (toBeDeleted == NULL) {
        return;
    }

    Individual* indi = ((Individual*)toBeDeleted);

    if (indi->givenName != NULL) {
        free( indi->givenName );
        indi->givenName = NULL;
    }
    if (indi->surname != NULL) {
        free( indi->surname );
        indi->surname = NULL;
    }
    if (getLength( indi->events ) != 0) {
        clearList( &(indi->events) );
    }
    if (getLength( indi->families ) != 0) {
        clearList( &(indi->families) );
    }
    if (getLength( indi->otherFields ) != 0) {
        clearList( &(indi->otherFields) );
    }
    free( indi );
}
int compareIndividuals(const void* first,const void* second) {
    if (first == NULL || second == NULL) {
        return OTHER_ERROR;
    }

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
    } else {
        return 0;
    }
}
char* printIndividual( void* toBePrinted ) {
    if (toBePrinted == NULL) {
        return NULL;
    }

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
            free( temp );
        }
    }
    return string;
}

void deleteFamily(void* toBeDeleted) {
    if (toBeDeleted == NULL) {
        return;
    }

    Family* fam = ((Family*)toBeDeleted);
    fam->husband = NULL;
    fam->wife = NULL;
    if (getLength( fam->children ) != 0) {
        clearList( &(fam->children) );
    }
    if (getLength( fam->otherFields ) != 0) {
        clearList( &(fam->otherFields) );
    }
    if (getLength( fam->events ) != 0) {
        clearList( &(fam->events) );
    }
    free( fam );
}
int compareFamilies( const void* first, const void* second ) {
    if (first == NULL || second == NULL) {
        return OTHER_ERROR;
    }

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
    if (toBePrinted == NULL) {
        return NULL;
    }

    Family fam = *((Family*)toBePrinted);
    char* string = calloc( sizeof( char ), 1 );
    if (fam.husband != NULL) {
        string = realloc( string, sizeof( char ) * 6);
        strcpy( string, "HUSB " );
        char* temp = printIndividual( fam.husband );
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
        strcat( string, temp );
        strcat( string, "\n" );
        free( temp );
    }
    if (fam.wife != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + 6));
        strcat( string, "WIFE " );
        char* temp = printIndividual( fam.wife );
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
        strcat( string, temp );
        strcat( string, "\n" );
        free( temp );
    }
    if (getLength( fam.events ) != 0) {
        ListIterator iter = createIterator( fam.events );
        void* element;
        while ((element = nextElement( &iter )) != NULL) {
            char* tmp = printEvent( element );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( tmp ) + 3) );
            strcat( string, tmp );
            strcat( string, "\n" );
            free( tmp );
        }
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
    if (string != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + 2) );
        strcat( string, "\n" );
    }
    return string;
}

void deleteField(void* toBeDeleted) {
    if (toBeDeleted == NULL) {
        return;
    }

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
        return OTHER_ERROR;
    }
    Field one = *((Field*)first);
    Field two = *((Field*)second);

    if (one.tag == NULL || one.value == NULL || two.tag == NULL || two.value == NULL) {
        return OTHER_ERROR;
    }
    char* stringOne = malloc( sizeof( char ) * (strlen( one.tag ) + strlen( one.value ) + 2) );
    char* stringTwo = malloc( sizeof( char ) * (strlen( two.tag ) + strlen( two.value ) + 2) );

    strcpy( stringOne, one.tag );
    strcpy( stringTwo, two.tag );
    stringOne[strlen( stringOne ) - 1] = ' ';
    stringTwo[strlen( stringTwo ) - 1] = ' ';
    strcat( stringOne, one.value );
    strcat( stringTwo, two.value );

    int comp = strcmp( stringOne, stringTwo );
    free( stringOne );
    free( stringTwo );
    if (comp > 0) {
        return 1;
    } else if (comp < 0) {
        return -1;
    } else {
        return 0;
    }
}
char* printField(void* toBePrinted) {
    if (toBePrinted == NULL) {
        return NULL;
    }
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
//****************************************** End of helpers ************************************************
