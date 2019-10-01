#include "dice_set.h"

int DiceSet::min_sum() {
    return elements.size();
}

int DiceSet::max_sum() {
    int res = 0;
    for (auto &e:elements)
        res += e.face_number;
    return res;
}