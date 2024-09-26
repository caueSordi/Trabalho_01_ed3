#include "cabecalho.h"


Cabecalho *cabecalho_readbin(FILE *file){
    // lê o cabecalho de um arquivo binário
       Cabecalho *cabecalho = malloc(sizeof(Cabecalho));
    if (cabecalho == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o cabecalho\n");
        exit(EXIT_FAILURE);
    }

    fread(&cabecalho->status, sizeof(char), 1, file);
    fread(&cabecalho->topo, sizeof(int), 1, file);
    fread(&cabecalho->proxRRN, sizeof(int), 1, file);
    fread(&cabecalho->nroRegRem, sizeof(int), 1, file);
    fread(&cabecalho->nroPagDisco, sizeof(int), 1, file);
    fread(&cabecalho->qttCompacta, sizeof(int), 1, file);

    // Debug prints
    printf("Lendo do arquivo binário:\n");
    printf("status: %c\n", cabecalho->status);
    printf("topo: %d\n", cabecalho->topo);
    printf("proxRRN: %d\n", cabecalho->proxRRN);
    printf("nroRegRem: %d\n", cabecalho->nroRegRem);
    printf("nroPagDisco: %d\n", cabecalho->nroPagDisco);
    printf("qttCompacta: %d\n", cabecalho->qttCompacta);

    return cabecalho;
}
Cabecalho* cabecalho_inicializa()
{

//estrutura padrao do cabecalho
    Cabecalho *cabecalho  = malloc(sizeof(Cabecalho));
    if (cabecalho == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o cabecalho\n");
        exit(EXIT_FAILURE);
    }

    //estrutura padrão do cabecalho
    Cabecalho *cabecalho  = malloc(sizeof(Cabecalho));
    cabecalho->status = '0';
    cabecalho->topo = -1;
    cabecalho->proxRRN = 0;
    cabecalho->nroRegRem = 0;
    cabecalho->nroPagDisco = 0;
    cabecalho->qttCompacta = 0;

        // Debug prints
    printf("Cabecalho inicializado:\n");
    printf("status: %c\n", cabecalho->status);
    printf("topo: %d\n", cabecalho->topo);
    printf("proxRRN: %d\n", cabecalho->proxRRN);
    printf("nroRegRem: %d\n", cabecalho->nroRegRem);
    printf("nroPagDisco: %d\n", cabecalho->nroPagDisco);
    printf("qttCompacta: %d\n", cabecalho->qttCompacta);


    return cabecalho;
}

bool cabecalho_getStatus(Cabecalho *cabecalho) {
    // Verifica o valor do campo status
    if (cabecalho->status == CABECALHO_STATUS_OK) {
        return true; // Retorna verdadeiro se o status for '1'
    } else {
        return false; // Retorna falso caso contrário
    }
}

void cabecalho_writebin(FILE *file, Cabecalho *cabecalho) {
    // Escreve o cabeçalho no arquivo binário
    if (cabecalho_getStatus(cabecalho)) {
        fwrite(&cabecalho->status, sizeof(char), 1, file); // Escreve o status se for OK
    } else {
        char status = '0'; // Escreve '0' se o status não for OK
        fwrite(&status, sizeof(char), 1, file);
    }

    fwrite(&cabecalho->topo, sizeof(int), 1, file);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, file);
    fwrite(&cabecalho->nroRegRem, sizeof(int), 1, file);
    fwrite(&cabecalho->nroPagDisco, sizeof(int), 1, file);

 // Debug prints
    printf("Escrevendo no arquivo binário:\n");
    printf("status: %c\n", cabecalho->status);
    printf("topo: %d\n", cabecalho->topo);
    printf("proxRRN: %d\n", cabecalho->proxRRN);
    printf("nroRegRem: %d\n", cabecalho->nroRegRem);
    printf("nroPagDisco: %d\n", cabecalho->nroPagDisco);
    printf("qttCompacta: %d\n", cabecalho->qttCompacta);



    // Preencher o restante da página de disco com o caractere '$'
    int resto = 1600 - (4 * sizeof(int)) - sizeof(char); // Calcula o espaço restante
    char aux[resto];
    for (int i = 0; i < resto; i++) {
        aux[i] = '$'; // Preenche o array com '$'
    }
    fwrite(aux, sizeof(char), resto, file); // Escreve o preenchimento no arquivo
}

void cabecalho_readcsv(FILE *csv) {
    char buffer[300];
    fgets(buffer, sizeof(buffer), csv); // Pula a linha de cabeçalho
}


int cabecalho_getTopo(Cabecalho *cabecalho){
    // verifica o valor do campo topo
    return cabecalho->topo;
}
int cabecalho_getProxRegRem(Cabecalho *cabecalho){
    // verifica o valor do campo proximo a ser removido
    return cabecalho->proxRRN;
}

int cabecalho_getNroPagDisco(Cabecalho *cabecalho){
    // verifica a quantidade de paginas 
    return cabecalho->nroRegRem;
}

int cabecalho_getQttCompacta(Cabecalho *cabecalho){
    // verifica quantas vezes foi compactado
    return cabecalho->qttCompacta;
}

void cabecalho_setStatus(Cabecalho *cabecalho, bool status){
    cabecalho->status = status;
}

void cabecalho_setTopo(Cabecalho *cabecalho, int topo){
    cabecalho->topo = topo;
}

void cabecalho_setProxRRN(Cabecalho *cabecalho, int proxRRN){
    cabecalho->proxRRN = proxRRN;
}

void cabecalho_setNroPagDisco(Cabecalho *cabecalho, int nropag){
    cabecalho->nroPagDisco = nropag;
}

void cabecalho_setQttCompacta(Cabecalho *cabecalho, int quant){
    cabecalho->qttCompacta = quant;
}