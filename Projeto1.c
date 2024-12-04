#include <stdio.h>
#include<string.h>

//funçao para codigo de cores
int obterValorCor(char cor[]) 
{
    if (strcmp(cor, "preto") == 0) return 0;
    if (strcmp(cor, "castanho") == 0) return 1;
    if (strcmp(cor, "vermelho") == 0) return 2;
    if (strcmp(cor, "laranja") == 0) return 3;
    if (strcmp(cor, "amarelo") == 0) return 4;
    if (strcmp(cor, "verde") == 0) return 5;
    if (strcmp(cor, "azul") == 0) return 6;
    if (strcmp(cor, "roxo") == 0) return 7;
    if (strcmp(cor, "cinza") == 0) return 8;
    if (strcmp(cor, "branco") == 0) return 9;
    return -1; // Cor inválida
}

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
    int opcao, opcaoPortugues;

    // Mensagem de boas-vindas ao utilizador
    printf("---Bem-vindo(a) à nossa aplicação de Calculos Eletricos---\n");

    // Apresentar opções de menu
    printf("1- Menu em Portugues \n");
    printf("2- English Menu \n");
    printf("3- Sair \n");

    // Ler a opção desejada
    printf("Digite a opção desejada: ");
    scanf("%d", &opcao);

    // Processar a opção escolhida
    switch (opcao)
    {
    case 1:
    {
        int CodigoR;

        // Mostrar opções em português
        printf("-------- MENU EM PORTUGUES -------- \n");
        printf("1- Calcular resistencias em serie ou paralelo \n");
        printf("2- Descobrir valores de resistencias atraves do codigo cores \n");
        scanf("%d", &CodigoR);
        printf("3- Descobrir codigo de cores atraves de resistencias \n");
        printf("4- Lei das Malhas e nos\n");
        printf("5- Teorema de Norton\n");
        printf("6- Teorema de Thevenin\n");
        printf("7- Gerador de Ondas \n");
        printf("8- Dimensionar dijuntores\n");
        printf("9- Direcionar seccao de cabos eletricos\n");
        printf("10- Voltar ao Menu Principal \n");
        printf("Escolha uma opçao: ");
        scanf("%d", &opcaoPortugues);

        switch (opcaoPortugues)
        {
        case 1:
        {
            int n, CalculoR;

            printf("----- Calculadora de Resistencias -----\n");
            printf("1- Calculo de Resistencias em Serie\n");
            printf("2- Calculo de Resistencias em Paralelo\n");
            printf("Escolha uma opçao: ");
            scanf("%d", &CalculoR);

            // Ler o número de resistências
            printf("Quantas resistencias deseja calcular? ");
            scanf("%d", &n);

            // Validar número de resistências
            if (n <= 0)
            {
                printf("O número de resistencias deve ser maior que zero.\n");
                break;
            }

            float resistencia[n];
            for (int i = 0; i < n; i++)
            {
                printf("Digite o valor da resistencia %d (em ohms): ", i + 1);
                scanf("%f", &resistencia[i]);
            }

            switch (CalculoR)
            {
            case 1:
            {
                printf("-----Calculo em Serie-----\n");
                float resultadoSerie = RSerie(resistencia, n);
                printf("O valor total da resistencia em série e: %.2f ohms\n", resultadoSerie);
                break;
            }
            case 2:
            {
                printf("-----Calculo em Paralelo-----\n");
                float resultadoParalelo = RParalelo(resistencia, n);
                if (resultadoParalelo != 0)
                    printf("O valor total da resistencia em paralelo é: %.2f ohms\n", resultadoParalelo);
                else
                    printf("Neo foi possivel calcular a resistencia em paralelo devido a um valor invalido.\n");
                break;
            }
            default:
                printf("Opção invalida.\n");
                break;
            }
            break;
        }
        
        case 2:
        {
            int CodigoR;
            char cor1[20], cor2[20], cor3[20], cor4[20];
            int valor1, valor2, multiplicador;

            switch (CodigoR)
            {
            case 1:
                printf("Digite a primeira cor: ");
            scanf("%s", cor1);
            printf("Digite a segunda cor: ");
            scanf("%s", cor2);
            printf("Digite a terceira cor (multiplicador): ");
            scanf("%s", cor3);

            // Obter os valores das cores
            valor1 = obterValorCor(cor1);
            valor2 = obterValorCor(cor2);
            multiplicador = obterValorCor(cor3);

            if (valor1 == -1 || valor2 == -1 || multiplicador == -1) {
                printf("Cor inválida!\n");
                return 1;
            }

            // Calcular o valor da resistência
            long resistencia = (valor1 * 10 + valor2) * (1 << multiplicador); // Multiplicador em base de 2 (potência de 10)

            printf("O valor da resistência é: %ld ohms\n", resistencia);

                break;
            default:
                break;
            }

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
        printf("Saindo... Obrigado por usar nossa aplicacao!\n");
        break;

    default:
        printf("Opção inserida invalida.\n");
        break;
    }

    return 0;
}
