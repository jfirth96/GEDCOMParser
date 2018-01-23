/**
 * @file GEDCOMutilities.c
 * @author Jackson Firth 0880887
 * @version CIS2750 A1
 * @date January 2018
 *
 * @brief This file contains the implementation of the GEDCOMutilities module
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "GEDCOMutilities.h"
#include "GEDCOMparser.h"

/** Internal Functions */
GEDCOMline* parseGEDCOMline( char* line, char* filename );

bool addEventToIndividual( Individual* indiv, Event* toAdd ) {
    if (indiv == NULL || toAdd == NULL)
        return false;

    insertSorted( &(indiv->events), (void*)toAdd );
    return true;
}

bool addFieldToRecord( void* record, Field* toAdd, RecordType type ) {
    if (record == NULL || toAdd == NULL)
        return NULL;

    Family* fam;
    Individual* indi;
    Event* event;
    switch (type) {
        case FAM:
            fam = ((Family*)record);
            insertSorted( &(fam->otherFields), (void*)toAdd );
            break;
        case INDI:
            indi = ((Individual*)record);
            insertSorted( &(indi->otherFields), (void*)toAdd );
            break;
        case EVENT:
            event = ((Event*)record);
            insertSorted( &(event->otherFields), (void*)toAdd );
            break;
        default:
            return false;
    }
    return true;
}

bool addToFamily( Family* fam, Individual* indi, RelationType relation ) {
    if (fam == NULL || indi == NULL)
        return false;

    switch (relation) {
        case HUSB:
            if (fam->husband != NULL) {
                deleteIndividual( &(fam->husband) );
            }
            fam->husband = indi;
            break;
        case WIFE:
            if (fam->wife != NULL) {
                deleteIndividual( &(fam->wife) );
            }
            fam->wife = indi;
            break;
        case CHIL:
            insertSorted( &(fam->children), (void*)indi );
            break;
        default:
            return false;
    }
    insertSorted( &(indi->families), (void*)fam );
    return true;
}

GEDCOMerror* createError( ErrorCode type, int line ) {
    if (line < -1)
        return NULL;

    GEDCOMerror* err = malloc( sizeof( GEDCOMerror ) );
    err->type = type;
    err->line = line;
    return err;
}

Event* createEvent( char* type, char* date, char* place ) {
    if (type == NULL || date == NULL || place == NULL)
        return NULL;
    if (strlen( type ) > 4 || strlen( date ) > 11 || strlen( place ) > 120)
        return NULL;

    Event* event = malloc( sizeof( Event ) );
    strcpy( event->type, type );
    event->date = malloc( sizeof( char ) * (strlen( date ) + 1) );
    event->place = malloc( sizeof( char ) * (strlen( place ) + 1) );
    strcpy( event->type, type );
    strcpy( event->date, date );
    strcpy( event->place, place );
    event->otherFields = initializeList( &printField, &deleteField, &compareFields );

    return event;
}

Family* createFamily( Individual* husb, Individual* wife ) {
    Family* fam = malloc( sizeof( Family ) );
    fam->husband = husb;
    fam->wife = wife;
    fam->children = initializeList( &printIndividual, &deleteIndividual, &compareIndividuals );
    fam->otherFields = initializeList( &printField, &deleteField, &compareFields );

    return fam;
}

Field* createField( char* tag, char* value ) {
    if (tag == NULL || value == NULL)
        return NULL;
    if (strlen( tag ) > 31 || strlen( value ) > 120)
        return NULL;

    Field* field = malloc( sizeof( Field ) );
    field->tag = malloc( sizeof( char ) * (strlen( tag ) + 1) );
    field->value = malloc( sizeof( char ) * (strlen( value ) + 1) );
    strcpy( field->tag, tag );
    strcpy( field->value, value );

    return field;
}

GEDCOMline* createGEDCOMline( char* input, char* filename ) {
    if (input == NULL || filename == NULL)
        return NULL;

    GEDCOMline* line = parseGEDCOMline( input, filename );
    return line;
}

Header* createHeader( char* source, float version, CharSet encoding, Submitter* sub ) {
    if (source == NULL || strlen( source ) > 248 || version < 0.0 || sub == NULL)
        return NULL;

    Header* head = malloc( sizeof( Header ) );
    strcpy( head->source, source );
    head->gedcVersion = version;
    head->encoding = encoding;
    head->submitter = sub;
    head->otherFields = initializeList( &printField, &deleteField, &compareFields );
    return head;
}

Individual* createIndividual( char* givenName, char* surname ) {
    if (givenName == NULL || surname == NULL)
        return NULL;
    if (strlen( givenName ) > 120 || strlen( surname ) > 120)
        return NULL;

    Individual* indi = malloc( sizeof( Individual ) );
    indi->givenName = malloc( sizeof( char ) * (strlen( givenName ) + 1) );
    indi->surname = malloc( sizeof( char ) * (strlen( surname ) + 1) );
    strcpy( indi->givenName, givenName );
    strcpy( indi->surname, surname );
    indi->events = initializeList( &printEvent, &deleteEvent, &compareEvents );
    indi->families = initializeList( &printFamily, &deleteFamily, &compareFamilies );
    indi->otherFields = initializeList( &printField, &deleteField, &compareFields );

    return indi;
}

Submitter* createSubmitter( char* subName, char* address ) {
    if (subName == NULL)
        return NULL;

    if (strlen( subName ) > 60) {
        return NULL;
    }

    Submitter* sub = malloc( sizeof( Submitter ) + sizeof( char ) * (strlen( address ) + 1) );
    strcpy( sub->submitterName, subName );
    if (address != NULL) {
        strcpy( sub->address, address );
    } else {
        strcpy( sub->address, "" );
    }
    sub->otherFields = initializeList( &printField, &deleteField, &compareFields );
    return sub;
}

char* convertDate( char* toConvert ) {
    if (toConvert == NULL) return NULL;

    char* copy = malloc( sizeof( char ) * (strlen( toConvert ) + 1) );
    strcpy( copy, toConvert );
    char* token = strtok( copy, " " );
    char* toReturn = malloc( sizeof( char ) * (strlen( token ) + 1) );
    printf( "token: %s\n", token );
    strcpy( toReturn, token );
    //if (atoi( toReturn ) < 1 || atoi( toReturn ) > 31) return NULL;
    token = strtok( NULL, " " );
    printf( "token: %s\n", token );
    for (int i = 0; i < strlen( token ); i++) {
        token[i] = toupper( token[i] );
    }
    printf( "token: %s\n", token );

    toReturn = realloc( toReturn, sizeof( char ) * (strlen( toReturn ) + 3) );
    if (strcmp( token, "JAN" ) == 0) {
        strcat( toReturn, "01" );
    } else if (strcmp( token, "FEB" ) == 0) {
        strcat( toReturn, "02" );
    } else if (strcmp( token, "MAR" ) == 0) {
        strcat( toReturn, "03" );
    } else if (strcmp( token, "APR" ) == 0) {
        strcat( toReturn, "04" );
    } else if (strcmp( token, "MAY" ) == 0) {
        strcat( toReturn, "05" );
    } else if (strcmp( token, "JUN" ) == 0) {
        strcat( toReturn, "06" );
    } else if (strcmp( token, "JUL" ) == 0) {
        strcat( toReturn, "07" );
    } else if (strcmp( token, "AUG" ) == 0) {
        strcat( toReturn, "08" );
    } else if (strcmp( token, "SEP" ) == 0) {
        strcat( toReturn, "09" );
    } else if (strcmp( token, "OCT" ) == 0) {
        strcat( toReturn, "10" );
    } else if (strcmp( token, "NOV" ) == 0) {
        strcat( toReturn, "11" );
    } else if (strcmp( token, "DEC" ) == 0) {
        strcat( toReturn, "12" );
    } else {
    }

    toReturn = realloc( toReturn, sizeof( char ) * (strlen( toReturn ) + 5) );
    token = strtok( NULL, "\n\r" );
    printf( "token: %s\n", token );
    strcat( toReturn, token );

    free( copy );

    //printf( "new date: %s\n", toReturn );
    return toReturn;
}

void deleteGEDCOMline( GEDCOMline* line ) {
    if (line == NULL)
        return;

    free( line->tag );
    if (line->lineValue != NULL) {
        free( line->lineValue );
    }
    free( line );
}

void deleteHeader( Header* head ) {
    if (head == NULL)
        return;

    clearList( &(head->otherFields) );
    free( head );
}

void deleteSubmitter( Submitter* sub ) {
    if (sub == NULL)
        return;

    clearList( &(sub->otherFields) );
    //cfree( sub->address );
    free( sub );
}

int familyMemberCount( const void* family ) {
    if (family == NULL) return -1;

    Family fam = *((Family*)family);

    int members = 0;
    if (fam.husband != NULL)
        members++;
    if (fam.wife != NULL)
        members++;
    members += getLength( fam.children );

    return members;
}

bool modifyGEDCOMline( GEDCOMline* line, char* modValue ) {
    if (line == NULL || modValue == NULL)
        return false;

    line->lineValue = realloc( line->lineValue, sizeof( char ) * (strlen( line->lineValue ) + strlen( modValue ) + 1) );
    strcat( line->lineValue, modValue );
    return true;
}

GEDCOMline* parseGEDCOMline( char* line, char* filename ) {
    if (line == NULL || strlen( line ) > 255)
        return NULL;

    char* temp = strtok( line, " " );
    if (strlen( temp ) > 2 || (strlen( temp ) == 2 && temp[0] == '0')) {
        return NULL;
    }
    GEDCOMline* toReturn = malloc( sizeof( GEDCOMline ) );
    toReturn->level = atoi( temp );
    if (toReturn->level > 99 || toReturn->level < 0) {
        free( toReturn );
        return NULL;
    }
    temp = strtok( NULL, " " );
    if (temp[0] == '@') {
        toReturn->extRefID = malloc( sizeof( char ) * (strlen( temp ) + 1) );
        strcpy( toReturn->extRefID, temp );
        temp = strtok( NULL, " \n\r" );
    } else {
        toReturn->extRefID = NULL;
    }
    toReturn->tag = malloc( sizeof( char ) * (strlen( temp ) + 1) );
    strcpy( toReturn->tag, temp );

    temp = strtok( NULL, "\n\r" );
    if (temp != NULL) {
        toReturn->lineValue = malloc( sizeof( char ) * (strlen( temp ) + 1) );
        strcpy( toReturn->lineValue, temp );
    } else {
        toReturn->lineValue = NULL;
    }
    return toReturn;
}

/********************** START OF HASH TABLE FUNCTIONS *************************/

char* printHTableData( void* printMe ) {
    if (printMe == NULL)
        return NULL;

    ReferencePointerPair* refPtr = (ReferencePointerPair*)printMe;
    char* temp = malloc( sizeof( char ) * 23 );
    sprintf( temp, "%p", &(refPtr->indi) );
    char* string = malloc( sizeof( char ) * (strlen( refPtr->extRefID ) + 1) );
    string = realloc( string, sizeof( char ) * (strlen( temp ) + 2) );
    strcat( string, temp );
    strcat( string, "\n" );
    return string;
}

int compareHTableData( const void* first, const void* second ) {
    if (first == NULL || second == NULL)
        return OTHER;

    ReferencePointerPair* refPtr1 = (ReferencePointerPair*)first;
    ReferencePointerPair* refPtr2 = (ReferencePointerPair*)second;

    int compVal;
    if (refPtr1->extRefID == NULL && refPtr2->extRefID == NULL) {
        compVal = 0;
    } else if (refPtr1->extRefID == NULL && refPtr2->extRefID != NULL) {
        compVal = -1;
    } else if (refPtr1->extRefID != NULL && refPtr2->extRefID == NULL) {
        compVal = 1;
    } else {
        compVal = strcmp( refPtr1->extRefID, refPtr2->extRefID );
        if (compVal > 0)
            compVal = 1;
        else if (compVal < 0) {
            compVal = -1;
        }
    }
    return compVal;
}

void deleteHTableData( void* deleteMe ) {
    ReferencePointerPair* refPtr = (ReferencePointerPair*)deleteMe;
    free( refPtr->extRefID );
    free( refPtr );
}

HTable *createTable( size_t size, int (*hashFunction)( size_t tableSize, char *toHash ),
    void (*destroyData)( void *data ),char* (*printNode)( void *toBePrinted ), int compare( const void *first, const void *second ),
    void (*addFunction)( HTable *hashTable, void *data ) ) {

    if (hashFunction == NULL || destroyData == NULL || printNode == NULL || compare == NULL ) {
        printf( "ERROR - Passed NULL as argument.\n" );
        return NULL;
    }
    int i;
    HTable *newHashTable = malloc( sizeof( HTable ) );

    newHashTable->table = malloc( sizeof( List* ) * size );
    for (i = 0; i < size; i++) {
      *(newHashTable->table[i]) = initializeList( printNode, destroyData, compare );
    }
    newHashTable->destroyData = destroyData;
    newHashTable->hashFunction = hashFunction;
    newHashTable->printNode = printNode;
    newHashTable->addData = addFunction;
    newHashTable->size = size;

    return newHashTable;
}

HNode *createHNode (int key, void *data ) {
    HNode *newHNode = initializeHNode( data );
    newHNode->key = key;

    return newHNode;
}

void destroyTable( HTable *hashTable ) {
    int i;

    for (i = 0; i < hashTable->size; i++) {
        clearList( hashTable->table[i] );
    }
    free( hashTable->table );
    free( hashTable );
    printf( "Hash Table destroyed\n" );
    return;
}

HNode* initializeHNode( void* data ) {
	HNode* tmpNode;

	tmpNode = malloc( sizeof( HNode ) );

	if (tmpNode == NULL){
		return NULL;
	}

	tmpNode->data = data;
	tmpNode->previous = NULL;
	tmpNode->next = NULL;

	return tmpNode;
}

void insertData( HTable *hashTable, int key, void *data ) {
    HNode *newHNode = createHNode( key, data );
    List *list = hashTable->table[key];
    HNode *temp = (HNode*)(list->head);

    while (temp != NULL) {
        if (list->compare( temp->data, data ) == 0) {
            printf( "That value already exists, duplicate entries are not allowed.\n" );
            free( data );
            return;
        }
        temp = temp->next;
    }
    insertSorted( hashTable->table[key], newHNode );
    return;
}

void insertDataIntoMap( HTable *hashTable, void *data ) {
    insertData( hashTable, hashTable->hashFunction( hashTable->size, data ), data );
}

void removeData( HTable *hashTable, int key, void *toRemove ) {
    List *workingList = hashTable->table[key];
    HNode *temp = (HNode*)workingList->head;

    while (temp != NULL) {
        if (workingList->compare( toRemove, temp->data ) == 1) {
            temp = temp->next;
        } else {
            if (workingList->compare( toRemove, temp->data ) == 0) {
                printf( "Removing data\n" );
                deleteDataFromList( workingList, temp );
                return;
            } else {
                printf( "That value does not exist.\n" );
                return;
            }
        }
    }
    printf( "That value does not exist.\n" );
    return;
}

void *lookupData( HTable *hashTable, int key, void *toFind ) {
    List *workingList = hashTable->table[key];
    HNode *temp = (HNode*)(workingList->head);

    while (temp != NULL) {
        if (workingList->compare( toFind, temp->data ) == 1) {
            temp = temp->next;
        } else {
            if (workingList->compare( toFind, temp->data ) == 0) {
                return temp->data;
            } else {
                return NULL;
            }
        }
    }
    return NULL;
}

/************************ END OF HASH TABLE FUNCTIONS *************************/
