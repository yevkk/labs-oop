#ifndef LAB1_DICE_SET_H
#define LAB1_DICE_SET_H

#include "dice.h"
#include <iostream>
#include <vector>

class DiceSet {
public:
    std::vector<Dice> elements;

    DiceSet() = default;

    explicit DiceSet(std::vector<Dice> vec);

    int min_sum();

    int max_sum();

    std::vector<double> sum_probabilities();

    void print_sum_probabilities();

    double expected_value();

};

std::ostream &operator<< (std::ostream &os, DiceSet &obj);

#endif //LAB1_DICE_SET_H
