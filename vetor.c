#include <stdio.h>
#include <stdlib.h>
#include "vetor.h"

/*OPERACOES VETOR*/
//inicializar vetor
bool Vetor_init(Vetor *vetor, unsigned int n) {

	vetor->valor = (int*)malloc(n*sizeof(int));
	if (vetor->valor == NULL) {
		return false;
	}

	vetor->n = n;

	return true;

}

//liberar espaco alocado pelo vetor
void Vetor_free(Vetor *vetor) {

	free(vetor->valor);
	vetor->valor = NULL;
	vetor->n = 0;

}

//imprime vetor na saida desejada
void Vetor_print(FILE *saida, Vetor *vetor, char c) {

	unsigned int i;

	for (i = 0; i < vetor->n; i++) {

		fprintf(saida, "%d ", vetor->valor[i]);

	}

	fprintf(saida, "\n%c", c);

}
