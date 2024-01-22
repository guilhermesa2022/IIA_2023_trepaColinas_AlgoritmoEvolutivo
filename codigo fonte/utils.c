//
// Created by 35193 on 06/12/2023.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "algoritmo.h"
#include "funcao.h"


int **inicializar_Dados_Ficheiro(char *filename ,int *vert ,int *arestas, int *k, int *somaDoCustos){
    int **matriz = NULL;
    FILE *f;
    int  l=0, m=0, custo = 0;


    f=fopen(filename,"r");
    if(!f){
        printf("Erro no acesso ao ficheiro dos dados\n");
        exit(1);
    }

    fscanf(f, " k %d",k);
    fscanf(f, " p edge %d %d", vert, arestas);
    matriz = malloc(sizeof(int*)*(*vert));
    for(int i = 0; i < (*vert); i++){
        matriz[i] = malloc(sizeof(int)*(*vert));
    }

    if(matriz == NULL){
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }

    for(int i=0;i < (*vert); i++)
        for(int j=0; j < (*vert) ;j++)
            matriz[i][j] = 0;

    while (fscanf(f," e %d %d %d", &l, &m, &custo)!= EOF)
    {
        *somaDoCustos += custo;
        matriz[l-1][m-1] = custo;
        matriz[m-1][l-1] = custo;
    }
    printf(" \ndados do ficheiro => k = %d vert = %d arestas = %d \n",(*k),(*vert),(*arestas));
    fclose(f);
    return matriz;
}

void printMatriz(int** matriz, int lado){
    for (int i = 0; i < lado; ++i) {
        for (int j = 0; j < lado; ++j) {
            printf("%d ",matriz[i][j]);
        }
        printf("\n");
    }
}

void init_rand()
{
    srand((unsigned)time(NULL));
}

void gera_sol_inicial(int *sol, int v, int k){
    int x;
    for(int i=0; i<v; i++)
        sol[i]=0;

    for(int i=0; i<k; i++){
        do{
            x = random_l_h(0,(v-1));
        }while(sol[x] != 0);
        sol[x]=1;
    }
}

void gera_sol_inicial2(int *sol, int v, int k, int **mat){
    int random1;
    for (int i = 0; i < v; ++i) {
        sol[i] = 0;
    }
    int inicial = random_l_h(0, v -1);
    sol[inicial] = 1;

    for(int i = 0; i < k - 1; i++){
        do {
            random1 = random_l_h(0, v - 1);
        } while (mat[inicial][random1] != 0 || sol[random1] == 1);
        sol[random1] = 1;
        inicial = random1;
    }

}

int random_l_h(int min, int max){
    return min + rand() % (max-min+1);
}

void escreve_sol(int *sol, int v)
{
    printf("\nConjunto : ");
    printf("[");
    for(int i= 0; i<v; i++)
        if(sol[i]==1)
            printf("%d ",i+1);
    printf("]");


    printf("\n");
}

int flip(){
    if ((((float)rand()) / RAND_MAX) < 0.5)
        return 0;
    else
        return 1;
}

pchrom init_pop(struct info d)
{
    int     i, j, pos;
    pchrom  indiv = NULL;

    indiv = malloc(sizeof(chrom)*d.popsize);

    if (indiv==NULL){
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }

    for (i=0; i<d.popsize; i++)
    {
        indiv[i].p = malloc(sizeof(int) * (d.numVertice));

        if(indiv[i].p == NULL){
            printf("\nerro na malloc init_pop ");
            exit(1);
        }

        for (int k = 0; k < d.numVertice; ++k) {
            indiv[i].p[k] = 0;
        }

        for (j=0; j<d.numGenes; j++) {
            do {
                pos = random_l_h(0, d.numVertice - 1);
            } while (indiv[i].p[pos] == 1);
            indiv[i].p[pos] = 1;
        }
    }
    return indiv;
}

float rand_01() { return ((float)rand()) / RAND_MAX; }

void evaluate(pchrom pop, struct info d, int **mat){
    int num1;
    for (int i = 0; i < d.popsize; ++i) {
        num1 = 0;
        pop[i].fitness = calcula_fit(pop[i].p, mat, d.numVertice, 1, 3);

    }
}

chrom get_best(pchrom pop, struct info d, chrom best)
{
    int i;

    for (i=0; i<d.popsize; i++)
    {
        if (best.fitness > pop[i].fitness)
            best=pop[i];
    }
    return best;
}

void write_best(chrom x, struct info d)
{
    int i;

    printf("\nBest individual: %4.1f\n", x.fitness);
    for (i=0; i<d.numVertice; i++)
        printf("%d", x.p[i]);
    putchar('\n');
}