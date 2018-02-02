#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"
#include "LinkedListAPI.h"

int main( int argc, char** argv ) {
    GEDCOMobject* obj = NULL;
    GEDCOMerror err1 = createGEDCOM( argv[1], &obj );

    Individual* test = calloc( sizeof( Individual ), 1 );
    test->givenName = calloc( sizeof( char ), (strlen( "William" ) + 1) );
    strcpy( test->givenName, "William" );
    test->surname = calloc( sizeof( char ), (strlen( "Shakespeare" ) + 1) );
    strcpy( test->surname, "Shakespeare" );
    Individual* temp = findPerson( obj, &compareFunc, (void*)test );
    if (temp != NULL) {
        printf( "Found: %s %s\n", temp->givenName, temp->surname );
    } else {
        printf( "Could not find person\n" );
    }
    free( test->givenName );
    free( test->surname );
    free( test );

    char* print1;
    if (obj != NULL) {
        print1 = printGEDCOM( obj );
        printf( "%s", print1 );
        free( print1 );
        deleteGEDCOM( obj );
    }
    print1 = printError( err1 );
    printf( "%s", print1 );
    free( print1 );
    return 0;
}
