#ifndef ARVORE_H
#define ARVORE_H

typedef struct No {

    unsigned char dado;

    int posicao;

    struct No *esquerda;
    struct No *direita;

} No;

No *criarNo(
    unsigned char dado,
    int posicao
);

void inserir(
    No **raiz,
    unsigned char dado,
    int posicao
);

void preOrdem(No *raiz);

void emOrdem(No *raiz);

void posOrdem(No *raiz);

void liberarArvore(No *raiz);

#endif