#include "Window.h"

// Window::Window(unsigned wd, unsigned h, const std::string& nm){

//     scene = Scene(wd, h);
// }

void Window::createWindow(){
    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(width, height), "Bezier curves!");
    renderer = Renderer(window);
    scene.updateAllCurves();
    //starts update loop
    update();
}

void Window::update(){
    bool toRender = false;
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if(event.type == sf::Event::Resized){
                toRender = true;
                printf("new wid = %d\nnew hei = %d\n", event.size.width, event.size.height);
            }
            if(event.type == sf::Event::MouseButtonReleased){
                if(event.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2f a = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                    sf::Vector2f b = scene.getClosest(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                    renderer.drawLine(a, b);
                    toRender = true;
                }
            }
        }
        if(toRender){
            auto aux = scene.getData();
            renderer.render(std::get<0>(aux));
            toRender = false;
        }
    }
}