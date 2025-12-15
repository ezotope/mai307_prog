#pragma once

#include <string>

class Developer {
 public:
  std::string name;

  Developer() = default;

  explicit Developer(const std::string& developer_name) : name(developer_name) {}
};

