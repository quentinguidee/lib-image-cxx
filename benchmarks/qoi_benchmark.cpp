#include "benchmark/benchmark.h"
#include "qoi_decoder.hpp"
#include "qoi_encoder.hpp"
#include "qoi_format.hpp"
#include "raw_image.hpp"

static void BM_QOI_decode(benchmark::State& state)
{
    for (auto _ : state)
    {
        InputFileStream in("../test_images/image_1.qoi");

        RawImage image = RawImage();
        QOI::Decoder decoder = QOI::Decoder(in, image);
        decoder.decode();
    }
}

BENCHMARK(BM_QOI_decode)->Unit(benchmark::kMillisecond)->Iterations(10);

static void BM_QOI_encode(benchmark::State& state)
{
    InputFileStream in("../test_images/image_1.qoi");

    RawImage image = RawImage();
    QOI::Decoder decoder = QOI::Decoder(in, image);
    decoder.decode();

    for (auto _ : state)
    {
        OutputFileStream out("../test_images/image_1_out.qoi");
        QOI::Encoder encoder(out, image);
        encoder.encode();
    }

    std::remove("../test_images/image_1_out.qoi");
}

BENCHMARK(BM_QOI_encode)->Unit(benchmark::kMillisecond)->Iterations(10);

BENCHMARK_MAIN();
