//
// Created by yanam on 16.06.2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <cfloat>
#include <deque>

#include "InfoBox.h"
#include "raylib.h"


class Graph : public InfoBox {
private:
    std::deque<Vector2> _data{};
    int _maxAmountOfPoints = 3000;


    float _smallestX = FLT_MAX;
    float _biggestX = -FLT_MAX;
    float _smallestY= FLT_MAX;
    float _biggestY = -FLT_MAX;

public:
    Graph(const Rectangle rect, std::shared_ptr<Population> pop): InfoBox(rect, pop) {};

    void reset();

    void draw() override;

    void addPoint(float, float);
    void addPointV(Vector2);
};


#endif //GRAPH_H
