//
// Created by 35193 on 06/12/2023.
//

#include "algoritmo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "funcao.h"


int trepa_colinas_probabilistico(int sol[], int **mat, int vert, int num_iter, int arestas, int k)
{
    int *nova_sol = NULL ,*nova_sol2 = NULL ,custo ,custo_viz ,custo_viz2 ,i;

    nova_sol = malloc(sizeof(int)*vert);
    nova_sol2 = malloc(sizeof(int)*vert);


    if (nova_sol == NULL || nova_sol2 == NULL){
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Avalia solu��o inicial
    custo = calcula_fit(sol, mat, vert, arestas, k);

    for(i = 0; i < num_iter; i++)
    {
        gera_vizinho(sol, nova_sol, vert);
        custo_viz = calcula_fit(nova_sol, mat, vert,arestas,k);
        gera_vizinho(sol,nova_sol2,vert);
        custo_viz2 = calcula_fit(nova_sol2,mat,vert,arestas,k);

        if(custo_viz2 <= custo_viz){
            custo_viz = custo_viz2;
            substitui(nova_sol,nova_sol2,vert);
        }
        if (custo_viz <= custo){
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }else{
            // Fica com a solu��o vizinha, apesar de pior, se o valor aleat�rio gerado for abaixo da probabilidade definida
            if (rand_01() < PROB_RECUAR)
            {
                memcpy(sol,nova_sol,sizeof(int)*(vert));
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
    int p1, p2,temp;

    for(int i=0; i<n; i++)
        b[i]=a[i];
    do
        p1=random_l_h(0,n-1);
    while(b[p1] != 0);
    // Encontra posicao com valor 1
    do
        p2=random_l_h(0, n-1);
    while(b[p2] != 1);
    // Troca
    temp = b[p1];
    b[p1] = b[p2];
    b[p2] = temp;
}

void substitui(int a[], int b[], int n){
    for(int i=0; i < n; i++)
        a[i]=b[i];
}

void tournament(pchrom pop, struct info d, pchrom parents)
{
    int i, x1, x2;

    // Realiza popsize torneios
    for(i=0; i<d.popsize;i++)
    {
        x1 = random_l_h(0, d.popsize-1);
        do
            x2 = random_l_h(0, d.popsize-1);
        while(x1==x2);

        if(pop[x1].fitness < pop[x2].fitness)
            parents[i]=pop[x1];
        else
            parents[i]=pop[x2];
    }
}

void tournament2(pchrom pop, struct info d, pchrom parents)
{
    for (int i = 0; i < d.popsize; ++i) {
        if(i % 2 == 1){
            if(pop[i].fitness < pop[i-1].fitness)
                parents[i]=pop[i];
            else
                parents[i]=pop[i-1];
        }else{
            parents[i]=pop[i];
        }
    }
}
void tournament3(pchrom pop, struct info d, pchrom parents, chrom beste)
{
    for (int i = 0; i < d.popsize; ++i) {
        if(i % 2 == 1){
            if(pop[i].fitness < pop[i-1].fitness) {
                parents[i] = pop[i];
                if(rand_01() <= 0.05)
                    parents[i - 1] = beste;
                else
                    parents[i-1] = pop[i-1];
            }else{
                parents[i-1]=pop[i-1];
                if(rand_01() <= 0.05)
                    parents[i] = beste;
                else
                    parents[i] = pop[i];
            }
        }

    }
}
void genetic_operators(pchrom parents, struct info d, pchrom offspring)
{
    // Recombinação com um ponto de corte
    crossover3(parents, d, offspring);
    // Mutação binária
    mutation(offspring, d);

    repair(offspring, d);

}

void genetic_operators2 (pchrom parents, struct info d, pchrom offspring, chrom melhor)
{
    // Recombinação com um ponto de corte
    crossover2(parents, d, offspring, melhor);
    // Mutação binária
    mutation(offspring, d);

    repair(offspring, d);

}
void crossover(pchrom parents, struct info d, pchrom offspring)
{
    int i, j, point;

    for (i=0; i<d.popsize; i+=2)
    {
        if (rand_01() < d.pr)
        {
            point = random_l_h(0, d.numVertice-1);
            for (j=0; j<point; j++)
            {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i+1].p[j] = parents[i+1].p[j];
            }
            for (j=point; j<d.numVertice; j++)
            {
                offspring[i].p[j]= parents[i+1].p[j];
                offspring[i+1].p[j] = parents[i].p[j];
            }
        }
        else
        {
            offspring[i] = parents[i];
            offspring[i+1] = parents[i+1];
        }
    }
}


void crossover3(pchrom parents, struct info d, pchrom offspring)
{
    int i, j, point1, point2, point3;

    for (i=0; i<d.popsize; i+=2)
    {
        if (rand_01() < d.pr)
        {
            point1 = random_l_h(0, d.numVertice-1);
            point2 = random_l_h(point1, d.numVertice-1);
            point3 = random_l_h(point2, d.numVertice-1);
            for (j=0; j<point1; j++)
            {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i+1].p[j] = parents[i+1].p[j];
            }
            for (j=point1; j<point2; j++)
            {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i+1].p[j] = parents[i+1].p[j];
            }
            for (j=point2; j<point3; j++)
            {
                offspring[i].p[j] = parents[i].p[j];
                offspring[i+1].p[j] = parents[i+1].p[j];
            }
            for (j=point2; j<d.numVertice; j++)
            {
                offspring[i].p[j]= parents[i+1].p[j];
                offspring[i+1].p[j] = parents[i].p[j];
            }
        }
        else
        {
            offspring[i] = parents[i];
            offspring[i+1] = parents[i+1];
        }
    }
}

void crossover2(pchrom parents, struct info d, pchrom offspring, chrom melhor)
{
    int i, j, point;

    for (i=0; i<d.popsize; i+=2)
    {
        if (rand_01() < d.pr)
        {
            point = random_l_h(0, d.numVertice-1);
            for (j=0; j<point; j++)
            {
                if(rand_01() < d.pr * 0.50){
                    offspring[i].p[j] = melhor.p[j];
                    offspring[i+1].p[j] = melhor.p[j];
                }else{
                    offspring[i].p[j] = parents[i].p[j];
                    offspring[i+1].p[j] = parents[i+1].p[j];
                }

            }
            for (j=point; j<d.numVertice; j++)
            {
                if(rand_01() < d.pr * 0.50){
                    offspring[i].p[j] = melhor.p[j];
                    offspring[i + 1].p[j] = melhor.p[j];
                }else {
                    offspring[i].p[j] = parents[i + 1].p[j];
                    offspring[i + 1].p[j] = parents[i].p[j];
                }
            }
        }
        else
        {
            offspring[i] = parents[i];
            offspring[i+1] = parents[i+1];
        }
    }
}

void mutation(pchrom offspring, struct info d)
{
    int i, j;

    for (i=0; i<d.popsize; i++)
        for (j=0; j<d.numVertice; j++)
            if (rand_01() < d.pm)
                offspring[i].p[j] = !(offspring[i].p[j]);
}
void mutation_swap(pchrom offspring, struct info d)
{
    int i,x,y;

    for (i=0; i<d.popsize; i++)
            if (rand_01() < d.pm) {
                do {
                    x = random_l_h(0, d.numVertice - 1);
                } while (offspring[i].p[x] == 0);

                do {
                    y = random_l_h(0, d.numVertice - 1);
                } while (offspring[i].p[y] == 1);

                offspring[i].p[x] = 0;
                offspring[i].p[y] = 1;
            }
}

void mutation2(pchrom offspring, struct info d)
{
    int i, j, has0, has1, pos0, pos1;

    for (i=0; i<d.popsize; i++) {
        has0 = 0;
        has1 = 0;
        for (j = 0; j < d.numVertice; j++)
            if (offspring[i].p[j] == 0) {
                has0 = 1;
                pos0 = j;
            }
        if (offspring[i].p[j] == 1) {
            has1 = 1;
            has1 = j;
        }
        if (rand_01() < (d.pm * 0.50))
            if (has1 && has0) {
                offspring[i].p[pos0] = offspring[i].p[pos1];
                offspring[i].p[pos1] = 0;
            }
    }
}

void repair(pchrom offspring, struct info d){
    int cont, pos;
    for (int i = 0; i < d.popsize; ++i) {
        cont = 0;
        for (int j = 0; j < d.numVertice; ++j) {
            if(offspring[i].p[j] == 1)
                cont++;
        }
        if(cont < d.numGenes){
            for (cont = cont; cont < d.numGenes; ++cont) {
                do{
                    pos = random_l_h(0, d.numVertice - 1);
                } while (offspring[i].p[pos] == 1);
                offspring[i].p[pos] = 1;
            }
        }

        if(cont > d.numGenes){
            for (cont = cont; cont > d.numGenes; --cont) {
                do{
                    pos = random_l_h(0, d.numVertice - 1);
                } while (offspring[i].p[pos] == 0);
                offspring[i].p[pos] = 0;
            }
        }
    }
}