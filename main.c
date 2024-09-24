#include <stdio.h>
#include <stdlib.h>
#include "func.h"
#include <string.h>

int main() {
    
    int comando, rrn, Idbusca;
    char nomeCSV[100], nomearqbin[100];
    Registro *registro;
    
    // Allocate memory for Cabecalho
    Cabecalho *inicio = (Cabecalho *)malloc(sizeof(Cabecalho));
    if (inicio == NULL) {
        printf("Erro ao alocar memória para o cabeçalho.\n");
        return 1;
    }

    // Leitura dos parâmetros de arquivos
    printf("Digite o comando desejado:\n");
    scanf("%d", &comando);
    
    // Inicializa o cabeçalho
    inicializa_cabecalho(inicio);

    switch (comando)
    {
    case 1:
        scanf("%s", nomeCSV);
        scanf("%s", nomearqbin);

        // Abre o arquivo binário para leitura e escrita 
        FILE *arquivo_binario = fopen(nomearqbin, "wb");
        if (arquivo_binario == NULL) {
            printf("Falha ao abrir o arquivo binário.\n");
            return 1;
        }
        
         // Escreve o cabeçalho no arquivo binário
        //fwrite(inicio, sizeof(Cabecalho), 1, arquivo_binario);

        //leitura do arquivo csv
        lendo_csv(nomeCSV, arquivo_binario, inicio);
        binarioNaTela(nomearqbin);
        fclose(arquivo_binario);
        break;
    case 2:
        //leitura do arquvio binario
        scanf("%s", nomearqbin);
        // Chama a função para recuperar e mostrar os registros
        recuperar_todos_os_registros(nomearqbin);
        break;
    default:
        break;

    case 3:
        //leitura do arquvio binario
        scanf("%s", nomearqbin);
        // Chama a função para recuperar e mostrar os registros
        printf("Nome do arquivo binário: %s\n", nomearqbin);
        buscar_registros_por_campo(nomearqbin);
        break;
    }
    


    
    return 0;
}