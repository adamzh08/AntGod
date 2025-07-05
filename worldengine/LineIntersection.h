//
// Created by adam on 6/15/2025.
//

#ifndef LINEINTERSECTION_H
#define LINEINTERSECTION_H

#include <cassert>
#include <cmath>

#include  "Lines.h"

// Function to check if a point lies on a given line segment
inline bool onSegment(const ProVector2 p, const ProVector2 q, const ProVector2 r) {
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;
    return false;
}

// assuming p == {0, 0}
inline bool onSegment1(const ProVector2 q, const ProVector2 r) {
    if (q.x <= std::max(0.0f, r.x) && q.x >= std::min(0.0f, r.x) && q.y <= std::max(0.0f, r.y) && q.y >= std::min(0.0f, r.y))
        return true;
    return false;
}

// assuming q == {0, 0}
inline bool onSegment2(const ProVector2 p, const ProVector2 r) {
    if (0.0f <= std::max(p.x, r.x) && 0.0f >= std::min(p.x, r.x) && 0.0f <= std::max(p.y, r.y) && 0.0f >= std::min(p.y, r.y))
        return true;
    return false;
}

// Function to find the orientation of the ordered triplet
// (p, q, r) 0 -> p, q and r are collinear 1 -> Clockwise 2
// -> Counterclockwise
inline int orientation(const ProVector2 p, const ProVector2 q, const ProVector2 r) {
    double val = (q.y - p.y) * (r.x - q.x) + (p.x - q.x) * (r.y - q.y);

    if (val == 0)

        // Collinear
            return 0;

    // Clock or counterclock wise
    return (val > 0) ? 1 : 2;
}

// assuming p == {0, 0}
inline int orientation1(const ProVector2 q, const ProVector2 r) {
    double val = q.y * (r.x - q.x) + q.x * (q.y - r.y);
    if (val == 0)

        // Collinear
            return 0;

    // Clock or counterclock wise
    return (val > 0) ? 1 : 2;
}

// assuming r == {0, 0}
inline int orientation2(const ProVector2 p, const ProVector2 q) {
    double val = (p.y - q.y) * q.x + (q.x - p.x) * q.y;
    if (val == 0)

        // Collinear
            return 0;

    // Clock or counterclock wise
    return (val > 0) ? 1 : 2;
}

inline float square(const float x) {
    return x * x;
}

// Function to check if two lines (p1-q1 and p2-q2) intersect and calculate intersection point
inline bool doIntersect(const ProVector2 p1, const ProVector2 q1, const ProVector2 p2, const ProVector2 q2, ProVector2& res) {
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
        double c1 = a1 * p1.x + b1 * p1.y;

        double a2 = q2.y - p2.y;
        double b2 = p2.x - q2.x;
        double c2 = a2 * p2.x + b2 * p2.y;

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            res.x = (c1 * b2 - c2 * b1) / determinant;
            res.y = (a1 * c2 - a2 * c1) / determinant;
            return true;
        }
    }

    // Special Cases: check if the lines are collinear and
    // overlap
    if (o1 == 0 && onSegment(p1, p2, q1))
        return true;
    if (o2 == 0 && onSegment(p1, q2, q1))
        return true;
  	if (o3 == 0 && onSegment(p2, p1, q2))
        return true;
    if (o4 == 0 && onSegment(p2, q1, q2))
        return true;

    // Lines do not intersect in any case
    return false;
}

// Function to check if two lines (p1-q1 and p2-q2) intersect
inline bool doIntersect(const ProVector2 p1, const ProVector2 q1, const ProVector2 p2, const ProVector2 q2) {
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

    // Special Cases: check if the lines are collinear and
    // overlap
    if (o1 == 0 && onSegment(p1, p2, q1))
        return true;
    if (o2 == 0 && onSegment(p1, q2, q1))
        return true;
    if (o3 == 0 && onSegment(p2, p1, q2))
        return true;
    if (o4 == 0 && onSegment(p2, q1, q2))
        return true;

    // Lines do not intersect in any case
    return false;
}

// Function to check if two lines (mainPoint-secondPoint and p2-q2) intersect and calculate intersection distance
inline bool doIntersect(const ProVector2 mainPoint, const ProVector2 secondPoint, const ProVector2 p2, const ProVector2 q2, float& res) {
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(mainPoint, secondPoint, p2);
    int o2 = orientation(mainPoint, secondPoint, q2);
    int o3 = orientation(p2, q2, mainPoint);
    int o4 = orientation(p2, q2, secondPoint);

    // General case: lines intersect if they have different
    // orientations
    if (o1 != o2 && o3 != o4) {

        // Compute intersection point
        double a1 = secondPoint.y - mainPoint.y;
        double b1 = mainPoint.x - secondPoint.x;
        double c1 = a1 * mainPoint.x + b1 * mainPoint.y;

        double a2 = q2.y - p2.y;
        double b2 = p2.x - q2.x;
        double c2 = a2 * p2.x + b2 * p2.y;

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            float res_test = std::sqrt(square( c2*b1 + determinant*mainPoint.x - c1*b2 ) + square( c1*a2 + determinant*mainPoint.y - c2*a1 )) / determinant;
            res = std::sqrt(square(((c1 * b2 - c2 * b1) / determinant) - mainPoint.x) + square(((a1 * c2 - a2 * c1) / determinant) - mainPoint.y));

            assert(res_test == res);

            return true;
        }
    }

    // Special Cases: check if the lines are collinear and
    // overlap
    if (o1 == 0 && onSegment(mainPoint, p2, secondPoint)) {
        res = 0;
        return true;
    }
    if (o2 == 0 && onSegment(mainPoint, q2, secondPoint)) {
        res = 0;
        return true;
    }
    if (o3 == 0 && onSegment(p2, mainPoint, q2)) {
        res = 0;
        return true;
    }
    if (o4 == 0 && onSegment(p2, secondPoint, q2)) {
        res = 0;
        return true;
    }

    // Lines do not intersect in any case
    return false;
}

// Function to check if two lines ({0, 0}-secondPoint and p2-q2) intersect and calculate intersection distance
inline bool doIntersect(const ProVector2 secondPoint, ProVector2 p2, ProVector2 q2, float& res) {
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation1(secondPoint, p2);
    int o2 = orientation1(secondPoint, q2);
    int o3 = orientation2(p2, q2);
    int o4 = orientation(p2, q2, secondPoint);

    // General case: lines intersect if they have different
    // orientations
    if (o1 != o2 && o3 != o4) {

        // Compute intersection point
        double a1 = secondPoint.y;
        double b1 = -secondPoint.x;

        double a2 = q2.y - p2.y;
        double b2 = p2.x - q2.x;
        double c2 = a2 * p2.x + b2 * p2.y;

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            float res_test = sqrt(square( c2*b1 ) + square( -c2*a1 )) / determinant;
            res = sqrt(square((-c2 * b1) / determinant) + square((a1 * c2) / determinant));

            assert(res_test == res);

            return true;
        }
    }

    // Special Cases: check if the lines are collinear and
    // overlap
    if (o1 == 0 && onSegment1(p2, secondPoint)) {
        res = 0;
        return true;
    }
    if (o2 == 0 && onSegment1(q2, secondPoint)) {
        res = 0;
        return true;
    }
    if (o3 == 0 && onSegment2(p2, q2)) {
        res = 0;
        return true;
    }
    if (o4 == 0 && onSegment(p2, secondPoint, q2)) {
        res = 0;
        return true;
    }

    // Lines do not intersect in any case
    return false;
}

// Calculate distance between two points
inline double distance(const ProVector2& a, const ProVector2& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Calculate dot product of two vectors
inline double dot(const ProVector2& a, const ProVector2& b) {
    return a.x * b.x + a.y * b.y;
}

// Calculate triangle area using the cross product formula
inline double triangleArea(const ProVector2& o, const ProVector2& p, const ProVector2& q) {
    // Area = 0.5 * |cross product|
    // Cross product in 2D: (p-o) × (q-o) = (p.x-o.x)(q.y-o.y) - (p.y-o.y)(q.x-o.x)
    double cross = (p.x - o.x) * (q.y - o.y) - (p.y - o.y) * (q.x - o.x);
    return std::abs(cross) / 2.0;
}

inline bool lineSegmentCircleCollision(float radius, const ProVector2& O, const ProVector2& P, const ProVector2& Q) {
    // INPUT
    //    radius = the radius of the circle
    //    O = the origin of the circle
    //    P = one endpoint of the line segment
    //    Q = the other endpoint of the line segment
    // OUTPUT
    //    Returns true if the circle collides with the line.
    //    Otherwise, returns false.

    float min_dist = INFINITY;
    float max_dist = std::max(distance(O, P), distance(O, Q));

    // Calculate vectors OP, QP, OQ, PQ
    ProVector2 OP = P - O;
    ProVector2 QP = P - Q;
    ProVector2 OQ = Q - O;
    ProVector2 PQ = Q - P;

    if (dot(OP, QP) > 0 && dot(OQ, PQ) > 0) {
        // Point O projects onto the line segment PQ
        // Distance from O to line segment is the perpendicular distance
        min_dist = 2 * triangleArea(O, P, Q) / distance(P, Q);
    } else {
        // Point O projects outside the line segment
        // Distance is the minimum distance to either endpoint
        min_dist = std::min(distance(O, P), distance(O, Q));
    }

    if (min_dist <= radius && max_dist >= radius) {
        return true;
    } else {
        return false;
    }
}

#endif //LINEINTERSECTION_H
