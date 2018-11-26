#include "Curve.h"

Curve::Curve(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy){
    curve = sf::VertexArray(sf::PrimitiveType::LinesStrip, 4);
    a = sf::Vertex(sf::Vector2f(ax, ay), sf::Color::Blue);
    b = sf::Vertex(sf::Vector2f(bx, by), sf::Color::Blue);
    c = sf::Vertex(sf::Vector2f(cx, cy), sf::Color::Blue);
    d = sf::Vertex(sf::Vector2f(dx, dy), sf::Color::Blue);
    calculateCurve();
    color = sf::Color::Red;
}

void Curve::calculateCurve(){
    curve = sf::VertexArray (sf::PrimitiveType::LinesStrip);
    for(float t = 0.0f; t <= 1.0f; t += CURVE_PRECISION){
        sf::Vector2f p0 = a.position + t * (b.position - a.position);
        sf::Vector2f p1 = b.position + t * (c.position - b.position);
        sf::Vector2f p2 = c.position + t * (d.position - c.position);
        sf::Vector2f p3 = p0 + t * (p1 - p0);
        sf::Vector2f p4 = p1 + t * (p2 - p1);
        sf::Vector2f p5 = p3 + t * (p4 - p3);
        curve.append(sf::Vertex(p5, sf::Color::Red));
        // printf("p0 = (%f, %f)\n", p0.x, p0.y);
        // printf("p1 = (%f, %f)\n", p1.x, p1.y);
        // printf("p2 = (%f, %f)\n", p2.x, p2.y);
        // printf("p3 = (%f, %f)\n", p3.x, p3.y);
        // printf("FOI %f, com 05 = (%f, %f)\n", t, p5.x, p5.y);
    }
}

void Curve::changeColor(sf::Color c){
    for(int i = 0; i < curve.getVertexCount(); ++i){
        curve[i].color = c;
    }
    color = c;
}

void Curve::printCurve() const{
    printf("x(t) = %f*(1-t)^3 + %f*3*(1-t)^2*t + %f*3*(1-t)*t^2 + %f*t^3\n", a.position.x, b.position.x, c.position.x, d.position.x);
    printf("y(t) = %f*(1-t)^3 + %f*3*(1-t)^2*t + %f*3*(1-t)*t^2 + %f*t^3\n", a.position.y, b.position.y, c.position.y, d.position.y);
}