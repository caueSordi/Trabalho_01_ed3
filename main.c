#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main() {
    char nome_arquivo_binario[] = "cadeiaAlimentar.bin";

    // Abre o arquivo binário para leitura e escrita 
    FILE *arquivo_binario = fopen(nome_arquivo_binario, "w+b");
    if (arquivo_binario == NULL) {
        printf("Falha ao abrir o arquivo binário.\n");
        return 1;
    }

    // Inicializa o cabeçalho e escreve no arquivo binário
    Cabecalho cabecalho;
    inicializa_cabecalho(&cabecalho);
    fwrite(&cabecalho, sizeof(Cabecalho), 1, arquivo_binario);

    // Exemplo de leitura de arquivo CSV e inserção dos registros no arquivo binário
    char nome_arquivo_csv[] = "dados.csv"; // Nome do arquivo CSV de exemplo
    le_csv(nome_arquivo_csv, arquivo_binario, &cabecalho);

    // Exemplo de adição manual de um registro
    Registro novo_registro;
    novo_registro.id = 1; // Exemplo de ID
    snprintf(novo_registro.nome, sizeof(novo_registro.nome), "Exemplo de Nome");
    adiciona_registro(arquivo_binario, &novo_registro, &cabecalho);

    // Exemplo de remoção lógica de um registro pelo RRN
    int rrn_para_remover = 0; // RRN do registro que você quer remover
    remove_registro(arquivo_binario, rrn_para_remover, &cabecalho);

    // Compactação do arquivo para remover permanentemente os registros lógicos
    compacta_arquivo(arquivo_binario, &cabecalho);

    // Fecha o arquivo binário
    fclose(arquivo_binario);

    printf("Operações concluídas com sucesso!\n");
    return 0;
}
