#include <benchmark/benchmark.h>

#include "BenchmarkUtils.hpp"

#include "../src/RedBlackTree.hpp"


static void BM_insert(benchmark::State &state) {
    RedBlackTree<int> tree;
    for (auto _ : state) {
        for (std::size_t i = 0; i < state.range(0); i++) {
            tree.insert(
                    utils::randomInt(
                            utils::BMParams::int_lower_bound,
                            utils::BMParams::int_upper_bound
                    )
            );
        }
    }
}

BENCHMARK(BM_insert)
    ->RangeMultiplier(utils::BMParams::small::multiplier)
    ->Range(utils::BMParams::small::start_point, utils::BMParams::small::end_point)
    ->Unit(benchmark::kNanosecond);

BENCHMARK(BM_insert)
        ->RangeMultiplier(utils::BMParams::big::multiplier)
        ->Range(utils::BMParams::big::start_point, utils::BMParams::big::end_point)
        ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();