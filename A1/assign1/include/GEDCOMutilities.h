/**
 * @file GEDCOMutilities.h
 * @author Jackson Firth 0880887
 * @date January 2018
 * @version CIS2750 A1
 *
 * @brief This file contains the interface of the GEDCOMutilities module
 */
#ifndef GEDCOM_UTIL_H
#define GEDCOM_UTIL_H
#include <stdbool.h>
#include "GEDCOMparser.h"


typedef struct refPair {
    char* extRefID;
    Individual* indi;
} RefPair;

/* Used to keep track of what @I00@ tag maps to which Individual */
RefPair** referenceArray;
int refCount;

typedef enum relatCode { HUSB, WIFE, CHIL } RelationType;

/* Used for indicating the record type to provide a generic utility to add fields to the list
 * in the structures.
 */
typedef enum recType { FAM, INDI, EVENT, HEAD, SUBM } RecordType;

typedef struct gedcom_line {
    int level;
    char* extRefID;
    char* tag;
    char* lineValue;
    int lineNumber;
} GEDCOMline;

/* This structure is used internally to associate a GEDCOM reference string (i.e. @I001@)
 * with an appropriate Individual pointer
 */
typedef struct extRefPointerPair {
    Individual* indi;
    char* extRefID;
} ReferencePointerPair;

/**
 * Function to add an event to an individual's structure
 *
 * @param record
 * @param toAdd Pointer to the event to be added
 * @param type
 * @return True on success, false on failure
 */
bool addEventToIndividual( void* record, Event* toAdd, RecordType type );

/**
 * Function to add a field to any of family, individual, or event structures.
 *
 * @param record Pointer to the record to be added to
 * @param toAdd Pointer to the field to be added
 * @param type Enuerated type to indicate whether the record being added to is a
 *        family, individual, or event
 * @return true on succes, false otherwise
 */
bool addFieldToRecord( void* record, Field* toAdd, RecordType type );

/**
 * Adds the given individual to the given family under the given relationship.
 * NOTE: if an individual already exists in either of the husband or parent spots in the family,
 *       any new given value will overwrite the previous one
 *
 * @param fam The family to be added to
 * @param indi The individual to be added
 * @param relation The relationship of the individual to the family
 * @return True on successful modification, false otherwise
 */
bool addToFamily( Family* fam, Individual* indi, RelationType relation );

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
 * Constructor for a GEDCOMerror. Assings the given values to the values in the structure
 *
 * @param type String indicating the type of the error
 */
GEDCOMerror createError( ErrorCode type, int line );

/**
 * Function to allocate space for strings and initialize the list in an Event structure
 *
 * @pre type, date, and place must be null terminated strings
 * @post On success, an Event structure is returned with the proper space allocated for the
 *       given names. On failure, NULL is returned
 * @param record
 * @param count
 * @return Event structure with proper space allocated, or NULL
 */
Event* createEvent( GEDCOMline** record, int count );

/**
 * Constructor for a family. Assigns the pointers to the proper individuals and initializes
 * the lists.
 *
 * @post Family is created with husband and wife associated and lists initialized
 * @param husb Pointer to an individual. MAY BE NULL. This indicates no husband is part of the
 *        family.
 * @param wife Pointer to an individual. MAY BE NULL. This indicates no wife is part of the
 *        family.
 * @return Family with space allocated
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
 * @param input
 */
GEDCOMline* createGEDCOMline( char* input );

/**
 *
 */
Header* createHeader( GEDCOMline** record, int count );

/**
 * Function to allocate space for strings and initialize lists in the Individual structure
 *
 * @pre givenName and surname must be null terminated strings
 * @post On success, an Individual structure is returned with the proper space allocated for
 *       the given names. On failure, NULL is returned
 * @param givenName The string to be stored as the given name for the individual
 * @param surname The string to be stored as the surname for the individual
 * @return An Individual structure with space allocated and lists initialized, or NULL
 */
Individual* createIndividual( GEDCOMline** record, int count );

/**
 *
 */
Submitter* createSubmitter( GEDCOMline** record, int count );

/**
 * Destructor for a GEDCOM line. Frees any strings inside
 *
 * @param line The line to be destroyed;
 */
void deleteGEDCOMline( GEDCOMline* line );

/**
 *
 */
void deleteHeader( Header* head );

/**
 *
 */
void deleteSubmitter( Submitter* sub );

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
bool isValidHeadTag( char* tag );

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
char* printHeader( Header* head );

/**
 *
 */
char* printGEDCOMline( GEDCOMline* line );

/**
 *
 */
char* printSubmitter( Submitter* sub );

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


/************************ START OF HASH TABLE FUNCTIONS ***********************/

typedef struct hashNode{
    void *data;
    struct hashNode *previous;
    struct hashNode *next;
    int key; // used for hash table
} HNode;

/**
 * Hash table structure
 */
typedef struct HTable {
    size_t size; // number that represents the size of the hash table
    List **table; // array that contains all of the table nodes
    void (*destroyData)( void *data ); // function pointer to a function to delete a single piece of data from the hash table
    int (*hashFunction)( size_t tableSize, char *toHash ); // function pointer to a function to hash the data
    char* (*printNode)( void *toBePrinted ); // function pointer to a function that prints out a data element of the table
    void (*addData)( struct HTable *hashTable, void *data );
} HTable;

/**
 * Function to point the hash table to the appropriate functions. Allocates memory to the struct and table based on the size given.
 *
 * @param size size of the hash table
 * @param hashFunction function pointer to a function to hash the data
 * @param destroyData function pointer to a function to delete a single piece of data from the hash table
 * @param printNode function pointer to a function that prints out a data element of the table
 * @return pointer to the hash table
 */
HTable *createTable( size_t size, int (*hashFunction)( size_t tableSize, char *toHash ),
    void (*destroyData)( void *data ), char* (*printNode)( void *toBePrinted ), int compare( const void *first, const void *second ),
    void (*addFunction)( HTable *hashTable, void *data ) );

/**
 * Function for creating a node for the hash table.
 *
 * @pre Node must be cast to void pointer before being added.
 * @post Node is valid and able to be added to the hash table
 * @param key Integer that represents the data (eg 35->"hello")
 * @param data Generic pointer to any data type.
 * @return Node for the hash table
 */
HNode *createHNode (int key, void *data );

/**
 * Deletes the entire hash table and frees memory of every element.
 *
 * @pre Hash Table must exist.
 * @param hashTable Pointer to hash table containing elements of data
 */
void destroyTable( HTable *hashTable );

/**
 * Function for creating a node for the hash table.
 * This node contains abstracted (void *) data as well as previous and next pointers to connect to other nodes in the list.
 *
 * @pre data should be of same size of void pointer on the users machine to avoid size conflicts. data must be valid.
 *   data must be cast to void pointer before being added.
 * @post data is valid to be added to a hash table
 * @return On success returns a hash node that can be added to a hash table. On failure, returns NULL.
 * @param data - is a void * pointer to any data type.  Data must be allocated on the heap.
 **/
HNode* initializeHNode( void* data );

/**
 * Inserts a Node in the hash table.
 *
 * @pre hashTable type must exist and have data allocated to it
 * @param hashTable Pointer to the hash table
 * @param key Integer that represents the data (eg 35->"hello")
 * @param data Pointer to generic data that is to be inserted into the list
 */
void insertData( HTable *hashTable, int key, void *data );

/**
 * Wrapper function to encapsulate the key generation process for inserting data
 *
 * @param hashTable The hash table to add to
 * @param data Pointer to the data to be inserted
 */
void insertDataIntoMap( HTable *hashTable, void *data );

/**
 * Function to return the data from the key given.
 *
 * @pre The hash table exists and has memory allocated to it
 * @param hashTable Pointer to the hash table containing data nodes
 * @param key Integer that represents a piece of data in the table (eg 35->"hello")
 * @param toFind Pointer to the data to be removed. Used for finding proper value when collisions
 *        occur
 * @return Pointer to the data in the hash table. Returns NULL if no match is found.
 */
void *lookupData( HTable *hashTable, int key, void *toFind );

/**
 * Function to remove a node from the hash table
 *
 * @pre Hash table must exist and have memory allocated to it
 * @post Node at key will be removed from the hash table if it exists.
 * @param hashTable Pointer to the hash table struct
 * @param key Integer that represents a piece of data in the table (eg 35->"hello")
 * @param toRemove Pointer to the data to be removed. Used for finding proper value when collisions
 *        occur
 */
void removeData( HTable *hashTable, int key, void *toRemove );

/************************** END OF HASH TABLE FUNCTIONS ***********************/

#endif
