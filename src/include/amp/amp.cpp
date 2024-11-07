#include "amp.hpp"
#include "external/exprtk.hpp"
#include <cmath>
#include <iostream>
#include <regex>
#include <utility>

namespace {
auto computeProd(const std::string &prod_expr) -> double {
  // Remove the outer `prod_(i=1)^n i^x` or `prod_(i=1)^n x` part for easier parsing
  std::regex prodPattern(R"(prod_\((i=([0-9]+))\)\^([0-9]+) (.+))");
  std::smatch match;

  // Check if we match the pattern
  if (std::regex_search(prod_expr, match, prodPattern)) {
    int startIndex = std::stoi(match[2].str()); // Convert start index to integer
    int exponent = std::stoi(match[3].str());   // Convert exponent to integer
    std::string function = match[4].str();      // "i^3" or "2"

    // Compute the product for i=start_index to n
    double prod = 1;
    if (function.find('^') != std::string::npos) {                  // Function is in the form "i^x"
      int num = std::stoi(function.substr(function.find('^') + 1)); // Extract `n`
      for (int i = startIndex; i <= num; ++i) {
        prod *= std::pow(i, exponent);
      }
    } else { // Function is a constant value
      double constant = std::stod(function);
      for (int i = startIndex; i <= exponent; ++i) {
        prod *= constant;
      }
    }
    return prod;
  }

  throw std::invalid_argument("Invalid product format");
}

auto computeSum(const std::string &sum_expr) -> double {
  // Remove the outer `sum_(i=1)^n i^x` or `sum_(i=1)^n x` part for easier parsing
  std::regex sumPattern(R"(sum_\((i=([0-9]+))\)\^([0-9]+) (.+))");
  std::smatch match;

  // Check if we match the pattern
  if (std::regex_search(sum_expr, match, sumPattern)) {
    int startIndex = std::stoi(match[2].str()); // Convert start index to integer
    int exponent = std::stoi(match[3].str());   // Convert exponent to integer
    std::string function = match[4].str();      // "i^3" or "2"

    // Compute the sum for i=start_index to n
    double sum = 0;
    if (function.find('^') != std::string::npos) {                  // Function is in the form "i^x"
      int num = std::stoi(function.substr(function.find('^') + 1)); // Extract `n`
      for (int i = startIndex; i <= num; ++i) {
        sum += std::pow(i, exponent);
      }
    } else { // Function is a constant value
      double constant = std::stod(function);
      for (int i = startIndex; i <= exponent; ++i) {
        sum += constant;
      }
    }
    return sum;
  }

  throw std::invalid_argument("Invalid summation format");
}

auto parseAndCompute(const std::string &input) -> std::string {
  // We'll replace the summation/product part with its computed value
  std::string parsed = input;

  // Regular expression to detect the summation/product in the form: sum_(i=1)^5 i^3 or prod_(i=1)^5
  // i^3 or sum_(i=1)^5 2 or prod_(i=1)^5 2
  std::regex summationPattern(R"(sum_\((i=([0-9]+))\)\^([0-9]+) (.+))");
  std::regex productPattern(R"(prod_\((i=([0-9]+))\)\^([0-9]+) (.+))");
  std::smatch match;

  // Search for the summation pattern
  while (std::regex_search(parsed, match, summationPattern)) {
    // Extract the sum expression part
    std::string sumExpr = match.str(0); // Full match, e.g., "sum_(i=1)^5 i^3"

    // Compute the sum for the expression
    double sumResult = computeSum(sumExpr);

    // Replace the summation with the computed result in the string
    parsed.replace(match.position(0), match.length(0), std::to_string(sumResult));
  }

  // Search for the product pattern
  while (std::regex_search(parsed, match, productPattern)) {
    // Extract the product expression part
    std::string prodExpr = match.str(0); // Full match, e.g., "prod_(i=1)^5 i^3"

    // Compute the product for the expression
    double prodResult = computeProd(prodExpr);

    // Replace the product with the computed result in the string
    parsed.replace(match.position(0), match.length(0), std::to_string(prodResult));
  }

  return parsed;
}
} // namespace

auto amp::parse(std::string input) -> std::string {
  std::string parsed = std::move(input);
  parsed = std::regex_replace(parsed, std::regex("pi"), std::to_string(M_PI));
  parsed = std::regex_replace(parsed, std::regex("e"), std::to_string(M_E));
  parsed = parseAndCompute(parsed);
  return parsed;
}

auto amp::evaluate(const std::string &input) -> long double {
  exprtk::expression<long double> expression;
  exprtk::parser<long double> parser;
  if (!parser.compile(input, expression)) {
    std::cerr << "Error parsing the expression!" << '\n';
    return 0.0L;
  }
  return expression.value();
}
