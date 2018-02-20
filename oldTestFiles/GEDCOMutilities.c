/**
 * @file GEDCOMutilities.c
 * @author Jackson Firth 0880887
 * @version CIS2750 A1
 * @date February 2nd 2018
 *
 * @brief This file contains the implementation of the GEDCOMutilities module
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"
#include "LinkedListAPI.h"

/************************** Internal Functions ********************************/

/**
 * Comparison function for reference pairs
 *
 * @param first The first pair structure
 * @param second The second pair structure
 * @return 1 if first > second, -1 first < second, 0 if they are equal
 */
int compareReferencePairs( const void* first, const void* second );

/**
 * Find an individual in the reference structure given a GEDCOM external reference string in
 * the format "@I00x@"
 *
 * @param input The external reference string to find
 * @return The individual pointer that corresponds to the given reference or NULL on failure
 */
Individual* findReference( char* input );

/**
 * Returns a CharSet variable from the given string to use for assignment in the Header
 * @param string The string representation of a CharSet value
 * @return CharSet value able to be assigned
 */
CharSet getCharSetFromString( char* string );

/**
 * Converts a CharSet value into a string to be used for printing
 *
 * @param arg The CharSet to convert
 * @return string representation of the CharSet value, or NULL on failure
 */
char* getStringFromCharSet( CharSet arg );

/**
 * Determines if a given tag is a family event tag
 *
 * @param tag The tag to test
 * @return true if the tag is a family event, false otherwise
 */
bool isFamEvent( char* tag );

/**
 * Determines if a given tag is an individual event tag
 *
 * @param tag The tag to test
 * @return true if the tag is an individual event, false otherwise
 */
bool isIndividualEvent( char* tag );


/**
 * Function to parse a GEDCOMline from a given string
 *
 * @param line The string to be parsed
 * @return GEDCOMline pointer to the properly allocated line
 */
GEDCOMline* parseGEDCOMline( char* line );


/**
 * Removes the newline character from the given string
 *
 * @param line The string to remove the newline from
 */
void removeHardReturn( char *line );


/************************ End of Internal Functions ***************************/

bool addEventToRecord( void* record, Event* toAdd, RecordType type ) {
    if (record == NULL || toAdd == NULL) {
        return false;
    }
    Individual* indiv;
    Family* fam;
    switch (type) {
        case INDI:
            indiv = ((Individual*)record);
            insertSorted( &(indiv->events), (void*)toAdd );
            break;
        case FAM:
            fam = ((Family*)record);
            insertSorted( &(fam->events), (void*)toAdd );
            break;
        default:
            break;
    }
    return true;
}

bool addFieldToRecord( void* record, Field* toAdd, RecordType type ) {
    if (record == NULL || toAdd == NULL) {
        return NULL;
    }

    Family* fam;
    Individual* indi;
    Event* event;
    Header* head;
    Submitter* sub;
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
        case HEAD:
            head = ((Header*)record);
            insertSorted( &(head->otherFields), (void*)toAdd );
            break;
        case SUBM:
            sub = ((Submitter*)record);
            insertSorted( &(sub->otherFields), (void*)toAdd );
            break;
        default:
            return false;
    }
    return true;
}

bool addToFamily( Family* fam, Individual* indi, RelationType relation ) {
    if (fam == NULL || indi == NULL) {
        return false;
    }

    switch (relation) {
        case HUSB:
            fam->husband = indi;
            break;
        case WIFE:
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

bool compareFunc( const void* first, const void* second ) {
    if (first == NULL || second == NULL) {
        return false;
    }
    Individual* one = (Individual*)first;
    Individual* two = (Individual*)second;

    if (one->givenName != NULL && two->givenName != NULL) {
        if (strcmp( one->givenName, two->givenName ) == 0) {
            if (one->surname != NULL && two->surname != NULL) {
                if (strcmp( one->surname, two->surname ) == 0) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

GEDCOMerror createError( ErrorCode type, int line ) {
    GEDCOMerror err;
    err.type = type;
    err.line = line;
    return err;
}

Event* createEvent( GEDCOMline** record, int count ) {
    Event* event = malloc( sizeof( Event ) );
    event->date = NULL;
    event->place = NULL;
    event->otherFields = initializeList( &printField, &deleteField, &compareFields );

    if (strlen( record[0]->tag ) > 4) {
        clearList( &(event->otherFields) );
        free( event );
    }
    strncpy( event->type, record[0]->tag, 4 );
    event->type[4] = '\0';

    for (int i = 1; i < count; i++) {
        removeHardReturn( record[i]->lineValue );
        if (strcmp( record[i]->tag, "DATE" ) == 0) {
            event->date = malloc( sizeof( char ) * (strlen( record[i]->lineValue ) + 1) );
            strcpy( event->date, record[i]->lineValue );
        } else if (strcmp( record[i]->tag, "PLAC" ) == 0) {
            event->place = malloc( sizeof( char ) * (strlen( record[i]->lineValue ) + 1) );
            strcpy( event->place, record[i]->lineValue );
        } else {
            addFieldToRecord( (void*)event, createField( record[i]->tag, record[i]->lineValue ), EVENT );
        }
    }
    return event;
}

Family* createFamily( GEDCOMline** record, int count ) {
    if (record == NULL) {
        return NULL;
    }

    Family* fam = calloc( sizeof( Family ), 1 );
    fam->husband = NULL;
    fam->wife = NULL;
    fam->children = initializeList( &printIndividual, &dummyDelete, &compareIndividuals );
    fam->events = initializeList( &printEvent, &deleteEvent, &compareEvents );
    fam->otherFields = initializeList( &printField, &deleteField, &compareFields );

    for (int i = 1; i < count; i++) {
        if (isFamEvent( record[i]->tag )) {
            int j = i;
            int subCount = 1;

            // Grab the lines pertaining to this event and place in subarray
            while (j < count - 1) {
                if (record[j+1]->level != 1) {
                    subCount++;
                } else {
                    break;
                }
                j++;
            }
            GEDCOMline** subarray = calloc( sizeof( GEDCOMline* ), subCount );
            j = i;
            for (int k = 0; k < subCount; k++) {
                subarray[k] = record[j + k];
            }
            Event* temp = createEvent( subarray, subCount );
            if (temp == NULL) {
                free( subarray );
                free( fam );
                clearList( &(fam->events) );
                clearList( &(fam->children) );
                clearList( &(fam->otherFields) );
                return NULL;
            }
            addEventToRecord( (void*)fam, temp, FAM );
            free( subarray );
        } else {
            if (strcmp( record[i]->tag, "HUSB" ) == 0) {
                Individual* temp = findReference( record[i]->lineValue );
                if (temp != NULL) {
                    addToFamily( fam, temp, HUSB );
                }
            } else if (strcmp( record[i]->tag, "WIFE" ) == 0) {
                Individual* temp = findReference( record[i]->lineValue );
                if (temp != NULL) {
                    addToFamily( fam, temp, WIFE );
                }
            } else if (strcmp( record[i]->tag, "CHIL" ) == 0) {
                Individual* temp = findReference( record[i]->lineValue );
                if (temp != NULL) {
                    addToFamily( fam, temp, CHIL );
                }
            } else {
                addFieldToRecord( (void*)fam, createField( record[i]->tag, record[i]->lineValue ), FAM );
            }
        }
    }
    return fam;
}

Field* createField( char* tag, char* value ) {
    if (tag == NULL) {
        return NULL;
    }
    if (strlen( tag ) > 31 || (value != NULL && strlen( value ) > 120)) {
        return NULL;
    }

    Field* field = malloc( sizeof( Field ) );
    field->tag = malloc( sizeof( char ) * (strlen( tag ) + 1) );
    strcpy( field->tag, tag );
    if (value == NULL) {
        field->value = NULL;
    } else {
        field->value = malloc( sizeof( char ) * (strlen( value ) + 1) );
        strcpy( field->value, value );
    }
    return field;
}

GEDCOMline* createGEDCOMline( char* input ) {
    if (input == NULL) {
        return NULL;
    }
    GEDCOMline* line = parseGEDCOMline( input );
    return line;
}

Header* createHeader( GEDCOMline** record, int count ) {
    if (record == NULL) {
        return NULL;
    }
    if (record[0]->level != 0) {
        return NULL;
    }
    bool sourceFound, GEDCfound, submitFound, charSetFound;
    sourceFound = GEDCfound = submitFound = charSetFound = false;

    Header* head = malloc( sizeof( Header ) );
    head->otherFields = initializeList( &printField, &deleteField, &compareFields );
    int i;
    for (i = 1; i < count; i++) {
        if (isValidHeadTag( record[i]->tag )) {
            for (int j = 0; j < strlen( record[j]->tag ); j++) {
                record[i]->tag[j] = toupper( record[i]->tag[j] );
            }
            if (strcmp( record[i]->tag, "SOUR" ) == 0) {
                strncpy( head->source, record[i]->lineValue, 248 );
                head->source[strlen( head->source )] = '\0';
                sourceFound = true;
            } else if (strcmp( record[i]->tag, "GEDC" ) == 0) {
                if (strcmp( record[i + 1]->tag, "VERS" ) != 0) {
                    clearList( &head->otherFields );
                    free( head );
                    return NULL;
                } else {
                    head->gedcVersion = atof( record[i + 1]->lineValue );
                    i++;
                    GEDCfound = true;
                }
            } else if (strcmp( record[i]->tag, "CHAR" ) == 0) {
                if (record[i]->lineValue == NULL) {
                    clearList( &head->otherFields );
                    free( head );
                    return NULL;
                } else {
                    head->encoding = getCharSetFromString( record[i]->lineValue );
                    charSetFound = true;
                }
            } else if (strcmp( record[i]->tag, "SUBM" ) == 0) {
                // set submitter pointer
                submitFound = true;
            } else {
                addFieldToRecord( (void*)head, createField( record[i]->tag, record[i]->lineValue ), HEAD );
            }
        } else {
            clearList( &head->otherFields );
            free( head );
            return NULL;
        }
    }
    if (sourceFound && GEDCfound && submitFound && charSetFound) {
        return head;
    } else {
        // free everything and return NULL
        clearList( &(head->otherFields) );
        free( head );
        return NULL;
    }
}

Individual* createIndividual( GEDCOMline** record, int count ) {
    if (record == NULL || count < 0) {
        return NULL;
    }

    Individual* indi = malloc( sizeof( Individual ) );
    indi->givenName = NULL;
    indi->surname = NULL;
    indi->otherFields = initializeList( &printField, &deleteField, &compareFields );
    indi->events = initializeList( &printEvent, &deleteEvent, &compareEvents );
    indi->families = initializeList( &printFamily, &dummyDelete, &compareFamilies );

    for (int i = 1; i < count; i++) {
        if (isIndividualEvent( record[i]->tag )) {
            int j = i;
            int subCount = 1;

            while (j < count - 1) {
                if (record[j+1]->level != 1) {
                    subCount++;
                } else {
                    break;
                }
                j++;
            }
            GEDCOMline** subarray = malloc( sizeof( GEDCOMline* ) * subCount );
            for (int k = 0; k < subCount; k++) {
                subarray[k] = NULL;
            }
            j = i;
            for (int k = 0; k < subCount; k++) {
                subarray[k] = record[j + k];
            }
            Event* temp = createEvent( subarray, subCount );
            if (temp == NULL) {
                free( subarray );
                clearList( &(indi->events) );
                clearList( &(indi->otherFields) );
                free( indi );
                return NULL;
            }
            addEventToRecord( (void*)indi, temp, INDI );
            free( subarray );
        } else {
            if (strcmp( record[i]->tag, "NAME" ) == 0) {
                char* token = strtok( record[i]->lineValue, "/" );
                if (token == NULL) {
                    indi->givenName = malloc( sizeof( char ) * (strlen( "" ) + 1) );
                    strcpy( indi->givenName, "" );
                } else {
                    indi->givenName = malloc( sizeof( char ) * (strlen( token ) + 1) );
                    if (token[strlen( token ) - 1] == ' ') {
                        token[strlen( token ) - 1] = '\0';
                    }
                    strcpy( indi->givenName, token );
                }
                token = strtok( NULL, "/" );
                if (token == NULL) {
                    indi->surname = malloc( sizeof( char ) * (strlen( "" ) + 1) );
                    strcpy( indi->surname, "" );
                } else {
                    indi->surname = malloc( sizeof( char ) * (strlen( token ) + 1) );
                    strcpy( indi->surname, token );
                }
            } else {
                addFieldToRecord( (void*)indi, createField( record[i]->tag, record[i]->lineValue ), INDI );
            }
        }
    }
    return indi;
}

Submitter* createSubmitter( GEDCOMline** record, int count ) {
    if (record == NULL || count < 0) {
        return NULL;
    }
    Submitter* sub = malloc( sizeof( Submitter ) + (sizeof( char ) * 100) );
    sub->address[0] = '\0';
    sub->otherFields = initializeList( &printField, &deleteField, &compareFields );
    bool nameFound = false;
    for (int i = 0; i < count; i++) {
        if (strcmp( record[i]->tag, "ADDR" ) == 0) {
            strcpy( sub->address, record[i]->lineValue );
        } else if (strcmp( record[i]->tag, "NAME" ) == 0) {
            strncpy( sub->submitterName, record[i]->lineValue, 60 );
            sub->submitterName[strlen( record[i]->lineValue )] = '\0';
            nameFound = true;
        } else {
            addFieldToRecord( (void*)sub, createField( record[i]->tag, record[i]->lineValue ), SUBM );
        }
    }
    if (nameFound == false) {
        // free and return INV_RECORD
        deleteSubmitter( sub );
        return NULL;
    }
    return sub;
}

int compareReferencePairs( const void* first, const void* second ) {
    if (first == NULL || second == NULL) {
        return OTHER_ERROR;
    }

    RefPair* pair1 = ((RefPair*)first);
    RefPair* pair2 = ((RefPair*)second);

    if (strcmp( pair1->extRefID, pair2->extRefID ) > 1) {
        return 1;
    } else if (strcmp( pair1->extRefID, pair2->extRefID ) < 1) {
        return -1;
    } else {
        return 0;
    }
}

char* convertDate( char* toConvert ) {
    if (toConvert == NULL) {
        return NULL;
    }

    char* copy = malloc( sizeof( char ) * (strlen( toConvert ) + 1) );
    strcpy( copy, toConvert );
    char* token = strtok( copy, " " );
    if (!isdigit( token[0] )) {
        token = strtok( NULL, " " );
    } else {
        if (strlen( token ) > 2) {
            char* string = malloc( sizeof( char ) * (strlen( "unknown date" ) + 1) );
            strcpy( string, "unknown date" );
            free( copy );
            return string;
        }
    }
    char* toReturn = malloc( sizeof( char ) * (strlen( token ) + 1) );
    strcpy( toReturn, token );
    token = strtok( NULL, " " );
    if (token == NULL) {
        char* string = malloc( sizeof( char ) * (strlen( "unknown date" ) + 1) );
        strcpy( string, "unknown date" );
        free( copy );
        free( toReturn );
        return string;
    }
    for (int i = 0; i < strlen( token ); i++) {
        token[i] = toupper( token[i] );
    }

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
    }

    toReturn = realloc( toReturn, sizeof( char ) * (strlen( toReturn ) + 5) );
    token = strtok( NULL, "\n\r" );
    if (token == NULL) {
        char* string = malloc( sizeof( char ) * (strlen( "unknown date" ) + 1) );
        strcpy( string, "unknown date" );
        free( copy );
        free( toReturn );
        return string;
    }
    strcat( toReturn, token );
    free( copy );
    return toReturn;
}

void deleteGEDCOMline( GEDCOMline* line ) {
    if (line == NULL) {
        return;
    }

    free( line->tag );
    if (line->lineValue != NULL) {
        free( line->lineValue );
    }
    if (line->extRefID != NULL) {
        free( line->extRefID );
    }
    free( line );
}

void deleteHeader( Header* head ) {
    if (head == NULL) {
        return;
    }
    clearList( &(head->otherFields) );
    free( head );
}

void deleteSubmitter( Submitter* sub ) {
    if (sub == NULL) {
        return;
    }
    clearList( &(sub->otherFields) );
    free( sub );
}

void dummyDelete( void* arg ) {
    return;
}

int familyMemberCount( const void* family ) {
    if (family == NULL){
        return -1;
    }

    Family fam = *((Family*)family);
    int members = 0;
    if (fam.husband != NULL) {
        members++;
    }
    if (fam.wife != NULL) {
        members++;
    }
    members += getLength( fam.children );

    return members;
}

Individual* findReference( char* input ) {
    if (input == NULL) {
        return NULL;
    }

    for (int i = 0; i < refCount; i++) {
        if (strcmp( referenceArray[i]->extRefID, input ) == 0) {
            return referenceArray[i]->indi;
        }
    }
    return NULL;
}

CharSet getCharSetFromString( char* string ) {
    if (strcmp( string, "ANSEL") == 0) {
        return ANSEL;
    } else if (strcmp( string, "UTF-8") == 0) {
        return UTF8;
    } else if (strcmp( string, "UNICODE") == 0) {
        return UNICODE;
    } else if (strcmp( string, "ASCII") == 0) {
        return ASCII;
    } else {
        return -1;
    }
}

char* getStringFromCharSet( CharSet arg ) {
    char* string = calloc( sizeof( char ), 8 ); // 'UNICODE' is 7 chars long so alloc 1 extra
    switch (arg) {
        case ANSEL:
            strcpy( string, "ANSEL" );
            break;
        case UTF8:
            strcpy( string, "UTF8" );
            break;
        case UNICODE:
            strcpy( string, "UNICODE" );
            break;
        case ASCII:
            strcpy( string, "ASCII" );
            break;
        default:
            break;
    }
    if (strlen( string ) == 0) {
        return NULL;
    } else {
        return string;
    }
}

bool isValidHeadTag( char* tag ) {
    if (tag == NULL) {
        return false;
    } else {
        for (int i = 0; i < strlen( tag ); i++) {
            tag[i] = toupper( tag[i] );
        }
    }
    if (strcmp( tag, "SOUR" ) == 0) {
        return true;
    } else if (strcmp( tag, "GEDC" ) == 0) {
        return true;
    } else if (strcmp( tag, "CHAR" ) == 0) {
        return true;
    } else if (strcmp( tag, "SUBM" ) == 0) {
        return true;
    } else if (strcmp( tag, "DATE" ) == 0) {
        return true;
    } else if (strcmp( tag, "SUBN" ) == 0) {
        return true;
    } else if (strcmp( tag, "FILE" ) == 0) {
        return true;
    } else if (strcmp( tag, "COPR" ) == 0) {
        return true;
    } else if (strcmp( tag, "LANG" ) == 0) {
        return true;
    } else if (strcmp( tag, "PLAC" ) == 0) {
        return true;
    } else if (strcmp( tag, "NOTE" ) == 0) {
        return true;
    } else if (strcmp( tag, "CONC" ) == 0) {
        return true;
    } else if (strcmp( tag, "VERS" ) == 0) {
        return true;
    } else if (strcmp( tag, "NAME" ) == 0) {
        return true;
    } else if (strcmp( tag, "CORP" ) == 0) {
        return true;
    } else if (strcmp( tag, "ADDR" ) == 0) {
        return true;
    } else if (strcmp( tag, "DATA" ) == 0) {
        return true;
    } else if (strcmp( tag, "DEST" ) == 0) {
        return true;
    } else if (strcmp( tag, "TIME" ) == 0) {
        return true;
    } else if (strcmp( tag, "FORM" ) == 0) {
        return true;
    } else if (strcmp( tag, "DATE" ) == 0) {
        return true;
    } else {
        return false;
    }
}

bool isIndividualEvent( char* tag ) {
    if (strcmp( tag, "BIRT" ) == 0) {
        return true;
    } else if (strcmp( tag, "CHR" ) == 0) {
        return true;
    } else if (strcmp( tag, "DEAT" ) == 0) {
        return true;
    } else if (strcmp( tag, "BURI" ) == 0) {
        return true;
    } else if (strcmp( tag, "CREM" ) == 0) {
        return true;
    } else if (strcmp( tag, "ADOP" ) == 0) {
        return true;
    } else if (strcmp( tag, "BAPM" ) == 0) {
        return true;
    } else if (strcmp( tag, "BARM" ) == 0) {
        return true;
    } else if (strcmp( tag, "BASM" ) == 0) {
        return true;
    } else if (strcmp( tag, "BLES" ) == 0) {
        return true;
    } else if (strcmp( tag, "CHRA" ) == 0) {
        return true;
    } else if (strcmp( tag, "CONF" ) == 0) {
        return true;
    } else if (strcmp( tag, "FCOM" ) == 0) {
        return true;
    } else if (strcmp( tag, "ORDN" ) == 0) {
        return true;
    } else if (strcmp( tag, "NATU" ) == 0) {
        return true;
    } else if (strcmp( tag, "EMIG" ) == 0) {
        return true;
    } else if (strcmp( tag, "IMMI" ) == 0) {
        return true;
    } else if (strcmp( tag, "CENS" ) == 0) {
        return true;
    } else if (strcmp( tag, "PROB" ) == 0) {
        return true;
    } else if (strcmp( tag, "WILL" ) == 0) {
        return true;
    } else if (strcmp( tag, "GRAD" ) == 0) {
        return true;
    } else if (strcmp( tag, "RETI" ) == 0) {
        return true;
    } else if (strcmp( tag, "EVEN" ) == 0) {
        return true;
    } else {
        return false;
    }
}

bool isFamEvent( char* tag ) {
    if (strcmp( tag, "ANUL" ) == 0) {
        return true;
    } else if (strcmp( tag, "CENS" ) == 0) {
        return true;
    } else if (strcmp( tag, "DIV" ) == 0) {
        return true;
    } else if (strcmp( tag, "DIVF" ) == 0) {
        return true;
    } else if (strcmp( tag, "ENGA" ) == 0) {
        return true;
    } else if (strcmp( tag, "MARB" ) == 0) {
        return true;
    } else if (strcmp( tag, "MARC" ) == 0) {
        return true;
    } else if (strcmp( tag, "MARR" ) == 0) {
        return true;
    } else if (strcmp( tag, "MARL" ) == 0) {
        return true;
    } else if (strcmp( tag, "MARS" ) == 0) {
        return true;
    } else if (strcmp( tag, "RESI" ) == 0) {
        return true;
    } else if (strcmp( tag, "EVEN" ) == 0) {
        return true;
    } else {
        return false;
    }
}

bool isParent( Family* fam, const Individual* person ) {
    if (fam == NULL || person == NULL) {
        return false;
    }
    if (fam->husband != NULL) {
        if (compareIndividuals( fam->husband, person ) == 0) {
            return true;
        }
    }
    if (fam->wife != NULL) {
        if (compareIndividuals( fam->wife, person ) == 0) {
            return true;
        }
    }
    return false;
}

bool modifyGEDCOMline( GEDCOMline* line, char* modValue ) {
    if (line == NULL || modValue == NULL) {
        return false;
    }
    line->lineValue = realloc( line->lineValue, sizeof( char ) * (strlen( line->lineValue ) + strlen( modValue ) + 1) );
    strcat( line->lineValue, modValue );
    return true;
}

GEDCOMline* parseGEDCOMline( char* line ) {
    if (line == NULL || strlen( line ) > 255) {
        return NULL;
    }

    char* temp = strtok( line, " " );
    if (temp == NULL || strlen( temp ) > 2 || (strlen( temp ) == 2 && temp[0] == '0')) {
        return NULL;
    }
    GEDCOMline* toReturn = malloc( sizeof( GEDCOMline ) );
    toReturn->level = atoi( temp );
    if (toReturn->level > 99 || toReturn->level < 0) {
        free( toReturn );
        return NULL;
    }
    temp = strtok( NULL, " \n\r" );
    if (temp[0] == '@') {
        toReturn->extRefID = malloc( sizeof( char ) * (strlen( temp ) + 1) );
        strcpy( toReturn->extRefID, temp );
        temp = strtok( NULL, " \n\r" );
    } else {
        toReturn->extRefID = NULL;
    }
    if (strlen( temp ) > 31) { // max tag length
        free( toReturn );
        return NULL;
    }
    toReturn->tag = malloc( sizeof( char ) * (strlen( temp ) + 1) );
    strcpy( toReturn->tag, temp );
    for (int i = 0; i < strlen( toReturn->tag ); i++) { // convert to upper case
        toReturn->tag[i] = toupper( toReturn->tag[i] );
    }
    if (toReturn->tag[strlen( toReturn->tag ) - 1] == '\n' || toReturn->tag[strlen( toReturn->tag ) - 1] == '\r') {
        toReturn->tag[strlen( toReturn->tag ) - 1] = '\0';
    }
    if (toReturn->tag[strlen( toReturn->tag ) - 1] == '\n' || toReturn->tag[strlen( toReturn->tag ) - 1] == '\r') {
        toReturn->tag[strlen( toReturn->tag ) - 1] = '\0';
    }
    temp = strtok( NULL, "\n\r" );
    if (temp != NULL) {
        toReturn->lineValue = malloc( sizeof( char ) * (strlen( temp ) + 1) );
        strcpy( toReturn->lineValue, temp );
        removeHardReturn( toReturn->lineValue );
    } else {
        toReturn->lineValue = NULL;
    }
    return toReturn;
}

char* printGEDCOMline( GEDCOMline* line ) {
    if (line == NULL) {
        return NULL;
    }
    char* string = calloc( sizeof( char ), 255 );
    sprintf( string, "%d ", line->level );
    string[2] = '\0';
    if (line->extRefID != NULL) {
        strcat( string, line->extRefID );
        strcat( string, " " );
    }
    strcat( string, line->tag );
    strcat( string, " " );
    if (line->lineValue != NULL) {
        strcat( string, line->lineValue );
    }
    removeHardReturn( string );
    return string;
}

char* printHeader( Header* head ) {
    if (head == NULL) {
        return NULL;
    }
    char* string = NULL;
    char* temp;
    string = calloc( sizeof( char ), (strlen( head->source ) + 4) );
    strcpy( string, head->source );
    strcat( string, " " );
    string = realloc( string, sizeof( char ) * (strlen( string ) + 15) );
    temp = calloc( sizeof( char ), 11 );
    strcat( string, "GEDC: " );
    sprintf( temp, "%.2f", head->gedcVersion );
    strcat( string, temp );
    strcat( string, " " );
    free( temp );
    temp = getStringFromCharSet( head->encoding );
    string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( temp ) + 3) );
    strcat( string, temp );
    strcat( string, " " );
    free( temp );

    return string;
}

char* printIndividualNames( void* toBePrinted ) {
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
    return string;
}

char* printSubmitter( Submitter* sub ) {
    if (sub == NULL) {
        return NULL;
    }
    char* string = NULL;
    string = malloc( sizeof( char ) * (strlen( sub->submitterName ) + 3) );
    strcpy( string, sub->submitterName );
    strcat( string, "\n" );
    if ((strlen( sub->address ) > 0)) {
        string = realloc( string, sizeof( char ) * (strlen( string ) + strlen( sub->address ) + 2) );
        strcat( string, sub->address );
    }
    return string;
}

void removeHardReturn( char *line ) {
    line = strtok( line, "\r\n" );
}

void recursiveGetDescendants( List* list, const Individual* person ) {
    Node* node1, *node2;
    Family* fam;

    node1 = person->families.head;

    while (node1 != NULL) {
        fam = node1->data;
        if (fam->husband == person || fam->wife == person) {
            node2 = fam->children.head;
            while (node2 != NULL) {
                insertSorted( list, node2->data );
                recursiveGetDescendants( list, node2->data );
                node2 = node2->next;
            }
        }
        node1 = node1->next;
    }
}

char* robust_fgets( char* dest, int max, FILE* stream ) {
    int tempChar;
    char* str = NULL;

    for (str = dest, max--; max > 0; max--) {
        if ((tempChar = fgetc( stream )) == EOF) {
            break;
        }
        *(str++) = tempChar;
        if (tempChar == '\n' || tempChar == '\r') {
            int temp;
            if ((temp = fgetc( stream )) == EOF) {
                break;
            }
            if (temp == '\n' && tempChar == '\r') {
                *(--str) = temp;
                str++;
            } else if (temp == '\r' && tempChar == '\n') {
                // discard temp
            } else {
                ungetc( temp, stream );
            }
            break;
        }
    }
    *str = 0;
    if (str == dest || tempChar == EOF) {
        return NULL;
    }
    return str;
}
