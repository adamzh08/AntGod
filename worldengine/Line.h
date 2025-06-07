//
// Created by adam on 6/6/2025.
//

#ifndef LINE_H
#define LINE_H

#include <algorithm>
#include "raylib.h"

class Line {
private:
    // Geeks for Geeks code
    static bool onSegment(Vector2 p, Vector2 q, Vector2 r);
    static int orientation(Vector2 p, Vector2 q, Vector2 r);

public:
    Vector2 start{};
    Vector2 end{};

    Line(const Vector2 start, const Vector2 end);
    bool doIntersect(Vector2 p1, Vector2 q1, Vector2& res) const;
};

#endif //LINE_H
