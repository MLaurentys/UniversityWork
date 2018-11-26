#include "Scene.h"

#include <gsl/gsl_poly.h> //used to solve equation

Scene::Scene(float width, float height){
    selectedIndex = -1;

    //x(t) info
    int hrange = width*0.8f;
    int hmargin = width*0.1f;
    
    //y(t) info
    int vrange = height*0.8f;
    int vmargin = height*0.2f;

    for(int i = 0; i < amountOfCurves; ++i){
        curveBase temp;


        // //makes x(t)
        int wid = rand()%(hrange - hmargin) + hmargin;
        temp.ax = hmargin + rand()%(hrange-wid);
        temp.dx = temp.ax + wid;
        temp.bx = temp.ax + rand()%(hmargin + hrange/2) - hmargin;
        temp.cx = temp.dx - rand()%(hmargin + hrange/2) + hmargin;

        //makes y(t)
        int hei = rand()%(vrange - vmargin) + vmargin;
        temp.ay = vmargin + rand()%(vrange - hei);
        temp.dy = temp.ay + hei;
        temp.by = temp.ay + rand()%(vmargin + vrange/2) - vmargin;
        temp.cy = temp.ay - rand()%(vmargin + vrange/2) + vmargin;


        curvesPoints.push_back(temp);
    }
}

void Scene::updateAllCurves(){
    curves.clear();
    for(int i = 0; i < curvesPoints.size(); ++i){
        curveBase a = curvesPoints[i];
        curves.push_back(Curve(a.ax, a.ay, a.bx, a.by, a.cx, a.cy, a.dx, a.dy));
    }
}

void Scene::updateOneCurve(int index){
    curves[index].calculateCurve();
}

void Scene::selectOneCurve(int index){
    if(selectedIndex != index){
        if(selectedIndex != -1){
            curves[selectedIndex].changeColor(sf::Color::Red);
        }
        curves[index].changeColor(sf::Color::Green);
    }
    selectedIndex = index;
}

const std::tuple<std::vector<Curve>&> Scene::getData(){
    return std::forward_as_tuple(curves);
}

std::tuple<float, sf::Vector2f> Scene::getMinRoot(const Curve& curve, const sf::Vector2f& mouse){
    //coefficients of the derivative
    sf::Vector2f A = curve.a.position;
    sf::Vector2f B = curve.b.position;
    sf::Vector2f C = curve.c.position;
    sf::Vector2f D = curve.d.position;
    double coefs[6] = {
        //a0 * t^0
        -6*(A.x * A.x + A.x*B.x + A.x*mouse.x - B.x*mouse.x -
            A.y * A.y + A.y*B.y + A.y*mouse.y - B.y*mouse.y),
        //a1 * t^1
        2*(15*A.x*A.x - 30*A.x*B.x + 6*A.x*C.x - 6*A.x*mouse.x + 9*B.x*B.x + 12*B.x*mouse.x -6*C.x*mouse.x + 
           15*A.y*A.y - 30*A.y*B.y + 6*A.y*C.y - 6*A.y*mouse.y + 9*B.y*B.y + 12*B.y*mouse.y -6*C.x*mouse.y),
        //a1 * t^2
        6*(-10*A.x*A.x + 30*A.x*B.x - 12*A.x*C.x + A.x*D.x + A.x*mouse.x - 18*B.x*B.x  + 9*B.x*C.x - 3*B.x*mouse.x + 3*C.x*mouse.x - D.x*mouse.x +
           -10*A.y*A.y + 30*A.y*B.y - 12*A.y*C.y + A.y*D.y + A.y*mouse.y - 18*B.y*B.y  + 9*B.y*C.y - 3*B.y*mouse.y + 3*C.y*mouse.y - D.y*mouse.y),
        //a1 * t^3
        12*(5*A.x*A.x - 20*A.x*B.x + 12*A.x*C.x - 2*A.x*D.x + 18*B.x*B.x - 18*B.x*C.x + 2*B.x*D.x + 3*C.x*C.x +
            5*A.y*A.y - 20*A.y*B.y + 12*A.y*C.y - 2*A.y*D.y + 18*B.y*B.y - 18*B.y*C.y + 2*B.y*D.y + 3*C.y*C.y),
        //a1 * t^4
        30*(-A.x*A.x + 5*A.x*B.x - 4*A.x*C.x + A.x*D.x - 6*B.x*B.x + 9*B.x*C.x - 2*B.x*D.x - 3*C.x*C.x + C.x*D.x +
            -A.y*A.y + 5*A.y*B.y - 4*A.y*C.y + A.y*D.y - 6*B.y*B.y + 9*B.y*C.y - 2*B.y*D.y - 3*C.y*C.y + C.y*D.y),
        //a1 * t^5
        6*(A.x*A.x - 6*A.x*B.x + 6*A.x*C.x - 2*A.x*D.x + 9*B.x*B.x - 18*B.x*C.x + 6*B.x*D.x + 9*C.x*C.x - 6*C.x*D.x + D.x*D.x +
           A.y*A.y - 6*A.y*B.y + 6*A.y*C.y - 2*A.y*D.y + 9*B.y*B.y - 18*B.y*C.y + 6*B.y*D.y + 9*C.y*C.y - 6*C.y*D.y + D.y*D.y)
    };

    double roots[10];

    gsl_poly_complex_workspace * space  = gsl_poly_complex_workspace_alloc (6);

    gsl_poly_complex_solve (coefs, 6, space, roots);
    float minDist = std::numeric_limits<float>::infinity();
    
    sf::Vector2f t;
    for(int i = 0; i < 5; ++i)
    {
        if(roots[2*i + 1] == 0){
            //printf("Root = %f %fi\n", roots[2*i], roots[2*i + 1]);
            float term1 = (1-roots[2*i]);
            float term2 = roots[2*i];
            sf::Vector2f func = {A.x*term1*term1*term1 + 3*B.x*term1*term1*term2 + 3*C.x*term1*term2*term2 + D.x*term2*term2*term2,
                                 A.y*term1*term1*term1 + 3*B.y*term1*term1*term2 + 3*C.y*term1*term2*term2 + D.y*term2*term2*term2};
            sf::Vector2f aux = (mouse - func);
            float dist = aux.x*aux.x + aux.y*aux.y;
            if(dist < minDist){
                t = func;
                minDist = dist;
            }
        }
    }
    printf("Root dist: %f\n", sqrt(minDist));
    //printf("t value: %f\n", t);
    
    return std::make_tuple(minDist, t);
}

sf::Vector2f Scene::getClosest(const sf::Vector2f& mouseClick){
    int closestIndex = 0;
    float minimumDist = std::numeric_limits<float>::infinity();
    printf("Mouse Click: (%f, %f)\n", mouseClick.x, mouseClick.y);
    sf::Vector2f closestRoot;

    for(int i = 0; i < curves.size(); ++i){
        //calculate distance between curve and point
        // min( derivative(f(x, y, t) = (x, y) - c(t)) ) 
        //curves[i].printCurve();
        sf::Vector2f aux = (curves[i].a.position - mouseClick);
        float aDist = aux.x * aux.x + aux.y * aux.y;
        sf::Vector2f aux2 = (curves[i].d.position - mouseClick);
        float dDist = aux2.x * aux2.x + aux2.y * aux2.y;
        float rDist;
        sf::Vector2f rootPt;
        std::tie(rDist, rootPt) = getMinRoot(curves[i], mouseClick);
        //float rDist = std::numeric_limits<float>::infinity();
        printf("aDist = %f\ndDist = %f\n", sqrt(aDist), sqrt(dDist));
        float minDist;
        //extremes (vertices a and d)
        if(aDist < dDist){
            if(rDist < aDist){
                minDist = rDist;
            }
            else{
                minDist = aDist;
            }
        }
        else{
            if(rDist < dDist){
                minDist = rDist;
            }
            else{
                minDist = dDist;
            }
        }
        //printf("minDist = %f\n", minDist);
        if(minDist < minimumDist){
            //printf("new Minimum = %f\n", minDist);
            minimumDist = minDist;
            closestIndex = i;
            closestRoot = rootPt;
        }

    }
    printf("Min Distance = %f\n\n\n", sqrt(minimumDist));

    selectOneCurve(closestIndex);

    return closestRoot;
}