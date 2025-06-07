//
// Created by adam on 6/7/2025.
//

#include "Lines.h"
#include <cmath>

Lines::Lines() {}

Lines::Lines(const std::vector<Line> &lines) {
    this->_lines = lines;
}

float Lines::_compare_distance(const Vector2 start, const Vector2 end) {
    return std::pow(start.x - end.x, 2) + std::pow(start.y - end.y, 2);
}

float Lines::get_intersection(const Vector2 start, const Vector2 end) const {
    float distance = 100000;
    for (auto &line : _lines) {
        if (Vector2 intersection_temp{}; line.doIntersect(start, end, intersection_temp)) {
            if (const float distance_temp = _compare_distance(start, intersection_temp); distance_temp < distance) {
                distance = distance_temp;
            }
        }
    }

    return sqrt(distance);
}

float Lines::get_intersection_delta(const Vector2 start, const Vector2 delta) const {
    Vector2 end{};
    end.x = start.x + delta.x;
    end.y = start.y + delta.y;

    return get_intersection(start, end);
}
