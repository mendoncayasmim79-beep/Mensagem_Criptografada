#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "historico.h"
#include "criptografia.h"

#define ARQUIVO_HISTORICO "historico.dat"
#define CHAVE_HISTORICO "chave_historico"

static int salvarHistorico(
    RegistroHistorico *registro
) {

    unsigned char dados[
        TAMANHO_MENSAGEM_HISTORICO + 300
    ];

    int tamanho = snprintf(
        (char *)dados,
        sizeof(dados),
        "%d|%s|%s|%d|%d|%s",
        registro->idMensagem,
        registro->nomeRemetente,
        registro->nomeLeitor,
        registro->enviada,
        registro->lida,
        registro->mensagem
    );

    if (
        tamanho <= 0 ||
        tamanho >= (int)sizeof(dados)
    ) {
        return 0;
    }

    criptografarDados(
        dados,
        tamanho,
        CHAVE_HISTORICO
    );

    FILE *arquivo = fopen(
        ARQUIVO_HISTORICO,
        "ab"
    );

    if (arquivo == NULL) {
        return 0;
    }

    if (
        fwrite(
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
        fwrite(
            dados,
            sizeof(unsigned char),
            tamanho,
            arquivo
        ) != (size_t)tamanho
    ) {

        fclose(arquivo);

        return 0;
    }

    fclose(arquivo);

    return 1;
}

int registrarEnvio(
    int idMensagem,
    const char *nomeRemetente,
    const char *mensagem
) {

    RegistroHistorico registro;

    memset(
        &registro,
        0,
        sizeof(RegistroHistorico)
    );

    registro.idMensagem = idMensagem;

    strncpy(
        registro.nomeRemetente,
        nomeRemetente,
        TAMANHO_NOME - 1
    );

    strncpy(
        registro.mensagem,
        mensagem,
        TAMANHO_MENSAGEM_HISTORICO - 1
    );

    registro.enviada = 1;
    registro.lida = 0;

    return salvarHistorico(
        &registro
    );
}

int registrarLeitura(
    int idMensagem,
    const char *nomeLeitor
) {

    RegistroHistorico registro;

    memset(
        &registro,
        0,
        sizeof(RegistroHistorico)
    );

    registro.idMensagem = idMensagem;

    strncpy(
        registro.nomeLeitor,
        nomeLeitor,
        TAMANHO_NOME - 1
    );

    registro.lida = 1;

    return salvarHistorico(
        &registro
    );
}

static int separarRegistro(
    char *dados,
    int *idMensagem,
    char *nomeRemetente,
    char *nomeLeitor,
    int *enviada,
    int *lida,
    char *mensagem
) {

    char *p1;
    char *p2;
    char *p3;
    char *p4;
    char *p5;

    p1 = strchr(dados, '|');

    if (p1 == NULL) {
        return 0;
    }

    *p1 = '\0';

    p2 = strchr(p1 + 1, '|');

    if (p2 == NULL) {
        return 0;
    }

    *p2 = '\0';

    p3 = strchr(p2 + 1, '|');

    if (p3 == NULL) {
        return 0;
    }

    *p3 = '\0';

    p4 = strchr(p3 + 1, '|');

    if (p4 == NULL) {
        return 0;
    }

    *p4 = '\0';

    p5 = strchr(p4 + 1, '|');

    if (p5 == NULL) {
        return 0;
    }

    *p5 = '\0';

    *idMensagem = atoi(dados);

    strncpy(
        nomeRemetente,
        p1 + 1,
        TAMANHO_NOME - 1
    );

    nomeRemetente[
        TAMANHO_NOME - 1
    ] = '\0';

    strncpy(
        nomeLeitor,
        p2 + 1,
        TAMANHO_NOME - 1
    );

    nomeLeitor[
        TAMANHO_NOME - 1
    ] = '\0';

    *enviada = atoi(
        p3 + 1
    );

    *lida = atoi(
        p4 + 1
    );

    strncpy(
        mensagem,
        p5 + 1,
        TAMANHO_MENSAGEM_HISTORICO - 1
    );

    mensagem[
        TAMANHO_MENSAGEM_HISTORICO - 1
    ] = '\0';

    return 1;
}

int visualizarHistorico(void) {

    FILE *arquivo = fopen(
        ARQUIVO_HISTORICO,
        "rb"
    );

    if (arquivo == NULL) {

        printf(
            "\nNenhum historico encontrado.\n"
        );

        return 0;
    }

    printf("\n=================================\n");
    printf("           HISTORICO\n");
    printf("=================================\n");

    int quantidade = 0;

    while (1) {

        int tamanho;

        if (
            fread(
                &tamanho,
                sizeof(int),
                1,
                arquivo
            ) != 1
        ) {
            break;
        }

        if (
            tamanho <= 0 ||
            tamanho > TAMANHO_MENSAGEM_HISTORICO + 300
        ) {

            break;
        }

        unsigned char *dados = malloc(
            tamanho + 1
        );

        if (dados == NULL) {

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

            free(dados);

            break;
        }

        dados[tamanho] = '\0';

        criptografarDados(
            dados,
            tamanho,
            CHAVE_HISTORICO
        );

        int idMensagem;
        int enviada;
        int lida;

        char nomeRemetente[TAMANHO_NOME];
        char nomeLeitor[TAMANHO_NOME];

        char mensagem[
            TAMANHO_MENSAGEM_HISTORICO
        ];

        memset(
            nomeRemetente,
            0,
            sizeof(nomeRemetente)
        );

        memset(
            nomeLeitor,
            0,
            sizeof(nomeLeitor)
        );

        memset(
            mensagem,
            0,
            sizeof(mensagem)
        );

        if (
            separarRegistro(
                (char *)dados,
                &idMensagem,
                nomeRemetente,
                nomeLeitor,
                &enviada,
                &lida,
                mensagem
            )
        ) {

            printf(
                "\n---------------------------------\n"
            );

            printf(
                "MENSAGEM %03d\n",
                idMensagem
            );

            if (enviada) {

                printf(
                    "Enviada por: %s\n",
                    nomeRemetente
                );

                printf(
                    "Mensagem: %s\n",
                    mensagem
                );
            }

            if (lida) {

                printf(
                    "Lida por: %s\n",
                    nomeLeitor
                );
            }

            quantidade++;
        }

        free(dados);
    }

    fclose(arquivo);

    if (quantidade == 0) {

        printf(
            "\nNenhum registro encontrado.\n"
        );
    }

    printf(
        "\n---------------------------------\n"
    );

    return 1;
}