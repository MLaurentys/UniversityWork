#include "Scene.h"

std::vector<Point> Scene::GenerateSampleData(){
    float spread = 
}

std::tuple<std::vector<Point>, sf::VertexArray> Scene::GenerateBetaSplineScene(){
    std::vector<Point> sample = GenerateSampleData();
    sf::VertexArray curve = GenerateCurve(sample);
    return std::make_tuple(sample, curve);
}