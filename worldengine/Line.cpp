//
// Created by adam on 6/6/2025.
//

#include "Line.h"

Line::Line(const Vector2 start, const Vector2 end) {
    this->start = start;
    this->end = end;
}

// Function to check if a point lies on a given line segment
bool Line::onSegment(const Vector2 p, const Vector2 q, const Vector2 r) {
    if (q.x <=  std::max(p.x, r.x) && q.x >=  std::min(p.x, r.x)
        && q.y <=  std::max(p.y, r.y) && q.y >=  std::min(p.y, r.y))
        return true;
    return false;
}

// Function to find the orientation of the ordered triplet
// (p, q, r) 0 -> p, q and r are collinear 1 -> Clockwise 2
// -> Counterclockwise
int Line::orientation(const Vector2 p, const Vector2 q, const Vector2 r) {
    const double val = (q.y - p.y) * (r.x - q.x)
                 - (q.x - p.x) * (r.y - q.y);
    if (val == 0)

        // Collinear
        return 0;

    // Clock or counterclockwise
    return (val > 0) ? 1 : 2;
}

// Function to check if two lines (p1-q1 and p2-q2) intersect
bool Line::doIntersect(const Vector2 p1, const Vector2 q1, Vector2& res) const {

    const Vector2 p2 = this->start;
    const Vector2 q2 = this->end;

    // Find the four orientations needed for general and
    // special cases
    const int o1 = orientation(p1, q1, p2);
    const int o2 = orientation(p1, q1, q2);
    const int o3 = orientation(p2, q2, p1);
    const int o4 = orientation(p2, q2, q1);

    // General case: lines intersect if they have different
    // orientations
    if (o1 != o2 && o3 != o4) {

        // Compute intersection point
        const double a1 = q1.y - p1.y;
        const double b1 = p1.x - q1.x;
        const double c1 = a1 * p1.x + b1 * p1.y;

        const double a2 = q2.y - p2.y;
        const double b2 = p2.x - q2.x;
        const double c2 = a2 * p2.x + b2 * p2.y;

        const double determinant = a1 * b2 - a2 * b1;

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
