Jackson Firth
0880887
jfirth@uoguelph.ca

CIS2750 Assignment 1 W18

This assignment was to create a parsing library to handle the input of GEDCOM formatted files into our system.

/** DESCRIPTION **/
This public library is divided into 6 major modules that handle the GEDCOM object; creating the object, deleting the object,
printing the object, printing the error returned by an attempted creation, finding a person in the object, and getting the
descendants of a person.

The algorithms for the above mentioned modules are outlined below.

/** ALGORITHMS **/

createGEDCOM:
	All parameters are checked for NULL, then the passed object is allocated.
	The function then grabs a GEDCOMline, as well as any subordinate GEDCOMlines to be used for creating a record.
	These lines are checked for errors, and INV_RECORD is returned if any are found.
	Depending on the tag of the first GEDCOMline, the array of structures is converted into one of the 4 records used in
	this assignment. These records are checked for errors, and the appropriate error is returned if necessary.
	Upon successful creation of the record, it is assigned to the appropriate member of the GEDCOMobject.
	This process is repeated until the end of file is reached.

deleteGEDCOM:
	Free all pointers in the object that have been allocated, and clear the list if they have contents.

printGEDCOM:
	Allocate a string to be large enough to hold the human readable representations of the Header, Submitter, the list of
	individuals, and the list of families.
	The string to be returned is reallocated to hold the additional required characters for each string returned by the print
	functions, one at a time.
	The string is the returned to the calling function, leaving the memory clean-up responsibilities to the caller.

printError:
	Allocate a string to hold the human readable version of an ErrorCode, which is just a string representation of the
	label, as well as the line of the error.
	The string is the returned to the calling function, leaving the memory clean-up responsibilities to the caller.

findPerson:
	If the list of individuals in the given GEDCOMobject is initialized, then loop over every indivual in the list, comparing 
	each one to the given individual. If there is a match, return the individual. If the loop runs to completion without finding 
	a match, return NULL to indicate no match.

getDescendants:
	This function acts as a wrapper for a recursive version of the same function. The recursive function takes a list and
	an Individual pointer. The list will be populated with the descendants of the Individual given to getDescendants (non-recursive).
	Recursive:
	For each family in the individual's list of families
		if the individual is a parent of said family
			for each individual in the list of children for the current family
				add this individual to list
				call recursiveGetDescendants, passing in the current individual and the list

/** COMPILATION **/

NOTE: all commands are to be entered at the root level of this project directory

To compile this project in it's entirety:
	make all
	
To compile the GEDCOM parsing library:
	make parser
	
To compile the LinkedList library:
	make list
	
To clean up the binaries produced by the makefile:
	make clean
