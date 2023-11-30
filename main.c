#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "algoritmo.h"
#include "funcao.h"



void printMatriz(int** matriz, int lado){
    for (int i = 0; i < lado; ++i) {
        for (int j = 0; j < lado; ++j) {
            printf("%d ",matriz[i][j]);
        }
        printf("\n");
    }
}
int main(int argc, char *argv[]){

    int **matriz = NULL;
    int somaDoCustos = 0;
    char filename[100];
    int runs,custo,best_custo,opcao,bestRunFitness, bestEverFitness;
    int v,a,k=0,cont=1,i,l;
    int *sol_inic, *sol_best, *pop[POP_SIZE], qualidade[POP_SIZE];
    int* parents[POP_SIZE];
    float mbf = 0.0, Prob_Reprod = 0.2;
    int *bestRun = NULL, *bestEver = NULL;
    int genAtual;
    int aux;
    FILE *f;
// indicação do executavel do ficheiro de texto e numero de runs
//validação de parametros e controlo de entrada
    if(argc == 3){

        runs = atoi(argv[2]);
        strcpy(filename, argv[1]);
    }
    else if(argc == 2) {

        runs = DEFAULT_RUNS;
        strcpy(filename, argv[1]);
    }else{
        runs = DEFAULT_RUNS;
        printf("Nome do Ficheiro: ");
        gets(filename);
    }

    f=fopen(filename,"r");
    if(!f){
        printf("Erro no acesso ao ficheiro dos dados\n");
        exit(1);
    }

    fscanf(f, " k %d",&k);
    fclose(f);
    aux = k;


    printf("1- Trepa Colinas\n");
    printf("2- Algoritmo Evolutivo\n");
    printf("3- Algoritmo Hibrido\n");
    scanf("%d",&opcao);

    matriz = init_dados(filename,&v,&a,&k, &somaDoCustos);
    printMatriz(matriz, v);


    if(opcao == 1){
        printf("Quantas iteracoes deseja fazer?\n\tnumero: ");
        scanf("%d",&runs);

        sol_inic = malloc(sizeof(int)* (v+1));
        sol_best = malloc(sizeof(int)* (v+1));

        //MOD nao liverta memoria inicial
        if(sol_inic == NULL || sol_best == NULL){
            printf("Erro na alocacao de memoria\n");
            exit(1);
        }
        for(i = 0; i < runs; i++){
            // gera e mostra solucao inicial
            gera_sol_inicial(sol_inic,v,k);
            escreve_sol(sol_inic,v,cont);

            // trepa colinas
            custo = trepa_colinas_probabilistico(sol_inic,matriz,v,runs, a, k);
            escreve_sol(sol_inic,v,cont);

            if(custo < somaDoCustos)
                mbf += (float)custo;

            if(i==0 || custo <= best_custo)
            {
                best_custo = custo;
                substitui(sol_best,sol_inic,v);
            }
            printf("\nRepeticao %d:", i);
        }
        // Escreve eresultados globais
        printf("\n*********************RESPOSTA FINAL*************************");
        printf("\nMBF: %.2f\n", mbf/i);
        printf("\nMelhor solucao encontrada");
        escreve_sol(sol_best,v,cont);
        printf("Custo final: %d\n", best_custo);
        free(matriz);
        free(sol_inic);
        free(sol_best);
        return 0;
    }else
    if(opcao == 2) {
        init_rand();
        printf("Quantas iteracoes deseja fazer ?");
        scanf("%d",&runs);

        mbf = 0;
        bestRun = malloc(sizeof(int) * (v+1));
        bestEver = malloc(sizeof(int) * (v+1));

        if(bestRun == NULL || bestEver == NULL){
            printf("Erro na alocacao de memoria\n");
            exit(1);
        }

        bestEverFitness = 0;
        bestRunFitness = 0;

        for (int i = 0; i < runs; i++) {

            printf("\n-> Repeticao %d", i);

            // Gera população inicial (algoritmo genetico)
            initPop(pop,matriz,v,a,k);

            // verifica a qualidade na popula��o
            k = aux;
            qualidadePop(pop, qualidade,matriz,v,a,k);


            // Mostra populacao inicial (algoritmo genetico)
            printf("\n\n---- POPULACAO INICIAL ----\n");
            //escrevePop(stdout,pop,qualidade,v,a,k);


            // Como ainda n�o existe, escolhe-se como melhor solu��o a primeira da popula��o (poderia ser outra qualquer)
            memcpy(bestRun, pop[0], sizeof(int) * (v+1));

            //Alocar memoria necessaria para os pais da geração.
            for (int j = 0; j <=POP_SIZE; j++) { //eu comeco no 1 e acabo em 100
                parents[j] = malloc(sizeof(int) * (v+1));
                if (parents[j] == NULL) {
                    fprintf(stderr, "<ERRO> Alocacao de memoria para os pais falhou.\n");
                    exit(-1);
                }
            }
            genAtual = 1;
            while (genAtual <= GER_NUM) {
                // Metodo de seleção
                //tournament(pop, parents, qualidade, v);
                //SEGUNDO METODO DE SELEÇÃO
                tournament2(pop,parents,qualidade,v);

                //AQUI DENTRO ESTÃO PRESENTES AMBOS OS OPERADORES GENETICOS NORMAIS E OS NOVOS
                geneticOperators(parents, pop, qualidade,matriz,v,a,k);

                //verificar qualidade depois de atuarem os operadores geneticos
                k = aux;
                qualidadePop(pop, qualidade,matriz,v,a,k);

                //Verificar a qualidade de cada solucao na população,
                //encontrar a maior e armazena-la no ponteiro bestrun e tambem o valor da qualidade dessa solucao em bestrunfitness
                for (int j = 0; j < POP_SIZE; j++)
                {
                    if (qualidade[j] > bestRunFitness) {
                        memcpy(bestRun, pop[j], sizeof(int) * (v+1));
                        bestRunFitness = qualidade[j];
                    }
                }
                genAtual++;
            }

            mbf += bestRunFitness;
            if (i == 0 || bestRunFitness > bestEverFitness) {
                memcpy(bestEver, bestRun, sizeof(int) * (v+1));
                bestEverFitness = bestRunFitness;
            }
//limpar o buffer
            fflush(stdout);
        }

        //Resultados do modelo

        printf("\n\nMBF: %.2f\n", mbf / runs);
        printf("\n-- Melhor solucao encontrada --");
        escreve_sol_final(bestEver, bestEverFitness, v);


        //libertação da memoria dinamica completa
        free(matriz);
        free(bestEver);
        free(bestRun);

        return 0;
    }

    if(opcao == 3)
    {
        printf("Quantas iteracoes deseja fazer ?");
        scanf("%d",&runs);
        init_rand();

        mbf = 0;
        bestRun = malloc(sizeof(int) * (v+1));
        bestEver = malloc(sizeof(int) * (v+1));

        if(bestRun == NULL || bestEver == NULL){
            printf("Erro na alocacao de memoria\n");
            exit(1);
        }

        bestEverFitness = 0;
        bestRunFitness = 0;
        for (int i = 0; i < runs; i++) {

            printf("\n-> Repeticao %d", i);

            // Gera população inicial (algoritmo genetico)
            initPop(pop,matriz,v,a,k);

            //Gera população inicial (algoritmo hibrido)
            //initPop_Evol(pop,matriz,v,a,k);

            k = aux;
            qualidadePop(pop, qualidade,matriz,v,a,k);
            // Mostra populacao inicial (algoritmo genetico)
            printf("\n\n---- POPULACAO INICIAL ----\n");

            // Como ainda n�o existe, escolhe-se como melhor solu��o a primeira da popula��o (poderia ser outra qualquer)
            memcpy(bestRun, pop[0], sizeof(int) * (v+1));


            //Alocar memoria necessaria para os pais da geração.
            for (int j = 0; j <=POP_SIZE; j++) {
                parents[j] = malloc(sizeof(int) * (v+1));
                if (parents[j] == NULL) {
                    fprintf(stderr, "<ERRO> Alocacao de memoria para os pais falhou.\n");
                    exit(-1);
                }
            }

            genAtual = 1;

            while (genAtual <=GER_NUM) {
                //tournament(pop, parents, qualidade, v);

                //SEGUNDO METODO DE SELEÇÃO
                tournament2(pop,parents,qualidade,v);


                //AQUI DENTRO ESTÃO PRESENTES AMBOS OS OPERADORES GENETICOS NORMAIS E OS NOVOS
                geneticOperators(parents, pop, qualidade,matriz,v,a,k);
                k = aux;
                qualidadePop(pop, qualidade,matriz,v,a,k);


                //gera um numero aleatorio entre 0 e 1 ( l )
                //vai à entidade l da população pop e aplica o algoritmo de pesquisa local (trepa-colias)
                //finalmente guarda a qualidade ou fitness ou custo dessa entidade l no array que contem as qualidades da população
                // algoritmo hibrido

                if(rand_01() <= PROB_REPROD)
                {
                    l = random_l_h(1,POP_SIZE-1);
                    if(qualidade[l] != 0)
                    {
                        qualidade[l] = trepa_colinas(pop[l],matriz,v,a,k,runs);
                    }
                }

                for (int j = 1; j < POP_SIZE; j++)
                {
                    if (qualidade[j] > bestRunFitness) {
                        memcpy(bestRun, pop[j], sizeof(int) * (v+1));
                        bestRunFitness = qualidade[j];
                    }
                }
                genAtual++;
            }


            mbf += bestRunFitness;
            if (i == 0 || bestRunFitness > bestEverFitness) {
                memcpy(bestEver, bestRun, sizeof(int) * (v+1));
                bestEverFitness = bestRunFitness;
            }

            fflush(stdout);
        }

        printf("\n\nMBF: %.2f\n", mbf / runs);
        printf("\n-- Melhor solucao encontrada --");
        escreve_sol_final(bestEver, bestEverFitness,v);

        return 0;
    }
}