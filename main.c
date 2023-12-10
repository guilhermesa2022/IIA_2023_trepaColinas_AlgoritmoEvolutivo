#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "algoritmo.h"
#include "funcao.h"

int main(int argc, char **argv) {
    FILE *file;
    int runs, k, opcao;
    //struct info EA_param;
    int **matriz = NULL;
    char filename[100];

    if (argc == 3) {
        runs = atoi(argv[2]);
        strcpy(filename, argv[1]);
    } else if (argc == 2) {
        runs = DEFAULT_RUNS;
        strcpy(filename, argv[1]);
    } else {
        runs = DEFAULT_RUNS;
        printf("Nome do Ficheiro: ");
        gets(filename);
    }

    file = fopen(filename, "r");
    if (!file) {
        printf("Erro no acesso ao ficheiro dos dados\n");
        exit(1);
    }
    init_rand();
    fscanf(file, " k %d", &k);
    fclose(file);

    printf("1- Trepa Colinas probabilistico\n");
    printf("2- Algoritmo Evolutivo\n");
    printf("3- Algoritmo Hibrido\n");
    printf("4- 2 Algoritmo Hibrido\n");
    scanf("%d", &opcao);

    if (opcao == 1) {
        int *solucaoInicial = NULL, *solucaoBest = NULL;
        int vert = 0, arestas = 0, somaDoCustos = 0, i;
        int custo = 0, best_custo, inter;
        float mbf = 0;
        matriz = inicializar_Dados_Ficheiro(filename, &vert, &arestas, &k, &somaDoCustos);
        printMatriz(matriz, vert);
        printf("Quantas iteracoes deseja fazer?  \n\tnumero: ");
        scanf("%d", &inter);

        solucaoInicial = malloc(sizeof(int) * (vert));
        solucaoBest = malloc(sizeof(int) * (vert));
        if (solucaoInicial == NULL || solucaoBest == NULL) {
            free(solucaoInicial);
            free(solucaoBest);
            printf("Erro na alocacao de memoria\n");
            exit(1);
        }

        for (i = 0; i < runs; ++i) {
            gera_sol_inicial(solucaoInicial, vert, k);
            escreve_sol(solucaoInicial, vert);


            custo = trepa_colinas_probabilistico(solucaoInicial, matriz, vert, inter, arestas, k);
            escreve_sol(solucaoInicial, vert);

            if (custo < somaDoCustos)
                mbf += (float) custo;

            if (i == 0 || custo <= best_custo) {
                best_custo = custo;
                substitui(solucaoBest, solucaoInicial, vert);
            }
            if (i % 20 == 0) {
                printf("\nRepeticao %d:", i);
            }

        }

        printf("\n-----------------------------------------------------");
        printf("\nMBF: %.2f\n", mbf / i);
        printf("\nMelhor solucao encontrada");
        escreve_sol(solucaoBest, vert);
        printf("Custo final: %d\n", best_custo);

        free(matriz);
        free(solucaoInicial);
        free(solucaoBest);
        return 0;
    } else if (opcao == 2) {
        init_rand();
        pchrom pop = NULL, parents = NULL;
        struct info EA_param;
        int vert = 0, arestas = 0, somaDoCustos = 0, i, inv, r;
        float mbf;
        int gen_actual;
        chrom best_run, best_ever;
        best_ever.fitness = 99999;
        matriz = inicializar_Dados_Ficheiro(filename, &vert, &arestas, &k, &somaDoCustos);
        printMatriz(matriz, vert);
        EA_param.popsize = 120;
        EA_param.pm = 0.4;
        EA_param.pr = 0.2;
        EA_param.tsize = 2;
        EA_param.numGenerations = 2500;
        EA_param.numGenes = k;
        EA_param.numVertice = vert;
        EA_param.sumaDoscustos = somaDoCustos;
        int teste;
        for (teste = 0; teste < 5; teste++) {
            printf("********************************************************** teste: %i", teste);
            for (r = 0; r < runs; ++r) {
                printf("\n\t\tRepeticao %d\n", r + 1);
                pop = init_pop(EA_param);

                /*for (int j = 0; j < EA_param.popsize; ++j) {
                    printf("[[");
                    for (int l = 0; l < EA_param.numVertice; ++l) {
                        printf(" %d ", pop[j].p[l]);
                    }
                    printf("]]\n");
                }*/
                evaluate(pop, EA_param, matriz);
                /*
    ////////////////////////////////////////////////////////////* teste
                printf("\n{");
                for (int j = 0; j < EA_param.popsize; ++j) {
                    printf("%.0f ------- %d |!| ", pop[j].fitness, pop[j].valido);
                }
                printf("}\n");
    ////////////////////////////////////////////////////////////
                 */

                gen_actual = 1;
                best_run = pop[0];

                //write_best(best_run, EA_param);

                best_run = get_best(pop, EA_param, best_run);
                escreve_sol(best_run.p, EA_param.numVertice);

                parents = malloc(sizeof(chrom) * EA_param.popsize);

                if (parents == NULL) {
                    printf("Erro na alocacao de memoria\n");
                    for (int i = 0; i < EA_param.popsize; ++i) { free(pop[i].p); }
                    free(pop);
                    exit(1);
                }

                while (gen_actual <= EA_param.numGenerations) {
                    tournament2(pop, EA_param, parents);
                    //tournament2(pop, EA_param, parents);
                    /*
                    if(r == 0)
                        tournament3(pop, EA_param, parents, best_run);
                    else
                        tournament3(pop, EA_param, parents, best_ever);
                        */

                    /*
                    if(r == 0)
                        genetic_operators2(parents, EA_param, pop, best_run);
                    else
                        genetic_operators2(parents, EA_param, pop, best_ever);
                        */

                    genetic_operators(parents, EA_param, pop);
                    evaluate(pop, EA_param, matriz);
                    best_run = get_best(pop, EA_param, best_run);
                    gen_actual++;
                }

                //printf("}\n");
                for (inv = 0, i = 0; i < EA_param.popsize; i++)
                    if (pop[i].valido == 1)
                        inv++;

                printf("\nRepeticao %d:", r);
                write_best(best_run, EA_param);
                printf("\nPercentagem Invalidos: %.2f\n", 100 * (float) inv / EA_param.popsize);
                if (best_run.fitness < somaDoCustos)
                    mbf += best_run.fitness;
                if (r == 0 || best_run.fitness < best_ever.fitness)
                    best_ever = best_run;
                // Liberta a memória usada
                free(parents);
                free(pop);

                if (r == 4) {
                    printf("----teste ate 5------%f--------------", best_run.fitness);
                    printf("----teste ate 5------%f--------------", best_run.fitness);
                    printf("----teste ate 5------%f--------------", best_run.fitness);
                    printf("\n\nMBF: %f\n", mbf / r);
                    printf("\nMelhor solucao encontrada");
                    write_best(best_ever, EA_param);
                    escreve_sol(best_ever.p, EA_param.numVertice);
                }
                if (r == 9) {
                    printf("----teste ate 10------%f--------------", best_run.fitness);
                    printf("----teste ate 10------%f--------------", best_run.fitness);
                    printf("----teste ate 10------%f--------------", best_run.fitness);
                    printf("\n\nMBF: %f\n", mbf / r);
                    printf("\nMelhor solucao encontrada");
                    write_best(best_ever, EA_param);
                    escreve_sol(best_ever.p, EA_param.numVertice);
                }
                if (r == 14) {
                    printf("----teste ate 15------%f--------------", best_run.fitness);
                    printf("----teste ate 15------%f--------------", best_run.fitness);
                    printf("----teste ate 15------%f--------------", best_run.fitness);
                    printf("\n\nMBF: %f\n", mbf / r);
                    printf("\nMelhor solucao encontrada");
                    write_best(best_ever, EA_param);
                    escreve_sol(best_ever.p, EA_param.numVertice);
                }
            }
            printf("-----------------%f--------------", best_run.fitness);
            printf("\n\nMBF: %f\n", mbf / r);
            printf("\nMelhor solucao encontrada");
            write_best(best_ever, EA_param);
            escreve_sol(best_ever.p, EA_param.numVertice);
        }
        return 0;
    }else if (opcao == 3) {
        init_rand();
        pchrom pop = NULL, parents = NULL;
        struct info EA_param;
        int vert = 0, arestas = 0, somaDoCustos = 0, i, inv, r;
        float mbf, best_custo;
        int gen_actual;
        chrom best_run, best_ever, solucaoBest;
        best_ever.fitness = 99999;
        matriz = inicializar_Dados_Ficheiro(filename, &vert, &arestas, &k, &somaDoCustos);
        printMatriz(matriz, vert);
        EA_param.popsize = 120;
        EA_param.pm = 0.4;
        EA_param.pr = 0.2;
        EA_param.tsize = 2;
        EA_param.numGenerations = 2000;
        EA_param.numGenes = k;
        EA_param.numVertice = vert;
        EA_param.sumaDoscustos = somaDoCustos;

        printf("Quantas iteracoes deseja fazer?  \n\tnumero: ");
        scanf("%d", &runs);

            for (r = 0; r < runs; ++r) {
                printf("\n\t\tRepeticao %d\n", r + 1);
                pop = init_pop(EA_param);

                evaluate(pop, EA_param, matriz);
                gen_actual = 1;
                best_run = pop[0];

                best_run = get_best(pop, EA_param, best_run);
                escreve_sol(best_run.p, EA_param.numVertice);

                parents = malloc(sizeof(chrom) * EA_param.popsize);

                if (parents == NULL) {
                    printf("Erro na alocacao de memoria\n");
                    for (int i = 0; i < EA_param.popsize; ++i) { free(pop[i].p); }
                    free(pop);
                    exit(1);
                }

                while (gen_actual <= EA_param.numGenerations) {
                    tournament2(pop, EA_param, parents);
                    //tournament2(pop, EA_param, parents);
                    /*
                    if(r == 0)
                        tournament3(pop, EA_param, parents, best_run);
                    else
                        tournament3(pop, EA_param, parents, best_ever);
                        */

                    /*
                    if(r == 0)
                        genetic_operators2(parents, EA_param, pop, best_run);
                    else
                        genetic_operators2(parents, EA_param, pop, best_ever);
                        */

                    genetic_operators(parents, EA_param, pop);
                    evaluate(pop, EA_param, matriz);
                    best_run = get_best(pop, EA_param, best_run);
                    gen_actual++;
                }

                best_run.fitness = trepa_colinas_probabilistico(best_run.p, matriz, EA_param.numVertice, runs * 10000, arestas, k);

                    if (best_run.fitness < somaDoCustos)
                        mbf += best_run.fitness;
                    if (r == 0 ||best_run.fitness < best_ever.fitness)
                        best_ever = best_run;


                for (inv = 0, i = 0; i < EA_param.popsize; i++)
                    if (pop[i].valido == 1)
                        inv++;

                printf("\nRepeticao %d:", r);
                write_best(best_run, EA_param);
                printf("\nPercentagem Invalidos: %.2f\n", 100 * (float) inv / EA_param.popsize);
                // Liberta a memória usada
                free(parents);
                free(pop);

            }
            printf("-----------------%f--------------", best_run.fitness);
            printf("\n\nMBF: %f\n", mbf / r);
            printf("\nMelhor solucao encontrada");
            write_best(best_ever, EA_param);
            escreve_sol(best_ever.p, EA_param.numVertice);
            return 0;

    }else if(opcao == 4){
        init_rand();
        pchrom pop = NULL, parents = NULL;
        struct info EA_param;
        int vert = 0, arestas = 0, somaDoCustos = 0, i, inv, r;
        float mbf;
        int gen_actual;
        chrom best_run, best_ever;
        best_ever.fitness = 99999;
        matriz = inicializar_Dados_Ficheiro(filename, &vert, &arestas, &k, &somaDoCustos);
        printMatriz(matriz, vert);
        EA_param.popsize = 120;
        EA_param.pm = 0.4;
        EA_param.pr = 0.2;
        EA_param.tsize = 2;
        EA_param.numGenerations = 2500;
        EA_param.numGenes = k;
        EA_param.numVertice = vert;
        EA_param.sumaDoscustos = somaDoCustos;

            for (r = 0; r < runs; ++r) {
                printf("\n\t\tRepeticao %d\n", r + 1);
                pop = init_pop(EA_param);

                evaluate(pop, EA_param, matriz);

                gen_actual = 1;
                best_run = pop[0];

                //write_best(best_run, EA_param);

                best_run = get_best(pop, EA_param, best_run);
                escreve_sol(best_run.p, EA_param.numVertice);

                parents = malloc(sizeof(chrom) * EA_param.popsize);

                if (parents == NULL) {
                    printf("Erro na alocacao de memoria\n");
                    for (int i = 0; i < EA_param.popsize; ++i) { free(pop[i].p); }
                    free(pop);
                    exit(1);
                }

                printf("}\n");
                while (gen_actual <= EA_param.numGenerations) {
                    tournament2(pop, EA_param, parents);
                    genetic_operators(parents, EA_param, pop);

                    if(rand_01() < 0.4){
                        int numrero = random_l_h(0, EA_param.popsize - 1);

                        for (int j = 0; j < numrero; ++j) {
                            pop[j].fitness = trepa_colinas_probabilistico(pop[j].p, matriz, EA_param.numVertice, runs, arestas, k);
                        }
                    }

                    evaluate(pop, EA_param, matriz);
                    best_run = get_best(pop, EA_param, best_run);
                    gen_actual++;
                }

                printf("}\n");
                for (inv = 0, i = 0; i < EA_param.popsize; i++)
                    if (pop[i].valido == 1)
                        inv++;

                printf("\nRepeticao %d:", r);
                write_best(best_run, EA_param);
                printf("\nPercentagem Invalidos: %.2f\n", 100 * (float) inv / EA_param.popsize);
                if (best_run.fitness < somaDoCustos)
                    mbf += best_run.fitness;
                if (r == 0 || best_run.fitness < best_ever.fitness)
                    best_ever = best_run;
                // Liberta a memória usada
                free(parents);
                free(pop);
            }
            printf("_______________________________________________________");
            printf("\n\nMBF: %f\n", mbf / r);
            printf("\nMelhor solucao encontrada");
            write_best(best_ever, EA_param);
            escreve_sol(best_ever.p, EA_param.numVertice);


        return 0;
    }
    return 0;
}