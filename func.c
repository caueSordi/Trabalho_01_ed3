#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "func.h"

#define PAGE_SIZE 1600
#define FILL_CHAR '$'

// Função para inicializar o cabeçalho
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

// Função para ler o CSV e gravar os registros no arquivo binário
void lendo_csv(char *nomeCSV, FILE *nomeBin, Cabecalho *cabecalho) {
    Registro registro;
    FILE *arquivo_csv = fopen(nomeCSV, "r");
    if (arquivo_csv == NULL) {
        printf("Falha ao abrir o arquivo CSV\n");
        return;
    }

    // Pular a primeira linha do CSV (nomes das colunas)
    char linha[1024];
    fgets(linha, sizeof(linha), arquivo_csv);

    // Ler o CSV e gravar no binário
    while (fscanf(arquivo_csv, "%99[^,],%29[^,],%99[^,],%d,%29[^,],%d,%c,%f,%99[^,],%99[^\n]\n", 
                  registro.nome, registro.dieta, registro.habitat, &registro.populacao, 
                  registro.tipo, &registro.velocidade, registro.uniMedida, 
                  &registro.tamanho, registro.nEspecie, registro.alimento) == 10) {
        // Grava o registro no arquivo binário
        arquivobin(nomeBin, registro);
    }

    fclose(arquivo_csv);
}

// Função para escrever os registros no arquivo binário
void arquivobin(FILE *nomeBin, Registro registro) {
    fwrite(registro.nome, sizeof(char), 100, nomeBin);
    fwrite(registro.dieta, sizeof(char), 30, nomeBin);
    fwrite(registro.habitat, sizeof(char), 100, nomeBin);
    fwrite(&registro.populacao, sizeof(int), 1, nomeBin);
    fwrite(registro.tipo, sizeof(char), 30, nomeBin);
    fwrite(&registro.velocidade, sizeof(int), 1, nomeBin);
    fwrite(registro.uniMedida, sizeof(char), 1, nomeBin);
    fwrite(&registro.tamanho, sizeof(float), 1, nomeBin);
    fwrite(registro.nEspecie, sizeof(char), 100, nomeBin);
    fwrite(registro.alimento, sizeof(char), 100, nomeBin);
}

// Função para recuperar todos os registros e mostrar na saída padrão (função 2)
void recuperar_todos_os_registros(char *nomeBin) {
    FILE *arquivo_binario = fopen(nomeBin, "rb");
    if (arquivo_binario == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Cabecalho cabecalho;
    fread(&cabecalho, sizeof(Cabecalho), 1, arquivo_binario);

    // Verificar se o arquivo está consistente
    if (cabecalho.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivo_binario);
        return;
    }

    Registro registro;
    int registros_encontrados = 0;

    // Ler registros do arquivo binário
    while (fread(&registro, sizeof(Registro), 1, arquivo_binario) == 1) {
        if (registro.removido == '1') {
            continue; // Pular registros removidos
        }

        // Exibir os dados do registro
        printf("Nome: %s\n", registro.nome);
        printf("Dieta: %s\n", registro.dieta);
        printf("Habitat: %s\n", registro.habitat);
        if (registro.populacao != -1) {
            printf("População: %d\n", registro.populacao);
        }
        printf("Tipo: %s\n", registro.tipo);
        if (registro.velocidade != -1) {
            printf("Velocidade: %d %c/h\n", registro.velocidade, registro.uniMedida[0]);
        }
        if (registro.tamanho != -1) {
            printf("Tamanho: %.2f\n", registro.tamanho);
        }
        printf("Espécie: %s\n", registro.nEspecie);
        printf("Alimento: %s\n", registro.alimento);
        printf("\n");

        registros_encontrados++;
    }

    // Verificar se nenhum registro foi encontrado
    if (registros_encontrados == 0) {
        printf("Registro inexistente.\n");
    }

    fclose(arquivo_binario);
}
