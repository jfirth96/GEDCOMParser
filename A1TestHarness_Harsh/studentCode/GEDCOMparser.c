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
    bool fileError = false;
    if (fileName == NULL || strlen( fileName ) == 0) {
        fileError = true;
        goto FILE_ERROR;
    } else {
        // Validate filename has .ged extension
        char* temp;
        char* copy = malloc( sizeof( char ) * (strlen( fileName ) + 1) );
        strcpy( copy, fileName );
        temp = strtok( fileName, "." ); // should contain the file name portion
        if (temp == NULL) {
            free( copy );
            fileError = true;
            goto FILE_ERROR;
        }
        temp = strtok( NULL, "" ); // should contain the extension portion
        if (temp == NULL) {
            free( copy );
            fileError = true;
            goto FILE_ERROR;
        }
        if (strcmp( temp, "ged" ) != 0) { // extension should be "ged"
            free( copy );
            fileError = true;
            goto FILE_ERROR;
        }
        strcpy( fileName, copy );
        free( copy );
    }
    FILE* file = fopen( fileName, "r" );
    if (file == NULL) {
        fileError = true;
    }
    FILE_ERROR:
    if (fileError == true) {
        *obj = NULL;
        obj = NULL;
        return createError( INV_FILE, -1 );
    }

    // Initialize object
    *obj = calloc( sizeof( GEDCOMobject ), 1 );
    (*obj)->individuals = initializeList( &printIndividual, &deleteIndividual, &compareIndividuals );
    (*obj)->families = initializeList( &printFamily, &deleteFamily, &compareFamilies );

    GEDCOMline** record = NULL; // array of lines given as argument to constructor functions
    int count = 0, lineNumber = 0; // count is reset with each "record" in the file, lineNumber lasts the whole file
    char temp[MAX_LINE], *p;
    bool hasHead, hasSubmit, hasTrailer, hasRecord;
    hasHead = hasSubmit = hasTrailer = hasRecord = false;
    p = robust_fgets( temp, MAX_LINE - 1, file );
    while( temp[0] == '\n' ) { // discard blank linkes
        p = robust_fgets( temp, MAX_LINE - 1, file );
    }
    GEDCOMerror err;
    while (1) {
        count = 0;
        if (strlen( temp ) > 255) { // INVALID RECORD
            err = createError( INV_RECORD, lineNumber + 1);
            goto CLEANUP;
        }
        record = realloc( record, sizeof( GEDCOMline* ) * (count + 1) );
        record[count] = createGEDCOMline( temp );
        if (lineNumber == 0 && strcmp( record[count]->tag, "HEAD" ) != 0) {
            err = createError( INV_GEDCOM, -1 );
            goto CLEANUP;
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
            err = createError( INV_RECORD, record[count - 1]->lineNumber );
            goto CLEANUP;
        }
        record[count]->lineNumber = lineNumber + 1;
        count++;
        lineNumber++;
        if ((p = robust_fgets( temp, MAX_LINE - 1, file )) == NULL) {
            if (strcmp( record[count - 1]->tag, "TRLR" ) != 0) {
                err = createError( INV_GEDCOM, -1 );
                goto CLEANUP;
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
                err = createError( INV_RECORD, lineNumber + 1 );
                goto CLEANUP;
            }
            record = realloc( record, sizeof( GEDCOMline* ) * (count + 1) );
            record[count] = createGEDCOMline( temp );
            if (record[count] == NULL) {
                err = createError( INV_RECORD, record[count - 1]->lineNumber );
                goto CLEANUP;
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
            p = robust_fgets( temp, MAX_LINE - 1, file);
            while( p != NULL && temp[0] == '\n' ) {
                p = robust_fgets( temp, MAX_LINE - 1, file );
            }
        } while (1);

        for (int i = 1; i < count; i++) {
            if ((record[i]->level > record[i - 1]->level + 1)
                || (record[i]->level < record[i - 1]->level - 1 )) {
                err = createError( INV_RECORD, record[i]->lineNumber );
                goto CLEANUP;
            }
        }
        if (strcmp( record[0]->tag, "HEAD" ) == 0) {
            // create header, pass array of lines and parse in the function
            (*obj)->header = createHeader( record, count );
            if ((*obj)->header == NULL) {
                err = createError( INV_HEADER, record[count - 1]->lineNumber );
                goto CLEANUP;
            } else {
                hasHead = true;
            }
        } else if (strcmp( record[0]->tag, "SUBM" ) == 0) {
            // create submitter, pass array of lines and parse in the function
            bool subFlag = false;
            if (record[0]->level == 0) {
                (*obj)->submitter = createSubmitter( record, count );
            } else {
                (*obj)->submitter = NULL;
                subFlag = true;
            }
            if ((*obj)->submitter == NULL) {
                // free everything return INV_RECORD / INV_GEDCOM
                if (subFlag) {
                    err = createError( INV_GEDCOM, -1 );
                } else {
                    err = createError( INV_RECORD, record[count - 1]->lineNumber );
                }
                goto CLEANUP;
            } else {
                hasSubmit = true;
                hasRecord = true;
                (*obj)->header->submitter = (*obj)->submitter;
            }
        } else if (strcmp( record[0]->tag, "INDI" ) == 0) {
            // create individual, pass array of lines and parse in the function
            Individual* indi = createIndividual( record, count );
            if (indi == NULL) {
                err = createError( INV_RECORD, record[count - 1]->lineNumber );
                goto CLEANUP;
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
                err = createError( INV_RECORD, record[count - 1]->lineNumber );
                goto CLEANUP;
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
    referenceArray = NULL;
    if (hasHead && hasSubmit && hasTrailer && hasRecord ) {
        return createError( OK, -1 );
    } else {
        deleteGEDCOM( *obj );
        *obj = NULL;
        return createError( INV_GEDCOM, -1 );
    }

    CLEANUP:
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
/*
List getAncestorListN(const GEDCOMobject* familyRecord, const Individual* person, int maxGen) {
    List ancestors = initializeList( &printDescendants, &deleteDescendant, &mySurnameCompare );
    // Sanity checks
    if (familyRecord == NULL || person == NULL) {
        return ancestors;
    }

    // List of individuals to get the parents of
    List getParentsOf = initializeList( &printGenerationList, &dummyDelete, &mySurnameCompare );

    // Add given individual to list
    insertBack( &getParentsOf, (void*)person );

    // foreach generation ...
    for (int i = 0; i < maxGen; i++) {
        List generation = initializeList( &printIndividualNames, &dummyDelete, &mySurnameCompare );
        // foreach person in getChildrenOf list
        ListIterator iter = createIterator( getParentsOf );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            List tmp  = getParents( (Individual*)elem );
            combineLists( &generation, &(tmp), 1 );
        }
        clearList( &getParentsOf );
        if (getLength( generation ) == 0) {
            break;
        }
        // put generation list into descendants list (required list pointer)
        List* ptr = calloc( sizeof( List ), 1 );
        ptr->deleteData = &dummyDelete;
    	ptr->compare = &compareIndividuals;
    	ptr->printData = &printGenerationList;
        combineLists( ptr, &generation, 0 );
        insertBack( &ancestors, (void*)(ptr) );
        // put current generation of individuals into getChildrenOf list
        combineLists( &getParentsOf, &generation, 1 );
    }
    if (getLength( getParentsOf ) != 0) {
        clearList( &getParentsOf );
    }

    if (maxGen == 0) {
        // List of individuals to get the children of
        List getParentsOf = initializeList( &printGenerationList, &dummyDelete, &mySurnameCompare );
        // Put given individual as first person in list
        insertBack( &getParentsOf, (void*)person );
        do { // do while generation is not empty
            List generation = initializeList( &printGenerationList, &dummyDelete, &mySurnameCompare );
            ListIterator iter = createIterator( getParentsOf );
            void* elem;
            while ((elem = nextElement( &iter )) != NULL) {
                List tmp = getParents( (Individual*)elem );
                combineLists( &generation, &(tmp), 1 );
            }
            clearList( &getParentsOf );
            if (getLength( generation ) == 0) {
                break;
            }

            List* ptr = calloc( sizeof( List ), 1 );
            ptr->deleteData = &dummyDelete;
            ptr->compare = &compareIndividuals;
            ptr->printData = &printGenerationList;
            combineLists( ptr, &generation, 0 );
            insertBack( &ancestors, (void*)(ptr) );
            // put current generation of individuals into getChildrenOf list
            combineLists( &getParentsOf, &generation, 1 );
            //clearList( &generation );
        } while (1);
        clearList( &getParentsOf );
    }

    return ancestors;
}
*/
// Deprecated from A1
List getDescendants( const GEDCOMobject* familyRecord, const Individual* person ) {
    List toReturn = initializeList( &printIndividualNames, &dummyDelete, &compareIndividuals );

    if (familyRecord == NULL || person == NULL) {
        return toReturn;
    }

    recursiveGetDescendants( &toReturn, person );
    return toReturn;
}

/*List getDescendantListN(const GEDCOMobject* familyRecord, const Individual* person, unsigned int maxGen) {
    // Sanity checks
    List descendants = initializeList( &printDescendants, &deleteDescendant, &mySurnameCompare );
    if (familyRecord == NULL || person == NULL) {
        return descendants;
    }

    // List of individuals to get the children of
    List getChildrenOf = initializeList( &printGenerationList, &dummyDelete, &mySurnameCompare );

    // Put given individual as first person in list
    insertSorted( &getChildrenOf, (void*)person );

    // foreach generation ...
    for (int i = 0; i < maxGen; i++) {
        List generation = initializeList( &printGenerationList, &dummyDelete, &mySurnameCompare );
        // foreach person in getChildrenOf list
        ListIterator iter = createIterator( getChildrenOf );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            List tmp  = getChildren( (Individual*)elem );
            combineLists( &generation, &tmp, 1 );
        }
        clearList( &getChildrenOf );
        if (getLength( generation ) == 0) {
            break;
        }
        // put generation list into descendants list (required list pointer)
        List* ptr = calloc( sizeof( List ), 1 );
        ptr->deleteData = &dummyDelete;
    	ptr->compare = &mySurnameCompare;
    	ptr->printData = &printGenerationList;
        combineLists( ptr, &generation, 0 );
        insertBack( &descendants, (void*)(ptr) );
        // put current generation of individuals into getChildrenOf list
        combineLists( &getChildrenOf, &generation, 0 );
        clearList( &generation );
    }
    if (getLength( getChildrenOf ) != 0) {
        clearList( &getChildrenOf );
    }

    if (maxGen == 0) {
        // List of individuals to get the children of
        List getChildrenOf = initializeList( &printGenerationList, &dummyDelete, &mySurnameCompare );
        // Put given individual as first person in list
        insertBack( &getChildrenOf, (void*)person );
        do { // do while generation is not empty
            List generation = initializeList( &printGenerationList, &dummyDelete, &mySurnameCompare );
            // foreach person in getChildrenOf list
            ListIterator iter = createIterator( getChildrenOf );
            void* elem;
            while ((elem = nextElement( &iter )) != NULL) {
                List tmp  = getChildren( (Individual*)elem );
                combineLists( &generation, &tmp, 1 );
            }
            clearList( &getChildrenOf );
            if (getLength( generation ) == 0) {
                break;
            }
            // put generation list into descendants list (required list pointer)
            List* ptr = calloc( sizeof( List ), 1 );
            ptr->deleteData = &dummyDelete;
        	ptr->compare = &mySurnameCompare;
        	ptr->printData = &printGenerationList;
            combineLists( ptr, &generation, 0 );
            insertBack( &descendants, (void*)(ptr) );
            // put current generation of individuals into getChildrenOf list
            combineLists( &getChildrenOf, &generation, 0 );
            clearList( &generation );
        } while (1);
        clearList( &getChildrenOf );
    }
    return descendants;
}
*/
// ErrorCode validateGEDCOM( const GEDCOMobject* obj ) {
//     // INV_GEDCOM cases
//     if (obj->header == NULL || obj->submitter == NULL) {
//         return INV_GEDCOM;
//     }
//
//     // INV_HEADER cases
//     Header* head = obj->header;
//     if (strlen( head->source ) == 0 || head->submitter == NULL) {
//         return INV_HEADER;
//     }
//
//     // INV_RECORD cases
//     Submitter* sub = obj->submitter;
//     if (sub->submitterName != NULL && strlen( sub->submitterName ) > 0) {
//         // do nothing all is good
//     } else {
//         printf( "inv_sub\n" );
//         return INV_RECORD;
//     }
//     /* TODO
//         - check for fields that are too long (i.e. tag > 31, lineValue > 200, etc)
//     */
//
//     // INV_RECORD - NULL pointer in individual list
//     if (getLength( obj->individuals ) != 0) {
//         // foreach indiv in obj->indivs
//         ListIterator iter = createIterator( obj->individuals );
//         void* elem;
//         int index = 0;
//         while ((elem = nextElement( &iter )) != NULL) {
//             index++;
//             Individual* indiv = (Individual*)elem;
//             // verify line lengths ???
//
//             // foreach fam in indiv->families
//             ListIterator famIter = createIterator( indiv->families );
//             void* famElem;
//             int index1 = 0;
//             while ((famElem = nextElement( &famIter )) != NULL) {
//                 index1++;
//                 Family* fam = (Family*)famElem;
//                 /*if (fam->husband == NULL || fam->wife == NULL) {
//                     printf( "husb or wife NULL\n" );
//                     return INV_RECORD;
//                 }*/
//                 // foreach child in fam->children
//                 ListIterator childIter = createIterator( fam->children );
//                 void* childElem;
//                 int index2 = 0;
//                 while ((childElem = nextElement( &childIter )) != NULL) {
//                     index2++;
//                 }
//                 if (index2 < getLength( fam->children ) - 1) {
//                     // NULL pointer encountered before end of list
//                     printf( "NULL ptr in fam->children\n" );
//                     return INV_RECORD;
//                 }
//             }
//             if (index1 < getLength( indiv->families ) - 1) {
//                 // NULL pointer encountered before end of list
//                 return INV_RECORD;
//             }
//         }
//         if (index < getLength( obj->individuals ) - 1) {
//             // NULL pointer encountered before end of list
//             return INV_RECORD;
//         }
//     }
//
//     // INV_RECORD - NULL pointer in family list
//     if (getLength( obj->families ) > 0) {
//         // foreach fam in obj->fams
//         ListIterator iter = createIterator( obj->families );
//         void* elem;
//         int famIndex = 0;
//         while ((elem = nextElement( &iter )) != NULL) {
//             famIndex++;
//             Family* fam = (Family*)elem;
//             /*if (fam->husband == NULL || fam->wife == NULL) {
//                 return INV_RECORD;
//             }*/
//             if (getLength( fam->children ) != 0) {
//                 ListIterator indivIter = createIterator( fam->children );
//                 void* elem;
//                 int indivIndex = 0;
//                 while ((elem = nextElement( &indivIter )) != NULL) {
//                     indivIndex++;
//                 }
//                 if (indivIndex < getLength( fam->children ) - 1) {
//                     // NULL pointer encountered before end of list
//                     return INV_RECORD;
//                 }
//             }
//         }
//         if (famIndex < getLength( obj->families ) - 1) {
//             // NULL pointer encountered before end of list
//             return INV_RECORD;
//         }
//     }
//     return OK;
// }
//
// GEDCOMerror writeGEDCOM( char* filename, const GEDCOMobject* obj ) {
//     // Sanity checks
//     if (obj == NULL || filename == NULL || strlen( filename ) == 0) {
//         return createError( WRITE_ERROR, -1 );
//     }
//     FILE* stream = fopen( filename, "w" );
//     if (stream == NULL) {
//         return createError( WRITE_ERROR, -1 );
//     }
//
//     // Write GEDCOMobject components
//     if (writeHeader( obj->header, stream ) == false) {
//         fclose( stream );
//         return createError( WRITE_ERROR, -1 );
//     }
//     if (writeSubmitter( obj->submitter, stream ) == false) {
//         fclose( stream );
//         return createError( WRITE_ERROR, -1 );
//     }
//     refCount = famCount = 0;
//     ListIterator iter = createIterator( obj->families );
//     void* data;
//     while ((data = nextElement( &iter )) != NULL) { // add families to tracking array
//         famArray = realloc( famArray, sizeof( FamPtrPair*) * (famCount + 1) );
//         famArray[famCount] = calloc( sizeof( FamPtrPair ), 1 );
//         Family* fam = (Family*)data;
//         char temp[10];
//         sprintf( temp, "@F00%d@", famCount + 1 );
//         famArray[famCount]->extRefID = calloc( sizeof( char ), (strlen( temp ) + 1) );
//         strcpy( famArray[famCount]->extRefID, temp );
//         famArray[famCount]->fam = fam;
//         famCount++;
//     }
//     if (getLength( obj->individuals ) != 0) { // write individuals
//         ListIterator iter = createIterator( obj->individuals );
//         void* elem;
//         int recordNumber = 1;
//         while ((elem = nextElement( &iter )) != NULL) {
//             Individual* indi = (Individual*)elem;
//             addReferencePairToArray( indi, recordNumber );
//             if (writeIndividual( indi, stream, recordNumber++ ) == false) {
//                 fclose( stream );
//                 return createError( WRITE_ERROR, -1 );
//             }
//         }
//     }
//
//     if (getLength( obj->families ) != 0) {
//         ListIterator iter = createIterator( obj->families );
//         void* elem;
//         while ((elem = nextElement( &iter )) != NULL) {
//             Family* fam = (Family*)elem;
//             if (writeFamily( fam, stream ) == false) {
//                 fclose( stream );
//                 return createError( WRITE_ERROR, -1 );
//             }
//         }
//     }
//     fprintf( stream, "0 TRLR\n" );
//     fclose( stream );
//     for (int i = 0; i < refCount; i++) {
//         free( referenceArray[i]->extRefID );
//         free( referenceArray[i] );
//     }
//     for (int i = 0; i < famCount; i++) {
//         free( famArray[i]->extRefID );
//         free( famArray[i] );
//     }
//     free( famArray );
//     free( referenceArray );
//     referenceArray = NULL;
//     refCount = 0;
//     return createError( OK, -1 );
// }
//
// char* indToJSON(const Individual* ind) {
//     char* str = calloc( sizeof( char ), 1 );
//     strcpy( str, "" );
//     if (ind == NULL) {
//         return str;
//     }
//     str = realloc( str, sizeof( char ) * (strlen( str ) + strlen( "{\"givenName\":\"" ) + 1) );
//     strcat( str, "{\"givenName\":\"" );
//     if (ind->givenName != NULL) {
//         str = realloc( str, sizeof( char ) * (strlen( str ) + strlen( ind->givenName ) + 1) );
//         strcat( str, ind->givenName );
//     }
//     str = realloc( str, sizeof( char ) * (strlen( str ) + strlen( "\"," ) + 1) );
//     strcat( str, "\"," );
//     str = realloc( str, sizeof( char ) * (strlen( str ) + strlen( "\"surname\":\"" ) + 1) );
//     strcat( str, "\"surname\":\"" );
//     if (ind->surname != NULL) {
//         str = realloc( str, sizeof( char ) * (strlen( str ) + strlen( ind->surname ) + 1) );
//         strcat( str, ind->surname );
//     }
//     str = realloc( str, sizeof( char ) * (strlen( str ) + strlen( "\"}" ) + 1) );
//     strcat( str, "\"}" );
//     return str;
// }
//
// Individual* JSONtoInd(const char* str) {
//     if (str == NULL) {
//         return NULL;
//     }
//
//     char* string = calloc( sizeof( char ), strlen( str ) + 1 );
//     strcpy( string, str );
//
//     for (int i = 0; i < strlen( string ); i++) {
//         if (i < strlen( string ) - 1) {
//             if (string[i] == '\"' && string[i + 1] == '\"') {
//                 char insert = '|';
//                 insertChar( &string, insert, i + 1 );
//             }
//         }
//     }
//
//     Individual* indiv = calloc( sizeof( Individual ), 1 );
//     char* token = strtok( string, "\"" ); // should give <{> (1)
//     if (token == NULL) {
//         free( indiv );
//         free( string );
//         return NULL;
//     }
//     for (int i = 0; i < 8; i++) {
//         token = strtok( NULL, "\"" );
//         if (token == NULL) {
//             free( string );
//             free( indiv );
//             return NULL;
//         }
//         if (i == 2) { // index of given name field
//             if (strcmp( token, "|" ) == 0) { // placeholder inserted in case of empty name
//                 token[0] = '\0';
//             }
//             indiv->givenName = calloc( sizeof( char ), (strlen( token ) + 1) );
//             strcpy( indiv->givenName, token );
//         }
//         if (i == 6) { // index of surname field
//             if (strcmp( token, "|" ) == 0) { // placeholder inserted in case of empty name
//                 token[0] = '\0';
//             }
//             indiv->surname = calloc( sizeof( char ), (strlen( token ) + 1) );
//             strcpy( indiv->surname, token );
//         }
//     }
//
//     indiv->families = initializeList( &printFamily, &deleteFamily, &compareFamilies );
//     indiv->events = initializeList( &printEvent, &deleteEvent, &compareEvents );
//     indiv->otherFields = initializeList( &printField, &deleteField, &compareFields );
//     free( string );
//     return indiv;
// }
//
// GEDCOMobject* JSONtoGEDCOM(const char* str) {
//     if (str == NULL) {
//         return NULL;
//     }
//
//     char* string = calloc( sizeof( char ), strlen( str ) + 1 );
//     strcpy( string, str );
//
//     for (int i = 0; i < strlen( string ); i++) {
//         if (i < strlen( string ) - 1) {
//             if (string[i] == '\"' && string[i + 1] == '\"') {
//                 char insert = '|';
//                 insertChar( &string, insert, i + 1 );
//             }
//         }
//     }
//
//     GEDCOMobject* obj = calloc( sizeof( GEDCOMobject ), 1 );
//     char* token = strtok( string, "\"" ); // should give <{>
//     if (token == NULL) {
//         free( string );
//         free( obj );
//         return NULL;
//     }
//
//     Header* head = calloc( sizeof( Header ), 1 );
//     Submitter* sub = NULL;
//     sub = calloc( sizeof( Submitter ) + (sizeof( char ) * 100), 1 );
//     for (int i = 0; i < 20; i++) {
//         token = strtok( NULL, "\"" );
//         if (token == NULL) {
//             free( string );
//             free( obj );
//             return NULL;
//         }
//         switch (i) {
//             if (strcmp( token, "|" ) == 0) { // placeholder inserted in case of empty name
//                 token[0] = '\0';
//             }
//             case 2:
//                 strcpy( head->source, token );
//                 break;
//             case 6:
//                 head->gedcVersion = atof( token );
//                 break;
//             case 10:
//                 head->encoding = getCharSetFromString( token );
//                 break;
//             case 14:
//                 strncpy( sub->submitterName, token, 60 );
//                 if (strlen( token ) < 61) {
//                     sub->submitterName[strlen( token )] = '\0';
//                 } else {
//                     sub->submitterName[60] = '\0';
//                 }
//                 break;
//             case 18:
//                 strcpy( sub->address, token );
//                 break;
//             default:
//                 break;
//         }
//     }
//     free( string );
//     obj->header = head;
//     obj->submitter = sub;
//     obj->header->submitter = sub;
//     return obj;
// }
//
// void addIndividual(GEDCOMobject* obj, const Individual* toBeAdded) {
//     if (obj == NULL || toBeAdded == NULL) {
//         return;
//     }
//
//     insertSorted( &obj->individuals, (void*)toBeAdded );
// }
//
// char* iListToJSON(List iList) {
//     char* string = calloc( sizeof( char ), 3 );
//
//     if (getLength( iList ) == 0) {
//         strcpy( string, "[]" );
//         return string;
//     }
//     string[0] = '[';
//     ListIterator iter = createIterator( iList );
//     void* elem;
//     while ((elem = nextElement( &iter )) != NULL) {
//         Individual* indiv = (Individual*)elem;
//         char* temp = indToJSON( indiv );
//         string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
//         strcat( string, temp );
//         strcat( string, "," );
//         free( temp );
//     }
//     string[strlen( string ) - 1] = ']'; // replaces the comma after last value with ']'
//     return string;
// }
//
// char* gListToJSON(List gList) {
//     char* string = calloc( sizeof( char ), 3 );
//
//     if (getLength( gList ) == 0) {
//         strcpy( string, "[]" );
//         return string;
//     }
//     string[0] = '[';
//     ListIterator iter = createIterator( gList );
//     void* elem;
//     while ((elem = nextElement( &iter )) != NULL) {
//         List list = *((List*)elem);
//         char* temp = iListToJSON( list );
//         string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 4) );
//         strcat( string, temp );
//         strcat( string, "," );
//         free( temp );
//     }
//     string[strlen( string ) - 1] = ']'; // replaces comma afer last value with ']'
//     return string;
// }


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
        strcat( string, " " );
    }
    if (event.date != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( event.date ) + 3) );
        strcat( string, event.date );
        strcat( string, " " );
    }
    if (event.place != NULL) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( event.place ) + 3) );
        strcat( string, event.place );
        strcat( string, " " );
    }
    if (getLength( event.otherFields ) != 0) {
        ListIterator iter = createIterator( event.otherFields );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            char* temp = printField( elem );
            string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
            strcat( string, temp );
            strcat( string, " " );
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
        // compare birth dates
        char* birth1 = NULL;
        char* birth2 = NULL;
        if (getLength( indiv1.events ) > 0 ) {
            ListIterator iter = createIterator( indiv1.events );
            void* elem;
            while ((elem = nextElement( &iter )) != NULL) {
                Event* event = (Event*)elem;
                if (strcmp( event->type, "BIRT" ) == 0) {
                    birth1 = event->date;
                    break;
                }
            }
        }
        if (getLength( indiv2.events ) > 0 ) {
            ListIterator iter = createIterator( indiv2.events );
            void* elem;
            while ((elem = nextElement( &iter )) != NULL) {
                Event* event = (Event*)elem;
                if (strcmp( event->type, "BIRT" ) == 0) {
                    birth2 = event->date;
                    break;
                }
            }
        }
        if (birth1 != NULL && birth2 == NULL) {
            return 1;
        } else if (birth1 == NULL && birth2 != NULL) {
            return -1;
        } else if (birth1 != NULL && birth2 != NULL) {
            int compVal = strcmp( birth1, birth2 );
            if (compVal > 0) {
                return 1;
            } else if (compVal < 0) {
                return -1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
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
    if (getLength( indi.otherFields ) != 0) {
        ListIterator iter = createIterator( indi.otherFields );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            char* temp = printField( elem );
            if (temp) {
                string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
                strcat( string, temp );
                free( temp );
            }
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

    char* stringOne = calloc( sizeof( char ), (strlen( temp.tag ) + strlen( temp.value ) + 3) );
    strcpy( stringOne, temp.tag );
    strcat( stringOne, " " );
    strcat( stringOne, temp.value );
    strcat( stringOne, "\n" );
    return stringOne;
}
//****************************************** End of helpers ************************************************
