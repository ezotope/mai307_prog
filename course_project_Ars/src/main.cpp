#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <exception>
#include <functional>
#include <string>
#include <vector>

#include "ScrumBoard.h"

std::vector<std::string> StatusNames() {
  return {"Backlog", "Assigned", "InProgress", "Blocked", "Done"};
}

TaskStatus StatusFromIndex(int index) {
  switch (index) {
    case 0:
      return TaskStatus::Backlog;
    case 1:
      return TaskStatus::Assigned;
    case 2:
      return TaskStatus::InProgress;
    case 3:
      return TaskStatus::Blocked;
    case 4:
      return TaskStatus::Done;
    default:
      return TaskStatus::Backlog;
  }
}

int main() {
  using namespace ftxui;

  ScrumBoard board;

  ScreenInteractive screen = ScreenInteractive::Fullscreen();

  std::string developer_input;
  std::string task_title_input;
  std::string task_description_input;
  std::string assign_task_id_input;
  std::string assign_developer_input;
  std::string status_task_id_input;
  std::string file_path_input = "data/board.xml";

  std::string log_message = "Готовы к работе! Tab/Enter-навигация Ctrl+Q-выход";

  std::vector<std::string> status_names = StatusNames();
  int status_menu_index = 0;
  
  int tab_index = 0;
  auto add_developer_action = [&]() {
    if (developer_input.empty()) {
      log_message = "❌ Введите имя разработчика!";
      return;
    }
    if (board.AddDeveloper(developer_input)) {
      log_message = "✓ Разработчик '" + developer_input + "' добавлен";
      developer_input.clear();
    } else {
      log_message = "❌ Разработчик с таким именем уже существует";
    }
  };
  
  auto add_task_action = [&]() {
    if (task_title_input.empty()) {
      log_message = "❌ Введите название задачи!";
      return;
    }
    int new_id = board.AddTask(task_title_input, task_description_input);
    if (new_id > 0) {
      log_message = "✓ Задача #" + std::to_string(new_id) + " '" + task_title_input + "' создана";
      task_title_input.clear();
      task_description_input.clear();
    } else {
      log_message = "❌ Не удалось создать задачу";
    }
  };
  
  auto assign_task_action = [&]() {
    if (assign_task_id_input.empty() || assign_developer_input.empty()) {
      log_message = "❌ Заполните ID задачи и имя разработчика!";
      return;
    }
    try {
      int task_id = std::stoi(assign_task_id_input);
      if (board.AssignTask(task_id, assign_developer_input)) {
        log_message = "✓ Задача #" + std::to_string(task_id) + " назначена на " + assign_developer_input;
        assign_task_id_input.clear();
        assign_developer_input.clear();
      } else {
        log_message = "❌ Задача или разработчик не найдены";
      }
    } catch (const std::exception&) {
      log_message = "❌ ID задачи должен быть числом!";
    }
  };
  
  auto change_status_action = [&]() {
    if (status_task_id_input.empty()) {
      log_message = "❌ Введите ID задачи!";
      return;
    }
    try {
      int task_id = std::stoi(status_task_id_input);
      TaskStatus status = StatusFromIndex(status_menu_index);
      if (board.ChangeTaskStatus(task_id, status)) {
        log_message = "✓ Статус задачи #" + std::to_string(task_id) + " изменён на " + status_names[status_menu_index];
        status_task_id_input.clear();
      } else {
        log_message = "❌ Задача #" + std::to_string(task_id) + " не найдена";
      }
    } catch (const std::exception&) {
      log_message = "❌ ID задачи должен быть числом!";
    }
  };

  InputOption developer_input_opt;
  developer_input_opt.placeholder = "Введите имя...";
  developer_input_opt.on_enter = add_developer_action;
  Component developer_input_component = Input(&developer_input, developer_input_opt);
  
  InputOption task_title_opt;
  task_title_opt.placeholder = "Название задачи...";
  task_title_opt.on_enter = add_task_action;
  Component task_title_component = Input(&task_title_input, task_title_opt);
  
  InputOption task_desc_opt;
  task_desc_opt.placeholder = "Описание задачи...";
  task_desc_opt.multiline = false;
  task_desc_opt.on_enter = add_task_action;
  Component task_description_component = Input(&task_description_input, task_desc_opt);
  
  InputOption assign_id_opt;
  assign_id_opt.placeholder = "Номер задачи...";
  assign_id_opt.on_enter = assign_task_action;
  Component assign_task_id_component = Input(&assign_task_id_input, assign_id_opt);
  
  InputOption assign_dev_opt;
  assign_dev_opt.placeholder = "Имя разработчика...";
  assign_dev_opt.on_enter = assign_task_action;
  Component assign_developer_component = Input(&assign_developer_input, assign_dev_opt);
  
  InputOption status_id_opt;
  status_id_opt.placeholder = "Номер задачи...";
  status_id_opt.on_enter = change_status_action;
  Component status_task_id_component = Input(&status_task_id_input, status_id_opt);
  
  InputOption file_opt;
  file_opt.placeholder = "Путь к файлу...";
  Component file_path_component = Input(&file_path_input, file_opt);
  
  Component status_menu_component = Menu(&status_names, &status_menu_index);

  Component add_developer_button = Button("+ Добавить", add_developer_action);
  Component add_task_button = Button("+ Создать", add_task_action);
  Component assign_button = Button("→ Назначить", assign_task_action);
  Component change_status_button = Button("↻ Изменить", change_status_action);

  Component save_xml_button = Button("💾 Сохранить", [&] {
    if (board.SaveToXML(file_path_input)) {
      log_message = "✓ Доска сохранена в XML: " + file_path_input;
    } else {
      log_message = "❌ Ошибка сохранения XML";
    }
  });

  Component load_xml_button = Button("📂 Загрузить", [&] {
    if (board.LoadFromXML(file_path_input)) {
      log_message = "✓ Доска загружена из XML: " + file_path_input;
    } else {
      log_message = "❌ Ошибка загрузки XML";
    }
  });

  Component clear_button = Button("🗑 Очистить", [&] {
    board.ClearBoard();
    log_message = "✓ Доска очищена";
  });

  Component exit_button = Button("✕ Выход", [&] {
    screen.Exit();
  });

  std::vector<std::string> tab_names = {
    "👤 Разраб.", 
    "📝 Задачи", 
    "👥 Назнач.", 
    "🔄 Статус", 
    "💾 Файл"
  };
  
  Component dev_tab = Container::Vertical({
      developer_input_component,
      add_developer_button,
  });
  
  Component task_tab = Container::Vertical({
      task_title_component,
      task_description_component,
      add_task_button,
  });
  
  Component assign_tab = Container::Vertical({
      assign_task_id_component,
      assign_developer_component,
      assign_button,
  });
  
  Component status_tab = Container::Vertical({
      status_task_id_component,
      status_menu_component,
      change_status_button,
  });
  
  Component file_tab = Container::Vertical({
      file_path_component,
      save_xml_button,
      load_xml_button,
      clear_button,
      exit_button,
  });
  
  Component tab_container = Container::Tab({
      dev_tab,
      task_tab,
      assign_tab,
      status_tab,
      file_tab,
  }, &tab_index);
  
  Component tab_menu = Menu(&tab_names, &tab_index, MenuOption::HorizontalAnimated());
  
  Component form_container = Container::Vertical({
      tab_menu,
      tab_container,
  });

  Component main_component = CatchEvent(form_container, [&](Event event) {
    if (event.is_character() && event.character() == "q" && event.input() == "\x11") {
      screen.Exit();
      return true;
    }
    if (event == Event::F1) {
      log_message = "Tab-навигация Enter-действие Ctrl+Q-выход";
      return true;
    }
    return false;
  });

  Component main_renderer = Renderer(main_component, [&] {
    auto BuildTaskColumn = [&](TaskStatus status, const std::string& title, Color title_color) {
      std::vector<Element> task_lines;
      int task_count = 0;
      for (const ::Task& task : board.GetTasksByStatus(status)) {
        task_count++;
        std::string header = "#" + std::to_string(task.id) + " " + task.title;
        std::string description = task.description.empty() ? "нет описания" : task.description;
        std::string assigned = task.assigned_developer.empty() ? "не назначена" : task.assigned_developer;
        
        task_lines.push_back(text(header) | bold | color(Color::White));
        task_lines.push_back(text("👤 " + assigned) | color(Color::Yellow));
        if (!task.description.empty()) {
          task_lines.push_back(text("📄 " + description) | color(Color::GrayLight));
        }
        task_lines.push_back(separator());
      }
      if (task_lines.empty()) {
        task_lines.push_back(text("Пусто") | color(Color::GrayDark) | center);
      }
      
      std::string column_title = title + " (" + std::to_string(task_count) + ")";
      
      return window(text(column_title) | bold | color(title_color) | center,
                    vbox(task_lines) | flex);
    };

    std::vector<Element> columns;
    columns.push_back(BuildTaskColumn(TaskStatus::Backlog, "📋 Backlog", Color::Cyan));
    columns.push_back(BuildTaskColumn(TaskStatus::Assigned, "📌 Assigned", Color::Blue));
    columns.push_back(BuildTaskColumn(TaskStatus::InProgress, "⚙ InProgress", Color::Yellow));
    columns.push_back(BuildTaskColumn(TaskStatus::Blocked, "🚫 Blocked", Color::Red));
    columns.push_back(BuildTaskColumn(TaskStatus::Done, "✅ Done", Color::Green));

    std::vector<Element> developer_lines;
    int dev_count = 0;
    for (const Developer& developer : board.GetDevelopers()) {
      dev_count++;
      developer_lines.push_back(text("👤 " + developer.name) | color(Color::Cyan));
    }
    if (developer_lines.empty()) {
      developer_lines.push_back(text("Нет разработчиков") | color(Color::GrayDark) | center);
    }
    std::string dev_title = "👥 Команда (" + std::to_string(dev_count) + ")";
    auto developer_panel = window(text(dev_title) | bold | color(Color::Cyan), vbox(developer_lines) | flex);

    Element current_tab_content;
    
    if (tab_index == 0) {
      current_tab_content = vbox({
          text("Имя:") | bold,
          developer_input_component->Render() | border,
          text("") | size(HEIGHT, EQUAL, 1),
          add_developer_button->Render() | center,
      });
    } else if (tab_index == 1) {
      current_tab_content = vbox({
          text("Название:") | bold,
          task_title_component->Render() | border,
          text("Описание:") | bold,
          task_description_component->Render() | border,
          text("") | size(HEIGHT, EQUAL, 1),
          add_task_button->Render() | center,
      });
    } else if (tab_index == 2) {
      current_tab_content = vbox({
          text("ID задачи:") | bold,
          assign_task_id_component->Render() | border,
          text("Разработчик:") | bold,
          assign_developer_component->Render() | border,
          text("") | size(HEIGHT, EQUAL, 1),
          assign_button->Render() | center,
      });
    } else if (tab_index == 3) {
      current_tab_content = vbox({
          text("ID задачи:") | bold,
          status_task_id_component->Render() | border,
          text("Статус:") | bold,
          status_menu_component->Render() | border,
          text("") | size(HEIGHT, EQUAL, 1),
          change_status_button->Render() | center,
      });
    } else {
      current_tab_content = vbox({
          text("Файл XML:") | bold,
          file_path_component->Render() | border,
          text("") | size(HEIGHT, EQUAL, 1),
          hbox({
              save_xml_button->Render(),
              text(" "),
              load_xml_button->Render(),
          }) | center,
          text("") | size(HEIGHT, EQUAL, 1),
          clear_button->Render() | center,
          text("") | size(HEIGHT, EQUAL, 1),
          exit_button->Render() | center,
      });
    }
    
    auto forms_panel = window(
        text("⚙ Управление") | bold | color(Color::Yellow),
        vbox({
            tab_menu->Render(),
            separator(),
            current_tab_content | flex,
        }) | flex
    );

    auto board_panel = hbox({
        columns[0] | flex,
        separator(),
        columns[1] | flex,
        separator(),
        columns[2] | flex,
        separator(),
        columns[3] | flex,
        separator(),
        columns[4] | flex,
    }) | flex;

    int total_tasks = board.GetTasks().size();
    int total_devs = board.GetDevelopers().size();
    std::string stats = "Задач: " + std::to_string(total_tasks) + " | Команда: " + std::to_string(total_devs);

    return vbox({
        hbox({
            text("🎯 SCRUM") | bold | color(Color::Magenta),
            text(" | "),
            text(stats) | color(Color::GrayLight),
        }) | center,
        separator(),
        hbox({
            vbox({
                board_panel | flex,
                separator(),
                developer_panel | size(HEIGHT, LESS_THAN, 10),
            }) | flex,
            separator(),
            forms_panel | size(WIDTH, EQUAL, 40),
        }) | flex,
        separator(),
        hbox({
            text(log_message) | color(Color::Green) | flex,
            text(" F1-помощь") | color(Color::GrayDark),
        }),
    });
  });

  screen.Loop(main_renderer);

  return 0;
}