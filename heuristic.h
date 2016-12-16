#include <stdio.h>
#include <stdlib.h>

struct Data {
    /**
     * The costs ijkt
     */
    int**** costs;

    /**
     * number of activity done by one user of type k
     */
    int* n;

    /**
     * Activities to be done in node i during the given time horizon
     */
    int* activities;

    /**
     * Number of users of type m in i during time period t (ikt)
     */
    int*** usersCell;
};

void createRandSol(int****,struct Data,int*,int,int,int);
