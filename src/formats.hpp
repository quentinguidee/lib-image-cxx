#pragma once

#include <unordered_map>

#include "format.hpp"
#include "qoi_format.hpp"

class Formats
{
public:
    static const std::unordered_map<std::string, Format> FORMATS;
    static const Format& get(const std::string& format_id);
};
