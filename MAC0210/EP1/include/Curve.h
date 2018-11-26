#pragma once
#ifndef CURVE_H
#define CURVE_H

#include <memory>

#include "Text.h"

const sf::Vector2f TEXTURE_POINT (100.0f, 100.0f);
const float CURVE_PRECISION = 0.01f;

class Curve{
private:
    //Text equationLabel;
    sf::Color color;
public:
    Curve(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy);
    sf::Vertex a;
    sf::Vertex b;
    sf::Vertex c;
    sf::Vertex d;
    sf::VertexArray curve;
    void calculateCurve();
    void changeColor(sf::Color c);
    void printCurve() const;
};

#endif