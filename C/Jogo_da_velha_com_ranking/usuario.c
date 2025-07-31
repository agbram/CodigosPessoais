#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include "usuario.h"

cadastro cadastroGlobal;
int totalCadastrados = 0;


void menu_cadastro() {
    int opcao, d, m, a;
    do {
        LIMPA_TELA();
        printf("\n====== MENU CADASTRO ======\n");
        printf("\n1. Cadastrar\n");
        printf("2. Pessoas cadastradas\n");
        printf("3. Excluir cadastro\n");
        printf("4. Editar usuario\n");
        printf("0. Sair\n");
        printf("Escolha a opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpa buffer

        switch (opcao) {
            case 1: cadastroJogador();      break;
            case 2: cadastrados();          break;
            case 3: excluirCadastro();      break;
            case 4: editarCadastro();       break;
            case 0: printf("Saindo...\n");  break;
            default: printf("Opcao invalida!\n"); break;
        }
    } while (opcao != 0);
}
void pausaErro() {
    // primeiro limpa qualquer '\n' pendente
    while (getchar() != '\n');
    // depois espera o ENTER de fato
    printf("Pressione ENTER para voltar...");
    getchar();
}

// Retorna 1 se for ano bissexto, 0 caso contrário
int eh_bissexto(int ano) {
    return (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
}

// Retorna 1 se a data for válida, 0 caso contrário
int valida_data(int dia, int mes, int ano) {
    // Ajuste este intervalo de anos conforme sua regra de negócio
    if (ano < 1900 || ano > 2100)
        return 0;
    if (mes < 1 || mes > 12)
        return 0;
    if (dia < 1)
        return 0;

    // Dias em cada mês (fevereiro = 28 por padrão)
    int dias_no_mes[13] = {
        0,31,28,31,30,31,30,31,31,30,31,30,31};

    // Corrige fevereiro em ano bissexto
    if (mes == 2 && eh_bissexto(ano)) {
        dias_no_mes[2] = 29;
    }

    if (dia > dias_no_mes[mes])
        return 0;

    return 1;
}


void limparCPF(char cpf[]) {
    char limpo[12];
    int j = 0;
    for (int i = 0; cpf[i]; i++) {
        if (isdigit((unsigned char)cpf[i]))
            limpo[j++] = cpf[i];
    }
    limpo[j] = '\0';
    strcpy(cpf, limpo);
}

int validarCPF(char cpf[]) {
    int i, soma = 0, resto, dig1, dig2;
    limparCPF(cpf);
    if (strlen(cpf) != 11) return 0;
    for (i = 1; i < 11 && cpf[i] == cpf[0]; i++);
    if (i == 11) return 0;
    for (i = 0; i < 9; i++) soma += (cpf[i] - '0') * (10 - i);
    resto = soma % 11;
    dig1 = (resto < 2) ? 0 : 11 - resto;
    soma = 0;
    for (i = 0; i < 10; i++) soma += (cpf[i] - '0') * (11 - i);
    resto = soma % 11;
    dig2 = (resto < 2) ? 0 : 11 - resto;
    return (cpf[9]-'0' == dig1 && cpf[10]-'0' == dig2);
}

void cadastroJogador() {
    if (totalCadastrados >= MAX) {
        printf("Limite maximo de cadastros atingido.\n");
        pausaErro();
        return;
    }

    char nomeN[TAM], nomeS[TAM];
    char cpfEnt[TAM], nick[TAM], email[TAM];
    int dia, mes, ano, idade;
    int cpfValido, cpfDuplicado, nicknameDuplicado;

    // Nome
    LIMPA_TELA();
    printf("\n-- CADASTRO DE JOGADOR --\n");
    do{printf("Primeiro nome: ");

    fgets(nomeN, TAM, stdin);  nomeN[strcspn(nomeN, "\n")] = '\0';

    } while (strlen(nomeN) == 0 || strspn(nomeN, " \t") == strlen(nomeN));
    
    do{printf("Ultimo nome:  ");

        fgets(nomeS, TAM, stdin);  nomeS[strcspn(nomeS, "\n")] = '\0';

    } while (strlen(nomeS) == 0 || strspn(nomeS, " \t") == strlen(nomeS));

    // Data de nascimento e idade
      printf("Data de nascimento (DD MM AAAA): ");
    if (scanf("%d %d %d", &dia, &mes, &ano) != 3) {
        printf("Formato invalido! Cadastro cancelado.\n");
        pausaErro();
        return;
    }
    getchar(); // limpa '\n'

    if (!valida_data(dia, mes, ano)) {
        printf("Data invalida! Informe dia, mes e ano corretos.\n");
        pausaErro();
        return;
    }

    // Cálculo de idade dinâmico
    time_t agora = time(NULL);
    struct tm hoje = *localtime(&agora);
    idade = hoje.tm_year + 1900 - ano;
    if (mes > hoje.tm_mon + 1 || (mes == hoje.tm_mon + 1 && dia > hoje.tm_mday))
        idade--;
    if (idade < 8 || idade > 100) {
        printf("Idade invalida (%d anos). Minimo 8 anos.\nCadastro cancelado.\n", idade);
        pausaErro();
        return;
    }

    // CPF (cancela se inválido ou duplicado)
    printf("Insira o CPF (somente dígitos ou formatado): ");
    fgets(cpfEnt, TAM, stdin);
    cpfEnt[strcspn(cpfEnt, "\n")] = '\0';
    if (!validarCPF(cpfEnt)) {
        printf("CPF invalido. Cadastro cancelado.\n");
        pausaErro();
        return;
    }
    for (int i = 0; i < totalCadastrados; i++) {
        if (strcmp(cadastroGlobal.cpf[i], cpfEnt) == 0) {
            printf("CPF ja cadastrado. Cadastro cancelado.\n");
            pausaErro();
            return;
        }
    }

    // Nickname (repetir até ficar único)
    do {
        printf("Nickname: ");
        fgets(nick, TAM, stdin);
        nick[strcspn(nick, "\n")] = '\0';
        if (strlen(nick) == 0 || strspn(nick, " \t") == strlen(nick)) {
            printf("O nickname não pode ficar em branco!\n");
            continue;
        }
        // verifica duplicação
        int dup = 0;
        for (int i = 0; i < totalCadastrados; i++) {
            if (strcmp(cadastroGlobal.nickname[i], nick) == 0) {
                printf("Nickname já em uso! Tente outro.\n");
                dup = 1;
                break;
            }
        }
        if (!dup) break;
    } while (1);

    // Gravação dos dados
    snprintf(cadastroGlobal.nomesN[totalCadastrados], TAM, "%s %s", nomeN, nomeS);
    cadastroGlobal.idades[totalCadastrados] = idade;
    strcpy(cadastroGlobal.cpf[totalCadastrados], cpfEnt);
    strcpy(cadastroGlobal.nickname[totalCadastrados], nick);
    

    totalCadastrados++;
    printf("\nUsuario cadastrado com sucesso!\n");
    printf("Pressione ENTER para voltar ao menu...");
    getchar();
}

void cadastrados() {
    LIMPA_TELA();
    printf("\n--- PESSOAS CADASTRADAS ---\n");
    if (totalCadastrados == 0) {
        printf("Nenhum jogador cadastrado.\n");
    } else {
        for (int i = 0; i < totalCadastrados; i++) {
            printf("\n--- ID %d ---\n", i + 1);
            printf("Nome: %s\n", cadastroGlobal.nomesN[i]);
            printf("Idade: %d\n", cadastroGlobal.idades[i]);
            printf("CPF:%s\n", cadastroGlobal.cpf[i]);
            printf("Nickname: %s\n", cadastroGlobal.nickname[i]);
        }
    }
    printf("\nPressione ENTER para voltar.");
    getchar();
}

void excluirCadastro() {
    char entrada[TAM];
    int index = -1, confirma;

    LIMPA_TELA();
    printf("\n--- EXCLUIR USUARIO ---\n");
    if (totalCadastrados == 0) {
        printf("Nenhum jogador cadastrado.\n");
        pausaErro();
        return;
    }

    printf("Informe ID ou nickname: ");
    fgets(entrada, TAM, stdin);
    entrada[strcspn(entrada, "\n")] = '\0';

    // Busca por ID
    int id = atoi(entrada);
    if (id > 0 && id <= totalCadastrados) {
        index = id - 1;
    } else {
        // Busca por nickname
        for (int i = 0; i < totalCadastrados; i++) {
            if (strcmp(cadastroGlobal.nickname[i], entrada) == 0) {
                index = i;
                break;
            }
        }
    }

    if (index < 0) {
        printf("Usuario nao encontrado.\n");
        pausaErro();
        return;
    }

    printf("Usuario encontrado: %s (%s)\n",
           cadastroGlobal.nomesN[index],
           cadastroGlobal.nickname[index]);
    printf("Confirma exclusao? (1-Sim / 2-Nao): ");
    scanf("%d", &confirma);
    getchar();

    if (confirma != 1) {
        printf("Exclusao cancelada.\n");
        return;
    }

    // Desloca tudo para “fechar o buraco”
    for (int i = index; i < totalCadastrados - 1; i++) {
        strcpy(cadastroGlobal.nomesN[i],cadastroGlobal.nomesN[i+1]);
        cadastroGlobal.idades[i] = cadastroGlobal.idades[i+1];
        strcpy(cadastroGlobal.cpf[i],cadastroGlobal.cpf[i+1]);
        strcpy(cadastroGlobal.nickname[i],cadastroGlobal.nickname[i+1]);
    }
    totalCadastrados--;
    printf("Usuario excluido com sucesso!\n");
}


void editarCadastro() {
    LIMPA_TELA();
    printf("\n--- EDITAR USUARIO ---\n");

    if (totalCadastrados == 0) {
        printf("Nenhum jogador cadastrado.\n");
        return;
    }

    char entrada[TAM];
    int index = -1, opcao;

    printf("Informe ID ou nickname: ");
    fgets(entrada, TAM, stdin);
    entrada[strcspn(entrada, "\n")] = '\0';

    int id = atoi(entrada);
    if (id > 0 && id <= totalCadastrados) {
        index = id - 1;
    } else {
        for (int i = 0; i < totalCadastrados; i++) {
            if (strcmp(cadastroGlobal.nickname[i], entrada) == 0) {
                index = i;
                break;
            }
        }
    }

    if (index < 0) {
        printf("Usuario nao encontrado.\n");
        return;
    }

    do {
        LIMPA_TELA();
        printf("\n--- Editando: %s ---\n", cadastroGlobal.nomesN[index]);
        printf("1. Primeiro nome\n");
        printf("2. Ultimo nome\n");
        printf("3. Nickname\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // Limpa buffer

        switch (opcao) {
            case 1: {
                char novo[TAM];
                printf("Novo primeiro nome: ");
                fgets(novo, TAM, stdin);
                novo[strcspn(novo, "\n")] = '\0';

                char *sep = strchr(cadastroGlobal.nomesN[index], ' ');
                if (sep) {
                    sprintf(cadastroGlobal.nomesN[index], "%s%s", novo, sep);
                }
                break;
            }

            case 2: {
                char novo[TAM], prim[TAM];
                printf("Novo ultimo nome: ");
                fgets(novo, TAM, stdin);
                novo[strcspn(novo, "\n")] = '\0';

                sscanf(cadastroGlobal.nomesN[index], "%s", prim);
                sprintf(cadastroGlobal.nomesN[index], "%s %s", prim, novo);
                break;
            }

            case 3: {
                char novo[TAM];
                printf("Antigo nickname: %s\n", cadastroGlobal.nickname[index]);
                printf("Novo nickname: ");
                fgets(novo, TAM, stdin);
                novo[strcspn(novo, "\n")] = '\0';
                strcpy(cadastroGlobal.nickname[index], novo);
                break;
            }

            case 0:
                printf("Saindo da edição...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);
}


