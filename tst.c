#include <stdio.h>

// Função para calcular resistência em série
float RSerie(float resistencia[], int n)
{
    float resultado = 0;
    for (int i = 0; i < n; i++)
    {
        resultado += resistencia[i];
    }
    return resultado;
}

// Função para calcular resistência em paralelo
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
            printf("A resistência %d não pode ser zero em paralelo.\n", i + 1);
            return 0;  // Retorna 0 para indicar erro
        }
    }
    return (resultado == 0.0) ? 0.0 : 1.0 / resultado;
}

int main()
{
    // Declarar variáveis para menu principal
    int opcao, opcaoPortugues,opcaoIngles;

    // Mensagem de boas-vindas ao utilizador
    printf("Bem-vindo(a) à nossa aplicação de Cálculos Elétricos \n");

    // Apresentar opções de menu
    printf("1- Menu em Português \n");
    printf("2- English Menu \n");
    printf("3- Sair \n");

    // Ler a opção desejada
    printf("Digite a opção desejada: ");
    scanf("%d", &opcao);

    // Processar a opção escolhida
    switch (opcao)
    {
    case 1:
        // Mostrar opções em português
        printf("-------- MENU EM PORTUGUÊS -------- \n");
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
        printf("Escolha uma opção: ");
        scanf("%d", &opcaoPortugues);

        switch (opcaoPortugues)
        {
        case 1:
        {
            int n, CalculoR;

            printf("----- Calculadora de Resistências -----\n");
            printf("1- Cálculo de Resistências em Série\n");
            printf("2- Cálculo de Resistências em Paralelo\n");
            printf("Escolha uma opção: ");
            scanf("%d", &CalculoR);

            // Ler o número de resistências
            printf("Quantas resistências deseja calcular? ");
            scanf("%d", &n);

            // Validar número de resistências
            if (n <= 0)
            {
                printf("O número de resistências deve ser maior que zero.\n");
                break;
            }

            float resistencia[n];
            for (int i = 0; i < n; i++)
            {
                printf("Digite o valor da resistência %d (em ohms): ", i + 1);
                scanf("%f", &resistencia[i]);
            }

            switch (CalculoR)
            {
            case 1:
            {
                printf("-----Calculo em Serie-----\n");
                float resultadoSerie = RSerie(resistencia, n);
                printf("O valor total da resistência em série é: %.2f ohms\n", resultadoSerie);
                break;
            }
            case 2:
            {
                printf("-----Calculo em Paralelo-----\n");
                float resultadoParalelo = RParalelo(resistencia, n);
                if (resultadoParalelo != 0)
                    printf("O valor total da resistência em paralelo é: %.2f ohms\n", resultadoParalelo);
                else
                    printf("Não foi possível calcular a resistência em paralelo devido a um valor inválido.\n");
                break;
            }
            default:
                printf("Opção inválida.\n");
                break;
            }
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
        case 7:
        {
            break;
        }
        case 8:
        {
            break;
        }
        case 9:
        {
            break;
        }
        case 10:
        {
            break;
        }
        default:
            printf("Opção inválida.\n");
            break;

        }
        break;

    case 2:
        printf("English menu is not implemented yet.\n");
        break;

    case 3:
        printf("Saindo... Obrigado por usar nossa aplicação!\n");
        break;

    default:
        printf("Opção inserida inválida.\n");
        break;
    }

    return 0;
}
