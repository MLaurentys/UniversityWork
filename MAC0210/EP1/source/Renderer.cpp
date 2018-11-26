#include "Renderer.h"

Renderer::Renderer(const std::shared_ptr<sf::RenderWindow>& windw){
    window = windw;
}

Renderer::Renderer(const Renderer& other){
    window = other.window;
}

Renderer::Renderer(){}


void Renderer::drawCurves(const std::vector<Curve>& curves){
    //printf("on render:\n");
    for(int i = 0; i < curves.size(); ++i){
        window->draw(curves[i].curve);
    }
}

void Renderer::drawLine(const sf::Vector2f& p1, const sf::Vector2f& p2){
    sf::VertexArray aux = sf::VertexArray (sf::PrimitiveType::LinesStrip);
    aux.append(p1);
    aux.append(p2);
    window->draw(aux);
}

void Renderer::render(const std::vector<Curve>& curves){
    //window->clear();
    drawCurves(curves);
    window->display();
}