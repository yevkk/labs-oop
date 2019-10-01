#ifndef LAB1_DICE_H
#define LAB1_DICE_H

class Dice {
public:
    int face_number;
    double *probability; //i-th element is a probability of dropping face with number (i+1)

    Dice(int Face_number, const double Probability[]) {
        double tmp = 1;
        face_number = Face_number;
        probability = new double[face_number];
        for (int i = 0; i < face_number; i++) {
            if (Probability[i] < tmp) {
                probability[i] = Probability[i];
                tmp -= Probability[i];
            } else {
                probability[i] = tmp;
                tmp = 0;
            }

        }
    }
};

#endif //LAB1_DICE_H
