//
// Created by yanam on 16.06.2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <cfloat>
#include <set>
#include <vector>

#include "raylib.h"


class Graph {
private:
    std::vector<Vector2> _data{};
    Rectangle _rect;


    float _smallestX = FLT_MAX;
    float _biggestX = -FLT_MAX;
    float _smallestY= FLT_MAX;
    float _biggestY = -FLT_MAX;

public:

    explicit Graph(Rectangle);

    void reset();

    void draw();

    void addPoint(int, int);
    void addPointV(Vector2);
};


#endif //GRAPH_H
