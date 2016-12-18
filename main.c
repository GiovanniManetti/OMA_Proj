#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "heuristic.h"

#define NLINE 2000


int main(int argc, char *argv[])
{
    /**
     * Number of periods
     */
    int nTimeSteps;

    /**
     * Number of customer types
     */
    int nCustomerTypes;

    /**
     * Number of cells
     */
    int nCells;

    /**
     * Problem structure for parameters
     */
    struct Data problem;

    /**
     * Flag equals to true if the problems has a solution
     */
    //bool hasSolution;

    /**
     * Variables of the problem (X in the model)
     */
    int**** solution;

    /**
     * Objective function value
     */
    int objFunc=0;



    char line[NLINE];

    FILE *f;

		/* apre il file */
    f = fopen(argv[1], "r");
    if( f == NULL ) {
        perror("Errore in apertura del file");
        exit(1);
    }


	/* lettura file e costruzione strutture dati*/
	// lettura numero celle, numero periodi e tipi di utente


    if(fgets(line, sizeof(line), f)!= NULL){

        sscanf (line, "%d %d %d", &nCells, &nTimeSteps, &nCustomerTypes);
    }

    // Memory allocation di matrce di costi e soluzioni
    solution = malloc(nCells*sizeof( int***));
    problem.costs = malloc(nCells*sizeof( int***));
    int i,j,m,t,tmp;
    float a;
    for (i = 0; i < nCells; i++) {
        problem.costs[i] = malloc(nCells*sizeof( int**));
        solution[i] = malloc(nCells*sizeof( int**));
        for (j = 0; j < nCells; j++) {
            problem.costs[i][j] = malloc(nCustomerTypes*sizeof( int*));
            solution[i][j] =  malloc(nCustomerTypes*sizeof( int*));
            for (m = 0; m < nCustomerTypes; m++) {
                problem.costs[i][j][m] =  malloc(nTimeSteps*sizeof( int));
                solution[i][j][m] = malloc(nTimeSteps*sizeof( int));
                for(t = 0; t < nTimeSteps; t++){
                    solution[i][j][m][t] = 0;
                }
            }
        }
    }


    /** memory allocation per numero di attività fatte da ogni utente,
    *   numero di attività da fare nelle celle e numero di utenti nelle celle
    **/

    problem.n = malloc(nCustomerTypes*sizeof(int));
    problem.activities = malloc(nCells*sizeof(int));
    problem.usersCell = malloc(nCells*sizeof( int**));
    for (i = 0; i < nCells; i++) {
        problem.usersCell[i] = malloc(nCustomerTypes*sizeof( int*));
        for (m = 0; m < nCustomerTypes; m++) {
            problem.usersCell[i][m] = malloc(nTimeSteps*sizeof( int));
        }
    }


    //riga bianca
    fgets(line, sizeof(line), f);

    // acquisizione dati. Numero task per tipo di utente
    for (m = 0; m < nCustomerTypes; m++)
        fscanf(f, "%d", &problem.n[m]);


    //riga bianca
    fgets(line, sizeof(line), f);

    //lettura di (t * m) matrici di costi
    for (m = 0; m < nCustomerTypes; m++) {
        for (t = 0; t < nTimeSteps; t++) {

            fscanf (f, "%d %d",&tmp,&tmp);// linea con m e t (non mi serve memorizzarla)
            printf("%d %d\n",m,t);
            for (i = 0; i < nCells; i++) {
                for (j = 0; j < nCells; j++){
                    fscanf (f, "%f ",&a);
                    problem.costs[i][j][m][t]= (int) a;
                    printf("%d\t",problem.costs[i][j][m][t]);
                }
                printf("\n");
            }
        }
    }


    // vettore di attività da svolgere
    for (i = 0; i < nCells; i++) {
        fscanf(f, "%d", &problem.activities[i]);
        printf("%d ",problem.activities[i]);
    }
     printf("\n");

    // acquisizione persone presenti nelle varie celle
    for (m = 0; m < nCustomerTypes; m++) {
        for (t = 0; t < nTimeSteps; t++) {
            fscanf(f, "%d %d",&tmp,&tmp);// linea con m e t (non mi serve memorizzarla)

            for (i = 0; i < nCells; i++) {
                fscanf(f, "%d", &problem.usersCell[i][m][t]);
                printf("%d ",problem.usersCell[i][m][t]);
                }
                printf("\n");
            }
        }


        /* chiude il file */
    fclose(f);

    createRandSol(solution,problem,&objFunc,nCells,nCustomerTypes,nTimeSteps);

    printf("%d", objFunc);




    /**
        liberare tutte la malloc
    **/
    for (i = 0; i < nCells; i++) {
        for (j = 0; j < nCells; j++) {
            for (m = 0; m < nCustomerTypes; m++) {
                free(problem.costs[i][j][m]);
                free(solution[i][j][m]);
            }
            free(problem.costs[i][j]);
            free(solution[i][j]);
        }
        free(problem.costs[i]);
        free(solution[i]);
    }
    free(problem.costs);
    free(solution);

    free(problem.n);
    free(problem.activities);
    for (i = 0; i < nCells; i++) {
        for (m = 0; m < nCustomerTypes; m++) {
            free(problem.usersCell[i][m]);
        }
        free(problem.usersCell[i]);
    }
    free(problem.usersCell);

    return 0;
}
