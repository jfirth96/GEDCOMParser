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

GEDCOMline* createGEDCOMline( int level, void* reference, char* tag, char* value ) {
    if (level < 0 || tag == NULL)
        return NULL;

    GEDCOMline* line = malloc( sizeof( GEDCOMline ) );
    line->level = level;
    line->extRefID = reference;
    line->tag = malloc( sizeof( char ) * (strlen( tag ) + 1) );
    strcpy( line->tag, tag );
    if (value != NULL) {
        line->lineValue = malloc( sizeof( char ) * (strlen( value ) + 1) );
        strcpy( line->lineValue, value );
    } else {
        line->lineValue = NULL;
    }
    return line;
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
