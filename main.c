#include <stdio.h>
#include <dirent.h>
#include "indices.h"
#include "operacoesUsuario.h"
#include "style.h"

int main() {
    FILE *movies;           //arquivo de dados
    FILE *iprimary;         //arquivo de índice primário
    FILE *ititle;           //arquivo de índice secundário (título em português)
    IndiceP *primarioMem;   //índice primário na memória
    IndiceS *secundarioMem; //índice secundário na memória

    short int op; //operação sendo executada pelo usuário no menu

    //processo de inicialização do programa ----------------------------------------------------------------------------
    opendir("data");
    if (ENOENT == errno)
        mkdir("data");

    /*
    *  verifica se existe um arquivo de dados
    *      se existe, abre para escrita e leitura
    *      se não existe, cria o arquivo para escrita e leitura
    */
    movies = fopen("data/movies.dat", "r+a");
    if (movies == NULL) //se é NULL
        movies = fopen("data/movies.dat", "w+");
    if (movies == NULL) {
        puts(ERROR"\tERRO: Impossível criar arquivo");
        return 1;
    }

    /*
     *  verifica se existem arquivos de índice
     *      se existirem, verifica se estão consistentes com o arquivo de dados (flag no cabeçalho)
     *          se estão consistentes, carregar na memória
     *          senão, refazer os índices na memória
     *      senão, criar os índices na RAM
     */
    iprimary = fopen("data/iprimary.idx", "r+a");
    if (iprimary != NULL) {
        char flag = fgetc(iprimary); //reads the very first character, the flag
        if (flag == CONSISTENTE)
            primarioMem = lerP(iprimary); //carrega na memória a partir do arquivo iprimary.idx
        else
            primarioMem = refazerP(movies); //refaz o índice na memória a partir do arquivo de filmes
    } else {
        iprimary = fopen("data/iprimary.idx", "w+");
        if (iprimary == NULL) {
            puts(ERROR "\tERRO: Impossível criar arquivo");
            return 1;
        }
        primarioMem = refazerP(movies); //faz o índice na memória a partir do arquivo de filmes
    }
    fseek(iprimary, 0, SEEK_SET);
    fputc(INCONSISTENTE, iprimary); //assume que o arquivo ficará inconsistente durante a execução do programa
    fclose(iprimary);

    ititle = fopen("data/ititle.idx", "r+a");
    if (ititle != NULL) {
        char flag = fgetc(ititle); //reads the very first character, the flag
        if (flag == CONSISTENTE)
            secundarioMem = lerS(ititle); //carrega na memória a partir do arquivo iprimary.idx
        else
            secundarioMem = refazerS(movies); //refaz o índice na memória a partir do arquivo de filmes
    } else {
        ititle = fopen("data/ititle.idx", "w+");
        if (ititle == NULL) {
            puts(ERROR "\tERRO: Impossível criar arquivo");
            return 1;
        }
        secundarioMem = refazerS(movies); //refaz o índice na memória a partir do arquivo de filmes
    }
    fseek(ititle, 0, SEEK_SET);
    fputc(INCONSISTENTE, ititle); //assume que o arquivo ficará inconsistente durante a execução do programa
    fclose(ititle);

    //menu do usuário --------------------------------------------------------------------------------------------------
    printf("\n");
    do {
        puts(TITLE "===========GERENCIADOR DE FILMES==========" CLEAR);
        puts(MENU "1." CLEAR " Inserir filme");
        puts(MENU "2." CLEAR " Remover filme");
        puts(MENU "3." CLEAR " Modificar nota");
        puts(MENU "4." CLEAR " Buscar filme");
        puts(MENU "5." CLEAR " Listar filmes\t" MENU "6. " CLEAR "Compactar base de dados");
        puts(MENU "0." CLEAR " Encerrar programa");
        printf(PROMPT "-> " CLEAR INPUT);
        scanf("%hd", &op);
        clearBuffer();

        switch (op) {
            case 1:
                inserirFilme(movies, &primarioMem, &secundarioMem);
                break;
            case 2:
                removerFilme(movies, &primarioMem, &secundarioMem);
                break;
            case 3:
                modificarNota(movies, primarioMem);
                break;
            case 4:
                buscarFilme(movies, primarioMem, secundarioMem);
                break;
            case 5:
                listarFilmes(movies);
                break;
            case 6:
                compactar(movies, &primarioMem);
                break;
            case 0:
                puts(SUCCESS "Encerrando programa..." CLEAR);
                break;
            default:
                puts(ERROR "\tERRO: Opcao invalida" CLEAR "\n");
                break;
        }
    } while (op != 0);

    //finalização do programa ------------------------------------------------------------------------------------------

    //salvar os arquivos
    fclose(movies);
    saveIndiceP(primarioMem);
    saveIndiceS(secundarioMem);

    //liberar a memória alocada
    freeIndiceP(primarioMem);
    freeIndiceS(secundarioMem);

    return 0;
}