#include "ctrl_arq.h"

// Inicializa e escreve o cabeçalho no arquivo binário
void inicializa_cabecalho(Cabecalho *c, FILE* bin) {
    fwrite(&c->status, sizeof(char), 1, bin);
    fwrite(&c->topo, sizeof(int), 1, bin);
    fwrite(&c->proxRRN, sizeof(int), 1, bin);
    fwrite(&c->nroRegRem, sizeof(int), 1, bin);
    fwrite(&c->nroPagDisco, sizeof(int), 1, bin);
    fwrite(&c->qttCompacta, sizeof(int), 1, bin);

    // Preencher o restante da página com '$'
    char padding[PAGE_SIZE - (sizeof(char) + 5 * sizeof(int))];
    memset(padding, FILL_CHAR, sizeof(padding));
    fwrite(padding, sizeof(padding), 1, bin);
}

// Lê o cabeçalho do arquivo binário
void ler_cabecalho(FILE *bin, Cabecalho *c) {
    fread(&c->status, sizeof(char), 1, bin);
    fread(&c->topo, sizeof(int), 1, bin);
    fread(&c->proxRRN, sizeof(int), 1, bin);
    fread(&c->nroRegRem, sizeof(int), 1, bin);
    fread(&c->nroPagDisco, sizeof(int), 1, bin);
    fread(&c->qttCompacta, sizeof(int), 1, bin);

    // Ignorar o padding (lixo)
    fseek(bin, PAGE_SIZE - (sizeof(char) + 5 * sizeof(int)), SEEK_CUR);
}

// Abre o arquivo binário e escreve o cabeçalho inicial
void abre_arq(char *nomeArquivo, FILE **arq, Cabecalho *cab) {
    *arq = fopen(nomeArquivo, "wb+");
    if (*arq == NULL) {
        printf("Falha ao abrir o arquivo binário.\n");
        exit(1);
    }

    // Inicializa e escreve o cabeçalho
    inicializa_cabecalho(cab, *arq);
}

// Fecha o arquivo binário e atualiza o cabeçalho com o status '1'
void fecha_arq(FILE *arq, Cabecalho *cab) {
    cab->status = '1';
    fseek(arq, 0, SEEK_SET);  // Voltar ao início para sobrescrever o cabeçalho
    inicializa_cabecalho(cab, arq);
    fclose(arq);
}
