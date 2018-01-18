#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

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

	printf( "testField: %s\n", printField( (void*)&testField ) );
	printf( "randField: %s\n", printField( (void*)&randField ) );
	printf( "Ordering: %d (test, rand)\n", compareFields( (void*)&testField, (void*)&randField ) );
	deleteField( (void*)&testField );
	deleteField( (void*)&randField );

	return 0;
}
