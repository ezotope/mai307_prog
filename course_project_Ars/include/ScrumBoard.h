#pragma once

#include <string>
#include <vector>

#include "Developer.h"

class ScrumBoard {
 public:
  ScrumBoard();

  void ClearBoard();

  bool AddDeveloper(const std::string& developer_name);

  const std::vector<Developer>& GetDevelopers() const;

  bool AssignTask(int task_id, const std::string& developer_name);

  bool SaveToXML(const std::string& file_path) const;

  bool LoadFromXML(const std::string& file_path);

 private:

  bool DeveloperExists(const std::string& developer_name) const;

  std::vector<Developer> developers_;

};