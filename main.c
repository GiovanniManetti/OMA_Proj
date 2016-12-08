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
    bool hasSolution;

    /**
     * Variables of the problem (X in the model)
     */
    int**** solution;



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
	 
    int i, j, m, t;
	
    for (i = 0; i < nCells; i++) {
        problem.costs[i] = malloc(nCells*sizeof( int**));
        solution[i] = malloc(nCells*sizeof( int**));
        for (j = 0; j < nCells; j++) {
            problem.costs[i][j] = malloc(nCustomerTypes*sizeof( int*));
            solution[i][j] =  malloc(nCustomerTypes*sizeof( int*));
            for (m = 0; m < nCustomerTypes; m++) {
                problem.costs[i][j][m] =  malloc(nTimeSteps*sizeof( int));
                solution[i][j][m] = malloc(nTimeSteps*sizeof( int));
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
    if(fgets(line, sizeof(line), f)!= NULL){
        for (m = 0; m < nCustomerTypes; m++) {
        sscanf (line, "%d", &problem.n[m]);
    }

    //riga bianca
    fgets(line, sizeof(line), f);

    //lettura di (t * m) matrici di costi
    for (m = 0; m < nCustomerTypes; m++) {
        for (t = 0; t < nTimeSteps; t++) {

            fgets(line, sizeof(line), f);// linea con m e t (non mi serve memorizzarla)

            for (i = 0; i < nCells; i++) {
                if(fgets(line, sizeof(line), f)!= NULL) {// linea della matrice c_{ij} per t ed m fissati
                    for (j = 0; j < nCells; j++) {
                        sscanf (line, "%d", &problem.costs[i][j][m][t]);
                    }
                }
            }
        }
    }

    //riga bianca
    fgets(line, sizeof(line), f);
    // vettore di attività da svolgere
    if(fgets(line, sizeof(line), f)!= NULL) {
        for (i = 0; i < nCells; i++) {
            sscanf (line, "%d", &problem.activities[i]);
        }
    }


    //riga bianca
    fgets(line, sizeof(line), f);

    // acquisizione persone presenti nelle varie celle
    for (m = 0; m < nCustomerTypes; m++) {
        for (t = 0; t < nTimeSteps; t++) {
            fgets(line, sizeof(line), f);// linea con m e t (non mi serve memorizzarla)

            for (i = 0; i < nCells; i++) {
                if(fgets(line, sizeof(line), f)!= NULL) {// linea di persone di tipo m presenti nella cella i al tempo t
                    for (j = 0; j < nCells; j++) {
                        sscanf (line, "%d", &problem.usersCell[i][m][t]);
                        }
                    }
                }
            }
        }

    }

        /* chiude il file */
    fclose(f);










    /**
        da scrivere: liberare tutte la malloc
    **/


    return 0;
}
