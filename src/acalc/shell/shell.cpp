#include "shell.hpp"

#include <iostream>

Shell::Shell(std::string prompt) : m_prompt(std::move(prompt)) {
  m_cf["exit"] = []() { std::exit(0); };
  m_cf["help"] = []() {
    std::cout << "Acalc uses AsciiMath as its markdown language." << '\n'
              << "Check out https://asciimath.org/#syntax to learn about its syntax" << '\n'
              << "Use the @ to run a command, for example if you want to exit type '@exit'." << '\n'
              << "Documentation: Coming soon!" << '\n';
  };
  m_cf["clear"] = []() { std::cout << "\033[2J\033[1;1H"; };
}

void Shell::prompt() { std::cout << m_prompt << ' '; }

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
auto Shell::getCommand() -> std::string {
  std::string line;
  std::getline(std::cin, line);
  return line;
}

void Shell::command(const std::string &command) {
  try {
    if (command.starts_with("@")) {
      std::string cmdName = command.substr(1);

      if (cmdName.empty()) {
        std::cout << "Empty command recieved.\n";
        return;
      }

      // NOLINTNEXTLINE(readability-*)
      auto it = m_cf.find(cmdName);
      if (it != m_cf.end()) {
        it->second();
        return;
      }

      std::cout << "Command '" << cmdName << "' not found.\n";
      return;
    }
  } catch (const std::out_of_range &e) {
    std::cerr << "Out of range error: " << e.what() << "\n";
    throw std::runtime_error("Out of range: " + std::string(e.what()));
  }
}
