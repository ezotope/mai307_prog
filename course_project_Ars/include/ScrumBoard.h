#pragma once

#include <string>
#include <vector>

#include "Developer.h"
#include "Task.h"

class ScrumBoard {
 public:
  ScrumBoard();

  void ClearBoard();

  bool AddDeveloper(const std::string& developer_name);

  const std::vector<Developer>& GetDevelopers() const;

  int AddTask(const std::string& title, const std::string& description);

  const std::vector<Task>& GetTasks() const;

  std::vector<Task> GetTasksByStatus(TaskStatus status) const;

  bool AssignTask(int task_id, const std::string& developer_name);

  bool ChangeTaskStatus(int task_id, TaskStatus new_status);

  bool SaveToXML(const std::string& file_path) const;

  bool LoadFromXML(const std::string& file_path);

  static std::string TaskStatusToString(TaskStatus status);

  static bool TaskStatusFromString(const std::string& text, TaskStatus& status_out);

 private:
  Task* FindTaskById(int task_id);

  bool DeveloperExists(const std::string& developer_name) const;

  std::vector<Developer> developers_;

  std::vector<Task> tasks_;

  int next_task_id_ = 1;
};
