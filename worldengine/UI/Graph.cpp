//
// Created by yanam on 16.06.2025.
//

#include "Graph.h"

#include <cfloat>
#include <iostream>


void Graph::addPointV(const Vector2 newPoint) {
    _data.push_back(newPoint);

    if (_data.size() > _maxAmountOfPoints) {
        _data.pop_front();
    }

    _smallestX = _biggestX = _data.front().x;
    _smallestY = _biggestY = _data.front().y;

    for (const auto &point: _data) {
        _smallestX = std::min(_smallestX, point.x);
        _biggestX = std::max(_biggestX, point.x);
        _smallestY = std::min(_smallestY, point.y);
        _biggestY = std::max(_biggestY, point.y);
    }
}

void Graph::addPoint(float x, float y) {
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
    auto pop = _pop.lock();
    if (!pop) {
        std::cerr << "pop is nullptr" << std::endl;
        return;
    }

    // graph
    for (int i = 1; i < _data.size(); i++) {
        double x1 = _rect.x + _rect.width * (_data[i - 1].x - _smallestX) / (_biggestX - _smallestX);
        double x2 = _rect.x + _rect.width * (_data[i].x - _smallestX) / (_biggestX - _smallestX);
        double y1 = _rect.y + _rect.height * (1 - (_data[i - 1].y - _smallestY) / (_biggestY - _smallestY));
        double y2 = _rect.y + _rect.height * (1 - (_data[i].y - _smallestY) / (_biggestY - _smallestY));
        DrawLineEx(Vector2(x1, y1), Vector2(x2, y2), 3, pop->_entityColor);
    }

    // y axis
    const float amplitude = _biggestY - _smallestY;
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
