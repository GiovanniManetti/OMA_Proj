#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "heuristic.h"

int min(int,int);

void createRandSol(int**** solution,struct Data problem,int* Totcost,int nCells,int nCustomerTypes,int nTimeSteps){
    int i,j,m,t,temp;
    int satisfied;

    /* Intializes random number generator */
    srand(time(NULL));
    printf("%d", rand()%152+1);
    for (i = 0; i < nCells; i++) {
        satisfied=0;
        for (j = 0; j < nCells && !satisfied; j++) {
            if(i!=j && problem.activities[j]>0){
                for (m = 0; m < nCustomerTypes && !satisfied; m++) {
                    for(t = 0; t < nTimeSteps && !satisfied; t++){
                        if(problem.usersCell[i][m][t]){
                            // numero di persone random spostate in una cella
                            printf("temp: %d %d\n", problem.activities[j], problem.usersCell[i][m][t]);
                            temp=rand()% (min(problem.activities[j],problem.usersCell[i][m][t]));
                            printf("temp: %d\n", temp);
                            // se il numero di task fatti da temp persone è > del numero di task nelle celle allora
                            //calcola il numero giusto di persone da spostare per fare tutti i task
                            if(temp * problem.n[m] > problem.activities[j]){
                                temp=floor(problem.activities[j]/ problem.n[m]);
                                satisfied=1;
                            }
                            // numero di task fatti da temp persone
                            solution[i][j][m][t]+=temp;
                            problem.usersCell[i][m][t]-=solution[i][j][m][t];
                            problem.activities[j]-=temp * problem.n[m];
                            *Totcost+= (temp * problem.costs[i][j][m][t]);

                        }
                    }
                }
            }
        }
    }

    printf("%d\n", *Totcost);

    for (i = 0; i < nCells; i++) {
        satisfied=0;
        for (j = 0; j < nCells && !satisfied; j++) {
            if(i!=j && problem.activities[j]){
                for (m = 0; m < nCustomerTypes && !satisfied; m++) {
                    for(t = 0; t < nTimeSteps && !satisfied; t++){
                        if(problem.usersCell[i][m][t]){
                            if (problem.activities[j] > problem.n[m] * problem.usersCell[j][m][t]) {
                            solution[j][i][m][t] += problem.usersCell[j][m][t];
                            problem.usersCell[j][m][t] -= solution[j][i][m][t];
                        }
                        else {
                            solution[j][i][m][t] += floor(problem.activities[j] / problem.n[m]);
                            satisfied=1;
                        }

                        *Totcost += solution[j][i][m][t] * problem.costs[j][i][m][t];
                        printf("tot_temp: %d\n", *Totcost);
                        problem.activities[j] -= problem.n[m]*solution[j][i][m][t];
                        }
                    }
                }
            }
        }
    }

    printf("%d\n", *Totcost);
}

int min(int a,int b){
    if(a>=b)
        return b;
    else
        return a;
}
