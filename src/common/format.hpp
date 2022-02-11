#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "decoder.hpp"
#include "encoder.hpp"

struct Format
{
    std::string id;
    std::string name;
    std::vector<std::string> extensions;
};
