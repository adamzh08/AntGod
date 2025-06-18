//
// Created by yanam on 18.06.2025.
//

#ifndef NEUROBOX_H
#define NEUROBOX_H
#include <memory>
#include <mutex>

#include "InfoBox.h"


class Ant;

class NeuroBox : public InfoBox {
private:
    std::weak_ptr<Ant> _ant;

public:
    NeuroBox(const Rectangle rect, Color const color): InfoBox(rect, color) {};

    void draw() const override;

    void setEntity(std::shared_ptr<Ant>);
};


#endif //NEUROBOX_H
