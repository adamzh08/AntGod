//
// Created by adam on 6/7/2025.
//

#ifndef LINES_H
#define LINES_H
#include <vector>

#include "Line.h"

class Lines {
public:
    // Array of lines
    std::vector<Line> _lines{};

    static float _compare_distance(Vector2 start, Vector2 end);

    Lines();
    explicit Lines(const std::vector<Line> &lines);

    // Get the nearest intersection using Start -> End points
    float get_intersection(Vector2 start, Vector2 end) const;

    // Get the nearest intersection using Start -> (Start + Delta) points
    float get_intersection_delta(Vector2 start, Vector2 delta) const;
};

#endif //LINES_H
