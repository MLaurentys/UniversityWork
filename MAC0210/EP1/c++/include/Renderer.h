#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Curve.h"
#include "SFML/Graphics.hpp"

#include <vector>
#include <memory>



class Renderer{
private:
    //copy of Window's (class) window
    std::shared_ptr<sf::RenderWindow> window;
public:
    void drawCurves(const std::vector<Curve>& curves);
    void render(const std::vector<Curve>& curves);
    void drawLine(const sf::Vector2f&, const sf::Vector2f&);
    Renderer(const std::shared_ptr<sf::RenderWindow>& windw);
    Renderer(const Renderer& other);
    Renderer();
};

#endif