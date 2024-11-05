#include "shell/shell.hpp"

auto main() -> int {
  Shell shell(">");

  while (true) {
    shell.prompt();
    shell.command(shell.getCommand());
  }
}
