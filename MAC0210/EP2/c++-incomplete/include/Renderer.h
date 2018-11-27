#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "SFML/Graphics.hpp"

#include <vector>
#include <memory>

#include "Point.h"

class Renderer{
private:
    //copy of Window's (class) window
    std::shared_ptr<sf::RenderWindow> window;
public:
    void DrawPoints(const std::vector<Point>& sample);
    void DrawCurve(const sf::VertexArray& curve);
    void ClearWindow();
};

#endif