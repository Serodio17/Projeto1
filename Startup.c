#include <stdio.h>
#include <stdlib.h>

// Function to execute the desired version of the program
void executeVersion(const char *fileName) {
    if (system(fileName) == -1) {
        perror("Erro ao executar o programa / Error executing the program");
    }
}

int main()
{
    int opcao;

    // Mensagem de boas-vindas
    printf("---Bem-vindo(a) a nossa aplicacao de Calculos Eletricos---\n");

    // Menu principal
    printf("1- Menu em Portugues \n");
    printf("2- English Menu \n");
    printf("3- Sair \n");
    printf("Digite a opcao desejada: ");
    scanf("%d", &opcao);

    if (opcao==1) {
        executeVersion(".\\PT.exe");  // Replace with actual portuguese version file path
    } else if (opcao==2){
        executeVersion(".\\EN.exe");  // Replace with actual English version file path
    } else if (opcao==3){
        printf("Obrigado por usar o programa!\n");
        printf("Thank you for using the program!\n");
    } else{
        printf("Opcao invalida.\nInvalid option.\n");
    }
}
