#include <benchmark/benchmark.h>

#include "../src/Matrix.hpp"

#include <random>
#include <ctime>

int randomInt(const int &lower_bound, const int &upper_bound) {
    static std::random_device rd;
    static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_int_distribution<int> dist(lower_bound, upper_bound);
    return dist(gen);
}

Matrix<int> randomSquareIntMatrix(const std::size_t &size) {
    std::vector<std::vector<int>> elements;
    for (std::size_t i = 0; i < size; i++) {
        std::vector tmp(size, 0);
        for (auto &item : tmp) {
            item = randomInt(-100, 100);
        }
        elements.push_back(tmp);
    }

    return Matrix<int>{elements};
}

static void BM_default_multiplication(benchmark::State &state) {
    auto A = randomSquareIntMatrix(state.range(0));
    auto B = randomSquareIntMatrix(state.range(0));

    for (auto _ : state) {
        multiply(A, B);
    }
}

static void CustomArguments(benchmark::internal::Benchmark* b) {
    for (int i = 16; i <= 2048; i *= 2)
        for (int j = 8; j <= i / 2; j *= 2)
            b->Args({i, j});
}

static void CustomArgumentsParallel(benchmark::internal::Benchmark* b) {
    for (int i = 16; i <= 2048; i *= 2)
        for (int j = (i >= 1024) ? 128 : 8; j <= i / 2; j *= 2)
            b->Args({i, j});
}

static void BM_multiplication(benchmark::State &state, MatrixMultiplicationPolicy policy) {
    auto A = randomSquareIntMatrix(state.range(0));
    auto B = randomSquareIntMatrix(state.range(0));

    if (state.range(1) > state.range(0) / 2) return;

    for (auto _ : state) {
        multiply(A, B, policy, state.range(1));
    }
}

BENCHMARK(BM_default_multiplication)
        ->RangeMultiplier(2)
        ->Range(16, 2048)
        ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_multiplication, "Strassen", MatrixMultiplicationPolicy::Strassen)
        ->Apply(CustomArguments)
        ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_multiplication, "Strassen parallel", MatrixMultiplicationPolicy::StrassenParallel)
        ->Apply(CustomArgumentsParallel)
        ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

