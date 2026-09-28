#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "arquivo.h"
#include "criptografia.h"

#ifdef _WIN32
#include <direct.h>
#define criarDiretorio(path) _mkdir(path)
#else
#define criarDiretorio(path) mkdir(path, 0777)
#endif

#define CHAVE_MENSAGEM "chave_sistema_mensagens"

int criarPastaMensagens(void) {

    struct stat informacoes;

    if (
        stat(
            PASTA_MENSAGENS,
            &informacoes
        ) == 0
    ) {

        return 1;
    }

    if (
        criarDiretorio(
            PASTA_MENSAGENS
        ) == 0
    ) {

        return 1;
    }

    return 0;
}

int proximoIdMensagem(void) {

    int id = 1;

    char nomeArquivo[100];

    while (1) {

        snprintf(
            nomeArquivo,
            sizeof(nomeArquivo),
            PASTA_MENSAGENS "/mensagem_%03d.dat",
            id
        );

        FILE *arquivo = fopen(
            nomeArquivo,
            "rb"
        );

        if (arquivo == NULL) {
            break;
        }

        fclose(arquivo);

        id++;
    }

    return id;
}

static void adicionarPreOrdem(
    No *raiz,
    unsigned char dados[],
    int *posicoes,
    int *indice
) {

    if (raiz == NULL) {
        return;
    }

    dados[*indice] = raiz->dado;
    posicoes[*indice] = raiz->posicao;

    (*indice)++;

    adicionarPreOrdem(
        raiz->esquerda,
        dados,
        posicoes,
        indice
    );

    adicionarPreOrdem(
        raiz->direita,
        dados,
        posicoes,
        indice
    );
}

static void adicionarEmOrdem(
    No *raiz,
    unsigned char dados[],
    int *posicoes,
    int *indice
) {

    if (raiz == NULL) {
        return;
    }

    adicionarEmOrdem(
        raiz->esquerda,
        dados,
        posicoes,
        indice
    );

    dados[*indice] = raiz->dado;
    posicoes[*indice] = raiz->posicao;

    (*indice)++;

    adicionarEmOrdem(
        raiz->direita,
        dados,
        posicoes,
        indice
    );
}

static void adicionarPosOrdem(
    No *raiz,
    unsigned char dados[],
    int *posicoes,
    int *indice
) {

    if (raiz == NULL) {
        return;
    }

    adicionarPosOrdem(
        raiz->esquerda,
        dados,
        posicoes,
        indice
    );

    adicionarPosOrdem(
        raiz->direita,
        dados,
        posicoes,
        indice
    );

    dados[*indice] = raiz->dado;
    posicoes[*indice] = raiz->posicao;

    (*indice)++;
}

int salvarMensagem(
    Mensagem *mensagem,
    int id
) {

    if (
        mensagem == NULL ||
        mensagem->arvore == NULL ||
        mensagem->tamanho <= 0
    ) {

        return 0;
    }

    if (!criarPastaMensagens()) {
        return 0;
    }

    unsigned char *dados = malloc(
        mensagem->tamanho
    );

    int *posicoes = malloc(
        sizeof(int) * mensagem->tamanho
    );

    if (
        dados == NULL ||
        posicoes == NULL
    ) {

        free(dados);
        free(posicoes);

        return 0;
    }

    int indice = 0;

    if (mensagem->ordem == 1) {

        adicionarPreOrdem(
            mensagem->arvore,
            dados,
            posicoes,
            &indice
        );

    } else if (mensagem->ordem == 2) {

        adicionarEmOrdem(
            mensagem->arvore,
            dados,
            posicoes,
            &indice
        );

    } else if (mensagem->ordem == 3) {

        adicionarPosOrdem(
            mensagem->arvore,
            dados,
            posicoes,
            &indice
        );

    } else {

        free(dados);
        free(posicoes);

        return 0;
    }

    criptografarDados(
        dados,
        mensagem->tamanho,
        CHAVE_MENSAGEM
    );

    char nomeArquivo[100];

    snprintf(
        nomeArquivo,
        sizeof(nomeArquivo),
        PASTA_MENSAGENS "/mensagem_%03d.dat",
        id
    );

    FILE *arquivo = fopen(
        nomeArquivo,
        "wb"
    );

    if (arquivo == NULL) {

        free(dados);
        free(posicoes);

        return 0;
    }

    fwrite(
        &mensagem->tamanho,
        sizeof(int),
        1,
        arquivo
    );

    fwrite(
        &mensagem->ordem,
        sizeof(int),
        1,
        arquivo
    );

    fwrite(
        posicoes,
        sizeof(int),
        mensagem->tamanho,
        arquivo
    );

    fwrite(
        dados,
        sizeof(unsigned char),
        mensagem->tamanho,
        arquivo
    );

    fclose(arquivo);

    free(dados);
    free(posicoes);

    mensagem->id = id;
    mensagem->criptografada = 1;

    return 1;
}

int carregarMensagem(
    Mensagem *mensagem,
    int id
) {

    if (mensagem == NULL) {
        return 0;
    }

    char nomeArquivo[100];

    snprintf(
        nomeArquivo,
        sizeof(nomeArquivo),
        PASTA_MENSAGENS "/mensagem_%03d.dat",
        id
    );

    FILE *arquivo = fopen(
        nomeArquivo,
        "rb"
    );

    if (arquivo == NULL) {
        return 0;
    }

    int tamanho;
    int ordem;

    if (
        fread(
            &tamanho,
            sizeof(int),
            1,
            arquivo
        ) != 1
    ) {

        fclose(arquivo);

        return 0;
    }

    if (
        fread(
            &ordem,
            sizeof(int),
            1,
            arquivo
        ) != 1
    ) {

        fclose(arquivo);

        return 0;
    }

    if (
        tamanho <= 0 ||
        tamanho >= TAMANHO_MAXIMO
    ) {

        fclose(arquivo);

        return 0;
    }

    int *posicoes = malloc(
        sizeof(int) * tamanho
    );

    unsigned char *dados = malloc(
        tamanho
    );

    unsigned char *original = malloc(
        tamanho
    );

    if (
        posicoes == NULL ||
        dados == NULL ||
        original == NULL
    ) {

        free(posicoes);
        free(dados);
        free(original);

        fclose(arquivo);

        return 0;
    }

    if (
        fread(
            posicoes,
            sizeof(int),
            tamanho,
            arquivo
        ) != (size_t)tamanho
    ) {

        free(posicoes);
        free(dados);
        free(original);

        fclose(arquivo);

        return 0;
    }

    if (
        fread(
            dados,
            sizeof(unsigned char),
            tamanho,
            arquivo
        ) != (size_t)tamanho
    ) {

        free(posicoes);
        free(dados);
        free(original);

        fclose(arquivo);

        return 0;
    }

    fclose(arquivo);

    criptografarDados(
        dados,
        tamanho,
        CHAVE_MENSAGEM
    );

    for (
        int i = 0;
        i < tamanho;
        i++
    ) {

        if (
            posicoes[i] < 0 ||
            posicoes[i] >= tamanho
        ) {

            free(posicoes);
            free(dados);
            free(original);

            return 0;
        }

        original[
            posicoes[i]
        ] = dados[i];
    }

    memcpy(
        mensagem->texto,
        original,
        tamanho
    );

    mensagem->texto[tamanho] = '\0';

    mensagem->tamanho = tamanho;
    mensagem->ordem = ordem;
    mensagem->id = id;
    mensagem->criptografada = 1;

    free(posicoes);
    free(dados);
    free(original);

    return 1;
}