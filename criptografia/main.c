#include <stdio.h>

#include "usuario.h"
#include "interface.h"

int main() {

    Usuario usuario;

    printf("=================================\n");
    printf("      SISTEMA DE MENSAGENS\n");
    printf("=================================\n");

    if (!cadastrarUsuarios()) {

        printf(
            "\nNao foi possivel preparar os usuarios.\n"
        );

        return 0;
    }

    printf(
        "\nAgora faca o login para entrar no sistema.\n"
    );

    if (!fazerLogin(&usuario)) {

        printf(
            "\nAcesso negado.\n"
        );

        return 0;
    }

    mostrarMenuPrincipal(
        &usuario
    );

    return 0;
}