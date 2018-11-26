#include "Text.h"

Text::Text(int fontSz, float xPos, float yPos, const std::string&ft){
    fontSize = fontSz;
    posX     = xPos;
    posY     = yPos;
    fontName = ft;
}

void Text::CreateDisplay(){
    sf::Font inUse;
    inUse.loadFromFile(fontName);
    disp = sf::Text(displayed, inUse, 100);
    disp.setPosition(posX, posY);

}

void Text::UpdateDisplay(){
    displayed = loaded;
}

void Text::UpdateText(const std::string& newText){
    loaded = newText;
}