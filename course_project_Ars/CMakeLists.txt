cmake_minimum_required(VERSION 3.16)

project(ScrumBoardGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui.git
  GIT_TAG v5.0.0
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(ftxui googletest)

file(GLOB PROJECT_HEADERS "include/*.h")
file(GLOB PROJECT_SOURCES "src/*.cpp")
add_executable(scrum_board_gui ${PROJECT_HEADERS} ${PROJECT_SOURCES})
target_include_directories(scrum_board_gui PRIVATE include)
target_link_libraries(scrum_board_gui PRIVATE ftxui::dom ftxui::component ftxui::screen)

enable_testing()

add_executable(scrum_board_tests tests/ScrumBoardTests.cpp ${PROJECT_HEADERS} ${PROJECT_SOURCES})
target_include_directories(scrum_board_tests PRIVATE include)
target_link_libraries(scrum_board_tests PRIVATE gtest_main ftxui::dom ftxui::component ftxui::screen)

add_test(NAME ScrumBoardTests COMMAND scrum_board_tests)

