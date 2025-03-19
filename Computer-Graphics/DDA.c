#include <math.h>
#include <graphics.h>

int round(float x) {
    return x > 0 ? int(x + 0.5) : int(x - 0.5);
}

int main() {
    float x1, x2, y1, y2, delta_x, delta_y, step, x_inc, y_inc, x, y;
    int gd = DETECT, gm;

    printf("Enter the end point coordinates (x1 y1 x2 y2): ");
    scanf("%f%f%f%f", &x1, &y1, &x2, &y2);

    initgraph(&gd, &gm, (char*)"");  // Adjust path to BGI folder

    // Calculate delta x and delta y
    delta_x = x2 - x1;
    delta_y = y2 - y1;

    // Assigning step value	
    if (abs(delta_x) > abs(delta_y)) {
        step = abs(delta_x);
    } else {
        step = abs(delta_y);
    }

    // Calculate x_inc and y_inc
    x_inc = delta_x / step;
    y_inc = delta_y / step;

    // Setting initial x and y
    x = x1 + (getmaxx()/2);
    y =  (getmaxy()/2) - y1;
    

    // Display pixels
    for (int i = 0; i <= step; i++) {
        putpixel(round(x), round(y), WHITE);
        x += x_inc;
        y -= y_inc;
        delay(100);  // Optional for debugging
    }

    getch();
    closegraph();
    return 0;
}
