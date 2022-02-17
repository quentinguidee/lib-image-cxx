#pragma once

#include <exception>
#include <string>

class DecodeException : public std::runtime_error
{
public:
    DecodeException(const std::string& error_message = "") :
        std::runtime_error { "Error while decoding image. " + error_message }
    {
    }
};

class UnsupportedVersionException : public std::runtime_error
{
public:
    UnsupportedVersionException(const std::string& error_message = "") :
        std::runtime_error { "Error while encoding/decoding image. Probably because of an unsupported version. " + error_message }
    {
    }
};
