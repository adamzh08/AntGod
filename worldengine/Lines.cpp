//
// Created by adam on 6/19/2025.
//

#include "Lines.h"

#include <algorithm>

#include "LineIntersection.h"

#include <iostream>

std::vector<RaysDB> Lines::_raysDB;

Lines::Lines() {
}

Lines Lines::addLine(Vector2 startPoint, Vector2 endPoint) {
    _lines.push_back(Line{startPoint, endPoint});

    return *this;
}

std::vector<float> Lines::getRays(Vector2 mainPoint, float raysRadius, int rays_count, float main_angle,
                                  float area_angle) const {
    const std::vector<Vector2> deltaPoints = _getRaysPoints(raysRadius, rays_count, main_angle, area_angle);

    std::vector rays(rays_count, raysRadius);

    for (int i = 0; i < rays_count; i++) {
        for (int j = 0; j < _lines.size(); j++) {
            rays[i] = fmin(
                rays[i], IntersectionLength(mainPoint, deltaPoints[i], _lines[j].startPoint, _lines[j].endPoint));
        }
    }

    for (int i = 0; i < rays_count; i++) {
        rays[i] = 1.0 - (rays[i] / raysRadius);
    }

    return rays;
}

void Lines::draw() const {
    for (auto [startPoint, endPoint]: _lines) {
        DrawLineEx(startPoint, endPoint, 1.5, BLACK);
    }
}

float Lines::addRecord(float raysRadius) {
    auto it = std::find_if(_raysDB.begin(), _raysDB.end(),
    [raysRadius](const RaysDB& p) {
        return p.raysRadius == raysRadius;
    });

    if (it != _raysDB.end()) {
        return raysRadius; // Already exists
    }

    std::cout << "Adding a record... " << std::endl;
    _raysDB.emplace_back();

    _raysDB.back().raysRadius = raysRadius;

    int pointCount = static_cast<int>(std::round(2 * PI * raysRadius));
    _raysDB.back().deltaPoints.resize(pointCount);

    const float angle = 1 / raysRadius;
    for (int i = 0; i < pointCount; i++) {
        _raysDB.back().deltaPoints[i].x = cos(angle * i) * raysRadius;
        _raysDB.back().deltaPoints[i].y = sin(angle * i) * raysRadius;
    }

    return raysRadius;
}

std::vector<Vector2> empty{};

std::vector<Vector2> &Lines::_searchRaysDB(float raysRadius) {
    // ReSharper disable once CppUseStructuredBinding
    auto it = std::find_if(_raysDB.begin(), _raysDB.end(),
    [raysRadius](const RaysDB& p) {
        return p.raysRadius == raysRadius;
    });

    if (it != _raysDB.end() && !it->deltaPoints.empty()) {
        return it->deltaPoints; // Already exists
    } else {
        return empty;
    }
}

std::vector<Vector2> Lines::_getRaysPoints(float raysRadius, int rays_count, float main_angle, float area_angle) {
    if (rays_count <= 1) return {};

    std::vector<Vector2> ray_points_source = _searchRaysDB(raysRadius);
    int start_index = static_cast<int>((main_angle - (area_angle / 2)) * raysRadius);

    std::vector<Vector2> ray_points_result;
    for (int i = 0; i < rays_count; i++) {
        int index = start_index + static_cast<int>(i * (area_angle * raysRadius) / (rays_count - 1));
        index = (index + ray_points_source.size()) % ray_points_source.size();
        ray_points_result.push_back(ray_points_source[index]);
    }

    return ray_points_result;
}

Vector2 vectorSum(const Vector2 point1, const Vector2 point2) {
    return Vector2{point1.x + point2.x, point1.y + point2.y};
}

void Lines::drawRays(Vector2 mainPoint, float raysRadius, int rays_count, float main_angle, float area_angle) {
    const std::vector<Vector2> deltaPoints = _getRaysPoints(raysRadius, rays_count, main_angle, area_angle);

    std::vector<Vector2> rays(rays_count, Vector2{1000, 1000});

    for (int i = 0; i < rays_count; i++) {
        for (int j = 0; j < _lines.size(); j++) {
            Vector2 temp = getIntersection(mainPoint, deltaPoints[i], _lines[j].startPoint, _lines[j].endPoint);
            if (sq(temp.x) + sq(temp.y) < sq(rays[i].x) + sq(rays[i].y)) {
                rays[i] = temp;
            }
        }
    }

    for (int i = 0; i < rays_count; i++) {
        DrawLineEx(mainPoint, vectorSum(mainPoint, rays[i]), 1, GRAY);
    }
}

bool Lines::validMove(const Vector2 startPoint, const Vector2 deltaPoint) const {
    // ReSharper disable once CppUseStructuredBinding
    for (const auto line: _lines) {
        if (doIntersect(startPoint, deltaPoint, line.startPoint, line.endPoint)) {
            return false;
        }
    }

    return true;
}
