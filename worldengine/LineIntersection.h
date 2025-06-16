//
// Created by adam on 6/15/2025.
//

#ifndef LINEINTERSECTION_H
#define LINEINTERSECTION_H

#include <cmath>

#include "raylib.h"

inline bool onSegment(Vector2 p, Vector2 q, Vector2 r) {
    if (q.x <= std::fmax(p.x, r.x) && q.x >= fmin(p.x, r.x)
        && q.y <= fmax(p.y, r.y) && q.y >= fmin(p.y, r.y))
        return true;
    return false;
}

// Function to find the orientation of the ordered triplet
// (p, q, r) 0 -> p, q and r are collinear 1 -> Clockwise 2
// -> Counterclockwise
inline int orientation(Vector2 p, Vector2 q, Vector2 r) {
    double val = (q.y - p.y) * (r.x - q.x)
                 - (q.x - p.x) * (r.y - q.y);
    if (val == 0)

        // Collinear
        return 0;

    // Clock or counterclock wise
    return (val > 0) ? 1 : 2;
}

inline double sq(double x) {
    return x*x;
}

inline void sub(Vector2 &a, Vector2 b) {
    a.x -= b.x;
    a.y -= b.y;
}

inline Vector2 sum(Vector2 a, Vector2 b) {
    return Vector2(a.x + b.x, a.y + b.y);
}

inline double IntersectionLength(const Vector2 mainPoint, const Vector2 deltaPoint, Vector2 linePoint1, Vector2 linePoint2) {
    sub(linePoint1, mainPoint);
    sub(linePoint2, mainPoint);

    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(Vector2{0, 0}, deltaPoint, linePoint1);
    int o2 = orientation(Vector2{0, 0}, deltaPoint, linePoint2);
    int o3 = orientation(linePoint1, linePoint2, Vector2{0, 0});
    int o4 = orientation(linePoint1, linePoint2, deltaPoint);

    // General case: lines intersect if they have different
    // orientations
    if (o1 != o2 && o3 != o4) {

        // Compute intersection point
        double a1 = deltaPoint.y;
        double b1 = -deltaPoint.x;

        double a2 = linePoint2.y - linePoint1.y;
        double b2 = linePoint1.x - linePoint2.x;
        double c2 = a2 * linePoint1.x + b2 * linePoint1.y;

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            return sqrt(sq(c2 * b1) + sq(a1 * c2)) / determinant;
        }
    }

    return INFINITY;
}

// Function to check if two lines (p1-q1 and p2-q2) intersect
inline bool doIntersect(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2) {

    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case: lines intersect if they have different
    // orientations
    if (o1 != o2 && o3 != o4) {

        // Compute intersection point
        double a1 = q1.y - p1.y;
        double b1 = p1.x - q1.x;

        double a2 = q2.y - p2.y;
        double b2 = p2.x - q2.x;

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            return true;
        }
    }

    // Lines do not intersect in any case
    return false;
}

inline Vector2 getIntersection(const Vector2 mainPoint, const Vector2 deltaPoint, Vector2 linePoint1, Vector2 linePoint2) {
    sub(linePoint1, mainPoint);
    sub(linePoint2, mainPoint);

    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(Vector2{0, 0}, deltaPoint, linePoint1);
    int o2 = orientation(Vector2{0, 0}, deltaPoint, linePoint2);
    int o3 = orientation(linePoint1, linePoint2, Vector2{0, 0});
    int o4 = orientation(linePoint1, linePoint2, deltaPoint);

    // General case: lines intersect if they have different
    // orientations
    if (o1 != o2 && o3 != o4) {

        // Compute intersection point
        double a1 = deltaPoint.y;
        double b1 = -deltaPoint.x;

        double a2 = linePoint2.y - linePoint1.y;
        double b2 = linePoint1.x - linePoint2.x;
        double c2 = a2 * linePoint1.x + b2 * linePoint1.y;

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            return Vector2{(-c2 * b1) / determinant, (a1 * c2) / determinant};
        }
    }

    return deltaPoint;
}

#endif //LINEINTERSECTION_H
