#include "ScrumBoard.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

ScrumBoard::ScrumBoard() = default;

void ScrumBoard::ClearBoard() {
  developers_.clear();
  tasks_.clear();
  next_task_id_ = 1;
}

bool ScrumBoard::AddDeveloper(const std::string& developer_name) {
  if (developer_name.empty()) {
    return false;
  }
  if (DeveloperExists(developer_name)) {
    return false;
  }
  developers_.push_back(Developer(developer_name));
  return true;
}

const std::vector<Developer>& ScrumBoard::GetDevelopers() const {
  return developers_;
}

int ScrumBoard::AddTask(const std::string& title, const std::string& description) {
  if (title.empty()) {
    return 0;
  }
  Task task;
  task.id = next_task_id_;
  task.title = title;
  task.description = description;
  task.status = TaskStatus::Backlog;
  task.assigned_developer.clear();
  tasks_.push_back(task);
  ++next_task_id_;
  return task.id;
}

const std::vector<Task>& ScrumBoard::GetTasks() const {
  return tasks_;
}

std::vector<Task> ScrumBoard::GetTasksByStatus(TaskStatus status) const {
  std::vector<Task> filtered;
  for (const Task& task : tasks_) {
    if (task.status == status) {
      filtered.push_back(task);
    }
  }
  return filtered;
}

bool ScrumBoard::AssignTask(int task_id, const std::string& developer_name) {
  if (!DeveloperExists(developer_name)) {
    return false;
  }
  Task* task = FindTaskById(task_id);
  if (task == nullptr) {
    return false;
  }
  task->assigned_developer = developer_name;
  if (task->status == TaskStatus::Backlog) {
    task->status = TaskStatus::Assigned;
  }
  return true;
}

bool ScrumBoard::ChangeTaskStatus(int task_id, TaskStatus new_status) {
  Task* task = FindTaskById(task_id);
  if (task == nullptr) {
    return false;
  }
  task->status = new_status;
  return true;
}

std::string ScrumBoard::TaskStatusToString(TaskStatus status) {
  switch (status) {
    case TaskStatus::Backlog:
      return "Backlog";
    case TaskStatus::Assigned:
      return "Assigned";
    case TaskStatus::InProgress:
      return "InProgress";
    case TaskStatus::Blocked:
      return "Blocked";
    case TaskStatus::Done:
      return "Done";
  }
  return "Backlog";
}

static TaskStatus AllStatuses[] = {
    TaskStatus::Backlog,
    TaskStatus::Assigned,
    TaskStatus::InProgress,
    TaskStatus::Blocked,
    TaskStatus::Done};

bool ScrumBoard::TaskStatusFromString(const std::string& text, TaskStatus& status_out) {
  for (TaskStatus status : AllStatuses) {
    if (TaskStatusToString(status) == text) {
      status_out = status;
      return true;
    }
  }
  return false;
}

Task* ScrumBoard::FindTaskById(int task_id) {
  for (Task& task : tasks_) {
    if (task.id == task_id) {
      return &task;
    }
  }
  return nullptr;
}

bool ScrumBoard::DeveloperExists(const std::string& developer_name) const {
  return std::any_of(developers_.begin(), developers_.end(),
                     [&](const Developer& dev) { return dev.name == developer_name; });
}

namespace {
std::string EscapeXML(const std::string& text) {
  std::string result;
  for (char ch : text) {
    if (ch == '<') {
      result += "&lt;";
    } else if (ch == '>') {
      result += "&gt;";
    } else if (ch == '&') {
      result += "&amp;";
    } else if (ch == '"') {
      result += "&quot;";
    } else if (ch == '\'') {
      result += "&apos;";
    } else {
      result += ch;
    }
  }
  return result;
}

std::string UnescapeXML(const std::string& text) {
  std::string result;
  for (size_t i = 0; i < text.size(); ++i) {
    if (text[i] == '&') {
      size_t end = text.find(';', i);
      if (end != std::string::npos) {
        std::string entity = text.substr(i + 1, end - i - 1);
        if (entity == "lt") {
          result += '<';
        } else if (entity == "gt") {
          result += '>';
        } else if (entity == "amp") {
          result += '&';
        } else if (entity == "quot") {
          result += '"';
        } else if (entity == "apos") {
          result += '\'';
        } else {
          result += text[i];
        }
        i = end;
      } else {
        result += text[i];
      }
    } else {
      result += text[i];
    }
  }
  return result;
}

std::string ExtractTagContent(const std::string& xml, const std::string& tag_name) {
  std::string open_tag = "<" + tag_name + ">";
  std::string close_tag = "</" + tag_name + ">";
  
  size_t start = xml.find(open_tag);
  if (start == std::string::npos) return "";
  
  start += open_tag.length();
  size_t end = xml.find(close_tag, start);
  if (end == std::string::npos) return "";
  
  return xml.substr(start, end - start);
}
}

bool ScrumBoard::SaveToXML(const std::string& file_path) const {
  std::ofstream file(file_path);
  if (!file) {
    std::cerr << "Не удалось открыть файл для записи: " << file_path << std::endl;
    return false;
  }
  
  file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  file << "<ScrumBoard>\n";
  
  file << "  <NextTaskID>" << next_task_id_ << "</NextTaskID>\n";
  
  file << "  <Developers>\n";
  for (const Developer& dev : developers_) {
    file << "    <Developer>\n";
    file << "      <Name>" << EscapeXML(dev.name) << "</Name>\n";
    file << "    </Developer>\n";
  }
  file << "  </Developers>\n";
  
  file << "  <Tasks>\n";
  for (const Task& task : tasks_) {
    file << "    <Task>\n";
    file << "      <ID>" << task.id << "</ID>\n";
    file << "      <Title>" << EscapeXML(task.title) << "</Title>\n";
    file << "      <Description>" << EscapeXML(task.description) << "</Description>\n";
    file << "      <Status>" << TaskStatusToString(task.status) << "</Status>\n";
    file << "      <AssignedDeveloper>" << EscapeXML(task.assigned_developer) << "</AssignedDeveloper>\n";
    file << "    </Task>\n";
  }
  file << "  </Tasks>\n";
  
  file << "</ScrumBoard>\n";
  return true;
}

bool ScrumBoard::LoadFromXML(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file) {
    std::cerr << "Не удалось открыть файл для чтения: " << file_path << std::endl;
    return false;
  }
  
  std::string xml_content((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
  
  if (xml_content.find("<ScrumBoard>") == std::string::npos) {
    std::cerr << "Некорректный XML: отсутствует корневой элемент ScrumBoard" << std::endl;
    return false;
  }
  
  std::vector<Developer> loaded_developers;
  std::vector<Task> loaded_tasks;
  int loaded_next_id = 1;
  
  std::string next_id_str = ExtractTagContent(xml_content, "NextTaskID");
  if (!next_id_str.empty()) {
    try {
      loaded_next_id = std::stoi(next_id_str);
    } catch (const std::exception&) {
      std::cerr << "Некорректное значение NextTaskID" << std::endl;
      return false;
    }
  }
  
  size_t dev_start = xml_content.find("<Developers>");
  size_t dev_end = xml_content.find("</Developers>");
  if (dev_start != std::string::npos && dev_end != std::string::npos) {
    std::string devs_section = xml_content.substr(dev_start, dev_end - dev_start);
    size_t pos = 0;
    while ((pos = devs_section.find("<Developer>", pos)) != std::string::npos) {
      size_t dev_close = devs_section.find("</Developer>", pos);
      if (dev_close == std::string::npos) break;
      
      std::string dev_xml = devs_section.substr(pos, dev_close - pos);
      std::string name = UnescapeXML(ExtractTagContent(dev_xml, "Name"));
      if (!name.empty()) {
        loaded_developers.emplace_back(name);
      }
      pos = dev_close + 1;
    }
  }
  
  size_t tasks_start = xml_content.find("<Tasks>");
  size_t tasks_end = xml_content.find("</Tasks>");
  if (tasks_start != std::string::npos && tasks_end != std::string::npos) {
    std::string tasks_section = xml_content.substr(tasks_start, tasks_end - tasks_start);
    size_t pos = 0;
    while ((pos = tasks_section.find("<Task>", pos)) != std::string::npos) {
      size_t task_close = tasks_section.find("</Task>", pos);
      if (task_close == std::string::npos) break;
      
      std::string task_xml = tasks_section.substr(pos, task_close - pos);
      Task task;
      
      std::string id_str = ExtractTagContent(task_xml, "ID");
      try {
        task.id = std::stoi(id_str);
      } catch (const std::exception&) {
        std::cerr << "Некорректный ID задачи" << std::endl;
        return false;
      }
      
      task.title = UnescapeXML(ExtractTagContent(task_xml, "Title"));
      task.description = UnescapeXML(ExtractTagContent(task_xml, "Description"));
      task.assigned_developer = UnescapeXML(ExtractTagContent(task_xml, "AssignedDeveloper"));
      
      std::string status_str = ExtractTagContent(task_xml, "Status");
      TaskStatus status;
      if (!TaskStatusFromString(status_str, status)) {
        std::cerr << "Неизвестный статус: " << status_str << std::endl;
        return false;
      }
      task.status = status;
      
      loaded_tasks.push_back(task);
      pos = task_close + 1;
    }
  }
  
  developers_ = loaded_developers;
  tasks_ = loaded_tasks;
  next_task_id_ = loaded_next_id;
  return true;
}

