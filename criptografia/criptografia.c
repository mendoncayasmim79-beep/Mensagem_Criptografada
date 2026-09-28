#include <string.h>
#include "criptografia.h"

void criptografarDados(
    unsigned char *dados,
    int tamanho,
    const char *chave
) {
    int tamanhoChave = strlen(chave);

    if (tamanhoChave == 0) {
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        dados[i] ^= chave[i % tamanhoChave];
    }
}

void descriptografarDados(
    unsigned char *dados,
    int tamanho,
    const char *chave
) {
    criptografarDados(dados, tamanho, chave);
}