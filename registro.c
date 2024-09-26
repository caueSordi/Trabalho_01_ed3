#include "registro.h"
#include "cabecalho.h"


Registro *registro_readbin(FILE *file) {
    // Aloca espaço para um novo registro
    Registro *registro = malloc(sizeof(Registro));
    if (registro == NULL) {
        printf("Erro ao alocar memória para o registro.\n");
        return NULL;
    }

    Cabecalho cabecalho;

    // Lê o cabeçalho do arquivo
    if (fread(&cabecalho, sizeof(Cabecalho), 1, file) != 1) {
        printf("Erro ao ler o cabeçalho do arquivo.\n");
        free(registro);
        return NULL;
    }

    // Verifica se o arquivo está inconsistente
    if (cabecalho.status == '0') {
        printf("Falha no processamento do arquivo: arquivo inconsistente.\n");
        free(registro);
        return NULL;
    }

    int registros_encontrados = 0;
    cabecalho_readbin(file);

    // Lê os registros enquanto não chegar ao final do arquivo
    while (!feof(file)) {
        // Lê o campo que indica se o registro foi removido
        if (fread(&registro->removido, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o campo removido.\n");
            break;
        }

        if (registro->removido == '1') {
            fseek(file, 160, SEEK_CUR); // Pula o registro lógico removido
            continue;
        }

        // Lê o restante dos campos fixos
        if (fread(&registro->encadeamento, sizeof(int), 1, file) != 1 ||
            fread(&registro->populacao, sizeof(int), 1, file) != 1 ||
            fread(&registro->tamanho, sizeof(float), 1, file) != 1 ||
            fread(&registro->uniMedida, sizeof(char), 1, file) != 1 ||
            fread(&registro->velocidade, sizeof(int), 1, file) != 1) {
            printf("Erro ao ler os campos fixos.\n");
            break;
        }

        printf("Início da leitura\n");

        // Campos variáveis - leitura segura
        char aux[10];  // Buffer temporário para delimitadores

        if (fread(&registro->nome, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o nome.\n");
            break;
        }

        if (fread(aux, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o delimitador do nome.\n");
            break;
        }
        printf("Nome: %s\n", registro->nome);

        if (fread(&registro->nEspecie, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler a espécie.\n");
            break;
        }

        if (fread(aux, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o delimitador da espécie.\n");
            break;
        }
        printf("Espécie: %s\n", registro->nEspecie);

        if (fread(&registro->habitat, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o habitat.\n");
            break;
        }

        if (fread(aux, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o delimitador do habitat.\n");
            break;
        }
        printf("Habitat: %s\n", registro->habitat);

        if (fread(&registro->tipo, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o tipo.\n");
            break;
        }

        if (fread(aux, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o delimitador do tipo.\n");
            break;
        }
        printf("Tipo: %s\n", registro->tipo);

        if (fread(&registro->dieta, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler a dieta.\n");
            break;
        }

        if (fread(aux, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o delimitador da dieta.\n");
            break;
        }
        printf("Dieta: %s\n", registro->dieta);

        if (fread(&registro->alimento, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o alimento.\n");
            break;
        }

        if (fread(aux, sizeof(char), 1, file) != 1) {
            printf("Erro ao ler o delimitador do alimento.\n");
            break;
        }
        printf("Alimento: %s\n", registro->alimento);

        registros_encontrados++;
    }

    if (registros_encontrados == 0) {
        printf("Nenhum registro encontrado.\n");
        free(registro);
        return NULL;
    }

    return registro;
}

void registro_writebin(FILE *nomebin, Registro *registro) {
    // Função para escrever o registro no arquivo binário

    // Obter o tamanho das strings de tamanho variável
    int tam = strlen(registro->nome);
    int tamA = strlen(registro->alimento);
    int tamD = strlen(registro->dieta);
    int tamE = strlen(registro->nEspecie);
    int tamT = strlen(registro->tipo);
    int tamH = strlen(registro->habitat);

    // Escreve campos de tamanho fixo
    fwrite(&registro->removido, sizeof(char), 1, nomebin);
    fwrite(&registro->encadeamento, sizeof(int), 1, nomebin);
    fwrite(&registro->populacao, sizeof(int), 1, nomebin);
    fwrite(&registro->tamanho, sizeof(float), 1, nomebin);
    fwrite(&registro->uniMedida, sizeof(char), 1, nomebin);
    fwrite(&registro->velocidade, sizeof(int), 1, nomebin);

    // Escreve campos de tamanho variável com delimitador #
    if (registro->nome != NULL && strcmp(registro->nome, "#") != 0) {
        fwrite(registro->nome, sizeof(char), tam, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->nEspecie != NULL && strcmp(registro->nEspecie, "#") != 0) {
        fwrite(registro->nEspecie, sizeof(char), tamE, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->habitat != NULL && strcmp(registro->habitat, "#") != 0) {
        fwrite(registro->habitat, sizeof(char), tamH, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->tipo != NULL && strcmp(registro->tipo, "#") != 0) {
        fwrite(registro->tipo, sizeof(char), tamT, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->dieta != NULL && strcmp(registro->dieta, "#") != 0) {
        fwrite(registro->dieta, sizeof(char), tamD, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    if (registro->alimento != NULL && strcmp(registro->alimento, "#") != 0) {
        fwrite(registro->alimento, sizeof(char), tamA, nomebin);
    }
    fwrite("#", sizeof(char), 1, nomebin);  // Delimitador

    // Verifica quanto falta para completar os 160 bytes
    int preenche = registro->tam_preenchimento;
    char aux[preenche];
    memset(aux, REGISTRO_FILL, preenche);  // Preenche com o caractere de preenchimento
    fwrite(aux, sizeof(char), preenche, nomebin);
}

Registro *registro_readcsv(FILE *csv) {
    Registro *registro = malloc(sizeof(Registro));
    char linha[300];
    fgets(linha, sizeof(linha), csv);

    // Remove os caracteres de nova linha (tanto \n quanto \r)
    linha[strcspn(linha, "\n")] = '\0';
    linha[strcspn(linha, "\r")] = '\0';

    char *linha_copy = linha;
    //alocando memoria
    registro->nome = calloc(160,sizeof(registro->nome));
    registro->nEspecie = calloc(160,sizeof(registro->nEspecie));
    registro->dieta = calloc(160,sizeof(registro->dieta));
    registro->habitat = calloc(160,sizeof(registro->habitat));
    registro->alimento = calloc(160,sizeof(registro->alimento));
    registro->tipo = calloc(160,sizeof(registro->tipo));

    // Nome
    strcpy(registro->nome,strsep(&linha_copy, ","));

    // Dieta
    strcpy(registro->dieta,strsep(&linha_copy, ","));

    // Habitat
    strcpy(registro->habitat,strsep(&linha_copy, ","));

    // População
    char *populacao = strsep(&linha_copy, ",");
    if (strlen(populacao) == 0) {
        registro->populacao = -1;
    }
    else{
        registro->populacao = atoi(populacao);
    }
    
    // Tipo
    strcpy(registro->tipo,strsep(&linha_copy, ","));
    

    // Velocidade
    char *velocidade = strsep(&linha_copy, ",");
    if (strlen(velocidade) == 0) {
        registro->velocidade = -1;
    }
    else{
        registro->velocidade = atoi(velocidade);
    }
    
    // Unidade de medida
    char *unidade = strsep(&linha_copy, ",");
    if (strlen(unidade) == 0) {
        registro->uniMedida = '$';
    }
    else{
        registro->uniMedida = unidade[0];
    }

    // Tamanho
    char *tamanho = strsep(&linha_copy, ",");
    if (strlen(tamanho) == 0) {
        registro->tamanho = -1;
    }
    else{
        registro->tamanho = atof(tamanho);
    }

    // Espécie
    strcpy(registro->nEspecie,strsep(&linha_copy, ","));
   
    // Alimento
    strcpy(registro->alimento,strsep(&linha_copy, ","));
    
    // Configurações adicionais para o registro
    registro->removido = REGISTRO_REMOVIDO_FALSE;
    registro->encadeamento = 0;

    // Calcular bytes restantes para completar os 160 bytes
    // calcular tam_preenchimento corretamente com base nos tamanhos das strings
    int tam = strlen(registro->nome);
    int tamE = strlen(registro->nEspecie);
    int tamA = strlen(registro->alimento);
    int tamH = strlen(registro->habitat);
    int tamD = strlen(registro->dieta);
    int tamT = strlen(registro->tipo);

    // 18 bytes para os campos fixos (população, tamanho, velocidade, uniMedida, removido e encadeamento) e 6 bytes para delimitadores '#'
    registro->tam_preenchimento = 160 - ((tam + tamD + tamH + tamT + tamE + tamA) * sizeof(char) + 18 +6);

    return registro;
}


void registro_print(Registro *registro){
    printf("Nome: %s\n", registro->nome);
    printf("Dieta: %s\n", registro->dieta);
    printf("Habitat: %s\n", registro->habitat);
    printf("População: %d\n", registro->populacao);
    printf("Tipo: %s\n", registro->tipo);
    printf("Velocidade: %d %c/h\n", registro->velocidade, registro->uniMedida);
    printf("Tamanho: %.2f\n", registro->tamanho);
    printf("Espécie: %s\n", registro->nEspecie);
    printf("Alimento: %s\n", registro->alimento);
    printf("\n");
}

bool registro_field(char *nome_campo){
    // retorna se é um campo de string ou inteiro
}

void registro_remove(Registro *registro){
    // remove logicamente um registro
}

bool registro_isValid(Registro *registro){
    // retorna true caso o registro não seja removido
}




int registro_getPop(Registro *registro){
    // verifica o valor do campo topo
    return registro->populacao;
}
float registro_getTam(Registro *registro){
    // verifica o valor do campo proximo a ser removido
    return registro->tamanho;
}

char registro_getUnimedida(Registro *registro){
    // verifica a quantidade de paginas 
    return registro->uniMedida;
}

int registro_getVelocidade(Registro *registro){
    // verifica quantas vezes foi compactado
    return registro->velocidade;
}

char* registro_getNome(Registro *registro){
    return registro->nome;
}

char* registro_getNEspecie(Registro *registro){
    return registro->nEspecie;
}

char* registro_getHabitat(Registro *registro){
    return registro->habitat;
}

char* registro_getTipo(Registro *registro){
    return registro->tipo;
}

char* registro_getDieta(Registro *registro){
    return registro->dieta;
}
char* registro_getAlimenta(Registro *registro){
    return registro->alimento;
}
char registro_getRemovido(Registro *registro){
    return registro->removido;
}

int registro_getEncadeamento(Registro *registro){
    return registro->encadeamento;
}








void registro_setPop(Registro *registro, int pop){
    // verifica o valor do campo topo
    registro->populacao = pop;
}
void registro_setTam(Registro *registro, float tam){
    // verifica o valor do campo proximo a ser removido
    registro->tamanho = tam;
}

void registro_setUnimedida(Registro *registro, char unidade){
    // verifica a quantidade de paginas 
    registro->uniMedida = unidade;
}

void registro_setVelocidade(Registro *registro, int velocidade){
    // verifica quantas vezes foi compactado
    registro->velocidade = velocidade;
}

void registro_setNome(Registro *registro, char *nome){
    strcpy(registro->nome, nome);
}

void registro_setNEspecie(Registro *registro, char *especie){
    strcpy(registro->nEspecie, especie);
}

void registro_setHabitat(Registro *registro, char *habitat){
    strcpy(registro->habitat, habitat);
}

void registro_setTipo(Registro *registro,char *tipo){
    strcpy(registro->tipo, tipo);
}

void registro_setDieta(Registro *registro, char *dieta){
    strcpy(registro->dieta, dieta);
}
void registro_setAlimenta(Registro *registro, char *alimento){
    strcpy(registro->alimento, alimento);
}

void registro_setRemovido(Registro *registro, bool removido){
    registro->removido=removido;
}

void registro_setEncadeamento(Registro *registro, int encadeamento) {
    registro->encadeamento = encadeamento;
}