#include "formats.hpp"

#include <exception>

#include "bmp_format.hpp"
#include "png_format.hpp"
#include "qoi_format.hpp"

const std::unordered_map<std::string, Format> Formats::FORMATS {
    { "qoi", QOI_FORMAT },
    { "bmp", BMP_FORMAT },
    { "png", PNG_FORMAT },
};

const Format& Formats::get_by_id(const std::string& format_id)
{
    return FORMATS.at(format_id);
}

const Format& Formats::get_by_extension(const std::string& format_extension)
{
    for (auto it = Formats::FORMATS.begin(); it != Formats::FORMATS.end(); ++it)
    {
        const std::vector<std::string> extensions = it->second.extensions;
        for (auto extension : extensions)
        {
            if (extension == format_extension)
                return it->second;
        }
    }
    // TODO: File not found
    throw std::exception();
}
