#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "func.h"


#define PAGE_SIZE 1600
#define FILL_CHAR '$'


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


void lendo_csv(char *nomeCSV, FILE *nomeBin, Cabecalho *cabecalho) {
    Registro registro;
    FILE *arquivo_csv = fopen(nomeCSV, "r");
    if (arquivo_csv == NULL) {
        printf("Falha ao abrir o arquivo CSV\n");
        return;
    }
    printf("Teste\n");
    
    while (!feof(arquivo_csv)) {
        fscanf(arquivo_csv, "%[^,],%[^,],%[^,],%d,%[^,],%d,%c,%d,%[^,],%[^,\n]\n", 
                   registro.nome, registro.dieta, registro.habitat, &registro.populacao, 
                   registro.tipo, &registro.velocidade, &registro.uniMedida, 
                   &registro.tamanho, registro.nEspecie, registro.alimento);
            printf("Teste: %s, %s, %s, %d, %s, %d, %c, %d, %s, %s\n", 
                   registro.nome, registro.dieta, registro.habitat, registro.populacao, 
                   registro.tipo, registro.velocidade, registro.uniMedida, 
                   registro.tamanho, registro.nEspecie, registro.alimento);
        }
    

    fclose(arquivo_csv);
}
