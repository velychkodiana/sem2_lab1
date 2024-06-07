#include <gtest/gtest.h>
#include "Task.h"
#include <fstream>
#include <filesystem>

TEST(TaskTest, SaveTasksToFile) {
    std::vector<Task> tasks = {{"Task 1", false}, {"Task 2", true}};
    std::string fileName = "test_tasks_save.txt";

    SaveTasksToFile(tasks, fileName);

    std::ifstream file(fileName);
    ASSERT_TRUE(file.is_open());

    int n;
    file >> n;
    ASSERT_EQ(n, tasks.size());

    for (const Task& task : tasks) {
        std::string description;
        bool done;

        file >> description >> done;
        std::replace(description.begin(), description.end(), '_', ' ');

        EXPECT_EQ(description, task.description);
        EXPECT_EQ(done, task.done);
    }
    
// щоб видалити тестового файлу після перевірки
    file.close();
    std::filesystem::remove(fileName);  
}

TEST(TaskTest, LoadTasksFromFile) {
    std::vector<Task> expectedTasks = {{"Task 1", false}, {"Task 2", true}};
    std::string fileName = "test_tasks_load.txt";

    // створимо тестовий файл з даними
    {
        std::ofstream file(fileName);
        file << expectedTasks.size();
        for (const Task& task : expectedTasks) {
            std::string description = task.description;
            std::replace(description.begin(), description.end(), ' ', '_');
            file << '\n' << description << ' ' << task.done;
        }
    }

    std::vector<Task> loadedTasks = LoadTasksFromFile(fileName);

    EXPECT_EQ(loadedTasks.size(), expectedTasks.size());

    for (size_t i = 0; i < expectedTasks.size(); ++i) {
        EXPECT_EQ(loadedTasks[i].description, expectedTasks[i].description);
        EXPECT_EQ(loadedTasks[i].done, expectedTasks[i].done);
    }

    // видаляємо тестовий файлу після перевірки
    std::filesystem::remove(fileName);  
}

TEST(TaskTest, LoadTasksFromFile_FileNotExist) {
    std::string fileName = "non_existent_file.txt";
    std::vector<Task> loadedTasks = LoadTasksFromFile(fileName);
    EXPECT_TRUE(loadedTasks.empty());
}
