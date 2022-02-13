#include "benchmark/benchmark.h"
#include "bmp_decoder.hpp"
#include "bmp_encoder.hpp"
#include "common.hpp"
#include "qoi_decoder.hpp"
#include "qoi_encoder.hpp"

BENCHMARK_IMAGE(QOI, "image_1.qoi");
BENCHMARK_IMAGE(BMP, "image_1.bmp");

BENCHMARK_MAIN();
