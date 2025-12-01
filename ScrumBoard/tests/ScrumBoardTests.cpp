#include <gtest/gtest.h>

#include "ScrumBoard.h"

TEST(ScrumBoardTest, AddDeveloper) {
  ScrumBoard board;
  EXPECT_TRUE(board.AddDeveloper("Anna"));
  EXPECT_FALSE(board.AddDeveloper("Anna"));
  EXPECT_EQ(board.GetDevelopers().size(), 1u);
}

TEST(ScrumBoardTest, AddTask) {
  ScrumBoard board;
  int id = board.AddTask("Task", "Description");
  ASSERT_GT(id, 0);
  EXPECT_EQ(board.GetTasks().size(), 1u);
  EXPECT_EQ(board.GetTasks()[0].status, TaskStatus::Backlog);
}

TEST(ScrumBoardTest, AssignTask) {
  ScrumBoard board;
  board.AddDeveloper("Bob");
  int id = board.AddTask("Fix bug", "details");
  EXPECT_TRUE(board.AssignTask(id, "Bob"));
  EXPECT_EQ(board.GetTasks()[0].assigned_developer, "Bob");
  EXPECT_EQ(board.GetTasks()[0].status, TaskStatus::Assigned);
}

TEST(ScrumBoardTest, ChangeStatus) {
  ScrumBoard board;
  int id = board.AddTask("Feature", "new feature");
  EXPECT_TRUE(board.ChangeTaskStatus(id, TaskStatus::InProgress));
  EXPECT_EQ(board.GetTasks()[0].status, TaskStatus::InProgress);
}

TEST(ScrumBoardTest, SaveAndLoadXML) {
  ScrumBoard board;
  board.AddDeveloper("Alice");
  board.AddDeveloper("Bob");
  int id1 = board.AddTask("Task 1", "Description with <special> & \"characters\"");
  int id2 = board.AddTask("Task 2", "Another task");
  board.AssignTask(id1, "Alice");
  board.ChangeTaskStatus(id1, TaskStatus::InProgress);
  board.AssignTask(id2, "Bob");
  board.ChangeTaskStatus(id2, TaskStatus::Done);

  const std::string xml_path = "data/test_board.xml";
  EXPECT_TRUE(board.SaveToXML(xml_path));

  ScrumBoard restored;
  EXPECT_TRUE(restored.LoadFromXML(xml_path));
  ASSERT_EQ(restored.GetDevelopers().size(), 2u);
  ASSERT_EQ(restored.GetTasks().size(), 2u);
  EXPECT_EQ(restored.GetDevelopers()[0].name, "Alice");
  EXPECT_EQ(restored.GetDevelopers()[1].name, "Bob");
  EXPECT_EQ(restored.GetTasks()[0].title, "Task 1");
  EXPECT_EQ(restored.GetTasks()[0].description, "Description with <special> & \"characters\"");
  EXPECT_EQ(restored.GetTasks()[0].status, TaskStatus::InProgress);
  EXPECT_EQ(restored.GetTasks()[0].assigned_developer, "Alice");
  EXPECT_EQ(restored.GetTasks()[1].status, TaskStatus::Done);
}

