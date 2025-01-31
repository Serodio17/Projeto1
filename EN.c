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
int obterCodigoCores(long resistencia, char codigo[3][20])
{
    if (resistencia <= 0)
    {
        printf("Invalid resistance value. It must be positive.\n");
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
        printf("Resistance value out of range (10-999999 ohms).\n");
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
        "black", "brown", "red", "orange", "yellow",
        "green", "blue", "violet", "grey", "white"};

    strcpy(codigo[0], cores_digitos[digito1]);
    strcpy(codigo[1], cores_digitos[digito2]);
    strcpy(codigo[2], cores_digitos[num_digits - 2]);

    return 0;
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

    float *resistencias = (float *)malloc(n * sizeof(float));
    float *tensoes = (float *)malloc(n * sizeof(float));
    float *correntes = (float *)malloc(n * sizeof(float));

    if (resistencias == NULL || tensoes == NULL || correntes == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        printf("Enter the equivalent resistance of the mesh %d (in ohms): ", i + 1);
        scanf("%f", &resistencias[i]);
        if (resistencias[i] <= 0)
        {
            printf("Resistance must be greater than zero.\n");
            free(resistencias);
            free(tensoes);
            free(correntes);
            return;
        }

        printf("Enter the total voltage in the mesh %d (in volts): ", i + 1);
        scanf("%f", &tensoes[i]);
        if (tensoes[i] < 0)
        {
            printf("Voltage cannot be negative.\n");
            free(resistencias);
            free(tensoes);
            free(correntes);
            return;
        }
    }

    printf("\n--- Results of the Law of Meshes ---\n");
    for (int i = 0; i < n; i++)
    {
        correntes[i] = tensoes[i] / resistencias[i];
        printf("Current in the mesh %d: %.2f A\n", i + 1, correntes[i]);
    }

    free(resistencias);
    free(tensoes);
    free(correntes);
}

// Função para resolver a Lei dos Nós
void leiDosNos()
{
    int entradas, saidas;
    float correnteEntrada = 0, correnteSaida = 0;

    printf("Enter the number of input currents in the node: ");
    scanf("%d", &entradas);
    if (entradas < 0)
    {
        printf("Number of input currents cannot be negative.\n");
        return;
    }

    for (int i = 0; i < entradas; i++)
    {
        float corrente;
        printf("Enter the input current %d (in A): ", i + 1);
        scanf("%f", &corrente);
        if (corrente < 0)
        {
            printf("Current cannot be negative.\n");
            return;
        }
        correnteEntrada += corrente;
    }

    printf("Enter the number of output currents from the node: ");
    scanf("%d", &saidas);
    if (saidas < 0)
    {
        printf("Number of output currents cannot be negative.\n");
        return;
    }

    for (int i = 0; i < saidas; i++)
    {
        float corrente;
        printf("Enter the output current %d (in A): ", i + 1);
        scanf("%f", &corrente);
        if (corrente < 0)
        {
            printf("Current cannot be negative.\n");
            return;
        }
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
        else
        {
            printf("Invalid wave type selected.\n");
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

    printf("SVG file '%s' successfully generated.\n", file_name);
}

// Função secundária para cálculos do disjuntor
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
    return -2; // Above 200A
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
        // Menu em Português
        printf("\n-------- MENU IN ENGLISH -------- \n");
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
            char codigo[3][20];

            printf("----- Discover the color code ----- \n");
            printf("Enter the resistance value (in ohms): ");
            scanf("%ld", &resistencia);

            if (obterCodigoCores(resistencia, codigo) == 0)
            {
                printf("Color code: %s, %s, %s\n", codigo[0], codigo[1], codigo[2]);
            }
            else
            {
                printf("Invalid input\n");
            }

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
            int num_sources, num_resistances;

            // Input the number of voltage sources and their values
            printf("Enter the number of voltage sources: ");
            scanf("%d", &num_sources);
            float voltages[num_sources];
            printf("Enter the voltages of the sources (in volts):\n");
            for (int i = 0; i < num_sources; i++)
            {
                do
                {
                    printf("Voltage of source %d: ", i + 1);
                    scanf("%f", &voltages[i]);
                    if (voltages[i] <= 0)
                    {
                        printf("Please enter a positive voltage value.\n");
                    }
                } while (voltages[i] <= 0);
            }

            // Input the number of resistances and their values
            printf("Enter the number of resistances: ");
            scanf("%d", &num_resistances);
            float resistances[num_resistances];
            printf("Enter the resistances (in ohms):\n");
            for (int i = 0; i < num_resistances; i++)
            {
                do
                {
                    printf("Resistance %d: ", i + 1);
                    scanf("%f", &resistances[i]);
                    if (resistances[i] <= 0)
                    {
                        printf("Please enter a positive resistance value.\n");
                    }
                } while (resistances[i] <= 0);
            }

            // Variables to hold the results of Thevenin and Norton calculations
            float Vth, Rth, In, Rn;

            // Call the function that calculates both Thevenin and Norton values
            calculate_thevenin_and_norton(voltages, num_sources, resistances, num_resistances, &Vth, &Rth, &In, &Rn);

            // Display the results
            printf("\n---------------------------------------------");
            printf("\nThevenin Equivalent Voltage (Vth): %.2f V\n", Vth);
            printf("Thevenin Equivalent Resistance (Rth): %.2f Ohms\n", Rth);
            printf("\nNorton Equivalent Current (In): %.2f A\n", In);
            printf("Norton Equivalent Resistance (Rn): %.2f Ohms\n", Rn);
            printf("---------------------------------------------\n");
            break;
        }

        case 6:
        {
            float frequency, amplitude, offset;
            int wave_type;
            char file_name[100];

            printf("Welcome to the oscilloscope simulator!\n");

            // Get frequency and amplitude from the user
            printf("Enter the frequency of the wave (Hz, e.g., 50): ");
            scanf("%f", &frequency);
            if (frequency <= 0)
            {
                printf("Error: Frequency must be a positive non-zero value.\n");
                return 1;
            }

            printf("Enter the amplitude of the wave (Volts, e.g., 5.0): ");
            scanf("%f", &amplitude);
            if (amplitude <= 0)
            {
                printf("Error: Amplitude must be a positive non-zero value.\n");
                return 1;
            }

            // Get offset from the user
            printf("Enter the offset of the wave (Volts, e.g., 1.0): ");
            scanf("%f", &offset);

            // Get wave type from the user
            printf("Choose the wave type (1 for Sine Wave, 2 for Square Wave): ");
            scanf("%d", &wave_type);

            // Get file name from the user
            printf("Enter the output file name (e.g., wave.svg): ");
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
                printf("Invalid choice for wave type.\n");
                return 1;
            }

            printf("Open the file '%s' in your browser to view the wave.\n", file_name);

            break;
        }

        case 7:
        {
            int fase, tipo_carga;
            double tensao, potencia, fp;
            char operacao_continua;

            // User input
            printf("Circuit Breaker Sizing Calculator\n");
            printf("---------------------------------\n");

            printf("1. System type (1-Single phase / 3-Three phase): ");
            scanf("%d", &fase);

            printf("2. Nominal voltage (V): ");
            scanf("%lf", &tensao);

            printf("3. Total active power (kW): ");
            scanf("%lf", &potencia);
            potencia *= 1000; // Convert to Watts

            printf("4. Power factor (0.1-1.0): ");
            scanf("%lf", &fp);

            printf("\nPredominant load type:\n");
            printf("1. Resistive (lighting, heating)\n");
            printf("2. Inductive (motors, transformers)\n");
            printf("3. Large motors\n");
            printf("Select (1-3): ");
            scanf("%d", &tipo_carga);

            printf("\nContinuous operation (>3h)? (y/n): ");
            getchar(); // Clear buffer before reading a character
            scanf("%c", &operacao_continua);
            int carga_continua = (operacao_continua == 'y' || operacao_continua == 'Y');

            // Input validation
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

            // Calculation
            double corrente;
            char tipo_disjuntor[20];
            int disjuntor = calcular_disjuntor(fase, tensao, potencia, fp, tipo_carga,
                                               carga_continua, &corrente, tipo_disjuntor);

            // Display results
            printf("\n=== Sizing Results ===\n");
            printf("%-20s: %.2fA\n", "Calculated current", corrente);

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
                printf("%-20s: %dA\n", "Recommended breaker", disjuntor);
            }

            printf("%-20s: %s\n", "Breaker type", tipo_disjuntor);

            // Final notes
            printf("\nImportant notes:\n");
            printf("- Always verify local electrical regulations\n");
            printf("- Consider ambient temperature and cable sizing\n");
            printf("- For motors, verify starting current requirements\n");
            printf("- Consult a qualified electrician for critical installations\n");

            break;
        }

        case 8:
        {
            printf("----- Cable Sizing Calculator (IEC 60228) -----\n\n");

            double corrente, comprimento, tensao, queda_tensao_permitida;
            int material;

            // User input with validation
            printf("Enter current (A): ");
            if (scanf("%lf", &corrente) != 1 || corrente <= 0)
            {
                printf("Invalid input. Current must be a positive number.\n");
                return 1;
            }

            printf("Enter cable length (m): ");
            if (scanf("%lf", &comprimento) != 1 || comprimento <= 0)
            {
                printf("Invalid input. Length must be a positive number.\n");
                return 1;
            }

            printf("Enter system voltage (V): ");
            if (scanf("%lf", &tensao) != 1 || tensao <= 0)
            {
                printf("Invalid input. Voltage must be a positive number.\n");
                return 1;
            }

            printf("Enter allowable voltage drop (V): ");
            if (scanf("%lf", &queda_tensao_permitida) != 1 || queda_tensao_permitida <= 0 || queda_tensao_permitida > tensao)
            {
                printf("Invalid input. Voltage drop must be positive and less than system voltage.\n");
                return 1;
            }

            printf("Select conductor material (1 for Copper, 2 for Aluminum): ");
            if (scanf("%d", &material) != 1 || (material != 1 && material != 2))
            {
                printf("Invalid input. Enter 1 for Copper or 2 for Aluminum.\n");
                return 1;
            }

            double tamanho_padrao;
            double tamanho_requerido = calcular_tamanho_cabo(corrente, comprimento, queda_tensao_permitida, material, &tamanho_padrao);

            printf("\nCalculated required cable size: %.2f mm2\n", tamanho_requerido);

            if (tamanho_padrao == tamanhos_padrao[num_tamanhos - 1])
            {
                printf("Warning: No exact match found. Using largest available standard cable size: %.2f mm2\n", tamanho_padrao);
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
