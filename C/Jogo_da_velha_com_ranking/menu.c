// menu.c
#include <stdio.h>
#include <stdlib.h>
#include "usuario.h"
#include "jogo.h"

void menuJogo(void) {
    int opcao;
    do {
        LIMPA_TELA();
        printf("\n=== TRUNFO ===\n\n");
        printf("1. Iniciar\n");
        printf("2. Configuração de usuário\n");
        printf("0. Sair\n");
        printf("Escolha a opção: ");
        if (scanf("%d", &opcao) != 1) {
            getchar(); // limpa lixo
            opcao = -1;
        }
        getchar(); // limpa '\n'

        switch (opcao) {
            case 1:
                inicio();
                break;
            case 2:
                menu_cadastro();
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
                pausaErro();
        }
    } while (opcao != 0);
}
