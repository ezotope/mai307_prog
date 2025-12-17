#pragma once

#include <string>

enum class TaskStatus {
  Backlog,
  Assigned,
  InProgress,
  Blocked,
  Done
};

class Task {
 public:
  int id = 0;
  std::string title;
  std::string description;
  TaskStatus status = TaskStatus::Backlog;
  std::string assigned_developer;

  Task() = default;

  Task(int task_id,
       const std::string& task_title,
       const std::string& task_description,
       TaskStatus task_status,
       const std::string& developer_name)
      : id(task_id),
        title(task_title),
        description(task_description),
        status(task_status),
        assigned_developer(developer_name) {}
};

