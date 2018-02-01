#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"
#include "LinkedListAPI.h"

int main( int argc, char** argv ) {
    char* print1;

    GEDCOMobject* obj = malloc( sizeof( GEDCOMobject ) );
    print1 = malloc( sizeof( char ) * 11 );
    strcpy( print1, "output.ged" );
    printf( "args\n" );
    for (int i = 0; i < argc; i++) {
        printf( "%s\n", argv[i] );
    }
    GEDCOMerror err1 = createGEDCOM( argv[1], &obj );
    free( print1 );
    /*print1 = printHeader( obj->header );
    printf( "HEAD> %s\n", print1 );
    free( print1 );
    print1 = printError( err1 );
    printf( "ERR> %s\n", print1 );
    free( print1 );
    print1 = printSubmitter( obj->submitter );
    printf( "SUBM> %s\n", print1 );
    free( print1 );
    print1 = printIndividual( obj->individuals.head->data );
    printf( "INDIV>\n%s", print1 );
    free( print1 );*/
    print1 = printGEDCOM( obj );
    printf( "%s", print1 );
    free( print1 );
    deleteGEDCOM( obj );
    /*deleteHeader( obj->header );
    deleteSubmitter( obj->submitter );
    clearList( &(obj->individuals) );
    clearList( &(obj->families) );
    free( obj );*/

    return 0;
}
