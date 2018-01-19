/**
 * @file GEDCOMutilities.c
 * @author Jackson Firth 0880887
 * @version CIS2750 A1
 * @date January 2018
 *
 * @brief This file contains the implementation details of the utility functions needed for our
 *        GEDCOM parsing library.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "GEDCOMutilities.h"
#include "GEDCOMparser.h"

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
