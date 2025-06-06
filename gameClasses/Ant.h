#ifndef ANT_H
#define ANT_H
#include "raylib.h"


class Ant {
private:
    Vector2 position;
    double rotationAngle;

    void move(double distance);

public:
    Ant(Vector2 startPos);

    void calculateFrame();

    Vector2 getPos();

    double getAngle();
};


#endif //ANT_H
