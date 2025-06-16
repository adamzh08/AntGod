//
// Created by adam on 6/15/2025.
//

#include "Lines.h"

#include <iostream>

#include "LineIntersection.h"

std::vector<RaysDB> Lines::_raysDB;
int Lines::_raysDBAmount = 0;

Lines::Lines() {}

Lines Lines::addLine(Vector2 startPoint, Vector2 endPoint) {
    _linesAmount += 1;
    _lines.resize(_linesAmount);
    _lines.back().startPoint = startPoint;
    _lines.back().endPoint = endPoint;

    return *this;
}

void Lines::draw() const {
    for (auto [startPoint, endPoint] : _lines) {
        DrawLineEx(startPoint, endPoint, 1.5, BLACK);
    }
}

std::vector<Vector2> Lines::_searchRaysDB(const int raysAmount, const float raysRadius) {
    // ReSharper disable once CppUseStructuredBinding
    for (auto raysRec: _raysDB) {
        if (raysAmount == raysRec.raysAmount && raysRadius == raysRec.raysRadius) {
            return raysRec.deltaPoints;
        }
    }
#pragma omp critical
    {
        std::cout << "Adding a record... " << std::endl;
        _raysDBAmount += 1;
        _raysDB.resize(_raysDBAmount);

        _raysDB.back().raysAmount = raysAmount;
        _raysDB.back().raysRadius = raysRadius;
        _raysDB.back().deltaPoints.resize(raysAmount);

        const float angle = 2*PI/raysAmount;
        for (int i = 0; i < raysAmount; i++) {
            _raysDB.back().deltaPoints[i].x = cos(angle*i) * raysRadius;
            _raysDB.back().deltaPoints[i].y = sin(angle*i) * raysRadius;
        }

        std::cout << "Returned... " << std::endl;
    }
    return _raysDB.back().deltaPoints;
}

Vector2 vectorSum(const Vector2 point1, const Vector2 point2) {
    return Vector2{point1.x + point2.x, point1.y + point2.y};
}

std::vector<float> Lines::getRays(const Vector2 mainPoint, const int raysAmount, const float raysRadius) const {
    const std::vector<Vector2> deltaPoints = _searchRaysDB(raysAmount, raysRadius);

    std::vector rays(raysAmount, raysRadius);

    for (int i = 0; i < raysAmount; i++) {
        for (int j = 0; j < _linesAmount; j++) {
            rays[i] = fmin(rays[i], IntersectionLength(mainPoint, deltaPoints[i], _lines[j].startPoint, _lines[j].endPoint));
        }
    }

    for (int i = 0; i < raysAmount; i++) {
        rays[i] = 1.0 - (rays[i] / raysRadius);
    }

    return rays;
}

void Lines::drawRays(Vector2 mainPoint, int raysAmount, float raysRadius) {
    //DrawLineEx(mainPoint, vectorSum(mainPoint, getIntersection(mainPoint, deltaPoints[i], _lines[j].startPoint, _lines[j].endPoint)), 1, GRAY);

    const std::vector<Vector2> deltaPoints = _searchRaysDB(raysAmount, raysRadius);

    std::vector<Vector2> rays(raysAmount, Vector2{1000, 1000});

    for (int i = 0; i < raysAmount; i++) {
        for (int j = 0; j < _linesAmount; j++) {
            Vector2 temp = getIntersection(mainPoint, deltaPoints[i], _lines[j].startPoint, _lines[j].endPoint);
            if (sq(temp.x) + sq(temp.y) < sq(rays[i].x) + sq(rays[i].y)) {
                rays[i] = temp;
            }
        }
    }

    for (int i = 0; i < raysAmount; i++) {
        DrawLineEx(mainPoint, vectorSum(mainPoint, rays[i]), 1, GRAY);
    }
}

bool Lines::validMove(const Vector2 startPoint, const Vector2 deltaPoint) const {
    // ReSharper disable once CppUseStructuredBinding
    for (const auto line : _lines) {
        if (doIntersect(startPoint, deltaPoint, line.startPoint, line.endPoint)) {
            return false;
        }
    }

    return true;
}
