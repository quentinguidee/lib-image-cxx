#pragma once

#include "benchmark/benchmark.h"
#include "raw_image.hpp"

#define TEST_IMAGES_PATH "../test_images/"

#define BENCHMARK_IMAGE(NAMESPACE, FILENAME)                                          \
    static void BM_##NAMESPACE##_decode(benchmark::State& state)                      \
    {                                                                                 \
        BM_decode<NAMESPACE::Decoder>(state, FILENAME);                               \
    }                                                                                 \
    BENCHMARK(BM_##NAMESPACE##_decode)->Unit(benchmark::kMillisecond)->Iterations(5); \
    static void BM_##NAMESPACE##_encode(benchmark::State& state)                      \
    {                                                                                 \
        BM_encode<NAMESPACE::Decoder, NAMESPACE::Encoder>(state, FILENAME);           \
    }                                                                                 \
    BENCHMARK(BM_##NAMESPACE##_encode)->Unit(benchmark::kMillisecond)->Iterations(5);

template <typename Decoder>
RawImage decode(const std::string& filename)
{
    InputFileStream in { TEST_IMAGES_PATH + filename };

    RawImage image;
    Decoder(in, image).decode();

    return image;
}

template <typename Encoder>
Encoder encode(const std::string& filename, RawImage& image)
{
    OutputFileStream out { TEST_IMAGES_PATH + filename };
    Encoder encoder(out, image);
    encoder.encode();

    return encoder;
}

template <typename Decoder>
void BM_decode(benchmark::State& state, const std::string& filename)
{
    for (auto _ : state)
        decode<Decoder>(filename);
}

template <typename Decoder, typename Encoder>
void BM_encode(benchmark::State& state, const std::string& filename)
{
    std::string output_filename { "temp_" + filename };

    RawImage image = decode<Decoder>(filename);

    for (auto _ : state)
        encode<Encoder>(output_filename, image);

    std::remove(output_filename.c_str());
}
