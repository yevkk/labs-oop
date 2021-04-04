## Matrix multiplication
Implemented `Matrix` class with base operators and some multiplication methods

Used [Doxygen style](https://www.doxygen.nl/index.html) comments to describe entities.

Implemented some unit tests with [Catch2](https://github.com/catchorg/Catch2)

### Benchmark
Used [benchmark library](https://github.com/google/benchmark) by Google. [Benchmark](https://github.com/google/benchmark) and [googletest](https://github.com/google/googletest) are added as submodules.

`Run on (8 X 1800 MHz CPU s)`

#### Default multiplication
| Matrix size | Time     |
|-------------|----------|
| 16 x 16     | 0.007 ms |
| 32 x 32     | 0.042 ms |
| 64 x 64     | 0.284 ms |
| 128 x 128   | 2.09 ms  |
| 256 x 256   | 18.4 ms  |
| 512 x 512   | 205 ms   |
| 1024 x 1024 | 2518 ms  |
| 2048 x 2048 | 51377 ms |

#### Strassen algorithm
| Matrix size | Bound size | Time     |
|-------------|------------|----------|
| 16 x 16     | 8          | 0.046 ms |
|||
| 32 x 32     | 8          | 0.345 ms |
| 32 x 32     | 16         | 0.105 ms |
|||
| 64 x 64     | 8          | 2.46 ms  |
| 64 x 64     | 16         | 0.837 ms |
| 64 x 64     | 32         | 0.345 ms |
|||
| 128 x 128   | 8          | 17.3 ms  |
| 128 x 128   | 16         | 5.86 ms  |
| 128 x 128   | 32         | 2.57 ms  |
| 128 x 128   | 64         | 1.61 ms  |
|||
| 256 x 256   | 8          | 120 ms   |
| 256 x 256   | 16         | 41.4 ms  |
| 256 x 256   | 32         | 18.6 ms  |
| 256 x 256   | 64         | 11.7 ms  |
| 256 x 256   | 128        | 11.2 ms  |
|||
| 512 x 512   | 8          | 844 ms   |
| 512 x 512   | 16         | 289 ms   |
| 512 x 512   | 32         | 134 ms   |
| 512 x 512   | 64         | 89.3 ms  |
| 512 x 512   | 128        | 81.6 ms  |
| 512 x 512   | 256        | 91.5 ms  |
|||
| 1024 x 1024 | 8          | 6109 ms  |
| 1024 x 1024 | 16         | 2109 ms  |
| 1024 x 1024 | 32         | 953 ms   |
| 1024 x 1024 | 64         | 641 ms   | 
| 1024 x 1024 | 128        | 594 ms   |
| 1024 x 1024 | 256        | 656 ms   |
| 1024 x 1024 | 512        | 937 ms   |
|||
| 2048 x 2048 | 8          | 41562 ms |
| 2048 x 2048 | 16         | 14031 ms |
| 2048 x 2048 | 32         | 6531 ms  |
| 2048 x 2048 | 64         | 4406 ms  |
| 2048 x 2048 | 128        | 4094 ms  |
| 2048 x 2048 | 256        | 4562 ms  |
| 2048 x 2048 | 512        | 6609 ms  |
| 2048 x 2048 | 1024       | 12516 ms |

#### Strassen algorithm with multithreading
| Matrix size | Bound size | Time     |
|-------------|------------|----------|
| 16 x 16     | 8          | 0.171 ms |
|||
| 32 x 32     | 8          | 0.973 ms |
| 32 x 32     | 16         | 0.268 ms |
|||
| 64 x 64     | 8          | 3.75 ms  |
| 64 x 64     | 16         | 1.00 ms  |
| 64 x 64     | 32         | 0.441 ms |
|||
| 128 x 128   | 8          | 17.3 ms  |
| 128 x 128   | 16         | 5.94 ms  |
| 128 x 128   | 32         | 2.20 ms  |
| 128 x 128   | 64         | 1.46 ms  |
|||
| 256 x 256   | 8          | 81.2 ms  |
| 256 x 256   | 16         | 29.9 ms  |
| 256 x 256   | 32         | 12.0 ms  |
| 256 x 256   | 64         | 7.81 ms  |
| 256 x 256   | 128        | 7.25 ms  |
|||
| 512 x 512   | 8          | 453 ms   |
| 512 x 512   | 16         | 156 ms   |
| 512 x 512   | 32         | 62.5 ms  |
| 512 x 512   | 64         | 41.1 ms  |
| 512 x 512   | 128        | 31.2 ms  |
| 512 x 512   | 256        | 53.1 ms  |
|||
| 1024 x 1024 | 128        | 164 ms   |
| 1024 x 1024 | 256        | 240 ms   |
| 1024 x 1024 | 512        | 562 ms   |
|||
| 2048 x 2048 | 128        | 875 ms   |
| 2048 x 2048 | 256        | 1156 ms  |
| 2048 x 2048 | 512        | 2312 ms  |
| 2048 x 2048 | 1024       | 8016 ms  |
