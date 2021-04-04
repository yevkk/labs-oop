#include "../src/Matrix.hpp"

#include "catch.hpp"

TEST_CASE("Matrix Test", "[Matrix]") {
    SECTION("operators overload") {
        Matrix<int> A{{12,  43},
                      {-22, 10}};

        Matrix<int> B{{-12, 123},
                      {3,   -20}};

        Matrix<int> C{{2, 4, 3}};

        Matrix<int> D{{-10, 4, 299}};

        Matrix<int> E{{23, 54, -23, 212},
                      {34, 12, 123, -433},
                      {1,  2,  0,   0},
                      {32, 10, 4,   2},
                      {12, 4,  65,  11}};

        Matrix<int> F{{-67,  45,  -5,   1},
                      {0,    0,   0,    0},
                      {3,    -5,  -223, -11},
                      {12,   34,  21,   45},
                      {-233, -33, -1,   9}};

        SECTION("Relative operators") {
            REQUIRE(A == Matrix<int>{{12,  43},
                                     {-22, 10}}
            );
            REQUIRE(A != Matrix<int>{{12, 43}}
            );
            REQUIRE(A != Matrix<int>{{12},
                                     {-22}}
            );
            REQUIRE(A != Matrix<int>{{13,  2},
                                     {-22, -10}}
            );
            REQUIRE(A != Matrix<int>{{11,  43},
                                     {-22, 10}}
            );
        }

        SECTION("Addition") {
            REQUIRE(A + B == Matrix<int>{{0,   166},
                                         {-19, -10}}
            );

            REQUIRE(C + D == Matrix<int>{{-8, 8, 302}});

            REQUIRE(E + F == Matrix<int>{{-44,  99,  -28,  213},
                                         {34,   12,  123,  -433},
                                         {4,    -3,  -223, -11},
                                         {44,   44,  25,   47},
                                         {-221, -29, 64,   20}}
            );
        }

        SECTION("Subtraction") {
            REQUIRE(A - B == Matrix<int>{{24,  -80},
                                         {-25, 30}}
            );

            REQUIRE(C - D == Matrix<int>{{12, 0, -296}});

            REQUIRE(E - F == Matrix<int>{{90,  9,   -18, 211},
                                         {34,  12,  123, -433},
                                         {-2,  7,   223, 11},
                                         {20,  -24, -17, -43},
                                         {245, 37,  66,  2}}
            );
        }
    }

    SECTION("Multiplication") {
        Matrix<int> A{{1, 2, 5},
                      {2, 5, 3},
                      {7, 4, 3}};

        Matrix<int> B{{5, 2, 1, 5},
                      {0, 6, 3, 7},
                      {1, 0, 0, 10}};

        Matrix<int> C{{12},
                      {14},
                      {3}};

        Matrix<int> D{{1, 0, 0},
                      {0, 1, 0},
                      {0, 0, 1}};

        Matrix<int> E{{1, 2, 5},
                      {2, 5, 3},
                      {7, 4, 3},
                      {7, 2, 9}};

        Matrix<int> AB{{10, 14, 7,  69},
                       {13, 34, 17, 75},
                       {38, 38, 19, 93}};

        Matrix<int> AC{{55},
                       {103},
                       {149}};

        Matrix<int> AD{{1, 2, 5},
                       {2, 5, 3},
                       {7, 4, 3}};

        Matrix<int> BE{{51, 34, 79},
                       {82, 56, 90},
                       {71, 22, 95}};


        SECTION("Default") {
            REQUIRE(AB == multiply(A, B));
            REQUIRE(AC == multiply(A, C));
            REQUIRE(AD == multiply(A, D));
            REQUIRE(BE == multiply(B, E));
        }

        SECTION("Strassen") {
            REQUIRE(AB == multiply(A, B, MatrixMultiplicationPolicy::Strassen, 2));
            REQUIRE(AC == multiply(A, C, MatrixMultiplicationPolicy::Strassen, 2));
            REQUIRE(AD == multiply(A, D, MatrixMultiplicationPolicy::Strassen, 2));
            REQUIRE(BE == multiply(B, E, MatrixMultiplicationPolicy::Strassen, 2));
        }

        SECTION("Strassen parallel") {
            REQUIRE(AB == multiply(A, B, MatrixMultiplicationPolicy::StrassenParallel, 2));
            REQUIRE(AC == multiply(A, C, MatrixMultiplicationPolicy::StrassenParallel, 2));
            REQUIRE(AD == multiply(A, D, MatrixMultiplicationPolicy::StrassenParallel, 2));
            REQUIRE(BE == multiply(B, E, MatrixMultiplicationPolicy::StrassenParallel, 2));
        }
    }
}