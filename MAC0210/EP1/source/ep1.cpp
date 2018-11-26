#include "SFML/Graphics.hpp"
#include "Window.h"

void module1(){
    Window w(1200, 800, "Bezier Curves!");
    w.createWindow();
}

void module2(){
    Curve c (1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f);
    c.printCurve();
}

int main()
{
    srand (time(NULL));

    module1();
    return 0;
}

