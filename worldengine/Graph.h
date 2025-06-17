//
// Created by yanam on 16.06.2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <cfloat>
#include <deque>

#include "raylib.h"


class Graph {
private:
    std::deque<Vector2> _data{};
    int _maxAmountOfPoints = 10000;

    Rectangle _rect{};

    Color _graphColor{};
    Color _axisColor{};

    float _smallestX = FLT_MAX;
    float _biggestX = -FLT_MAX;
    float _smallestY= FLT_MAX;
    float _biggestY = -FLT_MAX;

public:

    explicit Graph(Rectangle, Color, Color);

    void reset();

    void draw() const;

    void addPoint(int, int);
    void addPointV(Vector2);
};


#endif //GRAPH_H
