namespace utils {
/**
 * @brief provides const params for testing
 */
struct TestParam {
    static constexpr std::size_t size_small = 25;
    static constexpr std::size_t size_megium = 50;
    static constexpr std::size_t size_large = 100;
    static constexpr std::size_t size_extreme = 1000;
    static constexpr int int_lower_bound = -10000;
    static constexpr int int_upper_bound = 10000;
};
}
