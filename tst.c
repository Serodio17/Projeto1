#include <stdio.h>
#include <string.h>
#include <math.h>

#define RESISTIVIDADE_COBRE 0.0172    // Ω·mm²/m
#define RESISTIVIDADE_ALUMINIO 0.0282 // Ω·mm²/m

// Standard cable sizes in mm²
float tamanhos_padrao[] = {0.5, 0.75, 1.5, 2.5, 4, 6, 10, 16, 25, 35, 50, 70, 95, 120, 150, 185, 240, 300, 400, 500, 630, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500};
int num_tamanhos = sizeof(tamanhos_padrao) / sizeof(tamanhos_padrao[0]);

float calcular_tamanho_cabo(float corrente, float comprimento, float queda_tensao_permitida, int material, float *tamanho_padrao)
{
    float resistividade = (material == 1) ? RESISTIVIDADE_COBRE : RESISTIVIDADE_ALUMINIO;
    float tamanho_requerido = (2 * resistividade * comprimento * corrente) / queda_tensao_permitida; // mm²

    // Find the smallest standard cable size that meets the requirement
    for (int i = 0; i < num_tamanhos; i++)
    {
        if (tamanhos_padrao[i] >= tamanho_requerido)
        {
            *tamanho_padrao = tamanhos_padrao[i];
            return tamanho_requerido; // Return the exact calculated size
        }
    }

    *tamanho_padrao = -1; // Indicate that no suitable size was found
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
// Função para obter o valor da cor
int obterValorCor(char cor[])
{
    // Convert input color to lowercase for case-insensitive comparison
    toLowercase(cor);

    if (strcmp(cor, "black") == 0)
        return 0;
    if (strcmp(cor, "brown") == 0)
        return 1;
    if (strcmp(cor, "red") == 0)
        return 2;
    if (strcmp(cor, "orange") == 0)
        return 3;
    if (strcmp(cor, "yellow") == 0)
        return 4;
    if (strcmp(cor, "green") == 0)
        return 5;
    if (strcmp(cor, "blue") == 0)
        return 6;
    if (strcmp(cor, "purple") == 0)
        return 7;
    if (strcmp(cor, "grey") == 0)
        return 8;
    if (strcmp(cor, "white") == 0)
        return 9;

    return -1; // Invalid color
}

// Função para obter o código de cores de uma resistência
void obterCodigoCores(long resistencia, char codigo[4][20])
{
    if (resistencia <= 0)
    {
        printf("Invalid resistance value. It must be positive.\n");
        return;
    }

    int num_digits = log10(resistencia) + 1;

    if (num_digits < 2 || num_digits > 6)
    {
        printf("Resistance value out of range for standard color codes.\n");
        return;
    }

    // Extract first and second digits
    int digito1 = (resistencia / (long)pow(10, num_digits - 1)) % 10; // First digit
    int digito2 = (resistencia / (long)pow(10, num_digits - 2)) % 10; // Second digit

    // Multiplier
    int multiplicador = num_digits - 2;

    char cores[10][20] = {
        "black", "brown", "red", "orange",
        "yellow", "green", "blue", "purple",
        "grey", "white"};

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
            printf("The resistance %d cannot be zero in parallel.\n", i + 1);
            return 0; // Retorna 0 para indicar erro
        }
    }
    return (resultado == 0.0) ? 0.0 : 1.0 / resultado;
}

// Função para resolver a Lei das Malhas
void leiDasMalhas()
{
    int n;
    printf("Enter the number of meshes in the circuit: ");
    scanf("%d", &n);

    if (n <= 0)
    {
        printf("The number of meshes must be greater than zero.\n");
        return;
    }

    float resistencias[n], tensoes[n], correntes[n];
    for (int i = 0; i < n; i++)
    {
        printf("Enter the equivalent resistance of the mesh %d (in ohms): ", i + 1);
        scanf("%f", &resistencias[i]);
        printf("Enter the total voltage in the mesh %d (in volts): ", i + 1);
        scanf("%f", &tensoes[i]);
    }

    printf("\n--- Results of the Law of Meshes ---\n");
    for (int i = 0; i < n; i++)
    {
        correntes[i] = tensoes[i] / resistencias[i];
        printf("Current in the mesh %d: %.2f A\n", i + 1, correntes[i]);
    }
}

// Função para resolver a Lei dos Nós
void leiDosNos()
{
    int entradas, saidas;
    float correnteEntrada = 0, correnteSaida = 0;

    printf("Enter the number of input currents in the node: ");
    scanf("%d", &entradas);
    for (int i = 0; i < entradas; i++)
    {
        float corrente;
        printf("Enter the input current %d (in A): ", i + 1);
        scanf("%f", &corrente);
        correnteEntrada += corrente;
    }

    printf("Enter the number of output currents from the node: ");
    scanf("%d", &saidas);
    for (int i = 0; i < saidas; i++)
    {
        float corrente;
        printf("Enter the output current %d (in A): ", i + 1);
        scanf("%f", &corrente);
        correnteSaida += corrente;
    }

    printf("\n--- Results of the Law of Nodes ---\n");
    if (fabs(correnteEntrada - correnteSaida) < 1e-6)
    {
        printf("The node is balanced. Input and output currents are equal.\n");
    }
    else
    {
        printf("The node is NOT balanced. Current difference: %.2f A\n", correnteEntrada - correnteSaida);
    }
}

// Desenhaar Onda sinosoidal
#define PI 3.14159265358979323846

void generate_wave_svg(float frequency, float amplitude, const char *file_name, int wave_type)
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
        printf("Error opening the file for writing.\n");
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

        int y = center_axis - (int)((value * (height / 2 - 20)) / amplitude); // Map value to SVG height
        fprintf(file, "%d,%d ", x, y);
    }
    fprintf(file, "' />\n");

    // Close the SVG file
    fprintf(file, "</svg>\n");
    fclose(file);

    printf("SVG file '%s' successfully generated.\n", file_name);
}

// Função secundária para cálculos do disjuntor
int calcular_disjuntor(int fase, double tensao, double potencia, double fp,
                       int tipo_carga, int carga_continua, double *corrente,
                       char *tipo_disjuntor)
{
    // Cálculo da corrente base
    if (fase == 1)
    {
        *corrente = potencia / (tensao * fp);
    }
    else
    {
        *corrente = potencia / (sqrt(3) * tensao * fp);
    }

    // Aplicar fator de carga contínua
    if (carga_continua)
    {
        *corrente *= 1.25;
    }

    // Aplicar fator de tipo de carga
    switch (tipo_carga)
    {
    case 1: // Carga resistiva
        *corrente *= 1.0;
        strcpy(tipo_disjuntor, "B-Curve");
        break;
    case 2: // Carga indutiva
        *corrente *= 1.2;
        strcpy(tipo_disjuntor, "C-Curve");
        break;
    case 3: // Motores
        *corrente *= 2.0;
        strcpy(tipo_disjuntor, "D-Curve");
        break;
    default:
        return -1; // Tipo de carga inválido
    }

    // Encontrar disjuntor padrão
    int tamanhos[] = {15, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100, 125, 150, 175, 200, 225};
    for (int i = 0; i < sizeof(tamanhos) / sizeof(tamanhos[0]); i++)
    {
        if (tamanhos[i] >= *corrente)
        {
            return tamanhos[i];
        }
    }
    return -2; // Acima de 200A
}

// Main function
int main()
{
    int opcaoPortugues;

    do
    {
        // Menu em Português
        printf("-------- MENU IN ENGLISH -------- \n");
        printf("1- Calculate resistances in series or parallel \n");
        printf("2- Find out resistance values through color code \n");
        printf("3- Discover color code through resistors \n");
        printf("4- Law of Meshes and Nodes\n");
        printf("5- Norton and Thevenin theorem\n");
        printf("6- Wave generator \n");
        printf("7- Dimensioning circuit breaker\n");
        printf("8- Dimensioning electrical cable section\n");
        printf("0- Exit\n");
        printf("Choose an option: ");
        scanf("%d", &opcaoPortugues);

        switch (opcaoPortugues)
        {
        case 1:
        {
            int n, CalculoR;
            printf("----- Resistance Calculator -----\n");
            printf("1- Calculation of Series Resistances\n");
            printf("2- Calculation of Parallel Resistances\n");
            printf("Choose an option: ");
            scanf("%d", &CalculoR);

            printf("How many resistances do you want to calculate? ");
            scanf("%d", &n);

            if (n <= 0)
            {
                printf("The number of resistors must be greater than zero.\n");
                break;
            }

            float resistencia[n];
            for (int i = 0; i < n; i++)
            {
                printf("Enter the resistance value %d (in ohms): ", i + 1);
                scanf("%f", &resistencia[i]);
            }

            switch (CalculoR)
            {
            case 1:
                printf("----- Serial calculation ----- \n");
                float resultadoSerie = RSerie(resistencia, n);
                printf("The total value of the series resistance is: %.2f ohms\n", resultadoSerie);
                break;
            case 2:
                printf("----- Parallel Calculation -----\n");
                float resultadoParalelo = RParalelo(resistencia, n);
                if (resultadoParalelo != 0)
                    printf("The total value of parallel resistance is: %.2f ohms\n", resultadoParalelo);
                else
                    printf("Unable to calculate resistance in parallel due to an invalid value.\n");
                break;
            default:
                printf("Invalid option.\n");
                break;
            }
            break;
        }

        case 2:
        {
            char cor1[20], cor2[20], cor3[20];
            printf("----- Color code ----- \n");
            printf("Enter the first color: ");
            scanf("%s", cor1);
            printf("Enter the second color: ");
            scanf("%s", cor2);
            printf("Enter the third color (multiplier): ");
            scanf("%s", cor3);

            int valor1 = obterValorCor(cor1);
            int valor2 = obterValorCor(cor2);
            int multiplicador = obterValorCor(cor3);

            if (valor1 == -1 || valor2 == -1 || multiplicador == -1)
            {
                printf("Invalid color. Try again.\n");
                break;
            }

            long resistencia = (valor1 * 10 + valor2) * pow(10, multiplicador);
            printf("The resistance value is: %ld ohms\n", resistencia);
            break;
        }

        case 3:
        {
            long resistencia;
            char codigo[4][20];

            printf("----- Discover the color code ----- \n");
            printf("Enter the resistance value (in ohms): ");
            scanf("%ld", &resistencia);

            if (resistencia < 0)
            {
                printf("Invalid resistance. Try again.\n");
                break;
            }

            obterCodigoCores(resistencia, codigo);
            printf("Color code: %s, %s, %s\n", codigo[0], codigo[1], codigo[2]);
            break;
        }

        case 4:
        {
            printf("----- Law of Meshes and Nodes -----\n");
            int tipo;
            printf("Choice: 1 for Law of the Meshes, 2 for Law of the Nodes: ");
            scanf("%d", &tipo);
            if (tipo == 1)
                leiDasMalhas();
            else if (tipo == 2)
                leiDosNos();
            else
                printf("Invalid option.\n");
            break;
        }

        case 5:
        {
            printf("----- Norton and Thevenin theorem -----\n");
            double tnR1, tnR2, tnVth, tnRth, tnIn;

            // Entrada de valores
            printf("Enter the value of R1 (ohms): ");
            scanf("%lf", &tnR1);

            printf("Enter the value of R2 (ohms): ");
            scanf("%lf", &tnR2);

            printf("Enter the value of the Thevenin voltage (V): ");
            scanf("%lf", &tnVth);

            // Calculos
            tnRth = (tnR1 * tnR2) / (tnR1 + tnR2);
            tnIn = tnVth / tnRth;

            // Exibir resultado
            printf("\nNorton Resistance : %.2f ohms\n", tnRth);
            printf("Norton current: %.2f A\n", tnIn);
            break;
        }

        case 6:
        {
            float frequency, amplitude;
            int wave_type;

            printf("Welcome to the oscilloscope simulator!\n");
            printf("Enter the frequency of the wave (Hz, e.g., 50): ");
            scanf("%f", &frequency);
            printf("Enter the amplitude of the wave (Volts, e.g., 5.0): ");
            scanf("%f", &amplitude);

            printf("Choose the wave type (1 for Sine Wave, 2 for Square Wave): ");
            scanf("%d", &wave_type);

            if (wave_type == 1)
            {
                generate_wave_svg(frequency, amplitude, "sine_wave.svg", 1);
            }
            else if (wave_type == 2)
            {
                generate_wave_svg(frequency, amplitude, "square_wave.svg", 2);
            }
            else
            {
                printf("Invalid choice for wave type.\n");
                return 1;
            }

            printf("Open the file '%s' in your browser to view the wave.\n", wave_type == 1 ? "sine_wave.svg" : "square_wave.svg");

            break;
        }

        case 7:
        {
            int fase, tipo_carga;
            double tensao, potencia, fp;
            char operacao_continua;

            // Coleta de entradas
            printf("Circuit Breaker Sizing Calculator\n");
            printf("---------------------------------\n");

            printf("1. System type (1-Single phase / 3-Three phase): ");
            scanf("%d", &fase);

            printf("2. Nominal voltage (V): ");
            scanf("%lf", &tensao);

            printf("3. Total active power (kW): ");
            scanf("%lf", &potencia);
            potencia *= 1000; // Converter para Watts

            printf("4. Power factor (0.1-1.0): ");
            scanf("%lf", &fp);

            printf("\nPredominant load type:\n");
            printf("1. Resistive (lighting, heating)\n");
            printf("2. Inductive (motors, transformers)\n");
            printf("3. Large motors\n");
            printf("Select (1-3): ");
            scanf("%d", &tipo_carga);

            printf("\nContinuous operation (>3h)? (y/n): ");
            scanf(" %c", &operacao_continua);
            int carga_continua = (operacao_continua == 'y' || operacao_continua == 'Y');

            // Validações de entrada
            if (fase != 1 && fase != 3)
            {
                printf("\nError: Invalid system type!\n");
                return 1;
            }

            if (fp < 0.1 || fp > 1.0)
            {
                printf("\nError: Power factor out of range!\n");
                return 1;
            }

            if (tipo_carga < 1 || tipo_carga > 3)
            {
                printf("\nError: Invalid load type selection!\n");
                return 1;
            }

            // Executar cálculos
            double corrente;
            char tipo_disjuntor[20];
            int disjuntor = calcular_disjuntor(fase, tensao, potencia, fp, tipo_carga,
                                               carga_continua, &corrente, tipo_disjuntor);

            // Exibir resultados
            printf("\n=== Sizing Results ===\n");
            printf("Calculated current: %.2fA\n", corrente);

            if (disjuntor == -1)
            {
                printf("Error: Invalid load type in calculations\n");
                return 1;
            }
            else if (disjuntor == -2)
            {
                printf("Recommended breaker: >200A (consult specialist)\n");
            }
            else
            {
                printf("Recommended breaker: %dA\n", disjuntor);
            }

            printf("Breaker type: %s\n", tipo_disjuntor);

            // Notas finais
            printf("\nImportant notes:\n");
            printf("- Always verify local electrical regulations\n");
            printf("- Consider ambient temperature and cable sizing\n");
            printf("- For motors, verify starting current requirements\n");
            printf("- Consult a qualified electrician for critical installations\n");

            break;
        }

        case 8:
        {
            printf("----- Cable Sizing -----\n");
            float corrente, comprimento, tensao, queda_tensao_permitida;
            int material;

            // User input
            printf("Enter current (A): ");
            scanf("%f", &corrente);
            printf("Enter cable length (m): ");
            scanf("%f", &comprimento);
            printf("Enter system voltage (V): ");
            scanf("%f", &tensao);
            printf("Enter allowable voltage drop (V): ");
            scanf("%f", &queda_tensao_permitida);
            printf("Select conductor material (1 for Copper, 2 for Aluminum): ");
            scanf("%d", &material);

            float tamanho_padrao;
            float tamanho_requerido = calcular_tamanho_cabo(corrente, comprimento, queda_tensao_permitida, material, &tamanho_padrao);

            printf("\nCalculated required cable size: %.2f mm2\n", tamanho_requerido);

            if (tamanho_padrao == -1)
            {
                printf("No suitable IEC 60228 standard cable size found.\n");
            }
            else
            {
                printf("Recommended IEC 60228 standard cable size: %.2f mm2\n", tamanho_padrao);
            }

            break;
        }

        default:
            printf("Invalid option.\n");
            break;
        }

    } while (opcaoPortugues != 0);

    return 0;
}
