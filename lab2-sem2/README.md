## Lab 2
Here are implementations of some tree types (Red-Black tree and common B-tree) with usage of design patterns.

### Documentation
All classes and methods are documented with [doxygen-style](https://www.doxygen.nl/index.html) comments.
To see Doxywizard output with all descriptions and class diagrams visit [this page]() (link will be added soon)
 
### Used Patterns
- Decorator (for using testable instances of trees)
- Adapter (providing same interface for interacting with different trees)
- Factory (for creating new instances of trees (adapters))
- Command (allows to create command instance to save value and action on tree and undo given action)

(Using pre-implemented in c++ standard patterns like iterators (in `std::vector`) or proxy (smart pointers)) 

### GUI
*Not implemented yet*

### Unit tests
There are unit tests provided for trees' algorithms using [Catch 2](https://github.com/catchorg/Catch2).
Tests generate random keys for testing trees, to repeat tests with the same sequences of values use seed printed to console during testing. 

### Benchmark
Used [benchmark library](https://github.com/google/benchmark) by Google. [Benchmark](https://github.com/google/benchmark) and [googletest](https://github.com/google/googletest) are added as submodules.
Benchmark details and results are saved in [this file](benchmark.md). 