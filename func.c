#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "func.h"
#include <ctype.h>

#define PAGE_SIZE 1600 //tamanho de uma pagina do disco
#define FILL_CHAR '$'

// ------------ Funções Fornecidas --------------------------------------


void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------

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
void lendo_csv(char *nomeCSV, FILE *bin, Cabecalho *cabecalho) {
    FILE *csvFile = fopen(nomeCSV, "r");
    if (!csvFile) {
        perror("Erro ao abrir o arquivo CSV");
        return;
    }

    // Inicializa o cabeçalho
    inicializa_cabecalho(cabecalho);

    Registro registro;
    
    // Lê o arquivo CSV linha por linha
    while (fscanf(csvFile, "%d,%f,%c,%d,%m[^\n]", 
                  &registro.populacao, 
                  &registro.tamanho, 
                  &registro.uniMedida, 
                  &registro.velocidade, 
                  &registro.nome) == 5) {
        
        // Aqui você pode ler e alocar memória para os outros campos
        // Exemplo de leitura e alocação para nEspecie
        char *nEspecie = NULL;
        char *habitat = NULL;
        char *tipo = NULL;
        char *dieta = NULL;
        char *alimento = NULL;

        fscanf(csvFile, ",%m[^\n]", &nEspecie);
        fscanf(csvFile, ",%m[^\n]", &habitat);
        fscanf(csvFile, ",%m[^\n]", &tipo);
        fscanf(csvFile, ",%m[^\n]", &dieta);
        fscanf(csvFile, ",%m[^\n]", &alimento);

        // Atribuindo os valores lidos aos campos do registro
        registro.nEspecie = nEspecie;
        registro.habitat = habitat;
        registro.tipo = tipo;
        registro.dieta = dieta;
        registro.alimento = alimento;

        // Chama a função para gravar o registro no arquivo binário
        arquivobin(bin, registro, 0, cabecalho); // Ajuste o valor de 'aux' conforme necessário

        // Libera a memória alocada para strings
        free(registro.nome);
        free(nEspecie);
        free(habitat);
        free(tipo);
        free(dieta);
        free(alimento);
    }

    fclose(csvFile);
}

/*
void lendo_csv(char *nomeCSV, FILE *nomeBin, Cabecalho *cabecalho) {
    Registro registro;
    
    int tamT, tamD, tamA, tamE, tamH, tam;
    char linha[160]; // Tamanho máximo que o registro pode ocupar

    char nome[10], dieta[10], habitat[20], tipo[20], uniMedida[1], nespecie[30], alimento[30], nEspecie[50];
    int velocidade, populacao, tamanho;

    FILE *arquivo_csv = fopen(nomeCSV, "r");
    if (arquivo_csv == NULL) {
        printf("Falha ao abrir o arquivo CSV\n");
        return;
    }
    cabecalho->status = '1';
    fwrite(cabecalho, sizeof(Cabecalho), 1, nomeBin);

    //lendo o nome das colunas
    char buffer[100];
    
    fgets(buffer, sizeof(buffer), arquivo_csv);
    //printf("%s\n", buffer);
    char *campo;
    while (fgets(linha, sizeof(linha), arquivo_csv)) {
        //printf("%s\n", linha);
        linha[strcspn(linha, "\n")] = '\0'; // Remove o caractere de nova linha
        linha[strcspn(linha, "\r")] = '\0';
        campo = strtok(linha, ",");
        // Nome
        if (campo != NULL) {
            tam = strlen(campo);
            registro.nome = (char*) malloc((tam + 1) * sizeof(char));
            strcpy(registro.nome, campo);
            registro.nome[tam] = '#'; // Finaliza com '#'
        }
        // Dieta
        campo = strtok(NULL, ",");
        
        if (campo != NULL) {
             tamD = strlen(campo);
            registro.dieta = (char*) malloc((tamD + 1) * sizeof(char));
            strcpy(registro.dieta, campo);
            registro.dieta[tamD] = '#'; // Finaliza com '#'
        }

        // Habitat
        campo = strtok(NULL, ",");
        if (campo != NULL) {
             tamH = strlen(campo);
            registro.habitat = (char*) malloc((tamH + 1) * sizeof(char));
            strcpy(registro.habitat, campo);
            registro.habitat[tamH] = '#'; // Finaliza com '#'
        }

        // População
        campo = strtok(NULL, ",");
        registro.populacao = (campo != NULL) ? atoi(campo) : -1;

        // Tipo
        campo = strtok(NULL, ",");
        if (campo != NULL) {
             tamT = strlen(campo);
            registro.tipo = (char*) malloc((tamT + 1) * sizeof(char));
            strcpy(registro.tipo, campo);
            registro.tipo[tamT] = '#'; // Finaliza com '#'
        }

        // Velocidade
        campo = strtok(NULL, ",");
        registro.velocidade = (campo != NULL) ? atoi(campo) : -1;

        // Unidade de medida
        campo = strtok(NULL, ",");
        registro.uniMedida = (campo != NULL) ? campo[0] : '$';

        // Tamanho
        campo = strtok(NULL, ",");
        registro.tamanho = (campo != NULL) ? atof(campo) : -1;

        // Espécie
        campo = strtok(NULL, ",");
        if (campo != NULL) {
            tamE = strlen(campo);
            registro.nEspecie = (char*) malloc((tamE + 1) * sizeof(char));
            strcpy(registro.nEspecie, campo);
            registro.nEspecie[tamE] = '#'; // Finaliza com '#'
        }

        // Alimento
        campo = strtok(NULL, ",");
        if (campo != NULL) {
             tamA = strlen(campo);
            registro.alimento = (char*) malloc((tamA + 1) * sizeof(char));
            strcpy(registro.alimento, campo);
            registro.alimento[tamA] = '#'; // Finaliza com '#'
        }
        int soma = 160 - (tamA + tamT + tamD + tamE + tamH + tam + 18);
        
        // Armazena o registro no arquivo binário
        registro.removido = '0'; // assume nao removido inicialmente
        registro.encadeamento = 0; //registro válido, ainda não removido
        arquivobin(nomeBin, registro, soma, cabecalho);

        // Libera a memória alocada
        free(registro.nome);
        free(registro.dieta);
        free(registro.habitat);
        free(registro.tipo);
        free(registro.nEspecie);
        free(registro.alimento);
    }
    
    //printf("%c\n", cabecalho->status);
    fclose(arquivo_csv);

}*/
void arquivobin(FILE *nomebin, Registro registro, int aux, Cabecalho *cabecalho) {
    // Escreve campos de tamanho fixo
    fwrite(&registro.removido, sizeof(char), 1, nomebin);
    fwrite(&registro.encadeamento, sizeof(int), 1, nomebin);
    fwrite(&registro.populacao, sizeof(int), 1, nomebin);
    fwrite(&registro.tamanho, sizeof(float), 1, nomebin);
    fwrite(&registro.uniMedida, sizeof(char), 1, nomebin);
    fwrite(&registro.velocidade, sizeof(int), 1, nomebin);

    // Gravar o tamanho das strings e as próprias strings
    int tamanho_nome = strlen(registro.nome);
    fwrite(&tamanho_nome, sizeof(int), 1, nomebin);
    fwrite(registro.nome, sizeof(char), tamanho_nome, nomebin);

    int tamanho_dieta = strlen(registro.dieta);
    fwrite(&tamanho_dieta, sizeof(int), 1, nomebin);
    fwrite(registro.dieta, sizeof(char), tamanho_dieta, nomebin);

    int tamanho_habitat = strlen(registro.habitat);
    fwrite(&tamanho_habitat, sizeof(int), 1, nomebin);
    fwrite(registro.habitat, sizeof(char), tamanho_habitat, nomebin);

    int tamanho_tipo = strlen(registro.tipo);
    fwrite(&tamanho_tipo, sizeof(int), 1, nomebin);
    fwrite(registro.tipo, sizeof(char), tamanho_tipo, nomebin);

    int tamanho_nEspecie = strlen(registro.nEspecie);
    fwrite(&tamanho_nEspecie, sizeof(int), 1, nomebin);
    fwrite(registro.nEspecie, sizeof(char), tamanho_nEspecie, nomebin);

    int tamanho_alimento = strlen(registro.alimento);
    fwrite(&tamanho_alimento, sizeof(int), 1, nomebin);
    fwrite(registro.alimento, sizeof(char), tamanho_alimento, nomebin);

    // Preenchendo o espaço restante com '$' até completar 160 bytes
    int tamanhoEscrito = sizeof(char)    // removido
                       + sizeof(int)     // encadeamento
                       + sizeof(int)     // populacao
                       + sizeof(float)   // tamanho
                       + sizeof(char)    // uniMedida
                       + sizeof(int)     // velocidade
                       + sizeof(int) + tamanho_nome
                       + sizeof(int) + tamanho_dieta
                       + sizeof(int) + tamanho_habitat
                       + sizeof(int) + tamanho_tipo
                       + sizeof(int) + tamanho_nEspecie
                       + sizeof(int) + tamanho_alimento;

    int espacoRestante = 160 - tamanhoEscrito;
    if (espacoRestante > 0) {
        char preenchimento[espacoRestante];
        memset(preenchimento, '$', espacoRestante);
        fwrite(preenchimento, sizeof(char), espacoRestante, nomebin);
    }
}



// Função para recuperar todos os registros e mostrar na saída padrão (função 2)
//registro
// agora quando status 1 o arquivo  NAO ESTA SENDO MAIS LIDO CORRETAMENTE
// pois o proprio binario nao esta sendo salvo corretamente
// estando fragmentados, parece que o codigo nao esta separando corretamente oq deveria ser cada variavel 
// verificar se o registro foi logicamente removido
void recuperar_todos_os_registros(char *nomeBin) {
    FILE *arquivo_binario = fopen(nomeBin, "rb");
    if (arquivo_binario == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Cabecalho cabecalho;
    if (fread(&cabecalho, sizeof(Cabecalho), 1, arquivo_binario) != 1) {
        printf("Erro ao ler o cabeçalho do arquivo.\n");
        fclose(arquivo_binario);
        return;
    }

    printf("Status: %c\n", cabecalho.status);

    if (cabecalho.status == '0') {
        printf("Falha no processamento do arquivo: arquivo inconsistente.\n");
        fclose(arquivo_binario);
        return;
    }

    Registro registro;
    while (fread(&registro.removido, sizeof(char), 1, arquivo_binario) == 1) {
        // Verificar se o registro foi logicamente removido
        if (registro.removido == '1') {
            // Pular para o próximo registro
            fseek(arquivo_binario, 160 - sizeof(char), SEEK_CUR);
            continue; // Ignorar registros removidos
        }

        // Ler os campos restantes do registro
        fread(&registro.encadeamento, sizeof(int), 1, arquivo_binario);
        fread(&registro.populacao, sizeof(int), 1, arquivo_binario);
        fread(&registro.tamanho, sizeof(float), 1, arquivo_binario);
        fread(&registro.uniMedida, sizeof(char), 1, arquivo_binario);
        fread(&registro.velocidade, sizeof(int), 1, arquivo_binario);

        // Ler os tamanhos das strings
        int tamanho_nome, tamanho_dieta, tamanho_habitat, tamanho_tipo, tamanho_nEspecie, tamanho_alimento;

        fread(&tamanho_nome, sizeof(int), 1, arquivo_binario);
        registro.nome = malloc((tamanho_nome + 1) * sizeof(char));
        fread(registro.nome, sizeof(char), tamanho_nome, arquivo_binario);
        registro.nome[tamanho_nome] = '\0'; // Garantir que a string seja terminada corretamente

        fread(&tamanho_dieta, sizeof(int), 1, arquivo_binario);
        registro.dieta = malloc((tamanho_dieta + 1) * sizeof(char));
        fread(registro.dieta, sizeof(char), tamanho_dieta, arquivo_binario);
        registro.dieta[tamanho_dieta] = '\0';

        fread(&tamanho_habitat, sizeof(int), 1, arquivo_binario);
        registro.habitat = malloc((tamanho_habitat + 1) * sizeof(char));
        fread(registro.habitat, sizeof(char), tamanho_habitat, arquivo_binario);
        registro.habitat[tamanho_habitat] = '\0';

        fread(&tamanho_tipo, sizeof(int), 1, arquivo_binario);
        registro.tipo = malloc((tamanho_tipo + 1) * sizeof(char));
        fread(registro.tipo, sizeof(char), tamanho_tipo, arquivo_binario);
        registro.tipo[tamanho_tipo] = '\0';

        fread(&tamanho_nEspecie, sizeof(int), 1, arquivo_binario);
        registro.nEspecie = malloc((tamanho_nEspecie + 1) * sizeof(char));
        fread(registro.nEspecie, sizeof(char), tamanho_nEspecie, arquivo_binario);
        registro.nEspecie[tamanho_nEspecie] = '\0';

        fread(&tamanho_alimento, sizeof(int), 1, arquivo_binario);
        registro.alimento = malloc((tamanho_alimento + 1) * sizeof(char));
        fread(registro.alimento, sizeof(char), tamanho_alimento, arquivo_binario);
        registro.alimento[tamanho_alimento] = '\0';

        // Exibir o registro recuperado
        printf("Registro: %s, %s, %s, %d, %c, %d, %.2f, %s, %s\n", 
               registro.nome, registro.dieta, registro.habitat, 
               registro.populacao, registro.uniMedida, 
               registro.velocidade, registro.tamanho, 
               registro.nEspecie, registro.alimento);

        // Liberar memória alocada
        free(registro.nome);
        free(registro.dieta);
        free(registro.habitat);
        free(registro.tipo);
        free(registro.nEspecie);
        free(registro.alimento);
    }

    fclose(arquivo_binario);
}


// Função para recuperar todos os registros e mostrar na saída padrão (função 2)
void buscar_registros_por_campo(char *nomeBin, int n) {
    FILE *arquivo_binario = fopen(nomeBin, "rb");
    if (arquivo_binario == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    Cabecalho cabecalho;
    if (fread(&cabecalho, sizeof(Cabecalho), 1, arquivo_binario) != 1) {
        printf("Erro ao ler o cabeçalho do arquivo.\n");
        fclose(arquivo_binario);
        return;
    }

    // Verificar se o arquivo está consistente
    if (cabecalho.status == '0') {
        printf("Falha no processamento do arquivo: arquivo inconsistente.\n");
        fclose(arquivo_binario);
        return;
    }

    for (int i = 0; i < n; i++) {
        char nomeCampo[30];
        char valorCampo[100];
        int registros_encontrados = 0;

        // Lê o nome do campo e o valor do campo
        scanf("%s", nomeCampo);
        scan_quote_string(valorCampo); // Usa a função fornecida para tratar strings com aspas

        Registro registro;
        rewind(arquivo_binario); // Volta para o início do arquivo binário após o cabeçalho
        fseek(arquivo_binario, sizeof(Cabecalho), SEEK_SET); // Pula o cabeçalho do arquivo binário

        // Ler registros do arquivo binário
        while (fread(&registro.removido, sizeof(char), 1, arquivo_binario) == 1) {
            if (registro.removido == '1') {
                // Pular registros removidos
                fseek(arquivo_binario, sizeof(int) + sizeof(int) + sizeof(float) + sizeof(char) + sizeof(int) + 160, SEEK_CUR);
                continue;
            }

            fread(&registro.encadeamento, sizeof(int), 1, arquivo_binario);
            fread(&registro.populacao, sizeof(int), 1, arquivo_binario);
            fread(&registro.tamanho, sizeof(float), 1, arquivo_binario);
            fread(&registro.uniMedida, sizeof(char), 1, arquivo_binario);
            fread(&registro.velocidade, sizeof(int), 1, arquivo_binario);

            // Ler strings de tamanho variável (nome, dieta, habitat, tipo, etc.)
            char buffer[100];
            int tamanho_string;

            // Ler e comparar o campo especificado
            if (strcmp(nomeCampo, "nome") == 0) {
                fread(&tamanho_string, sizeof(int), 1, arquivo_binario);
                fread(buffer, sizeof(char), tamanho_string, arquivo_binario);
                buffer[tamanho_string] = '\0';
                if (strcmp(buffer, valorCampo) == 0) {
                    printf("Nome: %s\n", buffer);
                    registros_encontrados++;
                }
            } else if (strcmp(nomeCampo, "dieta") == 0) {
                fread(&tamanho_string, sizeof(int), 1, arquivo_binario);
                fread(buffer, sizeof(char), tamanho_string, arquivo_binario);
                buffer[tamanho_string] = '\0';
                if (strcmp(buffer, valorCampo) == 0) {
                    printf("Dieta: %s\n", buffer);
                    registros_encontrados++;
                }
            } else if (strcmp(nomeCampo, "habitat") == 0) {
                fread(&tamanho_string, sizeof(int), 1, arquivo_binario);
                fread(buffer, sizeof(char), tamanho_string, arquivo_binario);
                buffer[tamanho_string] = '\0';
                if (strcmp(buffer, valorCampo) == 0) {
                    printf("Habitat: %s\n", buffer);
                    registros_encontrados++;
                }
            } else if (strcmp(nomeCampo, "populacao") == 0) {
                fread(&tamanho_string, sizeof(int), 1, arquivo_binario);
                fread(buffer, sizeof(char), tamanho_string, arquivo_binario);
                buffer[tamanho_string] = '\0';
                if (atoi(buffer) == atoi(valorCampo)) {
                    printf("População: %s\n", buffer);
                    registros_encontrados++;
                }
            } // continue comparando para outros campos, como tipo, velocidade, etc.

            // Pular para o próximo registro no arquivo binário
            fseek(arquivo_binario, sizeof(int) + sizeof(float) + sizeof(int) + 160, SEEK_CUR);
        }

        if (registros_encontrados == 0) {
            printf("Registro inexistente.\n");
        }
    }

    fclose(arquivo_binario);
}