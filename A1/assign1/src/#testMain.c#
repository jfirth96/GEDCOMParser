#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"
#include "LinkedListAPI.h"

int main( int argc, char** argv ) {
    char* print1, *print2;
    GEDCOMobject* obj = NULL;
    GEDCOMerror err1 = createGEDCOM( argv[1], &obj );
    print1 = printError( err1 );

    if (obj != NULL) {
        Individual* test = calloc( sizeof( Individual ), 1 );
        test->givenName = calloc( sizeof( char ), (strlen( "John" ) + 1) );
        strcpy( test->givenName, "John" );
        test->surname = calloc( sizeof( char ), (strlen( "Shakespeare" ) + 1) );
        strcpy( test->surname, "Shakespeare" );
        Individual* temp = findPerson( obj, &compareFunc, (void*)test );
        if (temp != NULL) {
            printf( "Found: %s %s\n", temp->givenName, temp->surname );
        } else {
            printf( "Could not find person\n" );
        }
        List descendants = getDescendants( obj, temp );
        ListIterator iter = createIterator( descendants );
        void* elem;
        while ((elem = nextElement( &iter )) != NULL) {
            char* temp = printIndividualNames( elem );
            printf( "%s\n", temp );
            free( temp );
        }
        clearList( &descendants );
        free( test->givenName );
        free( test->surname );
        free( test );
    }


    if (obj != NULL) {
        print2 = printGEDCOM( obj );
        printf( "%s", print2 );
        printf( "%s\n", print1 );
        free( print1 );
        free( print2 );
        deleteGEDCOM( obj );
    } else {
        printf( "%s\n", print1 );
        free( print1 );
    }
    return 0;
}
