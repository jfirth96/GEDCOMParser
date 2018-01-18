#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GEDCOMparser.h"

//*********************
void deleteField(void* toBeDeleted) {
    if (toBeDeleted == NULL) return;

    Field temp = (Field)toBeDeleted;
    if (temp.tag != NULL) {
        free( temp.tag );
        temp.tag = NULL;
    }
    if (temp.value != NULL) {
        free( temp.value );
        temp.value = NULL;
    }
    free( temp );
}

int compareFields(const void* first, const void* second) {
    Field one = (Field)first;
    Field two = (Field)second;

    if (one == NULL || two == NULL) {
        return OTHER;
    }
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
    Field temp = (Field)toBePrinted;

    if (temp == NULL) return NULL;
    if (temp.tag == NULL || temp.value == NULL) {
        return NULL;
    }

    char* stringOne = malloc( sizeof( char ) * (strlen( one.tag ) + strlen( one.value ) + 2) );

    strcpy( stringOne, one.tag );
    stringOne[strlen( stringOne ) - 1] = '>';
    strcat( stringOne, one.value );
    return stringOne;
}
//********************


int main( int argc, char** argv ) {
	Field testField;
	Field randField;
	testField.tag = malloc( sizeof( char ) * 32 );
	testField.value = malloc( sizeof( char ) * 50 );
	strcpy( testField.tag, "BIRT" );
	strcpy( testField.value, "12 NOV 1996" );
	randField.tag = malloc( sizeof( char ) * 32 );
	randField.value = malloc( sizeof( char ) * 50 );
	strcpy( randField.tag, "DEAT" );
	strcpy( randField.value, "13 June 2100" );

	printf( "testField: %s\n", printField( (void*)testField ) );
	printf( "randField: %s\n", printField( (void*)randField ) );
	printf( "Ordering: %d (test, rand)\n", compareFields( (void*)testField, (void*)randField ) );
	deleteField( testField );
	deleteField( randField );

	return 0;
}