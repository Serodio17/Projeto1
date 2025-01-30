#include <stdio.h>
#include <string.h>
#include <math.h>

// Cálculo da seção de cabos
double calcularSeccao(double corrente, double comprimento, double condutividade, double deltaT)
{
    return (corrente * comprimento) / (condutividade * deltaT);
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

void generate_oscilloscope_wave_svg(float frequency, float amplitude, const char *file_name)
{
    int width = 800;         // Fixed width of the SVG
    int height = 400;        // Height of the SVG
    int center_axis = height / 2;
    float period = 1.0 / frequency;   // Calculate the period from the frequency
    float total_time = 2 * period;    // Display 2 full cycles
    float time_step = total_time / width; // Time increment for each pixel
    float wave_step = (2 * PI) / period; // Angular increment based on frequency
    int grid_spacing = 50;    // Spacing between grid lines in pixels

    // Calculate grid sizes
    float grid_size_ms = (float)grid_spacing / width * total_time * 1000;  // Horizontal grid size in ms
    float grid_size_volts = (float)grid_spacing / height * amplitude;  // Vertical grid size in volts

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
    fprintf(file, "<line x1='%d' y1='0' x2='%d' y2='%d' stroke='black' stroke-width='1' />\n", width / 2, width / 2, height);     // Y-axis

    // Axis labels
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>0ms</text>\n", width / 2 + 5, center_axis - 5);
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>Peak Voltage - %.1fV</text>\n", width / 2 + 5, 15, amplitude);  // Display peak voltage as "Peak Voltage - X V"

    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>-%.1fms</text>\n", 10, center_axis - 5, total_time * 500);
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>+%.1fms</text>\n", width - 50, center_axis - 5, total_time * 500);

    // Grid square size labels
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>Grid square size: %0.2fms</text>\n", 10, height - 40, grid_size_ms);
    fprintf(file, "<text x='%d' y='%d' font-size='12' fill='black'>Grid square size: %0.2fV</text>\n", 10, height - 20, grid_size_volts);

    // Generate wave points
    fprintf(file, "<polyline fill='none' stroke='blue' stroke-width='2' points='");
    for (int x = 0; x < width; x++)
    {
        float current_time = time_step * x - (total_time / 2); // Time relative to center
        float value = amplitude * sin(wave_step * current_time); // Voltage value
        int y = center_axis - (int)((value * (height / 2 - 20)) / amplitude); // Map value to SVG height
        fprintf(file, "%d,%d ", x, y);
    }
    fprintf(file, "' />\n");

    // Close the SVG file
    fprintf(file, "</svg>\n");
    fclose(file);

    printf("SVG file '%s' successfully generated.\n", file_name);
}

// Main function
int main()
{
    int opcaoPortugues;

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

        printf("Welcome to the oscilloscope simulator!\n");
        printf("Enter the frequency of the wave (Hz, e.g., 50): ");
        scanf("%f", &frequency);
        printf("Enter the amplitude of the wave (Volts, e.g., 5.0): ");
        scanf("%f", &amplitude);

        generate_oscilloscope_wave_svg(frequency, amplitude, "oscilloscope.svg");

        printf("Open the file 'oscilloscope.svg' in your browser to view the wave.\n");
        break;
    }

    case 7:
    {
        printf("----- Dimensioning circuit breaker -----\n");
        float potencia_total, tensao;
        float corrente_cal;
        float fat_seg;
        int disjuntor_selecionado = -1;

        // Lista padrão de disjuntores comerciais (em amperes)
        int disjuntores_comerciais[] = {2, 4, 6, 10, 16, 20, 25, 32, 40, 50, 63, 80, 100, 125, 160, 200, 250, 300, 400, 630, 800};
        int tamanho_lista = sizeof(disjuntores_comerciais) / sizeof(disjuntores_comerciais[0]);

        // Entrada de dados do usuário
        printf("Enter the total power consumed by the system in watts: ");
        scanf("%f", &potencia_total);

        printf("Enter the system voltage in volts (ex: 127 or 220): ");
        scanf("%f", &tensao);

        printf("Enter the security factor (suggestion: 1.25): ");
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
            printf("\nCalculated current: %.2f A\n", corrente_cal);
            printf("Recommended circuit breaker: %d A\n", disjuntor_selecionado);
        }
        else
        {
            printf("\nCalculated current: %.2f A\n", corrente_cal);
            printf("No suitable circuit breaker found.\n");
        }

        break;
    }

    case 8:
    {
        printf("----- Dimensioning section of cables -----\n");
        double corrente, comprimento, condutividade, deltaT, seccao;

        printf("Enter the electric current (in A): ");
        scanf("%lf", &corrente);

        printf("Enter the cable length (in m): ");
        scanf("%lf", &comprimento);

        printf("Enter the conductivity of the material (in S/m): ");
        scanf("%lf", &condutividade);

        printf("Enter the allowed temperature variation (in C):: ");
        scanf("%lf", &deltaT);

        seccao = calcularSeccao(corrente, comprimento, condutividade, deltaT);

        printf("The required cable section is: %.2f mm2\n", seccao);

        break;
    }

    default:
        printf("Invalid option.\n");
        break;
    }

    return 0;
}
