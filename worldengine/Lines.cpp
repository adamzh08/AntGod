//
// Created by adam on 6/19/2025.
//

#include "Lines.h"
#include <algorithm>
#include "LineIntersection.h"
#include <iostream>

Lines::Lines() {}

Lines Lines::addLine(ProVector2 startPoint, ProVector2 endPoint) {
    lines.push_back(Line{startPoint, endPoint});

    return *this;
}

void Lines::draw() const {
    for (const auto& line : lines) {
        DrawLineEx(line.startPoint, line.endPoint, 1.5, BLACK);
    }
}

std::vector<float> Lines::getRays(ProVector2 mainPoint, float raysRadius, float center_angle, float area_angle, int rays_count) const {
    std::vector<float> rays(rays_count, 0.0f);

    float angle_0 = center_angle - (area_angle / 2);
    float angle_step = area_angle / (rays_count - 1);

    for (auto [startPoint, endPoint]: lines) {
        if (lineSegmentCircleCollision(raysRadius, mainPoint, startPoint, endPoint)) {
            for (int i = 0; i < rays_count; i++) {
                float angle = angle_0 + static_cast<float>(i) * angle_step;
                ProVector2 deltaPoint{mainPoint.x + cos(angle) * raysRadius, mainPoint.y + sin(angle) * raysRadius};

                doIntersect(mainPoint, deltaPoint, startPoint, endPoint, deltaPoint);

                deltaPoint -= mainPoint;

                float temp_ray = 1.0f - (sqrt(square(deltaPoint.x) + square(deltaPoint.y)) / raysRadius);

                if (temp_ray - rays[i] >= 0.0001f) {
                    rays[i] = temp_ray;
                }
            }
        }
    }

    return rays;
}

int Lines::getIntersectionLine(ProVector2 mainPoint, float raysRadius, float center_angle, float area_angle, int rays_count) const {
    int j = -1;

    float ray = raysRadius;

    float angle_0 = center_angle - (area_angle / 2);
    float angle_step = area_angle / (rays_count - 1);

    for (int i = 0; i < rays_count; i++) {
        float angle = angle_0 + static_cast<float>(i) * angle_step;
        ProVector2 deltaPoint{mainPoint.x + cos(angle) * ray, mainPoint.y + sin(angle) * ray};

        for (int k = 0; k < lines.size(); k++) {
            if (doIntersect(mainPoint, deltaPoint, lines[k].startPoint, lines[k].endPoint, deltaPoint)) {
                j = k;
                ray = square(deltaPoint.x - mainPoint.x) + square(deltaPoint.y - mainPoint.y);
            }
        }

        ray = sqrt(ray);
    }

    return j;
}

void Lines::drawRays(ProVector2 mainPoint, float raysRadius, float center_angle, float area_angle, int rays_count) {

    float angle_0 = center_angle - (area_angle / 2);
    float angle_step = area_angle / (rays_count - 1);

    for (int i = 0; i < rays_count; i++) {
        float angle = angle_0 + static_cast<float>(i) * angle_step;
        ProVector2 deltaPoint{mainPoint.x + cos(angle) * raysRadius, mainPoint.y + sin(angle) * raysRadius};

        for (auto [startPoint, endPoint]: lines) {
            doIntersect(mainPoint, deltaPoint, startPoint, endPoint, deltaPoint);
        }

        DrawLineEx(mainPoint, deltaPoint, 1, GRAY);
    }
}

bool Lines::validMove(const ProVector2 startPoint, const ProVector2 endPoint) const {
    for (const auto line: lines) {
        if (doIntersect(startPoint, endPoint, line.startPoint, line.endPoint)) {
            return false;
        }
    }

    return true;
}
