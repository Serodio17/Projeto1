#include <stdio.h>
#include <string.h>
#include <math.h>

// funçao para codigo de cores
int obterValorCor(char cor[])
{
    if (strcmp(cor, "preto") == 0)
        return 0;
    if (strcmp(cor, "castanho") == 0)
        return 1;
    if (strcmp(cor, "vermelho") == 0)
        return 2;
    if (strcmp(cor, "laranja") == 0)
        return 3;
    if (strcmp(cor, "amarelo") == 0)
        return 4;
    if (strcmp(cor, "verde") == 0)
        return 5;
    if (strcmp(cor, "azul") == 0)
        return 6;
    if (strcmp(cor, "roxo") == 0)
        return 7;
    if (strcmp(cor, "cinza") == 0)
        return 8;
    if (strcmp(cor, "branco") == 0)
        return 9;
    return -1; // Cor inválida
}

// Função para obter o código de cores
void obterCodigoCores(long resistencia, char codigo[4][20])
{
    int digito1 = resistencia / 10;
    int digito2 = resistencia % 10;
    int multiplicador = log10(resistencia) - 1;

    char cores[10][20] = {
        "preto", "castanho", "vermelho", "laranja",
        "amarelo", "verde", "azul", "roxo",
        "cinza", "branco"};

    strcpy(codigo[0], cores[digito1]);
    strcpy(codigo[1], cores[digito2]);
    strcpy(codigo[2], cores[multiplicador]);
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
            return 0; // Retorna 0 para indicar erro
        }
    }
    return (resultado == 0.0) ? 0.0 : 1.0 / resultado;
}

// Função para resolver a Lei das Malhas
void leiDasMalhas() 
{
    int n;
    printf("Digite o número de malhas no circuito: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("O número de malhas deve ser maior que zero.\n");
        return;
    }

    float resistencias[n], tensoes[n], correntes[n];
    for (int i = 0; i < n; i++) {
        printf("Digite a resistência equivalente da malha %d (em ohms): ", i + 1);
        scanf("%f", &resistencias[i]);
        printf("Digite a tensão Total na malha %d (em volts): ", i + 1);
        scanf("%f", &tensoes[i]);
    }

    printf("\n--- Resultados da Lei das Malhas ---\n");
    for (int i = 0; i < n; i++) {
        correntes[i] = tensoes[i] / resistencias[i];
        printf("Corrente na malha %d: %.2f A\n", i + 1, correntes[i]);
    }
}

// Função para resolver a Lei dos Nós
void leiDosNos()
{
    int entradas, saidas;
    float correnteEntrada = 0, correnteSaida = 0;

    printf("Digite o número de correntes de entrada no nó: ");
    scanf("%d", &entradas);
    for (int i = 0; i < entradas; i++)
    {
        float corrente;
        printf("Digite a corrente de entrada %d (em A): ", i + 1);
        scanf("%f", &corrente);
        correnteEntrada += corrente;
    }

    printf("Digite o número de correntes de saída do nó: ");
    scanf("%d", &saidas);
    for (int i = 0; i < saidas; i++)
    {
        float corrente;
        printf("Digite a corrente de saída %d (em A): ", i + 1);
        scanf("%f", &corrente);
        correnteSaida += corrente;
    }

    printf("\n--- Resultados da Lei dos Nós ---\n");
    if (fabs(correnteEntrada - correnteSaida) < 1e-6)
    {
        printf("O nó está equilibrado. Correntes de entrada e saída são iguais.\n");
    }
    else
    {
        printf("O nó NÃO está equilibrado. Diferença de corrente: %.2f A\n", correnteEntrada - correnteSaida);
    }
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

        // Mostrar opções em português
        printf("-------- MENU EM PORTUGUES -------- \n");
        printf("1- Calcular resistencias em serie ou paralelo \n");
        printf("2- Descobrir valores de resistencias atraves do codigo cores \n");
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
                    printf("Nao foi possivel calcular a resistencia em paralelo devido a um valor invalido.\n");
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
            char cor1[20], cor2[20], cor3[20];
            printf("-----Codigo de Cores----- \n");
            printf("Digite a primeira cor: ");
            scanf("%s", cor1);
            printf("Digite a segunda cor: ");
            scanf("%s", cor2);
            printf("Digite a terceira cor (multiplicador): ");
            scanf("%s", cor3);

            int valor1 = obterValorCor(cor1);
            int valor2 = obterValorCor(cor2);
            int multiplicador = obterValorCor(cor3);

            if (valor1 == -99 || valor2 == -99 || multiplicador == -99)
            {
                printf("Uma ou mais cores são inválidas.\n");
            }
            else
            {

                double resistencia = (valor1 * 10 + valor2) * pow(10, multiplicador);
                printf("O valor da resistência é: %.2f ohms\n", resistencia);

                if (resistencia >= 1000000)
                {
                    printf("Ovalor da resistencia é: %1f M ohms \n", resistencia / 1000000);
                }
                else if (resistencia >= 1000)
                {
                    printf("Ovalor da resistencia é: %.2f K ohms \n", resistencia / 1000);
                }
            }
            break;
        }
        case 3:
        {
            long resistencia;
            printf("-----codigo de cores----- \n");
            printf("Digite o valor da resistência (em ohms): ");
            scanf("%ld", &resistencia);

            char codigo[4][20];
            obterCodigoCores(resistencia, codigo);
            printf("O código de cores para esta resistência é: %s %s %s\n", codigo[0], codigo[1], codigo[2]);
            break;
        }
        default:
            break;
        }
    case 4:
    {
        int opcaoLei;
        printf("-----Qual opçao deseja?---- \n");
        printf("1- Lei das Malhas\n");
        printf("2- Lei dos Nós\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcaoLei);

        switch (opcaoLei)
        {
        case 1:
            leiDasMalhas();
            break;
        case 2:
            leiDosNos();
            break;
        default:
            printf("Opção inválida para a Lei das Malhas e Nós.\n");
            break;
        }
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
    }

    return 0;
}
