#include "formats.hpp"

const std::unordered_map<std::string, Format> Formats::FORMATS{
    {"qoi", QOI_FORMAT},
};

const Format& Formats::get(const std::string& format_id)
{
    return FORMATS.at(format_id);
}
