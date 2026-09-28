#include <stdio.h>
#include <string.h>

#include "mensagem.h"

void inicializarMensagem(Mensagem *mensagem) {

    mensagem->id = 0;
    mensagem->tamanho = 0;
    mensagem->arvore = NULL;
    mensagem->ordem = -1;
    mensagem->criptografada = 0;

    memset(mensagem->texto, 0, TAMANHO_MAXIMO);
    memset(mensagem->remetente, 0, 100);
}

void liberarMensagem(Mensagem *mensagem) {

    if (mensagem == NULL) {
        return;
    }

    if (mensagem->arvore != NULL) {

        liberarArvore(mensagem->arvore);

        mensagem->arvore = NULL;
    }
}

int gerarArvoreMensagem(Mensagem *mensagem) {

    if (
        mensagem == NULL ||
        mensagem->tamanho <= 0
    ) {
        return 0;
    }

    if (mensagem->arvore != NULL) {
        liberarMensagem(mensagem);
    }

    mensagem->arvore = NULL;

    for (int i = 0; i < mensagem->tamanho; i++) {

        inserir(
            &mensagem->arvore,
            mensagem->texto[i],
            i
        );
    }

    return 1;
}

void mostrarMensagem(Mensagem *mensagem) {

    if (mensagem == NULL) {
        return;
    }

    printf("\n=================================\n");
    printf("          MENSAGEM\n");
    printf("=================================\n");

    printf("ID: %03d\n", mensagem->id);
    printf("Remetente: %s\n", mensagem->remetente);
    printf("Mensagem: %s\n", mensagem->texto);
}

int escreverMensagem(
    Mensagem *mensagem,
    Usuario *usuario
) {

    if (
        mensagem == NULL ||
        usuario == NULL
    ) {
        return 0;
    }

    inicializarMensagem(mensagem);

    strncpy(
        mensagem->remetente,
        usuario->nome,
        sizeof(mensagem->remetente) - 1
    );

    printf("\n=================================\n");
    printf("       ESCREVER MENSAGEM\n");
    printf("=================================\n");

    printf("Digite a mensagem:\n");

    if (
        fgets(
            (char *)mensagem->texto,
            TAMANHO_MAXIMO,
            stdin
        ) == NULL
    ) {
        return 0;
    }

    mensagem->texto[
        strcspn(
            (char *)mensagem->texto,
            "\n"
        )
    ] = '\0';

    mensagem->tamanho = strlen(
        (char *)mensagem->texto
    );

    if (mensagem->tamanho <= 0) {

        printf(
            "\nA mensagem nao pode estar vazia.\n"
        );

        return 0;
    }

    printf(
        "\nMensagem escrita com sucesso.\n"
    );

    return 1;
}