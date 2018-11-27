#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <tuple>

#include "SFML/Graphics.hpp"

#include "Point.h"

static const int sizeOfSample = 1000; 

class Scene{
private:
    std::vector<Point> GenerateSampleData();
    sf::VertexArray GenerateGraph();
    sf::VertexArray GenerateCurve(const std::vector<Point>& points);
public:
    Scene(float width, float height);
    std::tuple<std::vector<Point>, sf::VertexArray> GenerateBetaSplineScene();

};

#endif