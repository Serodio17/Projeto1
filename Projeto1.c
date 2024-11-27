#include <stdio.h>

// Declarar Calculo Resistencia em serie
float RSerie(float resistencia[], int n)
{
    float resultado = 0;
    for (int i = 0; i < n; i++)
    {
        resultado += resistencia[i];
    }
    return resultado;
}
// Declarar calculo resitencia Paralelo
float RParalelo(float resistencia[], int n)
{
    float resultado = 0;
    for (int i = 0; i < n; i++)
    {
        if (resistencia[i] != 0)
        {
            resultado += 1 / resistencia[i];
        }
        else
        {
            printf("A resistencia nao pode ser zero em Paralelo\n");
        }
    }
    return (resultado == 0.0) ? 0.0 : 1.0 / resultado;
}

int main()
{
    // Declarar variaveis para menu principal dando seguimento menus secundarios
    int opcao;
    int opcaoPortugues;
    int opcaoIngles;

    // mensagem de boas vindas ao utilizador
    printf("Bem-vindo(a) à nossa aplicação de Cálculos Eletricos \n");

    // Apresentar opções de menu
    printf("1- Menu em Portugues \n");
    printf("2- English Menu \n");
    printf("3- Sair \n");

    // Dar a escolher ao utilizador a opçao desejada
    printf("Preparado(a) para começar? Vamos facilitar seus cálculos elétricos! \n");
    printf("Digite a opçao Desejada:____");
    scanf("%d", &opcao);

    // Processar a opção escolhida
    switch (opcao)
    {
    default:
    {
        printf("Opcao Inserida Invalida");
    }
    case 1:
    {
        // Mostrar opçoes:
        printf("--------MENU_EM_PORTUGUES-------- \n");
        printf("1- Calcular resistencias em serie ou paralelo \n");
        printf("2- Descobrir valores de resistencias atraves de cores \n");
        printf("3- Descobrir codigo de cores atraves de resistencias \n");
        printf("4- Lei das Malhas e nos\n");
        printf("5- Teorema de Norton\n");
        printf("6- Teorema de Thevenin\n");
        printf("7- Gerador de Ondas \n");
        printf("8- Dimensionar dijuntores\n");
        printf("9- Direcionar seccao de cabos eletricos\n");
        printf("10- Voltar ao Menu Principal \n");
        printf("Escolha uma opcao:____");
        scanf("%d", &opcaoPortugues);

        switch(opcaoPortugues)
        {
        case 1:
            int n;
            float resistencia[n];
            float RParaleo;
            int CalculoR;
            printf("-----Calculadora de Resistencias-----\n");
            printf("1- Calculo de Resistencias em Serie\n");
            printf("2- Calculo de Resitencias em pParalelo");
            switch (CalculoR)
            {
            case 1:
                printf("-----Calculo em Serie-----\n");
                printf("Quantas Resistencias deseja calcular?:   ");
                scanf("%d", &n);

                for (int i = 0; i < n; i++)
                {
                    printf("Digite o valor da resistência %d (em ohms): ", i + 1);
                    scanf("%lf", &resistencia[i]);
                }
                float RSerie = RSerie(resistencia, n);
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }
    }
}
