#include "../include/amp/amp.hpp"
#include "shell/shell.hpp"
#include <iostream>

auto main() -> int {
  Shell shell(">");

  std::string command;

  while (true) {
    shell.prompt();

    command = shell.getCommand();
    if (command.starts_with("@")) {
      shell.command(command);
    } else {
      std::cout.precision(15);
      std::cout << amp::evaluate(amp::parse(command)) << '\n';
    }
  }
}
