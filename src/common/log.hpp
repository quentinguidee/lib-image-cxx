#pragma once

#include <iostream>

/*
 * DEBUG
 */

#if NDEBUG
#define LOG_DEBUG(message)
#else
#define LOG_DEBUG(message) (std::cout << "[LIB-IMAGE-CXX::DEBUG] " << message << std::endl)
#endif

/*
 * ERROR
 */

#define LOG_ERROR(message) (std::cerr << "[LIB-IMAGE-CXX::ERROR] " << message << std::endl)
