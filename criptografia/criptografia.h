#ifndef CRIPTOGRAFIA_H
#define CRIPTOGRAFIA_H

void criptografarDados(
    unsigned char *dados,
    int tamanho,
    const char *chave
);

void descriptografarDados(
    unsigned char *dados,
    int tamanho,
    const char *chave
);

#endif