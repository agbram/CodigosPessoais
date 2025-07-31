#ifndef USUARIO_H
#define USUARIO_H

#define MAX 100
#define TAM 100

#ifdef _WIN32
    #define LIMPA_TELA() system("cls")
#else
    #define LIMPA_TELA() system("clear")
#endif

typedef struct 
{
char nomesN[MAX][TAM];
char nomesS[MAX][TAM];
int idades[MAX];
char cpf[MAX][TAM];
char cpfEnt[TAM];
char nickname[MAX][TAM];
} cadastro;

extern cadastro cadastroGlobal;
extern int totalCadastrados;

void menu_cadastro(void);
void cadastroJogador();
void cadastrados();
void excluirCadastro();
void editarCadastro();
void pausaErro();


#endif
