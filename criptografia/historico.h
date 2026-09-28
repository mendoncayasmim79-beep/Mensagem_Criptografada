#ifndef HISTORICO_H
#define HISTORICO_H

#define TAMANHO_NOME 100
#define TAMANHO_MENSAGEM_HISTORICO 5000

typedef struct {

    int idMensagem;

    char nomeRemetente[TAMANHO_NOME];
    char nomeLeitor[TAMANHO_NOME];

    char mensagem[TAMANHO_MENSAGEM_HISTORICO];

    int enviada;
    int lida;

} RegistroHistorico;

int registrarEnvio(
    int idMensagem,
    const char *nomeRemetente,
    const char *mensagem
);

int registrarLeitura(
    int idMensagem,
    const char *nomeLeitor
);

int visualizarHistorico(void);

#endif