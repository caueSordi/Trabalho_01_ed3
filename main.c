#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "registro.h"
#include "cabecalho.h"
#include "funcoesFornecidas.h"
#include "comandos.h"

int main()
{
     int comando;
    char nomeCSV[100], nomearqbin[100];
    printf ("Digite o comando desejado: \n");
    
    scanf("%d%*c",&comando);
    switch(comando){
         case 1: //CREATE TABLE
            scanf("%s %s",nomeCSV,nomearqbin);
            //chamando o comando create table
            printf ("chamando o comando create table\n");
            CREATE_TABLE(nomeCSV, nomearqbin); //função que cria a tabela
            printf ("chamando o comando create table\n");
            binarioNaTela(nomearqbin); //função que imprime o arquivo binário
            
            break;
        case 2:
            SELECT_TABLE(nomearqbin);
            break;
        case 3:
            break;
    }
    //     default:
    //         printf("Comando desconhecido\n");
    //         exit(1);
    // }
        
    
    return 0;
    }