#pragma once

#include <string>

namespace amp {
auto parse(std::string input) -> std::string;
auto evaluate(const std::string &input) -> long double;
} // namespace amp
