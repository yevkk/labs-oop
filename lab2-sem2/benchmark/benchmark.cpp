#include <benchmark/benchmark.h>

#include <vector>

void BM_StringCreation(benchmark::State& state) {
    std::vector<int> a(state.range(0));
    for (auto i = 0; i < state.range(0); i++) {
        a.push_back(i);
    }
}
BENCHMARK(BM_StringCreation)->RangeMultiplier(2)->Range(8, 1024);

BENCHMARK_MAIN();