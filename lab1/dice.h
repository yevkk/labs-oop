#ifndef LAB1_DICE_H
#define LAB1_DICE_H

#include <iostream>
#include <vector>

class Dice {
public:
    int face_number;
    std::vector<double> probability; //i-th element is a probability of dropping face with number (i+1)

    Dice(int Face_number, std::vector<double> Probability) {
        double tmp = 1;
        if (Face_number < 4)
            face_number = 2;
        else if (Face_number < 6)
            face_number = 4;
        else if (Face_number < 8)
            face_number = 6;
        else if (Face_number < 12)
            face_number = 8;
        else if (Face_number < 20)
            face_number = 12;
        else face_number = 20;

        for (int i = 0; i < face_number; i++) {
            if (i == face_number - 1 || Probability[i] > tmp) {
                probability.emplace_back(tmp);
                tmp = 0;
            } else {
                probability.emplace_back(Probability[i]);
                tmp -= Probability[i];
            }


        }
    }
};

std::ostream &operator<<(std::ostream &os, Dice &obj) {
    os << obj.face_number << " (";
    for (int i = 0; i < obj.face_number; i++) {
        os << i + 1 << ": " << obj.probability[i];
        if (i + 1 != obj.face_number) os << ';';
    }
    os << ");";
}

#endif //LAB1_DICE_H
