#include <stdio.h>
#include <string.h>

#include "interface.h"
#include "mensagem.h"
#include "arquivo.h"
#include "historico.h"

static int autenticarUsuario(Usuario *usuario) {

    char email[TAMANHO_EMAIL];
    char senha[TAMANHO_SENHA];

    printf("\n=================================\n");
    printf("       AUTENTICACAO NECESSARIA\n");
    printf("=================================\n");

    printf("Email: ");

    fgets(
        email,
        TAMANHO_EMAIL,
        stdin
    );

    email[
        strcspn(email, "\n")
    ] = '\0';

    printf("Senha: ");

    fgets(
        senha,
        TAMANHO_SENHA,
        stdin
    );

    senha[
        strcspn(senha, "\n")
    ] = '\0';

    if (
        strcmp(email, usuario->email) == 0 &&
        strcmp(senha, usuario->senha) == 0
    ) {

        printf(
            "\nAutenticacao realizada com sucesso!\n"
        );

        return 1;
    }

    printf(
        "\nEmail ou senha incorretos.\n"
    );

    return 0;
}

static int escolherOrdem(Mensagem *mensagem) {

    int opcao;

    printf("\n=================================\n");
    printf("       ESCOLHA A ORDEM\n");
    printf("=================================\n");

    printf("1 - Pre-ordem\n");
    printf("2 - Em-ordem\n");
    printf("3 - Pos-ordem\n");

    printf("\nEscolha: ");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {

        case 1:

            mensagem->ordem = 1;

            printf(
                "\nOrdem escolhida: Pre-ordem\n"
            );

            return 1;

        case 2:

            mensagem->ordem = 2;

            printf(
                "\nOrdem escolhida: Em-ordem\n"
            );

            return 1;

        case 3:

            mensagem->ordem = 3;

            printf(
                "\nOrdem escolhida: Pos-ordem\n"
            );

            return 1;

        default:

            printf(
                "\nOpcao invalida.\n"
            );

            return 0;
    }
}

static void processarMensagem(
    Mensagem *mensagem,
    Usuario *usuario
) {

    int opcao;

    if (!escreverMensagem(mensagem, usuario)) {
        return;
    }

    if (!gerarArvoreMensagem(mensagem)) {

        printf(
            "\nErro ao gerar a arvore.\n"
        );

        return;
    }

    printf("\n=================================\n");
    printf("          ARVORE GERADA\n");
    printf("=================================\n");

    printf("\nPre-ordem: ");
    preOrdem(mensagem->arvore);

    printf("\nEm-ordem: ");
    emOrdem(mensagem->arvore);

    printf("\nPos-ordem: ");
    posOrdem(mensagem->arvore);

    printf("\n");

    if (!escolherOrdem(mensagem)) {
        return;
    }

    do {

        printf("\n=================================\n");
        printf("       MENSAGEM PRONTA\n");
        printf("=================================\n");

        printf("1 - Encriptar\n");
        printf("2 - Salvar\n");
        printf("3 - Voltar\n");

        printf("\nEscolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {

            case 1:

                mensagem->criptografada = 1;

                printf(
                    "\nMensagem encriptada com sucesso!\n"
                );

                break;

            case 2: {

                if (!mensagem->criptografada) {

                    printf(
                        "\nPrimeiro encripte a mensagem.\n"
                    );

                    break;
                }

                int id = proximoIdMensagem();

                if (salvarMensagem(mensagem, id)) {

                    if (
                        registrarEnvio(
                            id,
                            usuario->nome,
                            (char *)mensagem->texto
                        )
                    ) {

                        printf(
                            "\nMensagem %03d salva com sucesso!\n",
                            id
                        );

                        printf(
                            "O historico tambem foi atualizado.\n"
                        );

                    } else {

                        printf(
                            "\nMensagem salva, mas houve erro no historico.\n"
                        );
                    }

                } else {

                    printf(
                        "\nErro ao salvar a mensagem.\n"
                    );
                }

                break;
            }

            case 3:

                printf(
                    "\nVoltando...\n"
                );

                break;

            default:

                printf(
                    "\nOpcao invalida.\n"
                );
        }

    } while (opcao != 3);
}

static void lerMensagem(Usuario *usuario) {

    int id;

    Mensagem mensagem;

    inicializarMensagem(&mensagem);

    printf("\n=================================\n");
    printf("          LER MENSAGEM\n");
    printf("=================================\n");

    if (!autenticarUsuario(usuario)) {

        printf(
            "\nAcesso negado.\n"
        );

        return;
    }

    printf("\nDigite o ID da mensagem: ");
    scanf("%d", &id);
    getchar();

    if (!carregarMensagem(&mensagem, id)) {

        printf(
            "\nNao foi possivel carregar a mensagem.\n"
        );

        liberarMensagem(&mensagem);

        return;
    }

    printf("\n---------------------------------\n");

    printf(
        "Mensagem %03d:\n\n",
        mensagem.id
    );

    printf(
        "%s\n",
        mensagem.texto
    );

    printf("---------------------------------\n");

    if (
        registrarLeitura(
            id,
            usuario->nome
        )
    ) {

        printf(
            "\nLeitura registrada no historico.\n"
        );

    } else {

        printf(
            "\nMensagem lida, mas houve erro ao registrar o historico.\n"
        );
    }

    liberarMensagem(&mensagem);
}

static void acessarHistorico(Usuario *usuario) {

    printf("\n=================================\n");
    printf("           HISTORICO\n");
    printf("=================================\n");

    if (!autenticarUsuario(usuario)) {

        printf(
            "\nAcesso negado.\n"
        );

        return;
    }

    visualizarHistorico();
}

void menuMensagens(Usuario *usuario) {

    int opcao;

    do {

        Mensagem mensagem;

        inicializarMensagem(&mensagem);

        printf("\n=================================\n");
        printf("           MENSAGENS\n");
        printf("=================================\n");

        printf("1 - Escrever mensagem\n");
        printf("2 - Ler mensagem\n");
        printf("3 - Voltar\n");

        printf("\nEscolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {

            case 1:

                processarMensagem(
                    &mensagem,
                    usuario
                );

                liberarMensagem(&mensagem);

                break;

            case 2:

                lerMensagem(usuario);

                break;

            case 3:

                printf(
                    "\nVoltando...\n"
                );

                break;

            default:

                printf(
                    "\nOpcao invalida.\n"
                );
        }

    } while (opcao != 3);
}

void mostrarMenuPrincipal(Usuario *usuario) {

    int opcao;

    do {

        printf("\n=================================\n");
        printf("       SISTEMA DE MENSAGENS\n");
        printf("=================================\n");

        printf(
            "Ola, %s!\n\n",
            usuario->nome
        );

        printf("1 - Mensagens\n");
        printf("2 - Historico\n");
        printf("3 - Sair\n");

        printf("\nEscolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {

            case 1:

                menuMensagens(usuario);

                break;

            case 2:

                acessarHistorico(usuario);

                break;

            case 3:

                printf(
                    "\nSaindo do sistema...\n"
                );

                break;

            default:

                printf(
                    "\nOpcao invalida.\n"
                );
        }

    } while (opcao != 3);
}