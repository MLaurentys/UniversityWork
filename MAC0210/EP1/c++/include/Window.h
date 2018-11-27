#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>

#include "SFML/Graphics.hpp"
#include "Renderer.h"
#include "Scene.h"

class Window{
private:
    Renderer renderer;
    Scene scene;
    std::shared_ptr<sf::RenderWindow> window;
    unsigned width;
    unsigned height;
    std::string name;

public:
    Window(unsigned wd, unsigned h, const std::string& nm) : scene(wd, h){
        width = wd;
        height = h;
        name = nm;
    };
    void createWindow();
    void update();
};

#endif