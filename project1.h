/**
 * IAED-23 Project 1
 * File: project1.h
 * Author: Bibiana Andre LEIC-A 2022-23
 * 
 * Description: Header file for the 1st Project. Includes constants, 
 * prototypes, warnings and structs.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* ------------------------------ Constants ------------------------------- */


/* Array sizes */
#define MAX_LINE_NAME 21    /* Max length of line name. */
#define MAX_STOP_NAME 51    /* Max length of stop name. */
#define MAX_LINES 200       /* Max number of lines. */
#define MAX_STOPS 10000     /* Max number of stops. */
#define MAX_LINKS 30000     /* Max number of links. */

/* Words */
#define SORT "inverso"      /* Sort option input. */
#define MAX_SORT_LENGTH 7   /* Max length of valid sort option. */
#define MIN_SORT_LENGTH 3   /* Min length of valid sort option. */

/* Others */
#define YES 1               /* If a statement is true. */
#define NO 0                /* If a statement if false. */

                                
/* ------------------------------- Warnings ------------------------------- */


/* Sorting option */
#define WRONG_SORT "incorrect sort option.\n"

/* Stops */
#define STOP_ALREADY_EXISTS "%s: stop already exists.\n"
#define NO_SUCH_STOP "%s: no such stop.\n"

/* Links */
#define NO_SUCH_LINE "%s: no such line.\n"
#define CANNOT_LINK "link cannot be associated with bus line.\n"

/* Time and/or cost */
#define NEGATIVE_VALUE "negative cost or duration.\n"


/* ------------------------------ Structures ------------------------------ */


/* Structure of stop. */ 
typedef struct {               
    char name[MAX_STOP_NAME];
    double longitude;
    double latitude;
    int num_lines;
} Stop;

/* Structure of line. */
typedef struct {              
    char name[MAX_LINE_NAME];
    char start[MAX_STOP_NAME];
    char end[MAX_STOP_NAME];
    int num_stops;
    int *link_idx;    
    double cost;
    double duration;
} Line;

/* Structure of link. */
typedef struct {              
    Line line;
    Stop orig;
    Stop dest;
    double cost;
    double duration;
    int isValid;
} Link;


/* ------------------------------ Prototypes ------------------------------ */


/* Auxiliary functions: */

int compArgs(char string[], char args[]);

void freeLineLinks();


/* Input functions: */

int hasArgs();

int getArg(char* string, int n, int size);

int getArgWithNoSpaces(char str[], int ch, int n, int size);

int getArgWithSpaces(char str[], int n, int size);


/* Line functions: */

void listLines();

int lineExists(char string[]);

void showLineStops(int i, int sort);

void printStops(int k, int order);

void addLine(char string[]);

void addShowLine(char string[], int sort);

int assertSortOption(char opt[]);


/* Stop functions: */

void listStops();

int stopExists(char string[]);

void showStop(char string[]);

void addStop(char string[], double latitude, double longitude);


/* Link functions: */

Link processLinkArgs(Link link, char num1[], char num2[]);

void processLink(Link link, int l, int o, int d);

int stopExistsInLine(Line line, Stop stop);

void addFirstLink(Link link, int l, int o, int d);

void addLink(Link link, int l, int o, int d, int shift);

void addLoopLink(Link link, int l);

int assertNegativeValue(Link link);


/* Intersection functions: */

void bubbleSort(char v[][MAX_LINE_NAME], int size);

void lineIntersection(Stop stop);


/* Handle commands: */

void handleLineCommand();

void handleStopCommand();

void handleLinkCommand();

void handleIntersectionCommand();