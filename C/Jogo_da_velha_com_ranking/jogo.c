#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h"


void inicio(void) {
    char entrada[TAM];
    int index = -1, confirma;

    LIMPA_TELA();
    printf("\n--- ESCOLHA SEU USUÁRIO ---\n");

    if (totalCadastrados == 0) {
        printf("Nenhum jogador cadastrado.\n");
        pausaErro();
        return;
    }

    printf("Informe o ID (número) ou nickname: ");
    fgets(entrada, TAM, stdin);
    entrada[strcspn(entrada, "\n")] = '\0';  // Remove o \n

    // Tenta interpretar como ID
    int id = atoi(entrada);
    if (id > 0 && id <= totalCadastrados) {
        index = id - 1;
    } else {
        // Caso contrário, procura por nickname
        for (int i = 0; i < totalCadastrados; i++) {
            if (strcmp(cadastroGlobal.nickname[i], entrada) == 0) {
                index = i;
                break;
            }
        }
    }

    if (index == -1) {
        printf("Usuário não encontrado.\n");
        pausaErro();
        return;
    }

    
    printf("Usuário encontrado: %s\n", cadastroGlobal.nickname[index]);
    printf("Deseja jogar com esse usuário? (1-Sim / 2-Não): ");
    if (scanf("%d", &confirma) != 1) {
        getchar(); // Limpa o buffer
        printf("Entrada inválida.\n");
        pausaErro();
        return;
    }
    getchar(); // Limpa o '\n' deixado pelo scanf


    if (confirma == 1) {
        LIMPA_TELA();
        jogarComUsuario(index);
    } else {
        printf("Operação cancelada.\n");
        pausaErro();
    }
}

void jogarComUsuario(int index) {
    // Aqui começa o seu jogo de verdade...
    printf("=== Bem-vindo ao TRUNFO, %s! ===\n\n", cadastroGlobal.nickname[index]);
   
    
    printf("Pressione ENTER para iniciar o desafio...");
    getchar();

    printf("\n-- Fim do jogo para %s --\n", cadastroGlobal.nickname[index]);
    pausaErro();
}
