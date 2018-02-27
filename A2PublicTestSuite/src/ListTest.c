#include "LinkedListAPI.h"

#include "ListTestCases.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

#include "TestHarness.h"

#define TESTS 9
#define DEBUG 0
#define OUT stdout

int testNo;
testRec * * testRecords;
int studentScore;  //globals  required to handle segfaults gracefully
//------------ Something went wrong and student code segfaulted --------------


static void segfaultCatcher(int signal,  siginfo_t* si, void *arg)
{
    printf("\n\n************** Code Segfaulted: Partial Report ********************\n");
    int j;
    for(j=0; j<TESTS; j++){
        if(testRecords[j] != NULL){
            printRecord(testRecords[j]);
        }
    }
    
    printf("*******************\nPARTIAL RESULTS\nProgram caused segfault\n*******************\n");
    printf("Partial Score:%d\n",studentScore);
    
    
    exit(EXIT_FAILURE);
}

static void addTestResult(testRec* tmpRec){
    testRecords[testNo-1] = tmpRec;
    studentScore = studentScore + getScore(testRecords[testNo-1]);
    testNo++;
}

int main()
{
    
    studentScore = 0;
    testNo = 1;
    
    struct sigaction segfaultSignaler;
    // set up the segfault handler
    memset(&segfaultSignaler, 0, sizeof(struct sigaction));
    sigemptyset(&segfaultSignaler.sa_mask);
    segfaultSignaler.sa_sigaction = segfaultCatcher;
    segfaultSignaler.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &segfaultSignaler, NULL);
    
    
    //record keeping array
    testRecords =  malloc(sizeof(testRec *)* TESTS);
    
    if(DEBUG) fprintf(OUT, "************** Testing Details ********************\n\n");
    
    //Create list
    if (DEBUG) fprintf(OUT,"Testing list creation\n");
    testRec* tmpRec = initListTest(testNo);
    addTestResult(tmpRec);
    
    //Node Creation
    if (DEBUG) fprintf(OUT, "Testing Node Creation\n");
    tmpRec = initNodeTest(testNo);
    addTestResult(tmpRec);
    
    //Testing getLength
    if (DEBUG) fprintf(OUT, "Testing getLength\n");
    tmpRec = getLengthTest(testNo);
    addTestResult(tmpRec);
    
    //Insert Front and Back
    if (DEBUG) fprintf(OUT, "Testing insertion\n");
    tmpRec = insertTest(testNo);
    addTestResult(tmpRec);
    
    //Getter functions test
    if (DEBUG) fprintf(OUT, "Testing getter functions\n");
    tmpRec = getTest(testNo);
    addTestResult(tmpRec);
    
    //Insert Sorted
    if (DEBUG) fprintf(OUT, "Testing sorted insertion\n");
    tmpRec = insertSortedTest(testNo);
    addTestResult(tmpRec);
    
    //delete List
    if (DEBUG) fprintf(OUT, "Testing deletion functionality");
    //runTest(&deleteTest);
    tmpRec = deleteTest(testNo);
    addTestResult(tmpRec);
    
    //Printing
    if (DEBUG) fprintf(OUT, "Testing list printing");
    tmpRec = printTest(testNo);
    addTestResult(tmpRec);
    
    //search List
    if (DEBUG) fprintf(OUT, "Testing search list functionality");
    //runTest(&deleteTest);
    tmpRec = searchTest(testNo);
    addTestResult(tmpRec);
    
    printf("Last test completed");
    
    //Test Conclusion and report
    printf("\n\n************** Testing Report ********************\n");
    
    int j;
    for(j=0; j<TESTS; j++)
    {
        
        printRecord(testRecords[j]);
        //printf("\n");
    }
    //fclose(output);
    
    printf("Score: %d\n", studentScore);
    
    return 0;
    
}



