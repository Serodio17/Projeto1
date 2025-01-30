#include <stdio.h>
#include <string.h>
#include <math.h>

#define RESISTIVIDADE_COBRE 0.0172    // Ω·mm²/m
#define RESISTIVIDADE_ALUMINIO 0.0282 // Ω·mm²/m

// Standard cable sizes in mm²
double tamanhos_padrao[] = {0.5, 0.75, 1.5, 2.5, 4, 6, 10, 16, 25, 35, 50, 70, 95, 120, 150, 185, 240, 300, 400, 500, 630, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500};
int num_tamanhos = sizeof(tamanhos_padrao) / sizeof(tamanhos_padrao[0]);

double calcular_tamanho_cabo(double corrente, double comprimento, double queda_tensao_permitida, int material, double *tamanho_padrao)
{
    double resistividade = (material == 1) ? RESISTIVIDADE_COBRE : RESISTIVIDADE_ALUMINIO;
    double tamanho_requerido = (2 * resistividade * comprimento * corrente) / queda_tensao_permitida; // mm²

    // Find the smallest standard cable size that meets the requirement
    for (int i = 0; i < num_tamanhos; i++)
    {
        if (tamanhos_padrao[i] >= tamanho_requerido)
        {
            *tamanho_padrao = tamanhos_padrao[i];
            return tamanho_requerido;
        }
    }

    *tamanho_padrao = tamanhos_padrao[num_tamanhos - 1]; // Use the largest available size
    return tamanho_requerido;
}

// Helper function to convert string to lowercase
void toLowercase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

// Funcao para obter o valor da cor
int obterValorCor(char cor[])
{
    // Convert input color to lowercase for case-insensitive comparison
    toLowercase(cor);

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
    if (strcmp(cor, "violeta") == 0)
        return 7;
    if (strcmp(cor, "cinza") == 0)
        return 8;
    if (strcmp(cor, "branco") == 0)
        return 9;

    return -1; // Invalid color
}

// Funcao para obter o codigo de cores de uma resistencia
int obterCodigoCores(long resistencia, char codigo[3][20])
{
    if (resistencia <= 0)
    {
        printf("Valor de resistencia invalido. Deve ser positivo.\n");
        return -1;
    }

    // Calculate number of digits using integer arithmetic
    long temp = resistencia;
    int num_digits = 0;
    while (temp != 0)
    {
        temp /= 10;
        num_digits++;
    }

    if (num_digits < 2 || num_digits > 6)
    {
        printf("Valor da resistencia fora da faixa (10-999999 ohms).\n");
        return -1;
    }

    // Calculate divisors using integer arithmetic
    long divisor1 = 1, divisor2 = 1;
    for (int i = 0; i < num_digits - 1; i++)
        divisor1 *= 10;
    for (int i = 0; i < num_digits - 2; i++)
        divisor2 *= 10;

    int digito1 = (resistencia / divisor1) % 10;
    int digito2 = (resistencia / divisor2) % 10;

    char cores_digitos[10][20] = {
        "preto", "marrom", "vermelho", "laranja", "amarelo",
        "verde", "azul", "violeta", "cinza", "branco"};

    strcpy(codigo[0], cores_digitos[digito1]);
    strcpy(codigo[1], cores_digitos[digito2]);
    strcpy(codigo[2], cores_digitos[num_digits - 2]);

    return 0;
}

// Funcao para calcular resistencia em serie
float RSerie(float resistencia[], int n)
{
    float resultado = 0;
    for (int i = 0; i < n; i++)
    {
        resultado += resistencia[i];
    }
    return resultado;
}

// Funcao para calcular resistencia em paralelo
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
            printf("A resistencia %d nao pode ser zero em paralelo.\n", i + 1);
            return 0; // Retorna 0 para indicar erro
        }
    }
    return (resultado == 0.0) ? 0.0 : 1.0 / resultado;
}

// Funcao para resolver a Lei das Malhas
void leiDasMalhas()
{
    int n;
    printf("Digite o numero de malhas no circuito: ");
    scanf("%d", &n);

    if (n <= 0)
    {
        printf("O numero de malhas deve ser maior que zero.\n");
        return;
    }

    float *resistencias = (float *)malloc(n * sizeof(float));
    float *tensoes = (float *)malloc(n * sizeof(float));
    float *correntes = (float *)malloc(n * sizeof(float));

    if (resistencias == NULL || tensoes == NULL || correntes == NULL)
    {
        printf("Falha na alocacao de memoria.\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        printf("Digite a resistencia equivalente da malha %d (em ohms): ", i + 1);
        scanf("%f", &resistencias[i]);
        if (resistencias[i] <= 0)
        {
            printf("A resistencia deve ser maior que zero.\n");
            free(resistencias);
            free(tensoes);
            free(correntes);
            return;
        }

        printf("Digite a tensao total na malha %d (em volts): ", i + 1);
        scanf("%f", &tensoes[i]);
        if (tensoes[i] < 0)
        {
            printf("A tensao nao pode ser negativa.\n");
            free(resistencias);
            free(tensoes);
            free(correntes);
            return;
        }
    }

    printf("\n--- Resultados da Lei das Malhas ---\n");
    for (int i = 0; i < n; i++)
    {
        correntes[i] = tensoes[i] / resistencias[i];
        printf("Corrente na malha %d: %.2f A\n", i + 1, correntes[i]);
    }

    free(resistencias);
    free(tensoes);
    free(correntes);
}

/// Funcao para resolver a Lei dos nos
void leiDosNos()
{
    int entradas, saidas;
    float correnteEntrada = 0, correnteSaida = 0;

    printf("Introduza o numero de correntes de entrada no no: ");
    scanf("%d", &entradas);
    if (entradas < 0)
    {
        printf("O numero de correntes de entrada nao pode ser negativo.\n");
        return;
    }

    for (int i = 0; i < entradas; i++)
    {
        float corrente;
        printf("Introduza a corrente de entrada %d (em A): ", i + 1);
        scanf("%f", &corrente);
        if (corrente < 0)
        {
            printf("A corrente nao pode ser negativa.\n");
            return;
        }
        correnteEntrada += corrente;
    }

    printf("Introduza o numero de correntes de saida do no: ");
    scanf("%d", &saidas);
    if (saidas < 0)
    {
        printf("O numero de correntes de saida nao pode ser negativo.\n");
        return;
    }

    for (int i = 0; i < saidas; i++)
    {
        float corrente;
        printf("Introduza a corrente de saida %d (em A): ", i + 1);
        scanf("%f", &corrente);
        if (corrente < 0)
        {
            printf("A corrente nao pode ser negativa.\n");
            return;
        }
        correnteSaida += corrente;
    }

    printf("\n--- Resultados da Lei dos nos ---\n");
    if (fabs(correnteEntrada - correnteSaida) < 1e-6)
    {
        printf("O no esta equilibrado. As correntes de entrada e saida sao iguais.\n");
    }
    else
    {
        printf("O no nao esta equilibrado. Diferenca de corrente: %.2f A\n", correnteEntrada - correnteSaida);
    }
}

// Desenhar Onda sinosoidal e quadrada
#define PI 3.14159265358979323846

void generate_wave_svg(float frequency, float amplitude, float offset, const char *file_name, int wave_type)
{
    int width = 800;  // Largura fixa do SVG
    int height = 400; // Altura do SVG
    int center_axis = height / 2;
    float period = 1.0 / frequency;       // Calcular o periodo a partir da frequencia
    float total_time = 2 * period;        // Exibir 2 ciclos completos
    float time_step = total_time / width; // Incremento de tempo para cada pixel
    float wave_step = (2 * PI) / period;  // Incremento angular com base na frequencia
    int grid_spacing = 50;                // Espacamento entre as linhas da grade em pixels

    // Calcular os tamanhos das grades
    float grid_size_ms = (float)grid_spacing / width * total_time * 1000; // Tamanho horizontal da grade em ms
    float grid_size_volts = (float)grid_spacing / height * amplitude;     // Tamanho vertical da grade em volts

    // Abrir o arquivo SVG para escrita
    FILE *file = fopen(file_name, "w");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Cabecalho do SVG
    fprintf(file, "<svg xmlns='http://www.w3.org/2000/svg' width='%d' height='%d' viewBox='0 0 %d %d'>\n", width, height, width, height);
    fprintf(file, "<rect width='100%%' height='100%%' fill='white' />\n");

    // Desenhar as linhas da grade
    for (int x = 0; x <= width; x += grid_spacing)
    {
        fprintf(file, "<line x1='%d' y1='0' x2='%d' y2='%d' stroke='lightgray' stroke-width='1' />\n", x, x, height);
    }
    for (int y = 0; y <= height; y += grid_spacing)
    {
        fprintf(file, "<line x1='0' y1='%d' x2='%d' y2='%d' stroke='lightgray' stroke-width='1' />\n", y, width, y);
    }

    // Desenhar o eixo X (tempo) e o eixo Y (tensao)
    fprintf(file, "<line x1='0' y1='%d' x2='%d' y2='%d' stroke='black' stroke-width='1' />\n", center_axis, width, center_axis); // Eixo X
    fprintf(file, "<line x1='%d' y1='0' x2='%d' y2='%d' stroke='black' stroke-width='1' />\n", width / 2, width / 2, height);    // Eixo Y

    // Rotulos dos eixos
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>0ms</text>\n", width / 2 + 5, center_axis - 5);
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>tensao de Pico - %.1fV</text>\n", width / 2 + 5, 15, amplitude); // Exibir tensao de pico como "tensao de Pico - X V"

    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>-%.1fms</text>\n", 10, center_axis - 5, total_time * 500);
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>+%.1fms</text>\n", width - 50, center_axis - 5, total_time * 500);

    // Rotulos de tamanhos de quadrado da grade
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>Tamanho do quadrado da grade: %0.2fms</text>\n", 10, height - 40, grid_size_ms);
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>Tamanho do quadrado da grade: %0.2fV</text>\n", 10, height - 20, grid_size_volts);

    // Gerar pontos da onda (senoidal ou quadrada)
    fprintf(file, "<polyline fill='none' stroke='blue' stroke-width='2' points='");
    for (int x = 0; x < width; x++)
    {
        float current_time = time_step * x - (total_time / 2); // Tempo relativo ao centro
        float value;

        if (wave_type == 1)
        {
            // Onda senoidal: funcao seno padrao
            value = amplitude * sin(wave_step * current_time);
        }
        else if (wave_type == 2)
        {
            // Onda quadrada: alternar entre +amplitude e -amplitude
            value = (current_time - floor(current_time / period) * period) < (period / 2) ? amplitude : -amplitude;
        }
        else
        {
            printf("Tipo de onda invalido selecionado.\n");
            fclose(file);
            return;
        }

        // Aplicar o deslocamento
        value += offset;

        int y = center_axis - (int)((value * (height / 2 - 20)) / amplitude); // Mapear o valor para a altura do SVG
        fprintf(file, "%d,%d ", x, y);
    }
    fprintf(file, "' />\n");

    // Fechar o arquivo SVG
    fprintf(file, "</svg>\n");
    fclose(file);

    printf("Arquivo SVG '%s' gerado com sucesso.\n", file_name);
}

// Funcao secundaria para Calculos do disjuntor
// Funcao para calcular a classificacao do disjuntor
int calcular_disjuntor(int fase, double tensao, double potencia, double fp,
                       int tipo_carga, int carga_continua, double *corrente,
                       char *tipo_disjuntor)
{
    // Calculo da corrente base
    if (fase == 1)
    {
        *corrente = potencia / (tensao * fp);
    }
    else
    {
        *corrente = potencia / (sqrt(3) * tensao * fp);
    }

    // Fator de carga continua
    if (carga_continua)
    {
        *corrente *= 1.25;
    }

    // Ajuste conforme o tipo de carga
    switch (tipo_carga)
    {
    case 1: // Carga resistiva
        *corrente *= 1.0;
        strcpy(tipo_disjuntor, "Curva B");
        break;
    case 2: // Carga indutiva
        *corrente *= 1.2;
        strcpy(tipo_disjuntor, "Curva C");
        break;
    case 3: // Motores
        *corrente *= 2.0;
        strcpy(tipo_disjuntor, "Curva D");
        break;
    default:
        return -1; // Tipo de carga invalido
    }

    // Arredondar a corrente para evitar problemas de precisao com ponto flutuante
    *corrente = round(*corrente);

    // Tamanhos padrao de disjuntores
    int tamanhos[] = {1, 2, 3, 4, 6, 10, 13, 16, 20, 25, 32, 40, 50, 63, 80, 100, 125, 160, 200, 250, 300, 400, 500, 600, 630, 800, 1000, 1250, 1500, 1600, 2000, 2500, 3000, 3150, 3200, 4000, 5000, 6000, 6300};
    for (int i = 0; i < sizeof(tamanhos) / sizeof(tamanhos[0]); i++)
    {
        if (tamanhos[i] >= *corrente)
        {
            return tamanhos[i];
        }
    }
    return -2; // Acima de 200A
}

// Funcao para calcular a tensao e resistencia equivalente de Thevenin
// e a corrente e resistencia equivalente de Norton
void calculate_thevenin_and_norton(float voltages[], int num_sources, float resistances[], int num_resistances,
                                   float *Vth, float *Rth, float *In, float *Rn)
{
    // Calcular a tensao equivalente de Thevenin (Vth)
    *Vth = 0;
    for (int i = 0; i < num_sources; i++)
    {
        *Vth += voltages[i]; // Somando todas as tensoes (caso simples para fontes de tensao linear)
    }

    // Calcular a resistencia equivalente de Thevenin (Rth)
    *Rth = 0;
    for (int i = 0; i < num_resistances; i++)
    {
        *Rth += resistances[i]; // Somando todas as resistencias (assumindo resistencias em serie)
    }

    // Calcular a corrente de Norton (In)
    *In = *Vth / *Rth; // Corrente de Norton In = Vth / Rth

    // A resistencia de Norton e a mesma que a resistencia de Thevenin
    *Rn = *Rth;
}

// Funcao principal
int main()
{
    int opcaoPortugues;

    do
    {
        // Menu em Portugues
        printf("\n-------- MENU EM PORTUGUeS -------- \n");
        printf("1- Calcular resistencias em serie ou paralelo \n");
        printf("2- Descobrir valores de resistencia atraves do codigo de cores \n");
        printf("3- Descobrir codigo de cores atraves dos resistores \n");
        printf("4- Lei das Malhas e nos\n");
        printf("5- Teorema de Norton e Thevenin\n");
        printf("6- Gerador de ondas \n");
        printf("7- Dimensionamento de disjuntor\n");
        printf("8- Dimensionamento da secao do cabo eletrico\n");
        printf("0- Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcaoPortugues);

        switch (opcaoPortugues)
        {
        case 1:
        {
            int n, CalculoR;
            printf("----- Calculadora de resistencia -----\n");
            printf("1- Calculo de resistencias em serie\n");
            printf("2- Calculo de resistencias em Paralelo\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &CalculoR);

            printf("Quantas resistencias deseja calcular? ");
            scanf("%d", &n);

            if (n <= 0)
            {
                printf("O numero de resistores deve ser maior que zero.\n");
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
                printf("----- Calculo em serie ----- \n");
                float resultadoSerie = RSerie(resistencia, n);
                printf("O valor total da resistencia em serie e: %.2f ohms\n", resultadoSerie);
                break;
            case 2:
                printf("----- Calculo em Paralelo -----\n");
                float resultadoParalelo = RParalelo(resistencia, n);
                if (resultadoParalelo != 0)
                    printf("O valor total da resistencia em paralelo e: %.2f ohms\n", resultadoParalelo);
                else
                    printf("nao foi possivel calcular a resistencia em paralelo devido a um valor invalido.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
            }
            break;
        }

        case 2:
        {
            char cor1[20], cor2[20], cor3[20];
            printf("----- codigo de Cores ----- \n");
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
                printf("Cor invalida. Tente novamente.\n");
                break;
            }

            long resistencia = (valor1 * 10 + valor2) * pow(10, multiplicador);
            printf("O valor da resistencia e: %ld ohms\n", resistencia);
            break;
        }

        case 3:
        {
            long resistencia;
            char codigo[3][20];

            printf("----- Descubra o codigo de cores ----- \n");
            printf("Digite o valor da resistencia (em ohms): ");
            scanf("%ld", &resistencia);

            if (obterCodigoCores(resistencia, codigo) == 0)
            {
                printf("codigo de cores: %s, %s, %s\n", codigo[0], codigo[1], codigo[2]);
            }
            else
            {
                printf("Entrada invalida\n");
            }

            break;
        }

        case 4:
        {
            printf("----- Lei das Malhas e nos -----\n");
            int tipo;
            printf("Escolha: 1 para Lei das Malhas, 2 para Lei dos nos: ");
            scanf("%d", &tipo);

            if (tipo == 1)
                leiDasMalhas();
            else if (tipo == 2)
                leiDosNos();
            else
                printf("Opcao invalida.\n");

            break;
        }

        case 5:
        {
            int num_sources, num_resistances;

            // Entrada de fontes de tensao e resistencias
            printf("Digite o numero de fontes de tensao: ");
            scanf("%d", &num_sources);
            float voltages[num_sources];
            printf("Digite as tensoes das fontes (em volts):\n");
            for (int i = 0; i < num_sources; i++)
            {
                do
                {
                    printf("tensao da fonte %d: ", i + 1);
                    scanf("%f", &voltages[i]);
                    if (voltages[i] <= 0)
                    {
                        printf("Por favor, insira um valor positivo de tensao.\n");
                    }
                } while (voltages[i] <= 0);
            }

            // Entrada de resistencias
            printf("Digite o numero de resistencias: ");
            scanf("%d", &num_resistances);
            float resistances[num_resistances];
            printf("Digite as resistencias (em ohms):\n");
            for (int i = 0; i < num_resistances; i++)
            {
                do
                {
                    printf("resistencia %d: ", i + 1);
                    scanf("%f", &resistances[i]);
                    if (resistances[i] <= 0)
                    {
                        printf("Por favor, insira um valor positivo de resistencia.\n");
                    }
                } while (resistances[i] <= 0);
            }

            // Variaveis para resultados de Thevenin e Norton
            float Vth, Rth, In, Rn;

            // Chama a funcao para calcular os valores de Thevenin e Norton
            calculate_thevenin_and_norton(voltages, num_sources, resistances, num_resistances, &Vth, &Rth, &In, &Rn);

            // Exibe os resultados
            printf("\n---------------------------------------------");
            printf("\ntensao Equivalente de Thevenin (Vth): %.2f V\n", Vth);
            printf("resistencia Equivalente de Thevenin (Rth): %.2f Ohms\n", Rth);
            printf("\nCorrente Equivalente de Norton (In): %.2f A\n", In);
            printf("resistencia Equivalente de Norton (Rn): %.2f Ohms\n", Rn);
            printf("---------------------------------------------\n");
            break;
        }

        case 6:
        {
            float frequency, amplitude, offset;
            int wave_type;
            char file_name[100];

            printf("Bem-vindo ao simulador de osciloscopio!\n");

            // Obtemos a frequencia e a amplitude do usuario
            printf("Digite a frequencia da onda (Hz, ex: 50): ");
            scanf("%f", &frequency);
            if (frequency <= 0)
            {
                printf("Erro: A frequencia deve ser um valor positivo e nao nulo.\n");
                return 1;
            }

            printf("Digite a amplitude da onda (Volts, ex: 5.0): ");
            scanf("%f", &amplitude);
            if (amplitude <= 0)
            {
                printf("Erro: A amplitude deve ser um valor positivo e nao nulo.\n");
                return 1;
            }

            // Obtemos o deslocamento da onda
            printf("Digite o deslocamento da onda (Volts, ex: 1.0): ");
            scanf("%f", &offset);

            // Obtemos o tipo de onda
            printf("Escolha o tipo de onda (1 para Senoidal, 2 para Quadrada): ");
            scanf("%d", &wave_type);

            // Nome do arquivo de saida
            printf("Digite o nome do arquivo de saida (ex: onda.svg): ");
            scanf("%s", file_name);

            // Gerar a onda apropriada
            if (wave_type == 1)
            {
                generate_wave_svg(frequency, amplitude, offset, file_name, 1);
            }
            else if (wave_type == 2)
            {
                generate_wave_svg(frequency, amplitude, offset, file_name, 2);
            }
            else
            {
                printf("Escolha invalida para tipo de onda.\n");
                return 1;
            }

            printf("Abra o arquivo '%s' no seu navegador para visualizar a onda.\n", file_name);

            break;
        }

        case 7:
        {
            int fase, tipo_carga;
            double tensao, potencia, fp;
            char operacao_continua;

            // Entrada do usuario
            printf("Calculadora de Dimensionamento de Disjuntor\n");
            printf("---------------------------------\n");

            printf("1. Tipo de sistema (1-Fase unica / 3-Trifasico): ");
            scanf("%d", &fase);

            printf("2. tensao nominal (V): ");
            scanf("%lf", &tensao);

            printf("3. Potencia ativa total (kW): ");
            scanf("%lf", &potencia);
            potencia *= 1000; // Converter para Watts

            printf("4. Fator de potencia (0.1-1.0): ");
            scanf("%lf", &fp);

            printf("\nTipo de carga predominante:\n");
            printf("1. Resistiva\n");
            printf("2. Indutiva\n");
            printf("3. Capacitiva\n");
            printf("Escolha uma opcao: ");
            scanf("%d", &tipo_carga);

            printf("\nFase 1-Confirmar operacao continua (s/n)? ");
            scanf(" %c", &operacao_continua);

            // Calcula o dimensionamento do disjuntor
            double corrente = potencia / (tensao * fp);
            printf("A corrente necessaria para o disjuntor e %.2f A\n", corrente);

            break;
        }

        case 8:
        {
            printf("---- Dimensionamento da secao do Cabo eletrico ----\n");
            double potencia, tensao, corrente, resistividade, comprimento, area;
            char tipo_cabo;

            printf("Digite a potencia (W): ");
            scanf("%lf", &potencia);
            printf("Digite a tensao (V): ");
            scanf("%lf", &tensao);
            printf("Digite a resistividade do material do cabo (Ω.m): ");
            scanf("%lf", &resistividade);
            printf("Digite o comprimento do cabo (m): ");
            scanf("%lf", &comprimento);

            // Calcula a corrente
            corrente = potencia / tensao;

            // Escolha do tipo de cabo
            printf("Escolha o tipo de cabo (a-aluminio / c-cobre): ");
            scanf(" %c", &tipo_cabo);

            // Determina a area do cabo
            area = calcularAreaCabo(corrente, resistividade, comprimento, tipo_cabo);

            printf("A secao do cabo necessaria e %.2f mm²\n", area);

            break;
        }

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    } while (opcaoPortugues != 0);

    return 0;
}
