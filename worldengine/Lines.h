//
// Created by adam on 6/15/2025.
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
    int raysAmount;
    float raysRadius;

    std::vector<Vector2> deltaPoints;
};

class Lines {
private:
    std::vector<Line> _lines; // MKL allocator?
    int _linesAmount = 0;

    static std::vector<RaysDB> _raysDB;
    static int _raysDBAmount;

    static std::vector<Vector2> _searchRaysDB(int raysAmount, float raysRadius);
public:
    Lines();
    Lines addLine(Vector2 startPoint, Vector2 endPoint);

    void draw() const;

    std::vector<float> getRays(Vector2 mainPoint, int raysAmount, float raysRadius, bool draw) const;
    bool validMove(Vector2 startPoint, Vector2 deltaPoint) const;
};

#endif //LINES_H
