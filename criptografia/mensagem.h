#ifndef MENSAGEM_H
#define MENSAGEM_H

#include "arvore.h"
#include "usuario.h"

#define TAMANHO_MAXIMO 5000

typedef struct {

    int id;

    unsigned char texto[TAMANHO_MAXIMO];

    int tamanho;

    No *arvore;

    int ordem;

    int criptografada;

    char remetente[100];

} Mensagem;

void inicializarMensagem(Mensagem *mensagem);

void liberarMensagem(Mensagem *mensagem);

int gerarArvoreMensagem(Mensagem *mensagem);

void mostrarMensagem(Mensagem *mensagem);

int escreverMensagem(
    Mensagem *mensagem,
    Usuario *usuario
);

#endif