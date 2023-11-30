//
// Created by 35193 on 30/11/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "algoritmo.h"
#include "funcao.h"
// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
int trepa_colinas(int *sol, int **mat, int vert,int arest,int k, int num_iter)
{
    int *nova_sol, *nova_sol2, custo, custo_viz,custo_viz2, i;

    nova_sol = malloc(sizeof(int)*(vert+1));
    nova_sol2 = malloc(sizeof(int)*(vert+1));
    if(nova_sol == NULL || nova_sol2 ==NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    // Avalia solucao inicial
    custo = calcula_fit(sol,mat,vert,arest,k);
    for(i=0; i<num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, vert);
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol,mat,vert,arest,k);


        gera_vizinho_2(sol,nova_sol2,vert);
        custo_viz2 = calcula_fit(nova_sol2,mat,vert,arest,k);


        if(custo_viz2 > custo_viz)
        {
            custo_viz = custo_viz2;
            memcpy(nova_sol,nova_sol2,sizeof(int)*(vert+1));
        }

        // Aceita vizinho se o custo aumentar (problema de maximização)
        if(custo_viz > custo)
        {
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }
    }
    return custo;
}

int trepa_colinas_probabilistico(int sol[], int **mat, int vert, int num_iter, int a, int k)
{
    int *nova_sol ,*nova_sol2 ,custo ,custo_viz ,custo_viz2 ,i;

    // Aloca espa�o em mem�ria para guardar a nova solu��o
    nova_sol = malloc(sizeof(int)*(vert+1));
    nova_sol2 = malloc(sizeof(int)*(vert+1));

    // Caso n�o consiga fazer a aloca��o, envia aviso e termina o programa

    if (nova_sol == NULL || nova_sol2 == NULL){
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Avalia solu��o inicial
    custo = calcula_fit(sol, mat, vert,a,k);
    for(i = 0; i < num_iter; i++)
    {
        // Gera solu��o vizinha
        // Avalia solu��o vizinha
        gera_vizinho(sol, nova_sol, vert);
        custo_viz = calcula_fit(nova_sol, mat, vert,a,k);
        // Fica com a solu��o vizinha se o custo aumentar )

        gera_vizinho_2(sol,nova_sol2,vert);
        custo_viz2 = calcula_fit(nova_sol2,mat,vert,a,k);

        if(custo_viz2 <= custo_viz)
        {
            custo_viz = custo_viz2;
            substitui(nova_sol,nova_sol2,vert);
        }

        if (custo_viz <= custo)
        {
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }
        else
        {
            // Fica com a solu��o vizinha, apesar de pior, se o valor aleat�rio gerado for abaixo da probabilidade definida
            if (rand_01() < PROB_REPROD)
            {
                memcpy(sol,nova_sol,sizeof(int)*(vert+1));
                custo = custo_viz;
            }
        }
    }
    // Liberta a mem�ria usada para guardar a nova solu��o
    free(nova_sol);
    // Devolve o custo da melhor solu��o encontrada
    return custo;
}
void gera_vizinho(int a[], int b[], int n)
{
    int i, p1, p2,temp;

    for(i=1; i<=n; i++)
        b[i]=a[i];
    // Encontra posicao com valor 0
    do
        p1=random_l_h(1,n);
    while(b[p1] != 0);
    // Encontra posicao com valor 1
    do
        p2=random_l_h(1, n);
    while(b[p2] != 1);
    // Troca
    temp = b[p1];
    b[p1] = b[p2];
    b[p2] = temp;
}
void gera_vizinho_2(int a[], int b[], int n)
{
    int i, p1, p2,temp;

    for(i=1; i<=n; i++)
        b[i]=a[i];
    // Encontra posicao com valor 0
    do
        p1=random_l_h(1,n);
    while(b[p1] != 0);
    // Encontra posicao com valor 1
    do
        p2=random_l_h(1, n);
    while(b[p2] != 1);
    // Troca
    temp = b[p1];
    b[p1] = b[p2];
    b[p2] = temp;
}


void substitui(int a[], int b[], int n)
{
    int i;
    for(i=1; i<=n; i++)
        a[i]=b[i];
}
//*************************************Final Trepa-Colinas***********************//
//***********************************Modelo Evolutivo **************************//

// Torneio binário
void tournament(int* pop[POP_SIZE], int* parents[POP_SIZE], int qualidade[POP_SIZE], int v) {
    int i, x1, x2,j=0;
    // Realiza POP_SIZE torneios
    for (i = 0; i <POP_SIZE; i++) {
        x1 = random_l_h(0, POP_SIZE-1); // Escolher um individuo

        do
            x2 = random_l_h(0, POP_SIZE-1); // Escolher outro individuo
        while (x1 == x2);

        if (qualidade[x1] > qualidade[x2])   // Problema de maximizacao
            memcpy(parents[i], pop[x1], sizeof(int) * v+1);
        else
            memcpy(parents[i], pop[x2], sizeof(int) * v+1);

        j++;
    }
}
void tournament2(int* pop[POP_SIZE], int* parents[POP_SIZE], int qualidade[POP_SIZE], int M) {
    int i, j, x, k, winner;
    int pos[T_SIZE];

    if (T_SIZE < 2 || T_SIZE > M) {
        fprintf(stderr, "<ERRO> T_SIZE tem de estar entre 2 e M.\n");
        exit(-1);
    }

    // Realiza POP_SIZE torneios
    for (i = 0; i < POP_SIZE; i++) {
        for (k = 0; k < T_SIZE; k++) {
            pos[k] = -1;
            do {
                x = random_l_h(0, POP_SIZE - 1);
                for (j = 0; j < k; j++)
                    if (pos[j] == x)
                        break;
            } while (j != k);

            pos[k] = x;
        }

        winner = -1;
        for (k = 0; k < T_SIZE; k++)
            if (winner == -1 || qualidade[pos[k]] > qualidade[winner])
                winner = pos[k];

        memcpy(parents[i], pop[winner], sizeof(int) * M);
    }
}
void geneticOperators(int* parents[POP_SIZE], int* offspring[POP_SIZE], int qualidade[POP_SIZE],int** matriz, int v, int a,int k) {


    // Recombinação
    crossover(parents, offspring, qualidade,matriz, v, a, k);
    // Mutação
    mutation(parents, offspring, qualidade,matriz, v, a, k);


    //OPERADORES GENETICOS NOVOS


    //Recombinação com 3 Pontos de corte
    //crossover3P(parents,offspring,qualidade,v);

    //Mutação Swap
    //mutation_swap(offspring,v);



}
void crossover(int* parents[POP_SIZE], int* offspring[POP_SIZE], int qualidade[POP_SIZE],int** matriz, int v, int a,int k)
{
    int i, j,point;


    for (i = 0; i < POP_SIZE; i += 2) {
        if (rand_01() < PROB_REPROD) {
            point = random_l_h(1, v);
            for (j = 1; j < point; j++)
            {
                offspring[i][j] = parents[i][j]; // Colocar o cromossoma j do pai i no cromossoma j do filho i
                offspring[i + 1][j] = parents[i + 1][j];
            }
            for (j=point; j<=v; j++)
            {
                offspring[i][j] = parents[i + 1][j];
                offspring[i + 1][j] = parents[i][j];
            }

        }
        else {
            memcpy(offspring[i], parents[i], sizeof(int) * (v+1));
            memcpy(offspring[i + 1], parents[i + 1], sizeof(int) * (v+1));
        }
    }
}

void crossover3P(int* parents[POP_SIZE], int* offspring[POP_SIZE], int qualidade[POP_SIZE], int v) {
    int i, j, point1, point2, point3;
    for (i = 0; i < POP_SIZE; i += 2) {
        if (rand_01() < PROB_REPROD) {
            point1 = random_l_h(1, v - 2);
            for (j = 1; j < point1; j++) {
                offspring[i][j] = parents[i][j]; // Colocar o cromossoma j do pai i no cromossoma j do filho i
                offspring[i + 1][j] = parents[i + 1][j];
            }
            point2 = random_l_h(point1, v - 1);
            for (j = point1; j < point2; j++) {
                offspring[i][j] = parents[i + 1][j];
                offspring[i + 1][j] = parents[i][j];
            }
            point3 = random_l_h(point2, v);
            for (j = point2; j < point3; j++) {
                offspring[i][j] = parents[i][j];
                offspring[i + 1][j] = parents[i + 1][j];
            }
            for (j = point3; j < v; j++) {
                offspring[i][j] = parents[i + 1][j];
                offspring[i + 1][j] = parents[i][j];
            }
        } else {
            memcpy(offspring[i], parents[i], sizeof(int) * (v+1));
            memcpy(offspring[i + 1], parents[i + 1], sizeof(int) * (v+1));
        }
    }
}

void mutation(int* parents[POP_SIZE], int* offspring[POP_SIZE], int qualidade[POP_SIZE],int** matriz, int v, int a,int k) {
    int i, j, g1, g2, p1, count;
    int best_ratio_index, worst_ratio_index,pos;

    for (int i = 1; i < POP_SIZE; i++)
    {
        for(int j=1;j<=v;j++)
        {
            if(rand_01()<PROB_MUTAC)
            {
                offspring[i][j] = !(offspring[i][j]); //INVERTE O NUMERO NA POSICAO [I][J] OU SEJA 0=1 OU 1=0

                if(offspring[i][j] == 1) //DEVIDO A INVERSAO DO NUMERO TEMOS QUE INVERTER UM TAMBEM AO CONTRARIO
                {
                    do // Escolher um elemento
                    {
                        pos = random_l_h(1,v);
                    }while (offspring[i][pos] != 1);
                    offspring[i][pos] = 0;
                }
                if(offspring[i][j] == 0)
                {
                    do // Escolher um elemento
                    {
                        pos = random_l_h(1,v);
                    }while (offspring[i][pos] != 0);
                    offspring[i][pos] = 1;
                }
            }
        }
        j=1;
    }
}

void mutation_swap(int* offspring[POP_SIZE], int v)
{
    int i,obj1,obj2;
    for(i=0;i<POP_SIZE;i++)
    {
        if (rand_01()<PROB_MUTAC)
        {
            do
            {
                obj1=random_l_h(1,v);
            } while (offspring[i][obj1] != 1);

            do
            {
                obj2 = random_l_h(1,v);
            } while (offspring[i][obj2]!=0);

            offspring[i][obj1] = 0;
            offspring[i][obj2] = 1;
        }
    }
};
