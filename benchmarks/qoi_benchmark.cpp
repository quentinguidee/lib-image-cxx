#include "benchmark/benchmark.h"
#include "qoi_decoder.hpp"
#include "qoi_encoder.hpp"
#include "raw_image.hpp"

static void BM_QOI_decode(benchmark::State& state)
{
    for (auto _ : state)
    {
        InputFileStream in("../tests/test_images/image_1.qoi");

        RawImage image = RawImage();
        image.decode(QOI::Decoder(), in);
    }
}

BENCHMARK(BM_QOI_decode)->Unit(benchmark::kMillisecond)->Iterations(10);

static void BM_QOI_encode(benchmark::State& state)
{
    InputFileStream in("../tests/test_images/image_1.qoi");

    RawImage image = RawImage();
    image.decode(QOI::Decoder(), in);

    for (auto _ : state)
    {
        OutputFileStream out("../tests/test_images/image_1_out.qoi");
        image.encode(QOI::Encoder(), out);
        out.close();
    }
}

BENCHMARK(BM_QOI_encode)->Unit(benchmark::kMillisecond)->Iterations(10);

BENCHMARK_MAIN();
