#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <tuple>
#include <random>
#include <limits>

#include "Curve.h"

static const int amountOfCurves = 1;

struct curveBase{
    float ax;
    float ay;
    float bx;
    float by;
    float cx;
    float cy;
    float dx;
    float dy;
};


class Scene{
private:
    std::vector<curveBase> curvesPoints;
    std::vector<Curve> curves;
    int selectedIndex;
    std::tuple<float, sf::Vector2f> getMinRoot(const Curve& curve, const sf::Vector2f& mouse);
public:
    void updateAllCurves();
    void updateOneCurve(int index);
    void selectOneCurve(int index);
    Scene(float width, float height);
    sf::Vector2f getClosest(const sf::Vector2f& mouseClick);
    const std::tuple<std::vector<Curve>&> getData();
};

#endif