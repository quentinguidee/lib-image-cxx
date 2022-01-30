#pragma once

#include <functional>
#include <string>
#include <vector>

#include "decoder.hpp"
#include "encoder.hpp"

struct Format
{
    std::string id;
    std::string name;
    std::vector<std::string> extensions;
    std::function<std::unique_ptr<BaseEncoder>()> get_encoder;
    std::function<std::unique_ptr<BaseDecoder>()> get_decoder;
};
