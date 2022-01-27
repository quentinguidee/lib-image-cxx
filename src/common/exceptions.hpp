#pragma once

#include <exception>
#include <string>

class DecodeException : public std::exception
{
private:
    std::string error_message;

public:
    DecodeException(const std::string& error_message) :
        error_message(error_message)
    {
    }

    const char* what() const throw()
    {
        return "Error while decoding QOI image.";
    }

    const std::string message()
    {
        return error_message;
    }
};
