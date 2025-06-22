//
// Created by yanam on 22.06.2025.
//

#ifndef COLOREDITBOX_H
#define COLOREDITBOX_H
#include "InfoBox.h"
#include "../Population.h"


class ColorEditBox : public InfoBox {
public:
    ColorEditBox(std::shared_ptr<Population> pop, const Rectangle rect) : InfoBox(rect, pop) {
    }

    void draw() override;
};


#endif //COLOREDITBOX_H
