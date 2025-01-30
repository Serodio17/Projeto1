#include <stdio.h>
#include <math.h>

int main() {
    int option;
    
    do {
        menu();
        printf("Choose an option (0 to Exit): ");
        if (scanf("%d", &option) != 1) {  // Input validation
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');  // Clear buffer
            continue;
        }

        switch (option) {
            case 1: calculate_resistances(); break;
            //case 2: color_code_to_resistance(); break;
            //case 3: resistance_to_color_code(); break;
            //case 4: mesh_and_node_laws(); break;
            //case 5: norton_thevenin(); break;
            //case 6: wave_generator(); break;
            //case 7: circuit_breaker_sizing(); break;
            //case 8: cable_sizing(); break;
            //case 0: printf("Exiting program. Goodbye!\n"); break;
            default: printf("Invalid option. Please try again.\n");
        }
    } while (option != 0);

    return 0;
}

void menu() {
    printf("\n-------- ELECTRICAL ENGINEERING CALCULATOR --------\n");
    printf("1 - Calculate Resistances (Series/Parallel)\n");
    printf("2 - Find Resistance Values (Color Code)\n");
    printf("3 - Find Color Code (From Resistance Value)\n");
    printf("4 - Mesh and Node Laws\n");
    printf("5 - Norton and Thevenin Theorem\n");
    printf("6 - Wave Generator\n");
    printf("7 - Circuit Breaker Sizing\n");
    printf("8 - Cable Sizing\n");
    printf("0 - Exit\n");
    printf("-----------------------------------------------\n");
}

void calculate_resistances() {
    int n, option;
    printf("----- Resistance Calculator -----\n");
    printf("1- Series Resistance Calculation\n");
    printf("2- Parallel Resistance Calculation\n");
    printf("Choose an option: ");
    scanf("%d", &option);

    printf("How many resistances do you want to calculate? ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("The number of resistors must be greater than zero.\n");
        return;
    }

    float resistances[n];
    for (int i = 0; i < n; i++) {
        printf("Enter resistance value %d (in ohms): ", i + 1);
        scanf("%f", &resistances[i]);
    }

    if (option == 1) {
        float total = 0;
        for (int i = 0; i < n; i++) {
            total += resistances[i];
        }
        printf("Total Series Resistance: %.2f ohms\n", total);
    } else if (option == 2) {
        float reciprocal = 0;
        for (int i = 0; i < n; i++) {
            if (resistances[i] == 0) {
                printf("Cannot calculate parallel resistance with a zero-ohm resistor.\n");
                return;
            }
            reciprocal += 1 / resistances[i];
        }
        printf("Total Parallel Resistance: %.2f ohms\n", 1 / reciprocal);
    } else {
        printf("Invalid option.\n");
    }
}

//void color_code_to_resistance();
//void resistance_to_color_code();
//void norton_thevenin();
//void wave_generator();
//void circuit_breaker_sizing();
//void cable_sizing();