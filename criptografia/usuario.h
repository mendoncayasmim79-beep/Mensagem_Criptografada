#ifndef USUARIO_H
#define USUARIO_H

#define MAX_USUARIOS 2

#define TAMANHO_NOME 100
#define TAMANHO_EMAIL 100
#define TAMANHO_SENHA 100

typedef struct {

    char nome[TAMANHO_NOME];

    char email[TAMANHO_EMAIL];

    char senha[TAMANHO_SENHA];

} Usuario;

int cadastrarUsuarios(void);

int fazerLogin(Usuario *usuario);

#endif