#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vetor.h"

//#define MERGE_TESTE_DIM 10

//dimensoes iniciais e finais das execucoes
#define MERGE_RUN_INICIAL 2
#define MERGE_RUN_FINAL 10000000

//limite dos valores aleatorios
#define RAND_LIMITE MERGE_RUN_FINAL

//nome do arquivo onde os resultados das execucoes serao salvos
#define ARQUIVO "mergesort.txt"

//static const int TESTE[] = { 8, 3, 9, 7, 5, 10, 1, 4, 6, 2 };

//funcao recursiva
static void mergesort_rec(int *vetor, unsigned int n) {

	unsigned int i, j, k;
	unsigned int metadeL, metadeR;
	int *mistura;

	//se o vetor nao eh unitario
	if (n > 1) {

		//chamada recursiva do mergesort em duas metades do vetor
		metadeL = n/2;
		metadeR = metadeL + n%2;

		mergesort_rec(vetor, metadeL);
		mergesort_rec(vetor+metadeL, metadeR);

		//mistura as duas metades de forma ordenada
		i = 0;
		j = metadeL;
		k = 0;
		mistura = (int*)malloc(n*sizeof(int));
		while (i < metadeL || j < n) {

			if (i < metadeL && j < n) {
				if (vetor[i] < vetor[j]) {
					mistura[k] = vetor[i];
					i++;
				}else{
					mistura[k] = vetor[j];
					j++;
				}
			}else if (i < metadeL) {
				mistura[k] = vetor[i];
				i++;
			}else{
				mistura[k] = vetor[j];
				j++;
			}

			k++;

		}

		//copia a mistura para o vetor
		for (i = 0; i < n; i++) {
			vetor[i] = mistura[i];
		}

		free(mistura);

	}

}

void mergesort_recursivo(Vetor *vetor) {

	if (vetor == NULL) {
		return;
	} else if (vetor->valor == NULL) {
		return;
	} else if (vetor->n < 2) {
		return;
	}

	mergesort_rec(vetor->valor, vetor->n);

}

//funcao iterativa
void mergesort(Vetor *vetor) {

	if (vetor == NULL) {
		return;
	} else if (vetor->valor == NULL) {
		return;
	} else if (vetor->n < 2) {
		return;
	}

	unsigned int i, j, k;
	unsigned int inicioL, fimL, fimR, dimensao;
	int *mistura;	

	//dimensao determina tamanho de subvetores a serem misturados
	mistura = (int*)malloc(vetor->n*sizeof(int));
	dimensao = 2;
	while (dimensao/2 < vetor->n) {

		//determina-se as porcoes dos vetores a serem misturadas
		inicioL = 0;
		fimL = dimensao/2;
		if (dimensao < vetor->n) {
			fimR = dimensao;
		}else{
			fimR = vetor->n;
		}

		//mistura
		while (fimL < vetor->n) {

			//fprintf (stderr, "%u %u: %u %u %u\n", vetor->n, dimensao, inicioL, fimL, fimR);	//debug

			//inicializacao dos contadores
			i = inicioL;
			j = fimL;
			k = i;

			//as duas metades sao misturadas de maneira ordenada em um espaco no vetor mistura
			while (i < fimL || j < fimR) {

				if (i < fimL && j < fimR) {
					if (vetor->valor[i] < vetor->valor[j]) {
						mistura[k] = vetor->valor[i];
						i++;
					}else{
						mistura[k] = vetor->valor[j];
						j++;
					}
				}else if (i < fimL) {
					mistura[k] = vetor->valor[i];
					i++;
				}else{
					mistura[k] = vetor->valor[j];
					j++;
				}

				k++;

			}

			//mistura eh copiada para vetor->valor
			for (i = inicioL; i < fimR; i++) {
				vetor->valor[i] = mistura[i];
			}

			inicioL = fimR;
			fimL += dimensao;
			fimR += dimensao;
			if (fimR > vetor->n && fimL < vetor->n) {
				fimR = vetor->n;
			}

		}

		//dobra-se a dimensao dos subvetores
		dimensao *= 2;

	}

	free (mistura);

}

/*TESTE*/
int main (void) {

	unsigned int i, n;
	FILE *saida = fopen(ARQUIVO, "w");

	//variaveis para medicao do tempo em segundos
	double run_time;
	clock_t run_clocks;

	Vetor teste;

	fprintf (stderr, "------MERGESORT------\n");
	fprintf (stderr, "TESTE COM VETORES DE DIMENSAO ENTRE %d E %d\n", MERGE_RUN_INICIAL, MERGE_RUN_FINAL);

	n = MERGE_RUN_INICIAL;
	while (n <= MERGE_RUN_FINAL) {

		fprintf (stderr, "DIMENSAO = %u\r", n);

		//inicializa vetor com valores aleatorios
		if ( !(Vetor_init(&teste, n)) ) {
			printf ("Nao foi possivel alocar espaco para vetor de dimensao %u\n", n);
			return 1;
		}
		srand((unsigned int)clock());
		for (i = 0; i < teste.n; i++) {
			teste.valor[i] = rand() % RAND_LIMITE + 1;
		}

		//Vetor_print(stdout, &teste, '\n');	//debug

		//inicio da contagem de tempo
		run_clocks = clock();

		//mergesort_recursivo(&teste);
		mergesort(&teste);

		//Vetor_print(stdout, &teste, '\n');	//debug

		//fim da contagem de tempo
		run_clocks = clock() - run_clocks;
		run_time = (double)run_clocks / CLOCKS_PER_SEC;
		//escreve resultados da execucao na saida
		fprintf (saida, "%u %lf\n", n, run_time);

		//aumenta dimensao do vetor teste
		if (n < 10) {
			n = 10;
		} else if (n < 100) {
			n += 10;
		} else if (n < 1000) {
			n += 100;
		} else if (n < 10000){
			n += 1000;
		} else if (n < 100000){
			n += 5000;
		} else {
			n += 100000;
		}

		//libera espaco alocado pelo vetor
		Vetor_free(&teste);

	}

	printf ("\n");
	fclose(saida);

	return 0;

}
