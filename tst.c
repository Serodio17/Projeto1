#include <stdio.h>
#include <string.h>
#include <math.h>

#define RESISTIVIDADE_COBRE 0.0172    // Ω·mm2/m
#define RESISTIVIDADE_ALUMINIO 0.0282 // Ω·mm2/m

// Standard cable sizes in mm2
double tamanhos_padrao[] = {0.5, 0.75, 1.5, 2.5, 4, 6, 10, 16, 25, 35, 50, 70, 95, 120, 150, 185, 240, 300, 400, 500, 630, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500};
int num_tamanhos = sizeof(tamanhos_padrao) / sizeof(tamanhos_padrao[0]);

double calcular_tamanho_cabo(double corrente, double comprimento, double queda_tensao_permitida, int material, double *tamanho_padrao)
{
    double resistividade = (material == 1) ? RESISTIVIDADE_COBRE : RESISTIVIDADE_ALUMINIO;
    double tamanho_requerido = (2 * resistividade * comprimento * corrente) / queda_tensao_permitida; // mm2

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
// Funçao para obter o valor da cor
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
    if (strcmp(cor, "cinzento") == 0)
        return 8;
    if (strcmp(cor, "branco") == 0)
        return 9;

    return -1; // Invalid color
}

// Funçao para obter o codigo de cores de uma resistencia
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
        printf("Valor de resistencia fora da faixa (10-999999 ohms).\n");
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
        "preto", "castanho", "vermelho", "laranja", "amarelo",
        "verde", "azul", "violeta", "cinzento", "branco"};

    strcpy(codigo[0], cores_digitos[digito1]);
    strcpy(codigo[1], cores_digitos[digito2]);
    strcpy(codigo[2], cores_digitos[num_digits - 2]);

    return 0;
}

// Funçao para calcular resistencia em serie
float RSerie(float resistencia[], int n)
{
    float resultado = 0;
    for (int i = 0; i < n; i++)
    {
        resultado += resistencia[i];
    }
    return resultado;
}

// Funçao para calcular resistencia em paralelo
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

// Funçao para resolver a Lei das Malhas
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
        printf("Falha na alocaçao de memoria.\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        printf("Insira a resistencia equivalente da malha %d (em ohms): ", i + 1);
        scanf("%f", &resistencias[i]);
        if (resistencias[i] <= 0)
        {
            printf("A resistencia deve ser maior que zero.\n");
            free(resistencias);
            free(tensoes);
            free(correntes);
            return;
        }

        printf("Insira a tensao total na malha %d (em volts): ", i + 1);
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

// Funçao para resolver a Lei dos Nos
void leiDosNos()
{
    int entradas, saidas;
    float correnteEntrada = 0, correnteSaida = 0;

    printf("Digite o numero de correntes de entrada no no: ");
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
            printf("Corrente nao pode ser negativa.\n");
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

    printf("\n--- Resultados da Lei dos Nos ---\n");
    if (fabs(correnteEntrada - correnteSaida) < 1e-6)
    {
        printf("O no e balanceado. As correntes de entrada e saida sao iguais.\n");
    }
    else
    {
        printf("O no nao esta balanceado. Diferença atual: %.2f A\n", correnteEntrada - correnteSaida);
    }
}

// Desenhar Onda sinosoidal e quadrada
#define PI 3.14159265358979323846

void generate_wave_svg(float frequency, float amplitude, float offset, const char *file_name, int wave_type)
{
    int width = 800;  // Fixed width of the SVG
    int height = 400; // Height of the SVG
    int center_axis = height / 2;
    float period = 1.0 / frequency;       // Calculate the period from the frequency
    float total_time = 2 * period;        // Display 2 full cycles
    float time_step = total_time / width; // Time increment for each pixel
    float wave_step = (2 * PI) / period;  // Angular increment based on frequency
    int grid_spacing = 50;                // Spacing between grid lines in pixels

    // Calculate grid sizes
    float grid_size_ms = (float)grid_spacing / width * total_time * 1000; // Horizontal grid size in ms
    float grid_size_volts = (float)grid_spacing / height * amplitude;     // Vertical grid size in volts

    // Open the SVG file for writing
    FILE *file = fopen(file_name, "w");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo para gravaçao.\n");
        return;
    }

    // SVG header
    fprintf(file, "<svg xmlns='http://www.w3.org/2000/svg' width='%d' height='%d' viewBox='0 0 %d %d'>\n", width, height, width, height);
    fprintf(file, "<rect width='100%%' height='100%%' fill='white' />\n");

    // Draw grid lines
    for (int x = 0; x <= width; x += grid_spacing)
    {
        fprintf(file, "<line x1='%d' y1='0' x2='%d' y2='%d' stroke='lightgray' stroke-width='1' />\n", x, x, height);
    }
    for (int y = 0; y <= height; y += grid_spacing)
    {
        fprintf(file, "<line x1='0' y1='%d' x2='%d' y2='%d' stroke='lightgray' stroke-width='1' />\n", y, width, y);
    }

    // Draw the X-axis (time) and Y-axis (voltage)
    fprintf(file, "<line x1='0' y1='%d' x2='%d' y2='%d' stroke='black' stroke-width='1' />\n", center_axis, width, center_axis); // X-axis
    fprintf(file, "<line x1='%d' y1='0' x2='%d' y2='%d' stroke='black' stroke-width='1' />\n", width / 2, width / 2, height);    // Y-axis

    // Axis labels
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>0ms</text>\n", width / 2 + 5, center_axis - 5);
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>Peak Voltage - %.1fV</text>\n", width / 2 + 5, 15, amplitude); // Display peak voltage as "Peak Voltage - X V"

    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>-%.1fms</text>\n", 10, center_axis - 5, total_time * 500);
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>+%.1fms</text>\n", width - 50, center_axis - 5, total_time * 500);

    // Grid square size labels
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>Grid square size: %0.2fms</text>\n", 10, height - 40, grid_size_ms);
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>Grid square size: %0.2fV</text>\n", 10, height - 20, grid_size_volts);

    // Generate wave points (sine or square wave)
    fprintf(file, "<polyline fill='none' stroke='blue' stroke-width='2' points='");
    for (int x = 0; x < width; x++)
    {
        float current_time = time_step * x - (total_time / 2); // Time relative to center
        float value;

        if (wave_type == 1)
        {
            // Sine wave: standard sine function
            value = amplitude * sin(wave_step * current_time);
        }
        else if (wave_type == 2)
        {
            // Square wave: alternate between +amplitude and -amplitude
            value = (current_time - floor(current_time / period) * period) < (period / 2) ? amplitude : -amplitude;
        }
        else
        {
            printf("Tipo de onda invalido selecionado.\n");
            fclose(file);
            return;
        }

        // Apply the offset
        value += offset;

        int y = center_axis - (int)((value * (height / 2 - 20)) / amplitude); // Map value to SVG height
        fprintf(file, "%d,%d ", x, y);
    }
    fprintf(file, "' />\n");

    // Close the SVG file
    fprintf(file, "</svg>\n");
    fclose(file);

    printf("Arquivo SVG '%s' gerado com sucesso.\n", file_name);
}

// Funçao secundaria para calculos do disjuntor
// Function to calculate breaker rating
int calcular_disjuntor(int fase, double tensao, double potencia, double fp,
                       int tipo_carga, int carga_continua, double *corrente,
                       char *tipo_disjuntor)
{
    // Base current calculation
    if (fase == 1)
    {
        *corrente = potencia / (tensao * fp);
    }
    else
    {
        *corrente = potencia / (sqrt(3) * tensao * fp);
    }

    // Continuous load factor
    if (carga_continua)
    {
        *corrente *= 1.25;
    }

    // Load type adjustment
    switch (tipo_carga)
    {
    case 1: // Resistive load
        *corrente *= 1.0;
        strcpy(tipo_disjuntor, "B-Curve");
        break;
    case 2: // Inductive load
        *corrente *= 1.2;
        strcpy(tipo_disjuntor, "C-Curve");
        break;
    case 3: // Motors
        *corrente *= 2.0;
        strcpy(tipo_disjuntor, "D-Curve");
        break;
    default:
        return -1; // Invalid load type
    }

    // Round current to avoid floating-point precision issues
    *corrente = round(*corrente);

    // Standard breaker sizes
    int tamanhos[] = {1, 2, 3, 4, 6, 10, 13, 16, 20, 25, 32, 40, 50, 63, 80, 100, 125, 160, 200, 250, 300, 400, 500, 600, 630, 800, 1000, 1250, 1500, 1600, 2000, 2500, 3000, 3150, 3200, 4000, 5000, 6000, 6300};
    for (int i = 0; i < sizeof(tamanhos) / sizeof(tamanhos[0]); i++)
    {
        if (tamanhos[i] >= *corrente)
        {
            return tamanhos[i];
        }
    }
    return -2; // Above 6300A
}

// Function to calculate Thevenin equivalent voltage and resistance
// and Norton equivalent current and resistance
void calculate_thevenin_and_norton(float voltages[], int num_sources, float resistances[], int num_resistances,
                                   float *Vth, float *Rth, float *In, float *Rn)
{
    // Calculate Thevenin equivalent voltage (Vth)
    *Vth = 0;
    for (int i = 0; i < num_sources; i++)
    {
        *Vth += voltages[i]; // Summing up all voltages (simple case for linear voltage sources)
    }

    // Calculate Thevenin equivalent resistance (Rth)
    *Rth = 0;
    for (int i = 0; i < num_resistances; i++)
    {
        *Rth += resistances[i]; // Summing up all resistances (assuming series resistances)
    }

    // Calculate Norton current (In)
    *In = *Vth / *Rth; // Norton current In = Vth / Rth

    // Norton resistance is the same as Thevenin resistance
    *Rn = *Rth;
}

// Main function
int main()
{
    int opcaoPortugues;

    do
    {
        // Menu em Portugues
        printf("\n-------- MENU IN ENGLISH -------- \n");
        printf("1- Calcular resistencias em serie ou paralelo \n");
        printf("2- Descobrir valores de resistencia atraves do codigo de cores \n");
        printf("3- Descubra o codigo de cor atraves de resistencias \n");
        printf("4- Lei de Malhas e Nos\n");
        printf("5- Teorema de Norton e Thevenin\n");
        printf("6- Gerador de ondas \n");
        printf("7- Dimensionamento do disjuntor\n");
        printf("8- Dimensionamento da seçao do cabo eletrico\n");
        printf("0- Sair\n");
        printf("Escolha uma opçao: ");
        scanf("%d", &opcaoPortugues);

        switch (opcaoPortugues)
        {
        case 1:
        {
            int n, CalculoR;
            printf("----- Calculadora de resistencia -----\n");
            printf("1- Calculo de resistencias em serie\n");
            printf("2- Calculo de resistencias paralelas\n");
            printf("Escolha uma opçao: ");
            scanf("%d", &CalculoR);

            printf("Quantas resistencias voce quer calcular? ");
            scanf("%d", &n);

            if (n <= 0)
            {
                printf("O numero de resistencias deve ser maior que zero.\n");
                break;
            }

            float resistencia[n];
            for (int i = 0; i < n; i++)
            {
                printf("Introduza o valor da resistencia %d (em ohms): ", i + 1);
                scanf("%f", &resistencia[i]);
            }

            switch (CalculoR)
            {
            case 1:
                printf("----- Calculo em serie ----- \n");
                float resultadoSerie = RSerie(resistencia, n);
                printf("O valor total da resistencia de serie e: %.2f ohms\n", resultadoSerie);
                break;
            case 2:
                printf("----- Calculo em paralelo -----\n");
                float resultadoParalelo = RParalelo(resistencia, n);
                if (resultadoParalelo != 0)
                    printf("O valor total da resistencia paralela e: %.2f ohms\n", resultadoParalelo);
                else
                    printf("Unable to calculate resistance in parallel due to an invalid value.\n");
                break;
            default:
                printf("Opçao invalida.\n");
                break;
            }
            break;
        }

        case 2:
        {
            char cor1[20], cor2[20], cor3[20];
            printf("----- Codigo de cores ----- \n");
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
            printf("Introduza o valor da resistencia (em ohms): ");
            scanf("%ld", &resistencia);

            if (obterCodigoCores(resistencia, codigo) == 0)
            {
                printf("Codigo de cores: %s, %s, %s\n", codigo[0], codigo[1], codigo[2]);
            }
            else
            {
                printf("Entrada invalida\n");
            }

            break;
        }

        case 4:
        {
            printf("----- Lei de Malhas e Nos -----\n");
            int tipo;
            printf("Escolha: 1 para a Lei das Malhas, 2 para a Lei dos Nos: ");
            scanf("%d", &tipo);

            if (tipo == 1)
                leiDasMalhas();
            else if (tipo == 2)
                leiDosNos();
            else
                printf("Opçao invalida.\n");

            break;
        }

        case 5:
        {
            int num_sources, num_resistances;

            // Input the number of voltage sources and their values
            printf("Introduza o numero de fontes de tensao: ");
            scanf("%d", &num_sources);
            float voltages[num_sources];
            printf("Introduza as tensoes das fontes (em volts):\n");
            for (int i = 0; i < num_sources; i++)
            {
                do
                {
                    printf("Tensao da fonte %d: ", i + 1);
                    scanf("%f", &voltages[i]);
                    if (voltages[i] <= 0)
                    {
                        printf("Por favor, insira um valor de tensao positivo.\n");
                    }
                } while (voltages[i] <= 0);
            }

            // Input the number of resistances and their values
            printf("Digite o numero de resistencias: ");
            scanf("%d", &num_resistances);
            float resistances[num_resistances];
            printf("Digite as resistencias (em ohms):\n");
            for (int i = 0; i < num_resistances; i++)
            {
                do
                {
                    printf("Resistencia %d: ", i + 1);
                    scanf("%f", &resistances[i]);
                    if (resistances[i] <= 0)
                    {
                        printf("Por favor, insira um valor de resistencia positivo.\n");
                    }
                } while (resistances[i] <= 0);
            }

            // Variables to hold the results of Thevenin and Norton calculations
            float Vth, Rth, In, Rn;

            // Call the function that calculates both Thevenin and Norton values
            calculate_thevenin_and_norton(voltages, num_sources, resistances, num_resistances, &Vth, &Rth, &In, &Rn);

            // Display the results
            printf("\n---------------------------------------------");
            printf("\nTensao equivalente de Thevenin (Vth): %.2f V\n", Vth);
            printf("Resistencia equivalente de Thevenin (Rth): %.2f Ohms\n", Rth);
            printf("\nCorrente equivalente de Norton (In): %.2f A\n", In);
            printf("Resistencia equivalente de Norton (Rn): %.2f Ohms\n", Rn);
            printf("---------------------------------------------\n");
            break;
        }

        case 6:
        {
            float frequency, amplitude, offset;
            int wave_type;
            char file_name[100];

            printf("Bem-vindo ao simulador de osciloscopio!\n");

            // Get frequency and amplitude from the user
            printf("Digite a frequencia da onda (Hz, por exemplo, 50): ");
            scanf("%f", &frequency);
            if (frequency <= 0)
            {
                printf("Erro: A frequencia deve ser um valor positivo nao zero.\n");
                return 1;
            }

            printf("Digite a amplitude da onda (volts, por exemplo, 5.0): ");
            scanf("%f", &amplitude);
            if (amplitude <= 0)
            {
                printf("Erro: A amplitude deve ser um valor positivo nao zero.\n");
                return 1;
            }

            // Get offset from the user
            printf("Digite o deslocamento da onda (Volts, por exemplo, 1.0): ");
            scanf("%f", &offset);

            // Get wave type from the user
            printf("Escolha o tipo de onda (1 para Onda senoidal, 2 para Onda Quadrada): ");
            scanf("%d", &wave_type);

            // Get file name from the user
            printf("Insira o nome do arquivo de saida (por exemplo, wave.svg):");
            scanf("%s", file_name);

            // Generate the appropriate wave
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

            // User input
            printf("Calculadora de dimensionamento de disjuntores\n");
            printf("---------------------------------\n");

            printf("1. Tipo de sistema (1-Monofasico / 3-Trifasico):");
            scanf("%d", &fase);

            printf("2. Tensao nominal (V): ");
            scanf("%lf", &tensao);

            printf("3. Potencia ativa total (kW): ");
            scanf("%lf", &potencia);
            potencia *= 1000; // Convert to Watts

            printf("4. Fator de potencia (0,1-1,0): ");
            scanf("%lf", &fp);

            printf("\nTipo de carga predominante:\n");
            printf("1. Resistivo (iluminaçao, aquecimento)\n");
            printf("2. Indutivo (motores, transformadores)\n");
            printf("3. Motores grandes\n");
            printf("Selecione (1-3): ");
            scanf("%d", &tipo_carga);

            printf("\nOperaçao continua (>3h)? (s/n): ");
            getchar(); // Clear buffer before reading a character
            scanf("%c", &operacao_continua);
            int carga_continua = (operacao_continua == 's' || operacao_continua == 'S');

            // Input validation
            if (fase != 1 && fase != 3)
            {
                printf("\nErro: Tipo de sistema invalido!\n");
                return 1;
            }

            if (fp < 0.1 || fp > 1.0)
            {
                printf("\nErro: Fator de potencia fora da faixa!\n");
                return 1;
            }

            if (tipo_carga < 1 || tipo_carga > 3)
            {
                printf("\nErro: seleçao de tipo de carregamento invalida!\n");
                return 1;
            }

            // Calculation
            double corrente;
            char tipo_disjuntor[20];
            int disjuntor = calcular_disjuntor(fase, tensao, potencia, fp, tipo_carga, carga_continua, &corrente, tipo_disjuntor);

            // Display results
            printf("\n=== Resultados de dimensionamento===\n");
            printf("%-20s: %.2fA\n", "Corrente calculada", corrente);

            if (disjuntor == -1)
            {
                printf("Erro: tipo de carga invalido nos calculos\n");
                return 1;
            }
            else if (disjuntor == -2)
            {
                printf("Disjuntor recomendado: >200A (consultar especialista)\n");
            }
            else
            {
                printf("%-20s: %dA\n", "Disjuntor recomendado", disjuntor);
            }

            printf("%-20s: %s\n", "Tipo de disjuntor", tipo_disjuntor);

            // Final notes
            printf("\nObservaçoes importantes:\n");
            printf("- Verifique sempre as regulamentaçoes eletricas locais\n");
            printf("- Considere a temperatura ambiente e o dimensionamento do cabo\n");
            printf("- Para motores, verifique os requisitos de corrente de partida\n");
            printf("- Consultar um eletricista qualificado para instalaçoes criticas\n");

            break;
        }

        case 8:
        {
            printf("----- Calculadora de dimensionamento de cabos (IEC 60228) -----\n\n");

            double corrente, comprimento, tensao, queda_tensao_permitida;
            int material;

            // User input with validation
            printf("Digite a corrente (A): ");
            if (scanf("%lf", &corrente) != 1 || corrente <= 0)
            {
                printf("Entrada invalida. O atual deve ser um numero positivo.\n");
                return 1;
            }

            printf("Insira o comprimento do cabo (m): ");
            if (scanf("%lf", &comprimento) != 1 || comprimento <= 0)
            {
                printf("Entrada invalida. O comprimento deve ser um numero positivo.\n");
                return 1;
            }

            printf("Insira a tensao do sistema (V): ");
            if (scanf("%lf", &tensao) != 1 || tensao <= 0)
            {
                printf("Entrada invalida. A tensao deve ser um numero positivo\n");
                return 1;
            }

            printf("Insira a queda de tensao permitida (V): ");
            if (scanf("%lf", &queda_tensao_permitida) != 1 || queda_tensao_permitida <= 0 || queda_tensao_permitida > tensao)
            {
                printf("Entrada invalida. A queda de tensao deve ser positiva e menor que a tensao do sistema.\n");
                return 1;
            }

            printf("Selecione o material do condutor (1 para cobre, 2 para aluminio): ");
            if (scanf("%d", &material) != 1 || (material != 1 && material != 2))
            {
                printf("Entrada invalida. Insira 1 para cobre ou 2 para aluminio.\n");
                return 1;
            }

            double tamanho_padrao;
            double tamanho_requerido = calcular_tamanho_cabo(corrente, comprimento, queda_tensao_permitida, material, &tamanho_padrao);

            printf("\nTamanho do cabo necessario calculado: %.2f mm2\n", tamanho_requerido);

            if (tamanho_padrao == tamanhos_padrao[num_tamanhos - 1])
            {
                printf("Aviso: Nenhuma correspondencia exata encontrada. Usando o maior tamanho de cabo padrao disponivel: %.2f mm2\n", tamanho_padrao);
            }
            else
            {
                printf("Tamanho de cabo padrao IEC 60228 recomendado: %.2f mm2\n", tamanho_padrao);
            }

            break;
        }

        default:
            printf("Opçao invalida.\n");
            break;
        }

    } while (opcaoPortugues != 0);

    return 0;
}
