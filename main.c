#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include "funcoes_fornecidas.h"
#include <string.h>

int main() {
    int comando;
    char nomeCSV[100], nomearqbin[100];
    Cabecalho inicio;

    // Leitura dos parâmetros de arquivos
    scanf("%d", &comando);
    scanf("%s", nomeCSV);
    scanf("%s", nomearqbin);

    // Abre o arquivo binário para escrita ou leitura
    FILE *arquivo_binario;
    if (comando == 1) {
        // Abertura em modo "wb" para escrita binária
        arquivo_binario = fopen(nomearqbin, "wb");
        if (arquivo_binario == NULL) {
            printf("Falha ao abrir o arquivo binário.\n");
            return 1;
        }

        // Inicializa o cabeçalho e escreve no arquivo
        inicializa_cabecalho(&inicio);
        fwrite(&inicio, sizeof(Cabecalho), 1, arquivo_binario);

        // Lê o CSV e escreve os registros no arquivo binário
        lendo_csv(nomeCSV, arquivo_binario, &inicio);

        fclose(arquivo_binario);
    } else if (comando == 2) {
        // Chama a função para recuperar e mostrar os registros
        recuperar_todos_os_registros(nomearqbin);
    } else {
        printf("Comando inválido.\n");
    }

    return 0;
}
