#include <stdio.h>
#include <string.h>
#include <math.h>

// Cálculo da seção de cabos
double calcularSeccao(double corrente, double comprimento, double condutividade, double deltaT)
{
    return (corrente * comprimento) / (condutividade * deltaT);
}

// Função para obter o valor da cor
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

// Função para obter o código de cores de uma resistência
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

    if (n <= 0)
    {
        printf("O número de malhas deve ser maior que zero.\n");
        return;
    }

    float resistencias[n], tensoes[n], correntes[n];
    for (int i = 0; i < n; i++)
    {
        printf("Digite a resistência equivalente da malha %d (em ohms): ", i + 1);
        scanf("%f", &resistencias[i]);
        printf("Digite a tensão total na malha %d (em volts): ", i + 1);
        scanf("%f", &tensoes[i]);
    }

    printf("\n--- Resultados da Lei das Malhas ---\n");
    for (int i = 0; i < n; i++)
    {
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

// Desenhaar Onda sinosoidal
#define PI 3.14159265358979323846

void desenhar_onda_senoidal(float frequencia, float amplitude, int largura_terminal)
{
    int linhas = 21;                     // Número de linhas para representar a amplitude
    int eixo_central = linhas / 2;       // Linha central representando o eixo X
    char tela[linhas][largura_terminal]; // Matriz para armazenar o desenho da onda

    // Inicializar a matriz com espaços
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < largura_terminal; j++)
        {
            tela[i][j] = ' ';
        }
    }

    // Desenhar o eixo X
    for (int j = 0; j < largura_terminal; j++)
    {
        tela[eixo_central][j] = '-';
    }

    // Ajustar frequência para caber no terminal
    float ciclos_visiveis = 2; // Exibir 2 ciclos completos
    float passo = (2 * PI * ciclos_visiveis) / largura_terminal;

    // Gerar a onda senoidal
    for (int x = 0; x < largura_terminal; x++)
    {
        float valor = amplitude * sin(passo * x);                                 // Calcular valor da função seno
        int linha = eixo_central - (int)((valor * (linhas / 2 - 1)) / amplitude); // Mapear valor para a linha

        // Garantir que o ponto da onda esteja dentro dos limites
        if (linha >= 0 && linha < linhas)
        {
            tela[linha][x] = '*';
        }
    }

    // Imprimir a matriz no terminal
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < largura_terminal; j++)
        {
            printf("%c", tela[i][j]);
        }
        printf("\n");
    }
}

// Main function
int main()
{
    int opcaoPortugues;

    // Menu em Português
    printf("-------- MENU EM PORTUGUES -------- \n");
    printf("1- Calcular resistências em série ou paralelo \n");
    printf("2- Descobrir valores de resistências através do código de cores \n");
    printf("3- Descobrir código de cores através de resistências \n");
    printf("4- Lei das Malhas e Nós\n");
    printf("5- Teorema de Norton e de Thévenin\n");
    printf("6- Gerador de Ondas \n");
    printf("7- Dimensionar disjuntores\n");
    printf("8- Dimensionar seção de cabos elétricos\n");
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

        printf("Quantas resistências deseja calcular? ");
        scanf("%d", &n);

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
            printf("----- Cálculo em Série -----\n");
            float resultadoSerie = RSerie(resistencia, n);
            printf("O valor total da resistência em série é: %.2f ohms\n", resultadoSerie);
            break;
        case 2:
            printf("----- Cálculo em Paralelo -----\n");
            float resultadoParalelo = RParalelo(resistencia, n);
            if (resultadoParalelo != 0)
                printf("O valor total da resistência em paralelo é: %.2f ohms\n", resultadoParalelo);
            else
                printf("Não foi possível calcular a resistência em paralelo devido a um valor inválido.\n");
            break;
        default:
            printf("Opção inválida.\n");
            break;
        }
        break;
    }

    case 2:
    {
        char cor1[20], cor2[20], cor3[20];
        printf("----- Código de Cores ----- \n");
        printf("Digite a primeira cor: ");
        scanf("%s", cor1);
        printf("Digite a segunda cor: ");
        scanf("%s", cor2);
        printf("Digite a terceira cor (multiplicador): ");
        scanf("%s", cor3);

        int valor1 = obterValorCor(cor1);
        int valor2 = obterValorCor(cor2);
        int multiplicador = obterValorCor(cor3);

        if (valor1 == -1 || valor2 == -1 || multiplicador == -1)
        {
            printf("Cor inválida. Tente novamente.\n");
            break;
        }

        long resistencia = (valor1 * 10 + valor2) * pow(10, multiplicador);
        printf("O valor da resistência é: %ld ohms\n", resistencia);
        break;
    }

    case 3:
    {
        long resistencia;
        char codigo[4][20];

        printf("----- Descobrir o código de cores ----- \n");
        printf("Digite o valor da resistência (em ohms): ");
        scanf("%ld", &resistencia);

        if (resistencia < 0)
        {
            printf("Resistência inválida. Tente novamente.\n");
            break;
        }

        obterCodigoCores(resistencia, codigo);
        printf("Código de cores: %s, %s, %s\n", codigo[0], codigo[1], codigo[2]);
        break;
    }

    case 4:
    {
        printf("----- Lei das Malhas e Nós -----\n");
        int tipo;
        printf("Escolha: 1 para Lei das Malhas, 2 para Lei dos Nós: ");
        scanf("%d", &tipo);
        if (tipo == 1)
            leiDasMalhas();
        else if (tipo == 2)
            leiDosNos();
        else
            printf("Opção inválida.\n");
        break;
    }

    case 5:
    {
        printf("----- Teorema de Norton e Thévenin -----\n");
        double tnR1, tnR2, tnVth, tnRth, tnIn;

        // Entrada de valores
        printf("Digite o valor de R1 (ohms): ");
        scanf("%lf", &tnR1);

        printf("Digite o valor de R2 (ohms): ");
        scanf("%lf", &tnR2);

        printf("Digite o valor da tensão de Thévenin (V): ");
        scanf("%lf", &tnVth);

        // Calculos
        tnRth = (tnR1 * tnR2) / (tnR1 + tnR2);
        tnIn = tnVth / tnRth;

        // Exibir resultado
        printf("\nResistencia de Norton : %.2f ohms\n", tnRth);
        printf("Corrente de Norton: %.2f A\n", tnIn);
        break;
    }

    case 6:
    {
        printf("----- Gerador de Ondas -----\n");
        // Implementação específica deve ser feita aqui
        float frequencia, amplitude;
        int largura_terminal;

        printf("----- Gerador de Onda Senoidal -----\n");
        printf("Digite a frequência da onda (Hz, até 1000 kHz): ");
        scanf("%f", &frequencia);
        printf("Digite a amplitude da onda (V, até 220 V): ");
        scanf("%f", &amplitude);
        printf("Digite a largura do terminal (número de colunas): ");
        scanf("%d", &largura_terminal);

        // Limitar frequência e amplitude para valores máximos
        if (frequencia > 1000000.0)
        {
            printf("A frequência máxima permitida é 1000 kHz. Ajustando para 1000 kHz.\n");
            frequencia = 1000000.0;
        }
        if (amplitude > 220.0)
        {
            printf("A amplitude máxima permitida é 220 V. Ajustando para 220 V.\n");
            amplitude = 220.0;
        }
        if (largura_terminal < 20)
        {
            printf("A largura mínima do terminal é 20. Ajustando para 20.\n");
            largura_terminal = 20;
        }

        // Ajustar a frequência para visualização se muito alta
        if (frequencia > 10.0)
        {
            printf("Frequência muito alta! Reduzindo para escala visual.\n");
            frequencia = 10.0; // Escala a frequência para ser legível
        }

        desenhar_onda_senoidal(frequencia, amplitude, largura_terminal);

        break;
    }

    case 7:
    {
        printf("----- Dimensionar Disjuntores -----\n");
        float potencia_total, tensao;
        float corrente_cal;
        float fat_seg;
        int disjuntor_selecionado = -1;

        // Lista padrão de disjuntores comerciais (em amperes)
        int disjuntores_comerciais[] = {2, 4, 6, 10, 16, 20, 25, 32, 40, 50, 63, 80, 100, 125, 160, 200, 250, 300, 400, 630, 800};
        int tamanho_lista = sizeof(disjuntores_comerciais) / sizeof(disjuntores_comerciais[0]);

        // Entrada de dados do usuário
        printf("Digite a potência total consumida pelo sistema em watts: ");
        scanf("%f", &potencia_total);

        printf("Digite a tensão do sistema em volts (ex: 127 ou 220): ");
        scanf("%f", &tensao);

        printf("Digite o fator de segurança (sugestão: 1.25): ");
        scanf("%f", &fat_seg);

        // Cálculo da corrente necessária
        corrente_cal = (potencia_total / tensao) * fat_seg;

        // Encontrar o disjuntor adequado
        for (int i = 0; i < tamanho_lista; i++)
        {
            if (disjuntores_comerciais[i] >= corrente_cal)
            {
                disjuntor_selecionado = disjuntores_comerciais[i];
                break;
            }
        }

        // Exibir o resultado
        if (disjuntor_selecionado != -1)
        {
            printf("\nCorrente calculada: %.2f A\n", corrente_cal);
            printf("Disjuntor recomendado: %d A\n", disjuntor_selecionado);
        }
        else
        {
            printf("\nCorrente calculada: %.2f A\n", corrente_cal);
            printf("Não foi encontrado um disjuntor adequado.\n");
        }

        break;
    }

    case 8:
    {
        printf("----- Dimensionar seção de cabos -----\n");
        double corrente, comprimento, condutividade, deltaT, seccao;

        printf("Digite a corrente elétrica (em A): ");
        scanf("%lf", &corrente);

        printf("Digite o comprimento do cabo (em m): ");
        scanf("%lf", &comprimento);

        printf("Digite a condutividade do material (em S/m): ");
        scanf("%lf", &condutividade);

        printf("Digite a variação de temperatura permitida (em °C): ");
        scanf("%lf", &deltaT);

        seccao = calcularSeccao(corrente, comprimento, condutividade, deltaT);

        printf("A seção do cabo necessária é: %.2f mm²\n", seccao);

        break;
    }

    default:
        printf("Opção inválida.\n");
        break;
    }

    return 0;
}
