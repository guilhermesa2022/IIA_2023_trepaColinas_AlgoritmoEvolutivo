//
// Created by 35193 on 30/11/2023.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "algoritmo.h"
#include "funcao.h"
#define linhas 2

//*********************************Trepa colinas *************************//

// funcao Mudificada codigo: mod223
int **init_dados(char *filename ,int *v ,int *a, int *k, int*somaDoCustos){
    int **matbin = NULL;
    FILE *f;
    int  l=0, m=0, custo = 0;


    f=fopen(filename,"r");
    if(!f){
        printf("Erro no acesso ao ficheiro dos dados\n");
        exit(1);
    }

    fscanf(f, " k %d",k);
    fscanf(f, " p edge %d %d",v,a);
    ////////cria matriz com v linhas e v colunas com intenção de armazenar valores binarios
    matbin = malloc(sizeof(int*)*(*v));
    for(int i = 0; i < (*v); i++){
        matbin[i] = malloc(sizeof(int)*(*v));
    }
    ////////////////////////////////////////////////// se matbin der erro a abrir;
    if(matbin == NULL){
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }
    //////////////////////////////////////////////////preencher tudo com zero
    for(int i=0;i < (*v); i++)
        for(int j=0; j < (*v) ;j++)
            matbin[i][j] = 0;

    while (fscanf(f," e %d %d %d", &l, &m, &custo)!= EOF)
    {
        *somaDoCustos += custo;
        matbin[l-1][m-1] = custo;
        matbin[m-1][l-1] = custo;
    }
    printf(" \ndados do ficheiro => k = %d v = %d a = %d \n",(*k),(*v),(*a));
    fclose(f);
    return matbin;
}


void init_rand()
{
    srand((unsigned)time(NULL));
}
void mostraMat(int **p, int nLin, int nCol){
    int i,j,pos,x=0;

    for(i=0; i<nCol; i++){
        printf("\t \t \t");
        for(j=0; j<nLin; j++){
            printf("|  %d  |", p[j][i]); // escreve o x ou o O no tabuleiro
        }
        printf("\n");
    }
}

// funcao Mudificada codigo: mod223
void gera_sol_inicial(int *sol, int v, int k)
{
    int i, x;
    //iniciação de todas a posições
    for(i=1; i<=v; i++)
        sol[i]=0;

    for(i=0; i<k; i++){
        do{
            x = random_l_h(1,(v));
        }while(sol[x] != 0);
        sol[x]=1;
    }
}

int random_l_h(int min, int max)
{
    return min + rand() % (max-min+1);
}
void mostra_sol(int *sol,int vert)
{
    int j=0;
    for(int i=1;i<=vert;i++)
    {
        printf(" \n Sol[i] = %d ",sol[i]);
        j++;
    }
    printf(" j =  %d",j);
}
void escreve_sol(int *sol, int v, int cont)
{
    int i;
    printf("\nConjunto : ");
    printf("{");
    for(i=1; i<=v; i++)
        if(sol[i]==1)
            printf("%d ",i);
    printf("}");
    cont++;

    printf("\n");
}
//*************************************Final trepa colinas********************//

void initPop(int* pop[POP_SIZE],int** matriz, int v, int a,int k) {
    int i;
    // Gerar cada individuo da populacao
    for (i = 0; i <=POP_SIZE; i++) {

        pop[i] = malloc(sizeof(int) * (v+1));

        if (pop[i] == NULL) {
            fprintf(stderr, "<ERRO> Nao foi possivel alocar memoria para a solucao inicial.\n");
            exit(-1);
        }


        initSol(pop[i],v,k);
    }
}

void initSol(int* sol, int v,int k) {
    int i, j, pos,n;

    // Inicializar a solucao
    for (i = 1; i <= v; i++)
        sol[i] = 0;

    for (int j = 0; j<k;j++) { // Atribuir k elementos ao conjunto (de modo a dividir os elementos igualmente pelo subconjunto)
        do // Escolher um elemento sem conjunto
        {
            pos = random_l_h(1,v);
        }while (sol[pos] != 0);

        sol[pos] = 1; // Atribuir o elemento ao subconjunto
    }
}

void escrevePop(FILE* fs, int* pop[POP_SIZE], const int qualidade[POP_SIZE], int v, int a, int k) {
    int i;
    for (i = 0; i < POP_SIZE; i++) {
        fprintf(fs, "\n-- Individuo %d --", i);
        escreveSol(fs, pop[i], qualidade[i],v,a,k);
    }
}

void escreveSol(FILE* fs, const int* sol, const int qualidade, int v, int a, int k) {
    int i, j;
    for (j = 1; j <= v; j++) {
        if (sol[j] == i) {
            fprintf(fs, " %d", j);
        }
    }
    fprintf(fs, " ]");

    fprintf(fs, "\nQualidade: %d", qualidade);
}

float rand_01() { return ((float)rand()) / RAND_MAX; }

void escreve_sol_final(int *sol,int qualidade,int v)
{
    int i;
    printf("\nConjunto : ");
    printf("{");
    for(int i=1; i<=v; i++)
        if(sol[i]==1)
            printf("%d ",i);
    printf("}");

    printf("\nQualidade: %d", qualidade);

    printf("\n");
}




void initPop_Evol(int* pop[POP_SIZE],int** matriz, int v, int a,int k) {
    int i;
    // Gerar cada individuo da populacao
    for (i = 0; i <=POP_SIZE; i++) {

        pop[i] = malloc(sizeof(int) * (v+1));

        if (pop[i] == NULL) {
            fprintf(stderr, "<ERRO> Nao foi possivel alocar memoria para a solucao inicial.\n");
            exit(-1);
        }


        initSol_Evol(pop[i],v,a,k,matriz);
    }
}

void initSol_Evol(int* sol, int v,int a,int k, int** mat) {
    int i, j, pos,n,custo_viz,custo,custo_viz2;
    int *nova_sol,*nova_sol2;

    nova_sol = malloc(sizeof(int)*(v+1));
    nova_sol2 = malloc(sizeof(int)*(v+1));

    // Inicializar a solucao
    for (i = 1; i <= v; i++)
        sol[i] = 0;

    for (int j = 0; j<k;j++) { // Atribuir k elementos ao conjunto (de modo a dividir os elementos igualmente pelo subconjunto)
        do // Escolher um elemento sem conjunto
        {
            pos = random_l_h(1,v);
        }while (sol[pos] != 0);

        sol[pos] = 1; // Atribuir o elemento ao subconjunto
    }

    //Verifica a o fitness da solução
    custo = calcula_fit(sol,mat,v,a,k);

    //Gera vizinho da solucao
    gera_vizinho(sol,nova_sol,v);

    //Verifica a o fitness da solução
    custo_viz = calcula_fit(nova_sol,mat,v,a,k);

    gera_vizinho_2(sol,nova_sol2,v);
    custo_viz2 = calcula_fit(nova_sol2,mat,v,a,k);


    if(custo_viz2 > custo_viz)
    {
        custo_viz = custo_viz2;
        substitui(nova_sol,nova_sol2,v);
    }
    //Avalia as duas soluções
    if(custo_viz > custo)
    {
        //Substitui a solucao melhor
        substitui(sol,nova_sol,v);
    }
}