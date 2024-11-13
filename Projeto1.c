#include<stdio.h>

int main ()
{
    //Declarar variaveis para menu principal dando seguimento menus secundarios
    int opcao;
    int opcaoPortugues;
    int opcaoIngles;

    //mensagem de boas vindas ao utilizador
    printf("Bem-vindo(a) à nossa aplicação de Cálculos Eletricos \n");

    //Apresentar opções de menu
    printf("1- Menu em Portugues \n");
    printf("2- English Menu \n");
    printf("3- Sair \n");

    //Dar a escolher ao utilizador a opçao desejada
    printf("Preparado(a) para começar? Vamos facilitar seus cálculos elétricos! \n");
    printf ("Digite a opçao Desejada:____");
    scanf("%d", &opcao);

    //Processar a opção escolhida
    switch (opcao)
    {
        case 1:
            printf("Escolheste o Menu em Portugues, Vamos Começar! \n");
                while (1)
                {
                    //Mostrar opçoes:
                    printf("--------MENU_EM_PORTUGUES-------- \n");
                    printf("1- Calcular resistencias em serie ou paralelo \n");
                    printf("2- Calculos de bobines \n");
                    printf("3- Calcular capacitancia de condensadores \n");
                    printf("4- Conversoes Estrtela-Triangulo \n");
                    printf("5- Calculos Lei de Ohm \n");
                    printf("6- Descobrir valores de resistencias atraves de cores \n");
                    printf("7- Calcular Divisor de Corrente");
                    printf("8- Calcular Divisor de tensao \n");
                    printf("9- Gerador de Ondas \n");
                    printf("10- Voltar ao Menu Principal \n");

                    printf("Escolha uma opcao:____");
                    scanf("%d", &opcaoPortugues);

                    switch (opcaoPortugues)
                    {
                        case 1:
                            int TipoCalculo;
                            float R1, R2, R3, R4, R5, R6, R7;
                            
                            printf("Qual calculo deseja efetuar? \n");
                            printf("1- Resistencias em Serie \n");
                            printf("2- Resistencias em paralelo \n");
                            break;
                        
                        default:
                            printf("Opçao invalida, Tenta Outra vez.");
                            break;
                    }
                }
                
            
            break;
        
        case 2:
            printf("You selected the english menu \n");
            //mostrar opçoes 
            //dar a escolher
            break;

        case 3:
            printf("Encerrando a aplicaçao, Ate logo!");
            break;
        
        default:
            printf("Opçao Invalida, Tenta outra vez");
            break;
    }



}