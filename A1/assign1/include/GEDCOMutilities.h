#ifndef GEDCOM_UTIL_H
#define GEDCOM_UTIL_H
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
 *
 */
int familyMemberCount( const void* family );

#endif
