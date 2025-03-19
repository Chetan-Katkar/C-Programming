#include <graphics.h>
#include <stdio.h>
#include <conio.h>

int main() {
    int x_min, y_min, x_max, y_max;
    int x1, y1, x2, y2, i;
    float p[4]; // Array to store the values of p[i]
    float q[4]; // Array to store values of q[i]
    float u1s[] = {0, 0, 0, 0}; // Lower bound
    float u2s[] = {1, 1, 1, 1}; // Upper bound
    int delta_x, delta_y;
    float gamma;
    float u1max, u2min;
    float ints_x1, ints_y1, ints_x2, ints_y2;
    int gd = DETECT, gm;

    // Taking inputs
    printf("Enter the min and max coordinates of the window: ");
    scanf("%d%d%d%d", &x_min, &y_min, &x_max, &y_max);

    printf("Enter the coordinates of the line: ");
    scanf("%d%d%d%d", &x1, &y1, &x2, &y2);

    // Initialize graphics
    initgraph(&gd, &gm, (char *)""); // Ensure correct path!

    // Draw clipping window
    rectangle(x_min, y_min, x_max, y_max);

    // Calculate deltas
    delta_x = x2 - x1;
    delta_y = y2 - y1;

    if (delta_x == 0 && delta_y == 0) {
        printf("Line is a single point\n");
        getch();
        closegraph();
        return 0;
    }

    // Compute p[i] and q[i]
    for (i = 0; i < 4; i++) {
        if (i == 0) { // Left boundary
            p[i] = -delta_x;
            q[i] = x1 - x_min;
        } else if (i == 1) { // Right boundary
            p[i] = delta_x;
            q[i] = x_max - x1;
        } else if (i == 2) { // Bottom boundary
            p[i] = -delta_y;
            q[i] = y1 - y_min;
        } else if (i == 3) { // Top boundary
            p[i] = delta_y;
            q[i] = y_max - y1;
        }

        // Compute gamma
        gamma = q[i] / p[i];

        // Update u1s and u2s
        if (p[i] < 0) {
            u1s[i] = gamma;
        } else {
            u2s[i] = gamma;
        }
    }

    // Find u1max and u2min
    u1max = u1s[0];
    u2min = u2s[0];
    for (i = 1; i < 4; i++) {
        if (u1max < u1s[i]) {
            u1max = u1s[i];
        }
        if (u2min > u2s[i]) {
            u2min = u2s[i];
        }
    }

    // Check if line is visible
    if (u1max > u2min) {
        printf("Line is outside the clipping window\n");
    } else {
        // Compute intersection points
        ints_x1 = x1 + (u1max * delta_x);
        ints_y1 = y1 + (u1max * delta_y);
        ints_x2 = x1 + (u2min * delta_x);
        ints_y2 = y1 + (u2min * delta_y);

        // Print for debugging
        printf("Clipped Line: (%.2f, %.2f) to (%.2f, %.2f)\n", ints_x1, ints_y1, ints_x2, ints_y2);

        // Draw clipped line (convert float to int)
        line((int)ints_x1, (int)ints_y1, (int)ints_x2, (int)ints_y2);
    }

    getch();
    closegraph();
    return 0;
}
