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

typedef enum relatCode { HUSB, WIFE, CHIL } RelationType;

/* Used for indicating the record type to provide a generic utility to add fields to the list
 * in the structures.
 */
typedef enum recType { FAM, INDI, EVENT } RecordType;

typedef struct gedcom_line {
    int level;
    Individual* extRefID;
    char* tag;
    char* lineValue;
} GEDCOMline;

/**
 * Function to add an event to an individual's structure
 *
 * @param indiv Pointer to the individual to be modified
 * @param toAdd Pointer to the event to be added
 * @return True on success, false on failure
 */
bool addEventToIndividual( Individual* indiv, Event* toAdd );

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
GEDCOMerror* createError( ErrorCode type, int line );

/**
 * Function to allocate space for strings and initialize the list in an Event structure
 *
 * @pre type, date, and place must be null terminated strings
 * @post On success, an Event structure is returned with the proper space allocated for the
 *       given names. On failure, NULL is returned
 * @param type The type of event. Parameter will be validated to be 4 characters or less
 * @param date The date of the event in the format DD MMM YYYY (i.e. 12 JAN 2017)
 * @param place The place of the event, no format requirements
 * @return Event structure with proper space allocated, or NULL
 */
Event* createEvent( char* type, char* date, char* place );

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
Family* createFamily( Individual* husb, Individual* wife );

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
 * @param level The level of the line
 * @param reference The external reference pointer, void because it can be referecing a variety
 *        of structures. May also be NULL
 * @param tag The tag of the line
 * @param value Optional line value, pointer may be NULL
 */
GEDCOMline* createGEDCOMline( int level, void* reference, char* tag, char* value );

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
Individual* createIndividual( char* givenName, char* surname );

/**
 * Destructor for a GEDCOM line. Frees any strings inside
 *
 * @param line The line to be destroyed;
 */
void deleteGEDCOMline( GEDCOMline* line );

/**
 * Return a count of the number of members in the family
 *
 * @param family THe family to be counted
 * @return The number of family members
 */
int familyMemberCount( const void* family );

#endif
