//
// Created by adam on 6/19/2025.
//

#ifndef LINES_H
#define LINES_H

#include "raylib.h"

#include <vector>

struct ProVector2 {
    float x;
    float y;

    ProVector2 operator-(const ProVector2& other) const {
        return {x - other.x, y - other.y};
    }

    ProVector2 operator+(const ProVector2& other) const {
        return {x + other.x, y + other.y};
    }

    void operator+=(const ProVector2& other) {
        x += other.x;
        y += other.y;
    }

    void operator-=(const ProVector2& other) {
        x -= other.x;
        y -= other.y;
    }

    bool operator<(const ProVector2& other) const {
        return x*x + y*y < other.x*other.x + other.y*other.y;
    }

    operator Vector2() const {
        return Vector2{x, y};
    }

    ProVector2(const Vector2& other) {
        x = other.x;
        y = other.y;
    }

    ProVector2(float x, float y) : x(x), y(y) {}
    ProVector2() : x(0), y(0) {}
};

struct Line {
    ProVector2 startPoint;
    ProVector2 endPoint;
};

class Lines {
public:
    std::vector<Line> lines;

    Lines();
    Lines addLine(ProVector2 startPoint, ProVector2 endPoint);
    void draw() const;

    std::vector<float> getRays(ProVector2 mainPoint, float raysRadius, float center_angle, float area_angle, int rays_count) const;

    int getIntersectionLine(ProVector2 mainPoint, float raysRadius, float center_angle, float area_angle, int rays_count) const;

    void drawRays(ProVector2 mainPoint, float raysRadius, float center_angle, float area_angle, int rays_count);

    bool validMove(ProVector2 startPoint, ProVector2 deltaPoint) const;
};

#endif //LINES_H