//
// Created by 35193 on 06/12/2023.
//

#ifndef IIAPT2_ALGORITMO_H
#define IIAPT2_ALGORITMO_H
#define MAX_OBJ 500

struct info
{
    int     popsize;// Tamanho da população
    float   pm;// Probabilidade de mutação
    float   pr;// Probabilidade de recombinação
    int     tsize;// Tamanho do torneio para seleção do pai da próxima geração
    float   ro;// Constante para avaliação com penalização
    int     numGenes;// Número de objetos que se podem colocar na mochila
    int     numGenerations;// Número de gerações
    int numVertice;

     int sumaDoscustos;
};

// Individuo (solução)
typedef struct individual chrom, *pchrom;

struct individual
{
    int     *p;// Solução
    float   fitness;// Valor da qualidade da solução
    int     valido;// 1 se for uma solução válida e 0 se não for
};

void crossover(pchrom parents, struct info d, pchrom offspring);
int trepa_colinas_probabilistico(int sol[], int **mat, int vert, int num_iter, int arestas, int k);
void gera_vizinho(int a[], int b[], int n);
void substitui(int a[], int b[], int n);
void mutation(pchrom offspring, struct info d);
void tournament(pchrom pop, struct info d, pchrom parents);
void genetic_operators(pchrom parents, struct info d, pchrom offspring);
void repair(pchrom offspring, struct info d);
void tournament2(pchrom pop, struct info d, pchrom parents);
void tournament3(pchrom pop, struct info d, pchrom parents, chrom beste);
void mutation2(pchrom offspring, struct info d);
void crossover2(pchrom parents, struct info d, pchrom offspring, chrom melhor);
void genetic_operators2 (pchrom parents, struct info d, pchrom offspring, chrom melhor);
void crossover3(pchrom parents, struct info d, pchrom offspring);
void mutation_swap(pchrom offspring, struct info d);
#endif //IIAPT2_ALGORITMO_H
