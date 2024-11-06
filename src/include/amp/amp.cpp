#include "amp.hpp"
#include "external/exprtk.hpp"

#include <cmath>
#include <iostream>
#include <regex>
#include <utility>

auto amp::parse(std::string input) -> std::string {
  std::string parsed = std::move(input);

  parsed = std::regex_replace(parsed, std::regex("\\^"), "**");
  parsed = std::regex_replace(parsed, std::regex("pi"), std::to_string(M_PI));
  parsed = std::regex_replace(parsed, std::regex("e"), std::to_string(M_E));

  return parsed;
}

auto amp::evaluate(const std::string &input) -> long double {
  exprtk::expression<long double> expression;

  exprtk::parser<long double> parser;

  if (!parser.compile(input, expression)) {
    std::cerr << "Error parsing the expression!" << '\n';
  }

  return expression.value();
}
