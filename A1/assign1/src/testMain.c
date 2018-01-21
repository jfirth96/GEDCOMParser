#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"
#include "LinkedListAPI.h"

int main( int argc, char** argv ) {
    // Create 2 test fields
    Field* field1 = (createField( "ADOP", "12 MAY 1923" )); // test1
    Field* field2 = (createField( "CHR", "15 JUN 1239" )); // test2

    // Create 3 test Individuals
    Individual *indiv1, *indiv2, *indiv3;
    if ((indiv1 = createIndividual( "Jackson", "Firth" )) == NULL) { // test1
        printf( "Failed to create individual\n" );
    }
    if ((indiv2 = createIndividual( "Elizabeth", "Standing" )) == NULL) { // test2
        printf( "Failed to create individual\n" );
    }
    if ((indiv3 = createIndividual( "Hendrix", "Firth" )) == NULL) { // test3
        printf( "Failed to create individual\n" );
    }

    // Create 2 test events
    Event *event1, *event2;
    if ((event1 = createEvent( "BIRT", "12 MAY 1985", "Georgetown, ON" )) == NULL) { // test1
        printf( "Failed to create Event\n" );
    }
    if ((event2 = createEvent( "DEAT", "13 JUN 2001", "Canada" )) == NULL) { // test2
        printf( "Failed to create Event\n" );
    }

    // Create 2 test families
    Family *fam1 = (createFamily( indiv1, indiv2 )); // test1
    Family *fam2 = (createFamily( indiv1, indiv3 )); // test2

    // Add children to families
    bool flag = addToFamily( fam1, indiv3, CHIL ); // test1
    if (flag == false) {
        printf( "Failed to add individual to family\n" );
    }
    flag = addToFamily( fam2, indiv2, CHIL ); // test2
    if (flag == false) {
        printf( "Failed to add individual to family\n" );
    }

    // Add fields to stuff
    bool flag1 = addFieldToRecord( (void*)indiv1, field1, INDI );
    if (flag1 == false) {
        printf( "Failed to add field to record\n" );
    }
    flag1 = addFieldToRecord( (void*)fam1, field2, FAM );
    if (flag1 == false) {
        printf( "Failed to add field to record\n" );
    }

    printf( "fam1 = %d members\n", familyMemberCount( (void*)fam1 ) ); // test1
    printf( "fam2 = %d members\n", familyMemberCount( (void*)fam2 ) ); // test2

    char* printMe = printField( (void*)field1 ); // test 1 printField
    printf( "printField test 1:\n%s\n", printMe );
    free( printMe );
    printMe = printField( (void*)field2 ); // test 2 printField
    printf( "printField test 2:\n%s\n", printMe );
    free( printMe );
    printMe = printIndividual( (void*)indiv1 ); // test 1 printIndividual
    printf( "printIndividual test 1:\n%s", printMe );
    free( printMe );
    printMe = printIndividual( (void*)indiv2 ); // test 2 printIndividual
    printf( "printIndividual test 2:\n%s", printMe );
    free( printMe );
    printMe = printIndividual( (void*)indiv3 ); // test 3 printIndividual
    printf( "printIndividual test 3:\n%s", printMe );
    free( printMe );
    printMe = printEvent( (void*)event1 ); // test 1 printEvent
    printf( "printEvent test 1:\n%s", printMe );
    free( printMe );
    printMe = printEvent( (void*)event2 ); // test 2 printEvent
    printf( "printEvent test 2:\n%s", printMe );
    free( printMe );
    printMe = printFamily( (void*)fam1 ); // test 1 printFamily
    printf( "printFamily test 1:\n%s", printMe );
    free( printMe );
    printMe = printFamily( (void*)fam2 ); // test 2 printFamily
    printf( "printFamily test 2:\n%s", printMe );
    free( printMe );

    char* print1 = printField( (void*)field1 );
    char* print2 = printField( (void*)field2 );
    printf( "(%s) vs (%s): %d - test 1 printField\n", print1, print2, compareFields( (void*)field1, (void*)field2 ) );
    printf( "(%s) vs (%s): %d - test 2 printField\n", print2, print1, compareFields( (void*)field2, (void*)field1 ) );
    free( print1 );
    free( print2 );
    print1 = printEvent( (void*)event1 );
    print2 = printEvent( (void*)event2 );
    printf( "(%s) vs (%s): %d - test 1 printEvent\n", print1, print2, compareEvents( (void*)event1, (void*)event2 ) );
    printf( "(%s) vs (%s): %d - test 2 printEvent\n", print2, print1, compareEvents( (void*)event2, (void*)event1 ) );
    free( print1 );
    free( print2 );
    print1 = printIndividual( (void*)indiv1 );
    print2 = printIndividual( (void*)indiv2 );
    char* print3 = printIndividual( (void*)indiv3 );
    printf( "(%s) vs (%s): %d - test 1 printIndividual\n", print1, print2, compareIndividuals( (void*)indiv1, (void*)indiv2 ) );
    printf( "(%s) vs (%s): %d - test 2 printIndividual\n", print2, print1, compareIndividuals( (void*)indiv2, (void*)indiv1 ) );
    printf( "(%s) vs (%s): %d - test 3 printIndividual\n", print1, print3, compareIndividuals( (void*)indiv1, (void*)indiv3 ) );
    printf( "(%s) vs (%s): %d - test 4 printIndividual\n", print3, print1, compareIndividuals( (void*)indiv3, (void*)indiv1 ) );
    printf( "(%s) vs (%s): %d - test 5 printIndividual\n", print2, print3, compareIndividuals( (void*)indiv2, (void*)indiv3 ) );
    printf( "(%s) vs (%s): %d - test 6 printIndividual\n", print3, print2, compareIndividuals( (void*)indiv3, (void*)indiv2 ) );
    free( print1 );
    free( print2 );
    free( print3 );
    print1 = printFamily( (void*)fam1 );
    print2 = printFamily( (void*)fam2 );
    //printf( "(%s) vs (%s): %d - test 1 printFamily\n", print1, print2, compareFamilies( (void*)fam1, (void*)fam2 ) );
    //printf( "(%s) vs (%s): %d - test 2 printFamily\n", print2, print1, compareFamilies( (void*)fam2, (void*)fam1 ) );
    free( print1 );
    free( print2 );

    //deleteField( (void*)field1 ); // test 1 deleteField
    //deleteField( (void*)field2 ); // test 2 deleteField
    deleteIndividual( (void*)indiv1 ); // test 1 deleteIndividual
    deleteIndividual( (void*)indiv2 ); // test 2 deleteIndividual
    deleteIndividual( (void*)indiv3 ); // test 3 deleteIndividual
    deleteFamily( (void*)fam1 ); // test 1 for deleteFamily
    deleteFamily( (void*)fam2 ); // test 2 for deleteFamily
    deleteEvent( (void*)event1 ); // test 1 deleteEvent
    deleteEvent( (void*)event2 ); // test 2 deleteEvent

    GEDCOMerror* err = createError( INV_FILE, 20 );
    print1 = printError( *err );
    printf( "%s", print1 );
    free( print1 );

    GEDCOMline *line = createGEDCOMline( 2, NULL, "DATE", "12 MAY 1987" );
    printf( "%d %s %s", line->level, line->tag, line->lineValue );
    deleteGEDCOMline( line );
    /*free( field1 );
    free( field2 );
    free( indiv1 );
    free( indiv2 );
    free( indiv3 );
    free( event1 );
    free( event2 );*/
    free( err );
    //free( line );
    //free( fam1 );
    //free( fam2 );

    return 0;
}
