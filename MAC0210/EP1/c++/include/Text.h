#pragma once
#ifndef DIALOGUE_BOX_H
#define DIALOGUE_BOX_H

#include "SFML/Graphics.hpp"
#include <string>

class Text{

private:
    sf::Text disp;
    sf::String displayed;
    
    std::string loaded;
    std::string fontName;

    int fontSize;
    float posX;
    float posY;
    float width;
    float height;

public:
    Text(int fontSz, float xPos, float yPos, const std::string&font);
    void CreateDisplay();
    void UpdateDisplay();
    void UpdateText(const std::string& newText);
};

#endif