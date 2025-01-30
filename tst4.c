#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int main()
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

    return 0;
}
