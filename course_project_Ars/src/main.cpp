#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <exception>
#include <string>

#include "ScrumBoard.h"

int main() {
  using namespace ftxui;

  ScrumBoard board;
  ScreenInteractive screen = ScreenInteractive::Fullscreen();

  std::string developer_input;
  std::string assign_task_id_input;
  std::string assign_developer_input;
  std::string file_path_input = "data/board.xml";

  auto add_developer_action = [&]() {
    if (developer_input.empty())
      return;
    board.AddDeveloper(developer_input);
    developer_input.clear();
  };

  auto assign_task_action = [&]() {
    if (assign_task_id_input.empty() || assign_developer_input.empty())
      return;
    try {
      int task_id = std::stoi(assign_task_id_input);
      board.AssignTask(task_id, assign_developer_input);
      assign_task_id_input.clear();
      assign_developer_input.clear();
    } catch (const std::exception&) {
    }
  };

  InputOption developer_opt;
  developer_opt.placeholder = "Имя разработчика...";
  developer_opt.on_enter = add_developer_action;
  Component developer_input_component = Input(&developer_input, developer_opt);

  InputOption assign_id_opt;
  assign_id_opt.placeholder = "ID задачи...";
  assign_id_opt.on_enter = assign_task_action;
  Component assign_task_id_component = Input(&assign_task_id_input, assign_id_opt);

  InputOption assign_dev_opt;
  assign_dev_opt.placeholder = "Имя разработчика...";
  assign_dev_opt.on_enter = assign_task_action;
  Component assign_developer_component = Input(&assign_developer_input, assign_dev_opt);

  InputOption file_opt;
  file_opt.placeholder = "Путь к XML...";
  Component file_path_component = Input(&file_path_input, file_opt);

  Component add_developer_button = Button("Добавить", add_developer_action);
  Component assign_button = Button("Назначить", assign_task_action);

  Component save_button = Button("Сохранить", [&] {
    board.SaveToXML(file_path_input);
  });

  Component load_button = Button("Загрузить", [&] {
    board.LoadFromXML(file_path_input);
  });

  Component clear_button = Button("Очистить", [&] {
    board.ClearBoard();
  });

  Component exit_button = Button("Выход", [&] {
    screen.Exit();
  });

  return 0;
}
