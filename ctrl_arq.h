#ifndef CTRL_ARQ_H
#define CTRL_ARQ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 1600    // Definição do tamanho de página
#define FILL_CHAR '$'     // Caractere de preenchimento

// Estrutura de cabeçalho
typedef struct {
    char status;                // 1 byte
    int topo;                   // 4 bytes
    int proxRRN;                // 4 bytes
    int nroRegRem;              // 4 bytes
    int nroPagDisco;            // 4 bytes
    int qttCompacta;            // 4 bytes
} Cabecalho;

// Função para inicializar e escrever o cabeçalho no arquivo binário
void inicializa_cabecalho(Cabecalho *c, FILE* bin);

// Função para ler o cabeçalho do arquivo binário
void ler_cabecalho(FILE *bin, Cabecalho *c);

// Função para abrir o arquivo e inicializar o cabeçalho
void abre_arq(char *nomeArquivo, FILE **arq, Cabecalho *cab);

// Função para fechar o arquivo e atualizar o cabeçalho
void fecha_arq(FILE *arq, Cabecalho *cab);

#endif
