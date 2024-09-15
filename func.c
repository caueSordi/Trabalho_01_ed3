#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "func.h"


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

// Exemplo de estrutura Registro (ajuste conforme necessário)
typedef struct {
    int32_t id;                // Exemplo de campo para um registro
    char nome[50];             // Exemplo de campo para um registro
    // Adicione mais campos conforme necessário
} Registro;

void inicializa_cabecalho(Cabecalho *c) {
    c->status = '0';
    c->topo = -1;
    c->proxRRN = 0;
    c->nroRegRem = 0;
    c->nroPagDisco = 0;
    c->qttCompacta = 0;

    // Preencher o restante da página de disco com o caractere '$'
    memset(c->padding, FILL_CHAR, sizeof(c->padding));
}


void le_csv(char *nome_arquivo, FILE *arquivo_binario, Cabecalho *cabecalho) {
    FILE *arquivo_csv = fopen(nome_arquivo, "r");
    if (arquivo_csv == NULL) {
        printf("Falha ao abrir o arquivo CSV.\n");
        return;
    }

    Registro registro;
    while (fscanf(arquivo_csv, "%d,%49[^\n]", &registro.id, registro.nome) == 2) {
        // Adiciona o registro ao arquivo binário
        adiciona_registro(arquivo_binario, &registro, cabecalho);
    }

    fclose(arquivo_csv);
}

void adiciona_registro(FILE *arquivo, Registro *novo_registro, Cabecalho *cabecalho) {
    // Move o ponteiro para o final do arquivo
    fseek(arquivo, 0, SEEK_END);

    // Escreve o novo registro no arquivo
    fwrite(novo_registro, sizeof(Registro), 1, arquivo);

    // Atualiza o valor de proxRRN e nroPagDisco no cabeçalho
    cabecalho->proxRRN++;
    cabecalho->nroPagDisco = (ftell(arquivo) + PAGE_SIZE - 1) / PAGE_SIZE; // Calcula o número de páginas ocupadas

    // Volta ao início do arquivo para atualizar o cabeçalho
    fseek(arquivo, 0, SEEK_SET);
    fwrite(cabecalho, sizeof(Cabecalho), 1, arquivo);
}

void remove_registro(FILE *arquivo, int rrn, Cabecalho *cabecalho) {
    // Calcula a posição do registro a ser removido com base no RRN
    long pos = sizeof(Cabecalho) + rrn * sizeof(Registro);
    
    // Vai para a posição do registro
    fseek(arquivo, pos, SEEK_SET);

    // Marca o registro como removido (por exemplo, gravando -1 no campo id)
    Registro registro;
    fread(&registro, sizeof(Registro), 1, arquivo);
    registro.id = -1;

    // Volta para a posição do registro e reescreve-o
    fseek(arquivo, pos, SEEK_SET);
    fwrite(&registro, sizeof(Registro), 1, arquivo);

    // Atualiza o número de registros removidos no cabeçalho
    cabecalho->nroRegRem++;

    // Atualiza o cabeçalho no arquivo
    fseek(arquivo, 0, SEEK_SET);
    fwrite(cabecalho, sizeof(Cabecalho), 1, arquivo);
}


void compacta_arquivo(FILE *arquivo, Cabecalho *cabecalho) {
    // Crie um novo arquivo temporário para armazenar os registros válidos
    FILE *arquivo_temp = fopen("temp.bin", "wb");

    // Copia o cabeçalho para o arquivo temporário
    fwrite(cabecalho, sizeof(Cabecalho), 1, arquivo_temp);

    Registro registro;
    while (fread(&registro, sizeof(Registro), 1, arquivo) == 1) {
        if (registro.id != -1) { // Se o registro não foi removido
            fwrite(&registro, sizeof(Registro), 1, arquivo_temp);
        }
    }

    // Fecha ambos os arquivos e substitui o arquivo original pelo temporário
    fclose(arquivo);
    fclose(arquivo_temp);
    remove("arquivo.bin");
    rename("temp.bin", "arquivo.bin");

    // Atualiza o cabeçalho (incrementa a quantidade de compactações)
    cabecalho->qttCompacta++;
}
