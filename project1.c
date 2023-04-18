/**
 * IAED-23 Project 1
 * File: project1.c
 * Author: Bibiana Andre LEIC-A 2022-23
 * 
 * Description: Main project file for the 1st Project (Busline Management).
*/

#include "project1.h"

/* --------------------------- Global variables --------------------------- */


Stop _stops[MAX_STOPS];         /* Array of all stops */
int stopCounter = 0;            /* Starting number of stops */

Line _lines[MAX_LINES];         /* Array of all lines */
int lineCounter = 0;            /* Starting number of lines */

Link _links[MAX_LINKS];         /* Array of all links */
int linkCounter = 0;            /* Starting number of links */


/* -------------------------- Auxiliary functions ------------------------- */


/**
 * Compare two strings and return YES if they're equal.
 * Else, return NO.
*/
int compArgs(char string[], char args[]) {

    if (strcmp(string, args) == 0) 
        return YES;
    
    return NO;

}

/**
 * Frees dynamically allocated memory in the lines' link index array 
 * and set the respective values to NULL.
*/
void freeLineLinks() {

    int i;

    for (i = 0; i < lineCounter; i++) {

        free(_lines[i].link_idx);
        _lines[i].link_idx = NULL;

    }
}


/* ---------------------------- Input functions --------------------------- */


/**
 * Indicates if there are more input arguments. If an '\n' is read, 
 * return NO. Else, return YES.
*/
int hasArgs() {

    int c = getchar();

    if (c == '\n' || c == EOF) 
        return NO;
    
    return YES;   

}

/**
 * Reads a string from the standard input. As characters are read, the command
 * length decreases. If an '\n' char is read, returns 0 as result. Else,
 * returns the remaining length of the command line (in bytes).
*/
int getArg(char* string, int n, int size) {

    int c = getchar(), result;
    
    /* If white spaces are read. */
    while(isspace(c) && c != '\n') {
        
        n--;    
        c = getchar(); 
    }

    /* Argument can contain white spaces. */
    if (c == '"') 
        return result = getArgWithSpaces(string, n, size);

    /* Argument without spaces (including numeric values). */ 
    return result = getArgWithNoSpaces(string, c, n, size);

}

/**
 * Reads a string without white spaces from the standard input. As characters
 * are read, the command length decreases. If an '\n' char is read, returns 0. 
 * Else, returns the remaining length of the command line (in bytes).
*/
int getArgWithNoSpaces(char str[], int ch, int n, int size) {

    int i = 0, len = n;

    while (!isspace(ch) && i < size && i < len) {

        str[i++] = ch;
        ch = getchar();
    }

    str[i] = '\0';
          
    if (ch == '\n' || len - strlen(str) <= 0) /* End of input. */
        return 0;

    
    return len - strlen(str);

}

/**
 * Reads a string containing white spaces from the standard input. As characters
 * are read, the command length decreases. If an '\n' char is read, returns 0. 
 * Else, returns the remaining length of the command line (in bytes). 
*/
int getArgWithSpaces(char str[], int n, int size) {

    int c, i = 0, len = n;

    len--; 

    while ((c = getchar()) != '"' && i < len && i < size) {

        str[i++] = c;  
    }

    len--; 
    str[i] = '\0';

    if (!hasArgs() || len - strlen(str) <= 0) /* End of input. */
        return 0;

    return len - strlen(str);

}


/* ---------------------------- Line functions ---------------------------- */


/**
 * List all the existing lines in the standard output, by order of creation, 
 * presenting the name, first and last stop (if the line has stops), number 
 * of stops, total cost and total duration of the itinerary.
*/
void listLines() {

    int i;

    for (i = 0; i < lineCounter; i++) {
        
        if (_lines[i].num_stops != 0) {  

            printf("%s %s %s %d %.2f %.2f\n", _lines[i].name, 
                                                _lines[i].start, 
                                                _lines[i].end, 
                                                _lines[i].num_stops,
                                                _lines[i].cost, 
                                                _lines[i].duration);

        } else {

            printf("%s %d %.2f %.2f\n", _lines[i].name, 
                                        _lines[i].num_stops,
                                        _lines[i].cost, 
                                        _lines[i].duration);

        }
    }       
}

/**
 * Looks for a given line in the '_lines' array. If the name of the line is 
 * found, return the array index with the said line. Else, return -1.
*/
int lineExists(char string[]) {

    int i;

    for (i = 0; i < lineCounter; i++) {

        if(compArgs(_lines[i].name, string) == YES)
            return i;

    }

    return -1;

}

/**
 * Shows the itinerary of a given line in the standard output, being 'i' its 
 * index in the '_lines' array and 'sort' the sort indicator: if YES, the 
 * itinerary is presented backwards; if NO, is presented as default.
*/
void showLineStops(int i, int sort) {

    if (_lines[i].num_stops == 2) {

        if (!sort) /* Default order. */  
           
            printf("%s, %s\n", _lines[i].start, _lines[i].end);

        else /* Backwards. */       
            
            printf("%s, %s\n", _lines[i].end, _lines[i].start);

    } else if (_lines[i].num_stops > 2){

       printStops(i, sort);
        
    }   
}

/**
 * Auxiliary line function. Shows the itinerary of a given line in the standard 
 * output with more than 2 stops according to the order option.
*/
void printStops(int k, int order) {

    int j;

    if (!order) { /* Default order. */
        
        for (j = 0; j < _lines[k].num_stops -1; j++) 
            printf("%s, ", _links[_lines[k].link_idx[j]].orig.name);
            
        printf("%s\n", _lines[k].end);

    } else { /* Backwards. */
        
        for (j = _lines[k].num_stops - 2; j >= 0 ; j--) 
            printf("%s, ", _links[_lines[k].link_idx[j]].dest.name);
            
        printf("%s\n", _lines[k].start);

    }
}

/**
 * Adds (if possible) a new, non existing line to the '_lines' array. 
*/
void addLine(char string[]) {

    if (lineCounter != MAX_LINES) {    

        strcpy(_lines[lineCounter].name, string);
        _lines[lineCounter].num_stops = 0;
        _lines[lineCounter].cost = 0.00;
        _lines[lineCounter].duration = 0.00;

        lineCounter++; /* Number of lines increased. */

    }
}

/**
 * Adds, or presents in the standard output, a given line. If it exists, its 
 * itinerary is presented according to the sort option. Else, adds the line.
*/
void addShowLine(char string[], int sort) {

    int i = lineExists(string);

    if (i > -1)  

        showLineStops(i, sort); /* Line exists. */

    else  

        addLine(string); /* Add line. */

}

/**
 * Assert if the input read sort option is a valid one.
 * Returns YES if so. Returns NO otherwise.
*/
int assertSortOption(char opt[]) {

    char sort[] = SORT;

    int i, len = strlen(opt);

    if (len < MIN_SORT_LENGTH || len > MAX_SORT_LENGTH) {

        printf(WRONG_SORT);
        return NO;
    }

    for (i = 0; i < len; i++) {

        if(opt[i] != sort[i]) {

            printf(WRONG_SORT);
            return NO;
        }
    }

    return YES;

}


/* ---------------------------- Stop functions ---------------------------- */


/**
 * List all the existing stops in the standard output, in the order they were 
 * created, presenting the name, coordinates and respective number of lines.
*/
void listStops() {

    int i;

    for (i = 0; i < stopCounter; i++) 
        printf("%s: %16.12f %16.12f %d\n", _stops[i].name, 
                                            _stops[i].latitude, 
                                            _stops[i].longitude, 
                                            _stops[i].num_lines);

}

/**
 * Looks for a given stop stored in the '_stops' array. If the name of the
 * stop is found, return the index of the array with the said stop. 
 * Else, return -1.
*/
int stopExists(char string[]) {

    int i;

    for (i = 0; i < stopCounter; i++) {
        
        if(compArgs(_stops[i].name, string) == YES)
            return i;

    }

    return -1;

}

/**
 * Shows the location (coordinates) of a given stop in the standard 
 * output. If the stop does not exist, a warning message is presented.
*/
void showStop(char string[]) {

    int i = stopExists(string);

    if (i >= 0) {

        printf("%16.12f %16.12f\n", _stops[i].latitude, _stops[i].longitude);
        return;

    }

    printf(NO_SUCH_STOP, string);

}

/**
 * Adds a new stop (if possible) to the '_stops' array. If 
 * the stop already exists, a warning message is presented.
*/
void addStop(char string[], double latitude, double longitude) {

    if (stopExists(string) > -1) {

        printf(STOP_ALREADY_EXISTS, string);

    } else if (stopCounter != MAX_STOPS) {

            strcpy(_stops[stopCounter].name, string);
            _stops[stopCounter].latitude = latitude;
            _stops[stopCounter].longitude = longitude;
            _stops[stopCounter].num_lines = 0;

            stopCounter++; /* Number of stops increased. */

    }       
}               


/* ---------------------------- Link functions ---------------------------- */


/**
 * Process the arguments read in the 'l' command line.
 * 
 * link: the new link to be created.
 * num1: the string storing the first value (cost).
 * num2: the string storing the second value (duration).
 * 
 * If the command line is successfully read, returns a valid link candidate.
*/
Link processLinkArgs(Link link, char num1[], char num2[]) {

    /* Bytes left in the command line after each word. */ 
    int a, b, c, e, f, g = BUFSIZ - 2;  

    /* Read line name: */
    if ((a = getArg(link.line.name, g, MAX_LINE_NAME)) > 0 && 
                                strlen(link.line.name) > 0) {

        /* Read origin stop name: */
        if ((b = getArg(link.orig.name, a, MAX_STOP_NAME)) > 0 && 
                                    strlen(link.orig.name) > 0) {

            /* Read destination stop name: */
            if ((c = getArg(link.dest.name, b, MAX_STOP_NAME)) > 0 && 
                                        strlen(link.orig.name) > 0) {

                /* Read cost value: */
                if ((e = getArg(num1, c, c)) > 0 && strlen(num1) > 0) {

                    /* Read duration value: */
                    if ((f = getArg(num2, e, e)) >= 0 && strlen(num2) > 0) {

                        /* Command line read successfully. */

                        /* Convert strings to floating point variables: */
                        link.cost = atof(num1);
                        link.duration = atof(num2);
                        link.isValid = YES;

                        return link;
                    }                  
                }
            }
        }
    }

    link.isValid = NO;
    return link;
}

/**
 * Process the data of the new link to be created.
 *  
 * link: the new link (to be created).
 * l: the index of the given line in the '_lines' array. 
 * o: the index of the given origin stop in the '_stops' array.
 * d: the index of the given destination stop in the '_stops' array.
 * 
 * If the link can't be created with the given arguments, a warning message 
 * is presented. 
*/
void processLink(Link link, int l, int o, int d) {

    /* Check if one or both of the stops exist in the line. */
    int origin = stopExistsInLine(_lines[l], _stops[o]);
    int destination = stopExistsInLine(_lines[l], _stops[d]);

    /* If the line has no itinerary: */
    if (_lines[l].num_stops == 0) {

        addFirstLink(link, l, o, d);
        return;
    } 

    /* If origin is the last stop and destination isn't in the line: */
    if (origin && !destination && compArgs(_lines[l].end, _stops[o].name)) {

            addLink(link, l, o, d, NO);
            return;
    }

    /* If destination's the first stop and origin isn't in the line: */
    if (!origin && destination && compArgs(_lines[l].start, _stops[d].name)) {

            addLink(link, l, o, d, YES);
            return;
    }
        
    /* If both exist in the line: */
    if (destination && origin && compArgs(_lines[l].end, _stops[o].name) &&
                                    compArgs(_lines[l].start, _stops[d].name)) {

            addLoopLink(link, l); /* Create a loop. */
            return;
    }
        
    printf(CANNOT_LINK);  

}

/**
 * Check if a given stop exists in the itinerary of a given line.
 * Returns YES if the stop exists in the line and returns NO if otherwise.
*/
int stopExistsInLine(Line line, Stop stop) {

    int i;

    for (i = 0; i < line.num_stops - 1; i++) {

        if (compArgs(_links[line.link_idx[i]].orig.name, stop.name) == YES ||
            compArgs(_links[line.link_idx[i]].dest.name, stop.name) == YES) {

            return YES;

        }
    }

    return NO;

}

/**
 * Creates the first link in the given line, with the given stops.
 * 
 * link: the new link.
 * l: the index of the line in the '_lines' array.
 * o: the index of the origin stop in the '_stops' array.
 * d: the index of the destination stop in the '_stops' array.
*/
void addFirstLink(Link link, int l, int o, int d) {

    if (assertNegativeValue(link) && linkCounter != MAX_LINKS) {

        /**
         * Initialize vector that will store the indexes in the '_links' array 
         * created for the given line, in route order. The vector will be 
         * re-sized every time a new link is created for the line.
        */
        _lines[l].link_idx = malloc(sizeof(int));
        _lines[l].link_idx[0] = linkCounter; 

        strcpy(_lines[l].start, link.orig.name);
        strcpy(_lines[l].end, link.dest.name);

        _lines[l].num_stops = 2;
        _lines[l].cost += link.cost;
        _lines[l].duration += link.duration;

        _stops[o].num_lines += 1;
        _stops[d].num_lines += 1;
        
        _links[linkCounter] = link;

        linkCounter++;  /* Number of links increased. */

    }
}

/**
 * Creates a new extensive/non-circular link in the given line with the 
 * given stops.
 * 
 * link: the new link.
 * l: the index of the line in the '_lines' array.
 * o: the index of the origin stop in the '_stops' array.
 * d: the index of the destination stop in the '_stops' array.
 * shift: if YES, add link to the beggining of the line; if NO, add to its end.
*/
void addLink(Link link, int l, int o, int d, int shift) {

    int i, size = _lines[l].num_stops;

    if (assertNegativeValue(link) && linkCounter != MAX_LINKS) {

        /* Re-size the vector to store one more link index. */
        _lines[l].link_idx = realloc(_lines[l].link_idx, size * sizeof(int));

        switch(shift) {

            case NO: /* Origin is the last stop of the line. */

                strcpy(_lines[l].end, link.dest.name);

                _lines[l].link_idx[size - 1] = linkCounter;
                _stops[d].num_lines += 1;

                break;

            case YES: /* Destination is the first stop of the line. */

                strcpy(_lines[l].start, link.orig.name);

                /* Shift vector one case to the right: */
                for (i = size - 1; i > 0; i--) 
                    _lines[l].link_idx[i] = _lines[l].link_idx[i-1];
                        
                _lines[l].link_idx[0] = linkCounter;
                _stops[o].num_lines += 1;

                break;
        }     
        
        _lines[l].num_stops += 1;
        _lines[l].cost += link.cost;
        _lines[l].duration += link.duration;

        _links[linkCounter] = link;

        linkCounter++; 

    }        
}

/**
 * Creates a new circular link in the given line, connecting the last stop to
 * the first of its itinerary.
 * 
 * link: the new link.
 * l: the index of the given line in the '_lines' array.
*/
void addLoopLink(Link link, int l) {

    int size = _lines[l].num_stops;

    if (assertNegativeValue(link) && linkCounter != MAX_LINKS) {

        /* Re-size the vector to store one more link index. */
        _lines[l].link_idx = realloc(_lines[l].link_idx, size * sizeof(int));

        strcpy(_lines[l].end, link.dest.name);

        _lines[l].link_idx[size - 1] = linkCounter;
        _lines[l].num_stops += 1;
        _lines[l].cost += link.cost;
        _lines[l].duration += link.duration;

        _links[linkCounter] = link;

        linkCounter++;  
    
    }
}

/**
 * Assert if the input-read link values of cost and duration are positive.
*/
int assertNegativeValue(Link link) {

    if (link.cost < 0 || link.duration < 0) {

        printf(NEGATIVE_VALUE);
        return NO;
    }

    return YES;

}


/* ------------------------ Intersection functions ------------------------ */


/**
 * Bubble sort algorithm.
 * Sorts a given list of strings in alphabetic order.
*/
void bubbleSort(char v[][MAX_LINE_NAME], int size) {

    int i, j;

    char aux[MAX_LINE_NAME];

    for (i = 0; i < size - 1; i++) {

        for (j = 0; j < size - 1 - i; j++) {

            if (strcmp(v[j], v[j + 1]) > 0) {

                strcpy(aux,v[j]);
                strcpy(v[j], v[j + 1]);
                strcpy(v[j + 1], aux);   

            }
        }
    }

    for (i = 0; i < size; i++) 
        printf(" %s", v[i]);

    putchar('\n');

}

/**
 * Find all the lines that intersect a given stop and present them
 * in alphabetic order.
*/
void lineIntersection(Stop stop) {
    
    int i, j = 0;

    char list[MAX_LINES][MAX_LINE_NAME];
    
        for (i = 0; i < lineCounter; i++) {

            if (j == stop.num_lines)  
                break;

            if (stopExistsInLine(_lines[i], stop)) {

                strcpy(list[j], _lines[i].name); 
                j += 1;

            }
        }

    printf("%s %d:", stop.name, j);

    bubbleSort(list, j);   

}


/* ---------------------------- Handle command ---------------------------- */

/* Maximum lenght of command line: BUFSIZ (8192 bytes). */


/**
 * Handles the 'c' command.
*/
void handleLineCommand() {

    int a, b;

    char name[MAX_LINE_NAME], option[BUFSIZ];

    if (hasArgs()) {

        if ((a = getArg(name, BUFSIZ - 2, MAX_LINE_NAME)) == 0) {

            addShowLine(name, NO);

        } else if ((b = getArg(option, a, a)) >= 0) {

            if (lineExists(name) == -1 || strlen(option) == 0) {

                /* Line does not exist, so sort option is irrelevant... */
                /* ... Or read an empty word. */
                addShowLine(name, NO);
                return;

            } 
                
            /* Check if the non-empty word is a valid sort option: */
            if (assertSortOption(option))
                addShowLine(name, YES);        
            
        }

    } else {

        listLines();
    }
}

/**
 * Handles the 'p' command.
*/
void handleStopCommand() {

    int a, b; /* Bytes left in the command line. */

    double lat, lon; /* Latitude and longitude (numeric values). */

    char str[MAX_STOP_NAME], val1[BUFSIZ], val2[BUFSIZ];

    if (hasArgs()) {

        if ((a = getArg(str, BUFSIZ - 2, MAX_STOP_NAME)) == 0) {

            showStop(str);

        } else if ((b = getArg(val1, a, a)) > 0 && !getArg(val2, b, b)) {

            /* Convert strings to floating point variables: */
            lat = atof(val1);
            lon = atof(val2);

            addStop(str, lat, lon);
        }

    } else {

        listStops();

    }   
}

/**
 * Handles the 'l' command.
*/
void handleLinkCommand() {

    int l, o, d; /* Line, origin and destination indexes. */

    char val1[BUFSIZ], val2[BUFSIZ];
    
    /* Read command line: */
    Link new_link = processLinkArgs(new_link, val1, val2);

    if (new_link.isValid) {
                            
        /* Check if line and stops exist. */ 
        l = lineExists(new_link.line.name);
        o = stopExists(new_link.orig.name);
        d = stopExists(new_link.dest.name);

        if (l < 0) {     

            /* Line does not exist. */
            printf(NO_SUCH_LINE, new_link.line.name);
            return;

        } else if (o < 0) {     

            /* Origin does not exist. */
            printf(NO_SUCH_STOP, new_link.orig.name);
            return;

        } else if (d < 0) {     

            /* Destination does not exist. */
            printf(NO_SUCH_STOP, new_link.dest.name);
            return;

        }  

        processLink(new_link, l, o, d);

    }                
}

/**
 * Handles the 'i' command.
*/
void handleIntersectionCommand() {

    int i, c, n = BUFSIZ - 1;

    /* Read buffer until end of line: */
    while ((c = getchar()) != '\n' && n > 0)
        n--;

    for (i = 0; i < stopCounter; i++) {

        if (_stops[i].num_lines > 1) 
            lineIntersection(_stops[i]);
        
    }
}


/* --------------------------------- Main --------------------------------- */


int main() {

    int c;

    while((c = getchar()) != EOF) {

        switch (c) {
            case 'c': handleLineCommand();
                break;
            case 'p': handleStopCommand(); 
                break;
            case 'l': handleLinkCommand();
                break;
            case 'i': handleIntersectionCommand();
                break;   
            case 'q': freeLineLinks(); 
                return 0; /* End the program. */
            default: 
                break; /* Ignore unknown commands. */
        }
    }

    return 0;
}