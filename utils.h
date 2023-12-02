//
// Created by 35193 on 30/11/2023.
//

#ifndef IIA_2023_TREPACOLINAS_ALGORITMOEVOLUTIVO_UTILS_H
#define IIA_2023_TREPACOLINAS_ALGORITMOEVOLUTIVO_UTILS_H
#include <stdio.h>
#define DEFAULT_RUNS 10
#define POP_SIZE 120
#define GER_NUM 1000
#define PROB_REPROD 0.05
#define PROB_MUTAC 0.2
#define PROB_REC 0.01
#define T_SIZE 4


int  **init_dados(char *filename,int *v,int *a,int *k, int*somaDoCustos);
int random_l_h(int min, int max);

void init_rand();

void mostraMat(int **p, int nLin, int nCol);
void gera_sol_inicial(int *sol, int v,int k);
void mostra_sol(int *sol,int vert);
void escreve_sol(int *sol, int vert, int cont);
//************************trepa colinas*******************//

void initSol(int* sol, int v,int k);
void initPop(int* pop[POP_SIZE],int** matriz, int v, int a,int k);
void escrevePop(FILE* fs, int* pop[POP_SIZE], const int qualidade[POP_SIZE], int v, int a, int k);
void escreveSol(FILE* fs, const int* sol, const int qualidade, int v, int a, int k);
float rand_01();
void escreve_sol_final(int *sol,int quantidade,int v);

void initSol_Evol(int* sol, int v,int a,int k, int **mat);
void initPop_Evol(int* pop[POP_SIZE],int** matriz, int v, int a,int k);
#endif //IIA_2023_TREPACOLINAS_ALGORITMOEVOLUTIVO_UTILS_H
