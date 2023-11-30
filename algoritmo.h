//
// Created by 35193 on 30/11/2023.
//

#ifndef IIA_2023_TREPACOLINAS_ALGORITMOEVOLUTIVO_ALGORITMO_H
#define IIA_2023_TREPACOLINAS_ALGORITMOEVOLUTIVO_ALGORITMO_H
int trepa_colinas(int sol[], int **mat, int vert,int arest,int k, int num_iter);
int trepa_colinas_probabilistico(int sol[], int **mat, int vert, int num_iter,int a, int k);
void gera_vizinho(int a[], int b[], int n);
void gera_vizinho_2(int a[], int b[], int n);
void substitui(int a[], int b[], int n);
//************************trepa colinas*******************//
void tournament(int* pop[POP_SIZE], int* parents[POP_SIZE], int qualidade[POP_SIZE], int v);

void tournament2(int* pop[POP_SIZE],int* parents[POP_SIZE],int qualidade[POP_SIZE],int v);
void geneticOperators(int* parents[POP_SIZE], int* offspring[POP_SIZE], int qualidade[POP_SIZE],int** matriz, int v, int a,int k);

void crossover(int* parents[POP_SIZE], int* offspring[POP_SIZE], int qualidade[POP_SIZE],int** matriz, int v, int a,int k);
void crossover3P(int* parents[POP_SIZE], int* offspring[POP_SIZE], int qualidade[POP_SIZE], int v);

void mutation(int* parents[POP_SIZE], int* offspring[POP_SIZE], int qualidade[POP_SIZE],int** matriz, int v, int a,int k);
void mutation_swap(int* offspring[POP_SIZE], int v);

#endif //IIA_2023_TREPACOLINAS_ALGORITMOEVOLUTIVO_ALGORITMO_H
