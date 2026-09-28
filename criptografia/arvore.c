#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

No *criarNo(unsigned char dado, int posicao) {

    No *novo = (No *)malloc(sizeof(No));

    if (novo == NULL) {
        return NULL;
    }

    novo->dado = dado;
    novo->posicao = posicao;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

void inserir(No **raiz, unsigned char dado, int posicao) {

    if (*raiz == NULL) {

        *raiz = criarNo(dado, posicao);

        return;
    }

    if (dado < (*raiz)->dado) {

        inserir(
            &(*raiz)->esquerda,
            dado,
            posicao
        );

    } else {

        inserir(
            &(*raiz)->direita,
            dado,
            posicao
        );
    }
}

void preOrdem(No *raiz) {

    if (raiz == NULL) {
        return;
    }

    printf("%u ", raiz->dado);

    preOrdem(raiz->esquerda);
    preOrdem(raiz->direita);
}

void emOrdem(No *raiz) {

    if (raiz == NULL) {
        return;
    }

    emOrdem(raiz->esquerda);

    printf("%u ", raiz->dado);

    emOrdem(raiz->direita);
}

void posOrdem(No *raiz) {

    if (raiz == NULL) {
        return;
    }

    posOrdem(raiz->esquerda);
    posOrdem(raiz->direita);

    printf("%u ", raiz->dado);
}

void liberarArvore(No *raiz) {

    if (raiz == NULL) {
        return;
    }

    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);

    free(raiz);
}