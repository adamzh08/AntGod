//
// Created by yanam on 16.06.2025.
//

#include "Graph.h"

#include <cfloat>
#include <iostream>
#include <ostream>


Graph::Graph(Rectangle rect) {
    _rect = rect;
}

void Graph::addPointV(const Vector2 newPoint) {
    _data.push_back(newPoint);
    _smallestX = std::min(_smallestX, newPoint.x);
    _biggestX = std::max(_biggestX, newPoint.x);
    _smallestY = std::min(_smallestY, newPoint.y);
    _biggestY = std::max(_biggestY, newPoint.y);
}

void Graph::addPoint(int x, int y) {
    addPointV(Vector2(x, y));
}

void Graph::reset() {
    _data.clear();
    _smallestX = FLT_MAX;
    _biggestX = -FLT_MAX;
    _smallestY = FLT_MAX;
    _biggestY = -FLT_MAX;
}

void Graph::draw() {
    // graph
    for (int i = 1; i < _data.size(); i++) {
        double x1 = _rect.x + _rect.width * (_data[i - 1].x - _smallestX) / (_biggestX - _smallestX);
        double x2 = _rect.x + _rect.width * (_data[i].x - _smallestX) / (_biggestX - _smallestX);
        double y1 = _rect.y + _rect.height * (1 - (_data[i - 1].y - _smallestY) / (_biggestY - _smallestY));
        double y2 = _rect.y + _rect.height * (1 - (_data[i].y - _smallestY) / (_biggestY - _smallestY));
        DrawLineEx(Vector2(x1, y1), Vector2(x2, y2), 3, GRAY);
    }
    DrawRectangleLines(_rect.x, _rect.y, _rect.width, _rect.height, RED);

    // y axis
    float amplitude = _biggestY - _smallestY;
    for (int i = 0; i < 5; i++) {
        DrawLineEx(
            Vector2(_rect.x - 5, _rect.y + 0.2 * i * _rect.height),
            Vector2(_rect.x + 5, _rect.y + 0.2 * i * _rect.height),
            1,
            BLACK
        );
        DrawText(
            std::to_string(static_cast<int>(_smallestY + (5 - i) * amplitude / 5)).c_str(),
            _rect.x + 10,
            _rect.y + 0.2 * i * _rect.height,
            10,
            BLACK
        );
    }
}
