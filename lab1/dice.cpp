#include "dice.h"

std::ostream &operator<<(std::ostream &os, Dice &obj) {
    os << obj.face_number << " (";
    for (int i = 0; i < obj.face_number; i++) {
        os << i + 1 << ": " << obj.probability[i];
        if (i + 1 != obj.face_number) os << "; ";
    }
    os << ");";
    return os;
}