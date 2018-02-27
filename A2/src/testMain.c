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
        //printf( "%s", print );
        free( print );

        // Write GEDCOM object
        GEDCOMerror writeError = writeGEDCOM( "input/sampleOut3.ged", obj );
        print = printError( writeError );
        printf( "WRITE: %s\n", print );
        free( print );

        // Validate GEDCOM object
        ErrorCode code = validateGEDCOM( obj );
        if (code != OK) {
            printf( "Validation test failed.\n" );
        } else {
            printf( "Valid GEDCOM object.\n" );
        }
        writeError = createError( code, -1 );
        print = printError( writeError );
        printf( "VALID: %s\n", print );
        free( print );

        // Try getDescendantsListN
        List descendants = getDescendantListN( obj, obj->individuals.head->data, 10 );
        print = toString( descendants );
        printf( "*********************************************\n" );
        printf( "-- getDescendantListN --\n%s\n----\n", print );
        free( print );
        printf( "*********************************************\n" );
        clearList( &descendants );

        // Try getAncestorListN
        List ancestors = getAncestorListN( obj, obj->individuals.head->data, 10 );
        print = toString( ancestors );
        printf( "*********************************************\n" );
        printf( "-- getAncestorListN --\n%s\n----\n", print );
        free( print );
        printf( "*********************************************\n" );

        // Test gListToJSON
        print = gListToJSON( ancestors );
        //printf( "-- GLISTTOJSON --\n%s\n----\n", print );
        free( print );
        clearList( &ancestors );

        // Test indToJSON
        print = indToJSON( obj->individuals.head->data );
        //printf( "-- INDtoJSON --\n%s\n----\n", print );

        // Test JSONtoInd
        Individual* indiv = JSONtoInd( print );
        free( print );
        print = printIndividual( indiv );
        printf( "-- JSONtoIND --\n%s\n----\n", print );
        //insertChar( &print, '|', 4 );
        //printf( "%s\n", print );
        free( print );
        deleteIndividual( indiv );

        // Test iListToJSON
        print = iListToJSON( obj->individuals );
        printf( "-- ILISTtoJSON --\n%s\n", print );
        free( print );


        // Test JSONtoGEDCOM
        char* ged = calloc( sizeof( char ), strlen( "{\"source\":\"PAF\",\"gedcVersion\":\"5.5\",\"encoding\":\"ASCII\",\"subname\":\"Jackson Firth\",\"subAddress\":\"55 Byron Street\"}" ) + 1 );
        strcpy( ged, "{\"source\":\"PAF\",\"gedcVersion\":\"5.5\",\"encoding\":\"ASCII\",\"subname\":\"Jackson Firth\",\"subAddress\":\"55 Byron Street\"}" );
        GEDCOMobject* obj1 = JSONtoGEDCOM( ged );
        free( ged );
        ged = printGEDCOM( obj1 );
        printf( "-- JSONtoGEDCOM --\n%s\n", ged );
        free( ged );
        deleteGEDCOM( obj1 );

        // Test validateGEDCOM
        Family* f = (Family*)(obj->families.head->data);
        obj->families.head->data = NULL;
        printf( "This should not validate\n" );
        code = validateGEDCOM( obj );
        deleteFamily( f );
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
