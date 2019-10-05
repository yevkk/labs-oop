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

    std::vector<double> sum_probabilities();

    void print_sum_probabilities();

    double expected_value();

};

#endif //LAB1_DICE_SET_H
