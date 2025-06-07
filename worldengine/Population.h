//
// Created by adam on 6/6/2025.
//

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>

#include "Ant.h"
#include "Line.h"
#include "Lines.h"

#define CARTESIAN_MOVE 0
#define RADIAL_MOVE 1

class Population {
private:
    Lines lines;

    Vector2 init_position{};
    int move_method = 0;

    std::vector<Vector2> rays_deltas;
    int rays_amount = 0;
    int rays_length = 0;

    float max_speed = 1;

    void sum_deltas(const Vector2 start, std::vector<Vector2> &temp) const;
    static float distance(Vector2 start, Vector2 end);
public:
    std::vector<Ant> ants;

    Population(const std::vector<Layer> &layers, const std::string& filename, const Lines &lines, int populationSize, int rays_amount, int rays_length, int move_method);

    void act();
};

#endif //POPULATION_H
