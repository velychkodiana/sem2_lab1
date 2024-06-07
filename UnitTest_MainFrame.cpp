#include <gtest/gtest.h>
#include <wx/wx.h>
#include <wx/sizer.h>
#include "MainFrame.h"
#include "Task.h"

// клас тесту для MainFrame
class MainFrameTest : public ::testing::Test 
{
protected:
    wxApp* app;       // вказівник на wxApp
    MainFrame* frame; // вказівник на MainFrame

    // ініціалізуємо перед кожним тестом
    virtual void SetUp() 
    {
        int argc = 0;
        char** argv = nullptr;
        
        // створюємо та ініціалізуємо wxApp
        app = new wxApp();
        wxApp::SetInstance(app);
        wxEntryStart(argc, argv);
        
        // робимо екземпляру MainFrame і показ його на екрані
        frame = new MainFrame("Test Frame");
        frame->Show();
    }

    // видаляємо після кожного тесту
    virtual void TearDown() 
    {
        // закриваємо і видалення MainFrame
        frame->Close(true);
        wxEntryCleanup();
        delete app;
    }
};

// перевіряємо додавання задачі
TEST_F(MainFrameTest, AddTask) 
{
    // вводимо значення у поле вводу
    frame->inputField->SetValue("Test Task");
    
    // імітуємо натискання кнопки додавання задачі
    wxCommandEvent evt(wxEVT_BUTTON, frame->addButton->GetId());
    frame->OnAddButtonClicked(evt);
    
    // перевіряємо чи задача додалася в checkListBox
    EXPECT_EQ(frame->checkListBox->GetCount(), 1);
    EXPECT_EQ(frame->checkListBox->GetString(0), "Test Task");
}

// перевіряємо видалення задачі
TEST_F(MainFrameTest, DeleteTask) 
{
    // вводимо значення у поле вводу та додавання задачі
    frame->inputField->SetValue("Test Task");
    wxCommandEvent evt(wxEVT_BUTTON, frame->addButton->GetId());
    frame->OnAddButtonClicked(evt);

    // обираємо задачу та імітацію натискання клавіші Delete
    frame->checkListBox->SetSelection(0);
    wxKeyEvent delEvt(wxEVT_KEY_DOWN);
    delEvt.m_keyCode = WXK_DELETE;
    frame->OnListKeyDown(delEvt);
    // Перевірка, що задача видалена з checkListBox
    EXPECT_EQ(frame->checkListBox->GetCount(), 0);
}

// перевіряємо переміщення задачі
TEST_F(MainFrameTest, MoveTask) 
{
    //  додаємо першу задау
    frame->inputField->SetValue("Task 1");
    wxCommandEvent evt1(wxEVT_BUTTON, frame->addButton->GetId());
    frame->OnAddButtonClicked(evt1);

    // додаємо другої задачі
    frame->inputField->SetValue("Task 2");
    wxCommandEvent evt2(wxEVT_BUTTON, frame->addButton->GetId());
    frame->OnAddButtonClicked(evt2);

    // обираємо другу задачу та імітацію натискання клавіші Up
    frame->checkListBox->SetSelection(1);
    wxKeyEvent upEvt(wxEVT_KEY_DOWN);
    upEvt.m_keyCode = WXK_UP;
    frame->OnListKeyDown(upEvt);

    // перевіряємо чи задачі помінялися місцями
    EXPECT_EQ(frame->checkListBox->GetString(0), "Task 2");
    EXPECT_EQ(frame->checkListBox->GetString(1), "Task 1");
}

// перевіряємо збереження і завантаження задач
TEST_F(MainFrameTest, SaveAndLoadTasks) 
{
    // створюємо і зберігаємо задачі у файл
    std::vector<Task> tasks = {{"Task 1", false}, {"Task 2", true}};
    SaveTasksToFile(tasks, "test_tasks.txt");
    
    // створюєм новий екземпляр MainFrame і завантажуємо задачі з файлу
    MainFrame* newFrame = new MainFrame("Test Frame");
    newFrame->AddSavedTasks();
    
    // перевіряємо чи задачі завантажилися правильно
    EXPECT_EQ(newFrame->checkListBox->GetCount(), 2);
    EXPECT_EQ(newFrame->checkListBox->GetString(0), "Task 1");
    EXPECT_EQ(newFrame->checkListBox->GetString(1), "Task 2");
    EXPECT_FALSE(newFrame->checkListBox->IsChecked(0));
    EXPECT_TRUE(newFrame->checkListBox->IsChecked(1));
    delete newFrame;
}
