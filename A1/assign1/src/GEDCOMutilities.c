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

/************************** Internal Functions ********************************/

/**
 * Function to parse a GEDCOMline from a given string
 *
 * @param line The string to be parsed
 * @return GEDCOMline pointer to the properly allocated line
 */
GEDCOMline* parseGEDCOMline( char* line );

/**
 * Used to implement strong and weak references
 */
void dummyDelete( void* arg );


/**
 *
 */
CharSet getCharSetFromString( char* string );

/**
 *
 */
char* getStringFromCharSet( CharSet arg );

/**
 *
 */
bool isFamEvent( char* tag );

/**
 *
 */
bool isIndividualEvent( char* tag );

/**
 * Removes the newline character from the given string
 *
 * @param line The string to remove the newline from
 */
void removeHardReturn( char *line );

/************************ End of Internal Functions ***************************/

bool addEventToRecord( void* record, Event* toAdd, RecordType type ) {
    if (record == NULL || toAdd == NULL)
        return false;

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
    if (record == NULL || toAdd == NULL)
        return NULL;

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
    event->otherFields = initializeList( &printEvent, &deleteEvent, &compareEvents );

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
    Family* fam = malloc( sizeof( Family ) );
    fam->husband = NULL;
    fam->wife = NULL;
    fam->children = initializeList( &printIndividual, &dummyDelete, &compareIndividuals );
    fam->events = initializeList( &printEvent, &deleteEvent, &compareEvents );
    fam->otherFields = initializeList( &printField, &deleteField, &compareFields );

    for (int i = 1; i < count; i++) {
        if (isFamEvent( record[i]->tag )) {
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
            addEventToRecord( (void*)fam, temp, FAM );
            free( subarray );
        } else {
            if (strcmp( record[i]->tag, "HUSB" ) == 0) {
                // form linkage
            } else if (strcmp( record[i]->tag, "WIFE" ) == 0) {
                // form linkage
            } else if (strcmp( record[i]->tag, "CHIL" ) == 0) {
                // add to family
            } else {
                addFieldToRecord( (void*)fam, createField( record[i]->tag, record[i]->lineValue ), FAM );
            }
        }
    }
    return fam;
}

Field* createField( char* tag, char* value ) {
    if (tag == NULL)
        return NULL;
    if (strlen( tag ) > 31 || (value != NULL && strlen( value ) > 120))
        return NULL;

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
    if (input == NULL)
        return NULL;
    GEDCOMline* line = parseGEDCOMline( input );
    return line;
}

Header* createHeader( GEDCOMline** record, int count ) {
    if (record == NULL)
        return NULL;
    bool sourceFound, GEDCfound, submitFound, charSetFound;
    sourceFound = GEDCfound = submitFound = charSetFound = false;

    Header* head = malloc( sizeof( Header ) );
    int i;
    for (i = 0; i < count; i++) {
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
                    /*clearList( &head->otherFields );
                    free( head );
                    return NULL;*/
                } else {
                    head->gedcVersion = atof( record[i + 1]->lineValue );
                    i++;
                    GEDCfound = true;
                }
            } else if (strcmp( record[i]->tag, "CHAR" ) == 0) {
                if (record[i]->lineValue == NULL) {
                    /*clearList( &head->otherFields );
                    free( head );
                    return NULL;*/
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
            /*clearList( &head->otherFields );
            free( head );
            return NULL;*/
        }
    }
    if (sourceFound == false || GEDCfound == false || submitFound == false || charSetFound == false) {
        // free everything and return NULL
        /*clearList( &head->otherFields );
        free( head );
        return NULL;*/
    }

    head->otherFields = initializeList( &printField, &deleteField, &compareFields );
    return head;
}

Individual* createIndividual( GEDCOMline** record, int count ) {
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
            addEventToRecord( (void*)indi, temp, INDI );
            free( subarray );
        } else {
            if (strcmp( record[i]->tag, "NAME" ) == 0) {
                char* token = strtok( record[i]->lineValue, "/" );
                indi->givenName = malloc( sizeof( char ) * (strlen( token ) + 1) );
                strcpy( indi->givenName, token );
                token = strtok( NULL, "/" );
                indi->surname = malloc( sizeof( char ) * (strlen( token ) + 1) );
                strcpy( indi->surname, token );
            } else {
                addFieldToRecord( (void*)indi, createField( record[i]->tag, record[i]->lineValue ), INDI );
            }
        }
    }
    return indi;
}

Submitter* createSubmitter( GEDCOMline** record, int count ) {
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
    }
    return sub;
}

char* convertDate( char* toConvert ) {
    if (toConvert == NULL) return NULL;

    char* copy = malloc( sizeof( char ) * (strlen( toConvert ) + 1) );
    strcpy( copy, toConvert );
    char* token = strtok( copy, " " );
    char* toReturn = malloc( sizeof( char ) * (strlen( token ) + 1) );
    strcpy( toReturn, token );
    token = strtok( NULL, " " );
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
    } else {
    }

    toReturn = realloc( toReturn, sizeof( char ) * (strlen( toReturn ) + 5) );
    token = strtok( NULL, "\n\r" );
    strcat( toReturn, token );

    free( copy );
    return toReturn;
}

void deleteGEDCOMline( GEDCOMline* line ) {
    if (line == NULL)
        return;

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
    if (head == NULL)
        return;

    clearList( &(head->otherFields) );
    free( head );
}

void deleteSubmitter( Submitter* sub ) {
    if (sub == NULL)
        return;

    clearList( &(sub->otherFields) );
    free( sub );
}

void dummyDelete( void* arg ) {
    return;
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

CharSet getCharSetFromString( char* string ) {
    if (strcmp( string, "ANSEL") == 0) {
        return ANSEL;
    } else if (strcmp( string, "UTF8") == 0) {
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
    char* string = malloc( sizeof( char ) * 8 ); // 'UNICODE' is 7 chars long so alloc 1 extra
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

bool modifyGEDCOMline( GEDCOMline* line, char* modValue ) {
    if (line == NULL || modValue == NULL)
        return false;

    line->lineValue = realloc( line->lineValue, sizeof( char ) * (strlen( line->lineValue ) + strlen( modValue ) + 1) );
    strcat( line->lineValue, modValue );
    return true;
}

GEDCOMline* parseGEDCOMline( char* line ) {
    if (line == NULL || strlen( line ) > 255)
        return NULL;

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

char* printHeader( Header* head ) {
    if (head == NULL)
        return NULL;
    char* string = NULL;
    char* temp;
    string = malloc( sizeof( char ) * (strlen( head->source ) + 3) );
    strcpy( string, head->source );
    strcat( string, " " );
    string = realloc( string, sizeof( char ) * (strlen( string ) + 15) );
    temp = calloc( sizeof( char ), 11 );
    strcat( string, "GEDC: " );
    sprintf( temp, "%.2f", head->gedcVersion );
    //temp[strlen( temp )] = '\0';
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

char* printGEDCOMline( GEDCOMline* line ) {
    if (line == NULL)
        return NULL;

    char* string = malloc( sizeof( char ) * 255 );
    string[0] = '\0';
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

char* printSubmitter( Submitter* sub ) {
    if (sub == NULL)
        return NULL;

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
