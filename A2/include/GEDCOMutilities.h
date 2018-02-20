/**
 * @file GEDCOMutilities.h
 * @author Jackson Firth 0880887
 * @version CIS2750 A1
 * @date February 2nd 2018
 *
 * @brief This file contains the interface of the GEDCOMutilities module
 */
#ifndef GEDCOM_UTIL_H
#define GEDCOM_UTIL_H
#include <stdbool.h>
#include "GEDCOMparser.h"

/* Structure used to hold external reference IDs and the related Individual pointer */
typedef struct refPair {
    char* extRefID;
    Individual* indi;
} RefPair;

typedef struct famPair {
    char* extRefID;
    Family* fam;
} FamPtrPair;

/* Data structure to hold record of (external reference, Individual) pair */
RefPair** referenceArray;
FamPtrPair** famArray;
int famCount;
int refCount;

/* Relation code to indicate how a person is related to a family */
typedef enum relatCode { HUSB, WIFE, CHIL, NONMEM } RelationType;

/* Used for indicating the record type to provide a generic utility to add fields to the list
 * in the structures.
 */
typedef enum recType { FAM, INDI, EVENT, HEAD, SUBM } RecordType;

/* Structure to hold to contents of a GEDCOM line */
typedef struct gedcom_line {
    int level;
    char* extRefID;
    char* tag;
    char* lineValue;
    int lineNumber;
} GEDCOMline;

/**
 * Function to add an event to a family's or individual's list of events
 *
 * @param record The record to add to
 * @param toAdd Pointer to the event to be added
 * @param type The Type of record (i.e. FAM | INDI)
 * @return True on success, false on failure
 */
bool addEventToRecord( void* record, Event* toAdd, RecordType type );

/**
 * Function to add a field to any of family, individual, or event structures.
 *
 * @param record Pointer to the record to be added to
 * @param toAdd Pointer to the field to be added
 * @param type Enumerated type to indicate whether the record being added to is a
 *        family, individual, or event
 * @return true on succes, false otherwise
 */
bool addFieldToRecord( void* record, Field* toAdd, RecordType type );

/**
 *
 */
void addReferencePairToArray( Individual* indi, int refNum );

/**
 * Adds the given individual to the given family under the given relationship.
 * NOTE: if an individual already exists in either of the parent spots in the family,
 *       any new given value will overwrite the previous one
 *
 * @param fam The family to be added to
 * @param indi The individual to be added
 * @param relation The relationship of the individual to the family
 * @return True on successful modification, false otherwise
 */
bool addToFamily( Family* fam, Individual* indi, RelationType relation );

/**
 *
 */
void combineLists( List* toKeep, List* toDestroy, int destroy );

/* Used for testing findPerson. Compares based on first and last names */
bool compareFunc( const void* first, const void* second );

/**
 * Function to convert from GEDCOM date format (i.e. 12 JAN 2018) to format able to be used in
 * lexicographic ordering (i.e. 12/01/2018)
 *
 * @pre toConvert is a character string with memory allocated to it
 * @post Returns a newly allocated string in the proper format
 * @param toConvert A pointer to the date string to be converted
 * @return string in the proper date format, or NULL on failure
 */
char* convertDate( char* toConvert );

/**
 * Constructor for a GEDCOMerror. Assigns the given values to the members in the structure
 *
 * @param type String indicating the type of the error
 * @param line The line number of the error
 * @return GEDCOMerror with appropriate field values
 */
GEDCOMerror createError( ErrorCode type, int line );

/**
 * Constructor for an event.
 *
 * @param record Array of GEDCOMlines associated with the event
 * @param count Number of items in record
 * @return Event structure with proper space allocated, or NULL
 */
Event* createEvent( GEDCOMline** record, int count );

/**
 * Constructor for a family. Parses values from an array of GEDCOMlines
 *
 * @post Family is created with husband and wife associated and lists initialized
 * @param record Array of GEDCOMlines associated with the family
 * @param count Number of items in record
 * @return Family with space allocated, or NULL on failure
 */
Family* createFamily( GEDCOMline** record, int count );

/**
 * Constructor for a Field.
 *
 * @pre tag and value must be null terminated strings
 * @post On success, a Field will be created with the given tag and value. On failure, NULL is
 *       returned.
 * @param tag String to be used as the tag for the Field
 * @param value String to be used as the value for the Field
 * @return Pointer to a Field structure with given strings, or NULL
 */
Field* createField( char* tag, char* value );

/**
 * Constructor for a GEDCOMline object.
 * NOTE: reference and value may be NULL
 *
 * @param input The line of text to parse
 * @return GEDCOMline object with appropriate values, or NULL on failure
 */
GEDCOMline* createGEDCOMline( char* input );

/**
 * Constructor for a Header. Parses values from an array of GEDCOMlines
 *
 * @post Header is created with members assigned and lists initialized
 * @param record Array of GEDCOMlines associated with the Header
 * @param count Number of items in record
 * @return Header with space allocated, or NULL on failure
 */
Header* createHeader( GEDCOMline** record, int count );

/**
 * Constructor for an Individual. Parses values from an array of GEDCOMlines
 *
 * @post Individual is created with pointers allocated and lists initialized
 * @param record Array of GEDCOMlines associated with the Individual
 * @param count Number of items in record
 * @return Individual with space allocated, or NULL on failure
 */
Individual* createIndividual( GEDCOMline** record, int count );

/**
 * Constructor for a Submitter. Parses values from an array of GEDCOMlines
 *
 * @post Submitter is created with pointers allocated and lists initialized
 * @param record Array of GEDCOMlines associated with the Submitter
 * @param count Number of items in record
 * @return Submitter with space allocated, or NULL on failure
 */
Submitter* createSubmitter( GEDCOMline** record, int count );

/**
 *
 */
void deleteDescendant( void* arg );

/**
 * Destructor for a GEDCOM line. Frees any strings inside
 *
 * @param line The line to be destroyed;
 */
void deleteGEDCOMline( GEDCOMline* line );

/**
 * Destructor for a Header. Frees any strings inside
 *
 * @param head The Header to be destroyed;
 */
void deleteHeader( Header* head );

/**
 * Destructor for a Submitter. Frees any strings inside
 *
 * @param sub The Submitter to be destroyed;
 */
void deleteSubmitter( Submitter* sub );

/**
 * Used to implement strong and weak references
 */
void dummyDelete( void* arg );

/**
 * Return a count of the number of members in the family
 *
 * @param family THe family to be counted
 * @return The number of family members
 */
int familyMemberCount( const void* family );

/**
 *
 */
Family* findFamilyReference( char* input );

/**
 *
 */
char* findExternalReferenceID( Individual* indi );

/**
 *
 */
char* findFamReferenceID( Family* fam );

/**
 *
 */
List getChildren( Individual* indiv );

/**
 *
 */
List getParents( Individual* indiv );

/**
 *
 */
RelationType getFamilyRelation( Family* fam, Individual* indi );

/**
 * Determines if a given tag is allowed to appear in a Header object
 *
 * @param tag The tag being tested
 * @return true if valid tag, false otherwise
 */
bool isValidHeadTag( char* tag );

/**
 * Determines if a given Individual is a parent in a given family
 *
 * @param fam The family to be tested
 * @param person The person to be tested
 * @return true if they are a parent, false otherwise
 */
bool isParent( Family* fam, const Individual* person );

/**
 * Mutator function to append more text to the value field of a GEDCOM line
 *
 * @param line The GEDCOM line to be modified
 * @param modValue The string to be appended to the line value
 * @return true on success, false otherwise
 */
bool modifyGEDCOMline( GEDCOMline* line, char* modValue );

/**
 *
 */
int mySurnameCompare( const void* first, const void* second );

/**
 *
 */
char* printDescendants( void* toBePrinted );

/**
 * Allocates a string for human readable version of GEDCOMline
 *
 * @param line THe line to be printed
 * @return allocated string on success, NULL otherwise
 */
char* printGEDCOMline( GEDCOMline* line );

/**
 *
 */
char* printGenerationList( void* toBePrinted );

/**
 * Allocates a string for human readable version of a Header
 *
 * @param head The Header to be printed
 * @return allocated string on success, NULL otherwise
 */
char* printHeader( Header* head );

/* Given as the function to the list returned by getDescendants in order to clean up the
 * output if the user decides to print the list contents
 *
 * @param toBePrinted The Individual to be printed
 * @return allocated string on succes, NULL otherwise
 */
char* printIndividualNames( void* toBePrinted );

/**
 * Allocates a string for human readable version of a Submitter
 *
 * @param sub The Submitter to be printed
 * @return allocated string on success, NULL otherwise
 */
char* printSubmitter( Submitter* sub );

/**
 * Recursive version of getDescendants
 *
 * @param list The list of descendants being built
 * @param person The current person being examined
 */
void recursiveGetDescendants( List* list, const Individual* person );

/**
 * Robust version of fgets() that will handle any combination of \r or \n as a line
 * terminator
 *
 * @param dest The string to store the read value in
 * @param max The maximum number of characters to be read
 * @param stream The stream to read from
 * @return character pointer to new string on success, NULL on failure
 */
char* robust_fgets( char* dest, int max, FILE* stream );

/**
 *
 */
bool writeEvent( Event* event, FILE* stream );

/**
 *
 */
bool writeHeader( Header* head, FILE* stream );

/**
 *
 */
bool writeIndividual( Individual* indi, FILE* stream, int recordNumber );

/**
 *
 */
bool writeFamily( Family* fam, FILE* stream );

/**
 *
 */
bool writeSubmitter( Submitter* sub, FILE* stream );

#endif
