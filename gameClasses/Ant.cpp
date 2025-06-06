#include "Ant.h"

#include <cmath>

Ant::Ant(const Vector2 startPos) {
    position = startPos;
}

void Ant::calculateFrame() {
    // 1 degree for test
    rotationAngle += M_PI / 180;

    // one pixel for test
    move(1);
}

void Ant::move(const double distance) {
    position.x += cos(rotationAngle) * distance;
    position.y += sin(rotationAngle) * distance;
}

// -------- getters ---------
Vector2 Ant::getPos() {
    return position;
}
double Ant::getAngle() {
    return rotationAngle;
}

