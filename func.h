#ifndef FUNC_H
#define FUNC_H

#include <stdint.h>
#include <stdio.h>

#define PAGE_SIZE 1600
#define FILL_CHAR '$'

// Estrutura que representa o registro de cabeçalho
typedef struct {
    char status;                // 1 byte
    int32_t topo;               // 4 bytes
    int32_t proxRRN;            // 4 bytes
    int32_t nroRegRem;          // 4 bytes
    int32_t nroPagDisco;        // 4 bytes
    int32_t qttCompacta;        // 4 bytes
    char padding[1578];         // Preenche o restante da página de disco com lixo
} Cabecalho;

// Estrutura que representa um registro de dados
typedef struct {
    int32_t id;                 // Exemplo de campo para um registro
    char nome[50];              // Exemplo de campo para um registro
    // Adicione mais campos conforme necessário
} Registro;

// Função para inicializar o cabeçalho
void inicializa_cabecalho(Cabecalho *c);

// Função para ler um arquivo CSV e adicionar registros ao arquivo binário
void le_csv(char *nome_arquivo, FILE *arquivo_binario, Cabecalho *cabecalho);

// Função para adicionar um registro ao arquivo binário
void adiciona_registro(FILE *arquivo, Registro *novo_registro, Cabecalho *cabecalho);

// Função para remover um registro logicamente
void remove_registro(FILE *arquivo, int rrn, Cabecalho *cabecalho);

// Função para compactar o arquivo de dados, removendo permanentemente registros excluídos
void compacta_arquivo(FILE *arquivo, Cabecalho *cabecalho);

#endif
