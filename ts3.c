#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int main()
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
    return 0;
}
