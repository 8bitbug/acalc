#pragma once

#include <functional>
#include <map>
#include <string>

class Shell {
public:
  explicit Shell(std::string prompt);

  void prompt();
  auto getCommand() -> std::string;
  void command(const std::string &command);

private:
  std::string m_prompt;
  // m_command_functions was shorten to m_cf for easier usage
  std::map<std::string, std::function<void(void)>> m_cf;
};
