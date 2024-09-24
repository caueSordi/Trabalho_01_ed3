#include "func.h"
#include "ctrl_arq.h"  // Inclusão do novo header

// Inicializa o arquivo binário com cabeçalho e grava os registros do CSV
void lendo_csv(char *nomeCSV, FILE *bin, Cabecalho *cabecalho) {
    Registro registro;
    int tamT, tamD, tamA, tamE, tamH, tam;
    char linha[160];  // Tamanho máximo que o registro pode ocupar

    FILE *arquivo_csv = fopen(nomeCSV, "r");
    if (arquivo_csv == NULL) {
        printf("Falha ao abrir o arquivo CSV\n");
        return;
    }

    // Inicializa e escreve o cabeçalho no arquivo binário
    inicializa_cabecalho(cabecalho, bin);

    // Lê o nome das colunas (descarta a primeira linha)
    char buffer[100];
    fgets(buffer, sizeof(buffer), arquivo_csv);

    // Lê cada linha do CSV e grava no binário
    while (fgets(linha, sizeof(linha), arquivo_csv)) {
        linha[strcspn(linha, "\n")] = '\0';  // Remove o caractere de nova linha

        char *campo = strtok(linha, ",");
        
        // Processar os campos do registro (ajustar conforme necessário)
        // Exemplo para o campo nome
        if (campo != NULL) {
            tam = strlen(campo);
            registro.nome = (char*) malloc((tam + 1) * sizeof(char));
            strcpy(registro.nome, campo);
            registro.nome[tam] = '#';  // Finaliza com '#'
        }

        // Processar outros campos como dieta, habitat, etc.
        campo = strtok(NULL, ",");
        if (campo != NULL) {
            tamD = strlen(campo);
            registro.dieta = (char*) malloc((tamD + 1) * sizeof(char));
            strcpy(registro.dieta, campo);
            registro.dieta[tamD] = '#';  // Finaliza com '#'
        }

        // Processar os campos restantes e gravar no binário
        // Função arquivobin grava no arquivo binário
        arquivobin(bin, registro, cabecalho);

        // Libera a memória alocada
        free(registro.nome);
        free(registro.dieta);
        free(registro.habitat);
        free(registro.tipo);
        free(registro.nEspecie);
        free(registro.alimento);
    }

    fclose(arquivo_csv);
}

// Função para gravar um registro no arquivo binário
void arquivobin(FILE *bin, Registro registro, Cabecalho *cabecalho) {
    // Escreve os campos de tamanho fixo
    fwrite(&registro.removido, sizeof(char), 1, bin);
    fwrite(&registro.encadeamento, sizeof(int), 1, bin);
    fwrite(&registro.populacao, sizeof(int), 1, bin);
    fwrite(&registro.tamanho, sizeof(float), 1, bin);
    fwrite(&registro.uniMedida, sizeof(char), 1, bin);
    fwrite(&registro.velocidade, sizeof(int), 1, bin);

    // Escreve o tamanho da string e a string
    int tamanho_nome = strlen(registro.nome);
    fwrite(&tamanho_nome, sizeof(int), 1, bin);
    fwrite(registro.nome, sizeof(char), tamanho_nome, bin);

    int tamanho_dieta = strlen(registro.dieta);
    fwrite(&tamanho_dieta, sizeof(int), 1, bin);
    fwrite(registro.dieta, sizeof(char), tamanho_dieta, bin);

    // Escrever outros campos da mesma maneira...

    // Calcula o tamanho total já escrito e preenche o restante com '$'
    int tamanhoEscrito = sizeof(char) + sizeof(int) * 4 + sizeof(float) + 
                         sizeof(char) * tamanho_nome + sizeof(char) * tamanho_dieta;
    int espacoRestante = 160 - tamanhoEscrito;
    char padding[espacoRestante];
    memset(padding, FILL_CHAR, espacoRestante);
    fwrite(padding, sizeof(padding), 1, bin);
}

// Função para recuperar todos os registros do arquivo binário
void recuperar_todos_os_registros(char *nomeBin) {
    FILE *arquivo_binario;
    Cabecalho cabecalho;

    // Abre o arquivo binário e lê o cabeçalho
    abre_arq(nomeBin, &arquivo_binario, &cabecalho);

    // Verifica se o arquivo está consistente
    if (cabecalho.status == '0') {
        printf("Falha no processamento do arquivo: arquivo inconsistente.\n");
        fclose(arquivo_binario);
        return;
    }

    Registro registro;
    int registros_encontrados = 0;

    // Lê cada registro do arquivo binário
    while (fread(&registro.removido, sizeof(char), 1, arquivo_binario) == 1) {
        if (registro.removido == '1') {
            fseek(arquivo_binario, sizeof(Registro) - sizeof(char), SEEK_CUR);  // Pula registro removido
            continue;
        }

        // Ler campos fixos e variáveis
        fread(&registro.encadeamento, sizeof(int), 1, arquivo_binario);
        fread(&registro.populacao, sizeof(int), 1, arquivo_binario);
        fread(&registro.tamanho, sizeof(float), 1, arquivo_binario);
        fread(&registro.uniMedida, sizeof(char), 1, arquivo_binario);
        fread(&registro.velocidade, sizeof(int), 1, arquivo_binario);

        // Ler tamanho e string nome
        int tamanho_nome;
        fread(&tamanho_nome, sizeof(int), 1, arquivo_binario);
        registro.nome = (char*) malloc(tamanho_nome + 1);
        fread(registro.nome, sizeof(char), tamanho_nome, arquivo_binario);
        registro.nome[tamanho_nome] = '\0';  // Termina a string

        // Ler outros campos variáveis (dieta, habitat, etc.) da mesma maneira...

        // Exibe os dados do registro
        registros_encontrados++;
        printf("Nome: %s\n", registro.nome);
        // Exibe outros campos...

        // Libera a memória alocada para as strings
        free(registro.nome);
        // Libere outros campos conforme necessário...
    }

    if (registros_encontrados == 0) {
        printf("Registro inexistente.\n");
    }

    // Fecha o arquivo e atualiza o cabeçalho
    fecha_arq(arquivo_binario, &cabecalho);
}
