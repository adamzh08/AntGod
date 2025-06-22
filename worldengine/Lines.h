//
// Created by adam on 6/19/2025.
//

#ifndef LINES_H
#define LINES_H

#include <vector>

#include "raylib.h"

struct Line {
    Vector2 startPoint;
    Vector2 endPoint;
};

struct RaysDB {
    float raysRadius;

    std::vector<Vector2> deltaPoints;
};

class Lines {
private:
    static std::vector<RaysDB> _raysDB;

    static std::vector<Vector2>& _searchRaysDB(float raysRadius);

public:
    std::vector<Line> _lines;

    Lines();
    Lines addLine(Vector2 startPoint, Vector2 endPoint);

    std::vector<float> getRays(Vector2 mainPoint, float raysRadius, int rays_count, float main_angle, float area_angle) const;
    static std::vector<Vector2> _getRaysPoints(float raysRadius, int rays_count, float main_angle, float area_angle);

    void draw() const;

    static float addRecord(float raysRadius);

    void drawRays(Vector2 mainPoint, float raysRadius, int rays_count, float main_angle, float area_angle);

    bool validMove(Vector2 startPoint, Vector2 deltaPoint) const;
};

#endif //LINES_H