#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GEDCOMparser.h"
#include "LinkedListAPI.h"

int main( int argc, char** argv ) {
    // Create 2 test fields
    Field testField, randField;
    testField.tag = malloc( sizeof( char ) * 32 );
    testField.value = malloc( sizeof( char ) * 50 );
    strcpy( testField.tag, "BIRT" );
    strcpy( testField.value, "12 NOV 1996" );
    randField.tag = malloc( sizeof( char ) * 32 );
    randField.value = malloc( sizeof( char ) * 50 );
    strcpy( randField.tag, "DEAT" );
    strcpy( randField.value, "13 June 2100" );

    // Create 3 test Individuals
    Individual indiv1, indiv2, indiv3;
    indiv1.givenName = malloc( sizeof( char ) * 120 );
    indiv1.surname = malloc( sizeof( char ) * 120 );
    indiv1.events = initializeList( &printEvent, &deleteEvent, &compareEvents );
    indiv1.families = initializeList( &printFamily, &deleteFamily, &compareFamilies );
    indiv1.otherFields = initializeList( &printField, &deleteField, &compareFields );
    strcpy( indiv1.givenName, "Jackson" );
    strcpy( indiv1.surname, "Firth" );
    indiv2.givenName = malloc( sizeof( char ) * 120 );
    indiv2.surname = malloc( sizeof( char ) * 120 );
    indiv2.events = initializeList( &printEvent, &deleteEvent, &compareEvents );
    indiv2.families = initializeList( &printFamily, &deleteFamily, &compareFamilies );
    indiv2.otherFields = initializeList( &printField, &deleteField, &compareFields );
    strcpy( indiv2.givenName, "Joshua" );
    strcpy( indiv2.surname, "Caravaggio" );
    indiv3.givenName = malloc( sizeof( char ) * 120 );
    indiv3.surname = malloc( sizeof( char ) * 120 );
    indiv3.events = initializeList( &printEvent, &deleteEvent, &compareEvents );
    indiv3.families = initializeList( &printFamily, &deleteFamily, &compareFamilies );
    indiv3.otherFields = initializeList( &printField, &deleteField, &compareFields );
    strcpy( indiv3.givenName, "Queen" );
    strcpy( indiv3.surname, "Elizabeth" );

    // Create 2 test events
    Event event1;
    strcpy( event1.type, "BIRT" );
    event1.date = malloc( sizeof( char ) * 13 );
    event1.place = malloc( sizeof( char ) * 120 );
    event1.otherFields = initializeList( &printField, &deleteField, &compareFields );
    strcpy( event1.date, "12 MAY 1985" );
    strcpy( event1.place, "Georgetown, ON" );
    Event event2;
    strcpy( event2.type, "DEAT" );
    event2.date = malloc( sizeof( char ) * 13 );
    event2.place = malloc( sizeof( char ) * 120 );
    event2.otherFields = initializeList( &printField, &deleteField, &compareFields );
    strcpy( event2.date, "13 JUN 2001" );
    strcpy( event2.place, "Canada" );

    // Create 2 test families
    Family fam1;
    fam1.husband = &indiv1;
    fam1.wife = &indiv3;
    fam1.children = initializeList( &printIndividual, &deleteIndividual, &compareIndividuals );
    insertSorted( &(fam1.children), (void*)&indiv2 );
    fam1.otherFields = initializeList( &printField, &deleteField, &compareFields );
    Family fam2;
    fam2.husband = &indiv1;
    fam2.wife = &indiv2;
    fam2.children = initializeList( &printIndividual, &deleteIndividual, &compareIndividuals );
    insertSorted( &(fam2.children), (void*)&indiv3 );
    fam2.otherFields = initializeList( &printField, &deleteField, &compareFields );

    char* printMe = printField( (void*)&testField ); // test 1 printField
    printf( "printField test 1:\n%s\n", printMe );
    free( printMe );
    printMe = printField( (void*)&randField ); // test 2 printField
    printf( "printField test 2:\n%s\n", printMe );
    free( printMe );
    printMe = printIndividual( (void*)&indiv1 ); // test 1 printIndividual
    printf( "printIndividual test 1:\n%s", printMe );
    free( printMe );
    printMe = printIndividual( (void*)&indiv2 ); // test 2 printIndividual
    printf( "printIndividual test 2:\n%s", printMe );
    free( printMe );
    printMe = printIndividual( (void*)&indiv3 ); // test 3 printIndividual
    printf( "printIndividual test 3:\n%s", printMe );
    free( printMe );
    printMe = printEvent( (void*)&event1 ); // test 1 printEvent
    printf( "printEvent test 1:\n%s", printMe );
    free( printMe );
    printMe = printEvent( (void*)&event2 ); // test 2 printEvent
    printf( "printEvent test 2:\n%s", printMe );
    free( printMe );
    printMe = printFamily( (void*)&fam1 ); // test 1 printFamily
    printf( "printFamily test 1:\n%s", printMe );
    free( printMe );
    printMe = printFamily( (void*)&fam2 ); // test 2 printFamily
    printf( "printFamily test 2:\n%s", printMe );
    free( printMe );

    char* print1 = printField( (void*)&testField );
    char* print2 = printField( (void*)&randField );
    printf( "(%s) vs (%s): %d - test 1 printField\n", print1, print2, compareFields( (void*)&testField, (void*)&randField ) );
    printf( "(%s) vs (%s): %d - test 2 printField\n", print2, print1, compareFields( (void*)&randField, (void*)&testField ) );
    free( print1 );
    free( print2 );
    print1 = printEvent( (void*)&event1 );
    print2 = printEvent( (void*)&event2 );
    printf( "(%s) vs (%s): %d - test 1 printEvent\n", print1, print2, compareEvents( (void*)&event1, (void*)&event2 ) );
    printf( "(%s) vs (%s): %d - test 2 printEvent\n", print2, print1, compareEvents( (void*)&event2, (void*)&event1 ) );
    free( print1 );
    free( print2 );
    print1 = printIndividual( (void*)&indiv1 );
    print2 = printIndividual( (void*)&indiv2 );
    char* print3 = printIndividual( (void*)&indiv3 );
    printf( "(%s) vs (%s): %d - test 1 printIndividual\n", print1, print2, compareIndividuals( (void*)&indiv1, (void*)&indiv2 ) );
    printf( "(%s) vs (%s): %d - test 2 printIndividual\n", print2, print1, compareIndividuals( (void*)&indiv2, (void*)&indiv1 ) );
    printf( "(%s) vs (%s): %d - test 3 printIndividual\n", print1, print3, compareIndividuals( (void*)&indiv1, (void*)&indiv3 ) );
    printf( "(%s) vs (%s): %d - test 4 printIndividual\n", print3, print1, compareIndividuals( (void*)&indiv3, (void*)&indiv1 ) );
    printf( "(%s) vs (%s): %d - test 5 printIndividual\n", print2, print3, compareIndividuals( (void*)&indiv2, (void*)&indiv3 ) );
    printf( "(%s) vs (%s): %d - test 6 printIndividual\n", print3, print2, compareIndividuals( (void*)&indiv3, (void*)&indiv2 ) );
    free( print1 );
    free( print2 );
    free( print3 );
    print1 = printFamily( (void*)&fam1 );
    print2 = printFamily( (void*)&fam2 );
    printf( "(%s) vs (%s): %d - test 1 printFamily\n", print1, print2, compareFamilies( (void*)&fam1, (void*)&fam2 ) );
    printf( "(%s) vs (%s): %d - test 2 printFamily\n", print2, print1, compareFamilies( (void*)&fam2, (void*)&fam1 ) );
    free( print1 );
    free( print2 );

    deleteField( (void*)&testField ); // test 1 deleteField
    deleteField( (void*)&randField ); // test 2 deleteField
    deleteIndividual( (void*)&indiv1 ); // test 1 deleteIndividual
    deleteIndividual( (void*)&indiv2 ); // test 2 deleteIndividual
    deleteIndividual( (void*)&indiv3 ); // test 3 deleteIndividual
    deleteFamily( (void*)&fam1 ); // test 1 for deleteFamily
    deleteFamily( (void*)&fam2 ); // test 2 for deleteFamily
    deleteEvent( (void*)&event1 ); // test 1 deleteEvent
    deleteEvent( (void*)&event2 ); // test 2 deleteEvent

    return 0;
}
