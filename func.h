#ifndef FUNC_H
#define FUNC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PAGE_SIZE 1600   // Tamanho da página de disco
#define FILL_CHAR '$'    // Caractere de preenchimento para completar registros
#define NULL_CHAR '#'    // Caractere usado como delimitador nas strings

// Estrutura que representa o registro de cabeçalho
typedef struct {
    char status;                // 1 byte - Status do arquivo ('0' = inconsistente, '1' = consistente)
    int topo;                   // 4 bytes - RRN do primeiro registro logicamente removido
    int proxRRN;                // 4 bytes - RRN do próximo registro a ser inserido
    int nroRegRem;              // 4 bytes - Número de registros logicamente removidos
    int nroPagDisco;            // 4 bytes - Número de páginas de disco ocupadas
    int qttCompacta;            // 4 bytes - Quantidade de vezes que o arquivo foi compactado
    char padding[1600 - (sizeof(char) + 5 * sizeof(int))];  // Preencher o restante da página de disco com lixo
} Cabecalho;

// Estrutura para armazenar os dados de um registro
typedef struct {
   int populacao;          // População do dinossauro
   float tamanho;          // Tamanho do dinossauro
   char uniMedida;         // Unidade de medida
   int velocidade;         // Velocidade do dinossauro
   char *nome;             // Nome do dinossauro
   char *nEspecie;         // Nome da espécie do dinossauro
   char *habitat;          // Habitat do dinossauro
   char *tipo;             // Tipo do dinossauro
   char *dieta;            // Dieta do dinossauro
   char *alimento;         // Alimento favorito do dinossauro

   char removido;          // Indica se o registro foi removido ('0' = ativo, '1' = removido)
   int encadeamento;       // Campo de encadeamento para lista de registros removidos
} Registro;

// -------- Funções Fornecidas -----------------
void binarioNaTela(char *nomeArquivoBinario);  // Função para exibir binário na tela
void scan_quote_string(char *str);             // Função fornecida para leitura de strings entre aspas
//----------------------------------------------

// Funções de cabeçalho e registros
void inicializa_cabecalho(Cabecalho *c);  // Inicializa o cabeçalho com valores padrão

// Funções de manipulação de arquivos CSV e binários
void lendo_csv(char *nomeCSV, FILE *bin, Cabecalho *cabecalho);  // Função para ler um arquivo CSV e converter em binário
void arquivobin(FILE *nomebin, Registro registro, int aux, Cabecalho *c);  // Função para gravar registros no binário

// Função para recuperar todos os registros do arquivo binário e exibi-los na saída padrão
void recuperar_todos_os_registros(char *nomeBin);

void buscar_registros_por_campo(char *nomeBin);
#endif
