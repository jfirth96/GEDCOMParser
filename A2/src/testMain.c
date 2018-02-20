#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"

int main( int argc, char** argv ) {

    char *filename = malloc( sizeof( char ) * (strlen( "input/testFile.ged" ) + 1) );
    strcpy( filename, "input/testFile.ged" );
    GEDCOMobject *obj = NULL;
    GEDCOMerror error = createGEDCOM( filename, &obj );
    free( filename );
    if (obj != NULL) {
        // Print out for debugging
        char* print = printGEDCOM( obj );
        printf( "%s", print );
        free( print );

        // Write GEDCOM object
        GEDCOMerror writeError = writeGEDCOM( "input/sampleOut2.ged", obj );
        print = printError( writeError );
        printf( "%s\n", print );
        free( print );

        // Validate GEDCOM object
        printf( "This should validate\n" );
        ErrorCode code = validateGEDCOM( obj );
        writeError = createError( code, -1 );
        print = printError( writeError );
        printf( "%s\n", print );
        free( print );

        // Try getDescendantsListN
        List descendants = getDescendantListN( obj, obj->individuals.head->data, 2 );
        print = toString( descendants );
        printf( "-- getDescendantListN --\n%s\n\n", print );
        free( print );
        Node* temp = descendants.head;
        for (int i = 0; i < getLength( descendants ); i++) {
            clearList( (List*)(temp->data) );
            temp = temp->next;
        }
        clearList( &descendants );

        // Try getAncestorListN
        List ancestors = getAncestorListN( obj, obj->individuals.tail->data, 1 );
        print = toString( ancestors );
        printf( "-- getAncestorListN --\n%s\n\n", print );
        free( print );
        Node* temp1 = ancestors.head;
        for (int i = 0; i < getLength( ancestors ); i++) {
            clearList( (List*)(temp1->data) );
            temp1 = temp1->next;
        }
        clearList( &ancestors );

        // Test Validation
        deleteIndividual( obj->individuals.head->data );
        obj->individuals.head->data = NULL;
        printf( "This should not validate\n" );
        code = validateGEDCOM( obj );
        writeError = createError( code, -1 );
        print = printError( writeError );
        printf( "%s\n", print );
        free( print );

        deleteGEDCOM( obj );
    } else {
        char* print = printError( error );
        printf( "%s", print );
        free( print );
    }
    return 0;
}
