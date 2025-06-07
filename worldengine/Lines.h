//
// Created by adam on 6/7/2025.
//

#ifndef LINES_H
#define LINES_H
#include <vector>

#include "Line.h"

class Lines {
public:
    std::vector<Line> lines{};

    explicit Lines(const std::vector<Line> &lines);

    static float compare_distance(Vector2 start, Vector2 end);

    float get_intersection(Vector2 start, Vector2 end) const;
    float get_intersection_delta(Vector2 start, Vector2 delta) const;
};

#endif //LINES_H
