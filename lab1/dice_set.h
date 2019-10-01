#ifndef LAB1_DICE_SET_H
#define LAB1_DICE_SET_H

#include "dice.h"
#include <iostream>
#include <vector>

class DiceSet {
public:
    std::vector<Dice> elements;

    DiceSet() = default;

    int min_sum();

    int max_sum();

};

#endif //LAB1_DICE_SET_H
