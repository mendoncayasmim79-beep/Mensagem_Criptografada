#include <stdio.h>
#include <string.h>

#include "usuario.h"

#define ARQUIVO_USUARIOS "usuario.dat"

static int carregarUsuarios(Usuario usuarios[]) {

    FILE *arquivo = fopen(
        ARQUIVO_USUARIOS,
        "rb"
    );

    if (arquivo == NULL) {
        return 0;
    }

    int quantidade = 0;

    if (
        fread(
            &quantidade,
            sizeof(int),
            1,
            arquivo
        ) != 1
    ) {

        fclose(arquivo);

        return 0;
    }

    if (
        quantidade < 0 ||
        quantidade > MAX_USUARIOS
    ) {

        fclose(arquivo);

        return 0;
    }

    if (
        fread(
            usuarios,
            sizeof(Usuario),
            quantidade,
            arquivo
        ) != (size_t)quantidade
    ) {

        fclose(arquivo);

        return 0;
    }

    fclose(arquivo);

    return quantidade;
}

static int salvarUsuarios(
    Usuario usuarios[],
    int quantidade
) {

    FILE *arquivo = fopen(
        ARQUIVO_USUARIOS,
        "wb"
    );

    if (arquivo == NULL) {
        return 0;
    }

    if (
        fwrite(
            &quantidade,
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
            usuarios,
            sizeof(Usuario),
            quantidade,
            arquivo
        ) != (size_t)quantidade
    ) {

        fclose(arquivo);

        return 0;
    }

    fclose(arquivo);

    return 1;
}

int cadastrarUsuarios(void) {

    Usuario usuarios[MAX_USUARIOS];

    /*
        Se o arquivo ja existe e possui usuarios,
        nao cadastra novamente.
    */

    int quantidade = carregarUsuarios(usuarios);

    if (quantidade == MAX_USUARIOS) {

        printf(
            "\nUsuarios ja cadastrados.\n"
        );

        return 1;
    }

    printf("\n=================================\n");
    printf("             CADASTRO\n");
    printf("=================================\n");

    for (
        int i = 0;
        i < MAX_USUARIOS;
        i++
    ) {

        printf(
            "\n--- Usuario %d ---\n",
            i + 1
        );

        printf("Nome: ");

        fgets(
            usuarios[i].nome,
            TAMANHO_NOME,
            stdin
        );

        usuarios[i].nome[
            strcspn(
                usuarios[i].nome,
                "\n"
            )
        ] = '\0';

        printf("Email: ");

        fgets(
            usuarios[i].email,
            TAMANHO_EMAIL,
            stdin
        );

        usuarios[i].email[
            strcspn(
                usuarios[i].email,
                "\n"
            )
        ] = '\0';

        printf("Senha: ");

        fgets(
            usuarios[i].senha,
            TAMANHO_SENHA,
            stdin
        );

        usuarios[i].senha[
            strcspn(
                usuarios[i].senha,
                "\n"
            )
        ] = '\0';
    }

    if (
        !salvarUsuarios(
            usuarios,
            MAX_USUARIOS
        )
    ) {

        printf(
            "\nErro ao salvar usuarios.\n"
        );

        return 0;
    }

    printf(
        "\nUsuarios cadastrados com sucesso!\n"
    );

    return 1;
}

int fazerLogin(Usuario *usuario) {

    Usuario usuarios[MAX_USUARIOS];

    int quantidade = carregarUsuarios(
        usuarios
    );

    if (quantidade == 0) {

        printf(
            "\nNenhum usuario cadastrado.\n"
        );

        return 0;
    }

    char email[TAMANHO_EMAIL];
    char senha[TAMANHO_SENHA];

    printf("\n=================================\n");
    printf("              LOGIN\n");
    printf("=================================\n");

    printf("Email: ");

    fgets(
        email,
        TAMANHO_EMAIL,
        stdin
    );

    email[
        strcspn(
            email,
            "\n"
        )
    ] = '\0';

    printf("Senha: ");

    fgets(
        senha,
        TAMANHO_SENHA,
        stdin
    );

    senha[
        strcspn(
            senha,
            "\n"
        )
    ] = '\0';

    for (
        int i = 0;
        i < quantidade;
        i++
    ) {

        if (
            strcmp(
                email,
                usuarios[i].email
            ) == 0
            &&
            strcmp(
                senha,
                usuarios[i].senha
            ) == 0
        ) {

            *usuario = usuarios[i];

            printf(
                "\nLogin realizado com sucesso!\n"
            );

            printf(
                "Bem-vindo, %s!\n",
                usuario->nome
            );

            return 1;
        }
    }

    printf(
        "\nEmail ou senha incorretos.\n"
    );

    return 0;
}