#include "benchmark/benchmark.h"
#include "common.hpp"
#include "qoi_decoder.hpp"
#include "qoi_encoder.hpp"

BENCHMARK_IMAGE(QOI, "image_1.qoi");

BENCHMARK_MAIN();
