#include "Window.h"

void Window::RunBetaSpline(){
    renderer.ClearWindow();
    std::vector<Point> samplePoints;
    sf::VertexArray graph;
    std::tie(samplePoints, graph) = scene.GenerateBetaSplineScene();
    renderer.DrawPoints(samplePoints);
    renderer.DrawCurve(graph);
}