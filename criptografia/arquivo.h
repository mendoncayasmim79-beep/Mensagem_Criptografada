#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "mensagem.h"

#define PASTA_MENSAGENS "mensagens"

int criarPastaMensagens(void);

int salvarMensagem(Mensagem *mensagem, int id);

int carregarMensagem(Mensagem *mensagem, int id);

int proximoIdMensagem(void);

#endif