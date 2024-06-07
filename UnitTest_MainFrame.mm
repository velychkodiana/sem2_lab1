#include <gtest/gtest.h>
#include <wx/wx.h>
#include <wx/sizer.h>
#include "MainFrame.h"
#include "Task.h"

class MainFrameTest : public ::testing::Test {
protected:
    wxApp* app;
    MainFrame* frame;

    virtual void SetUp() {
        int argc = 0;
        char** argv = nullptr;
        app = new wxApp();
        wxApp::SetInstance(app);
        wxEntryStart(argc, argv);
        frame = new MainFrame("Test Frame");
        frame->Show();
    }

    virtual void TearDown() {
        frame->Close(true);
        wxEntryCleanup();
        delete app;
    }
};

TEST_F(MainFrameTest, AddTask) {
    frame->inputField->SetValue("Test Task");
    wxCommandEvent evt(wxEVT_BUTTON, frame->addButton->GetId());
    frame->OnAddButtonClicked(evt);
    EXPECT_EQ(frame->checkListBox->GetCount(), 1);
    EXPECT_EQ(frame->checkListBox->GetString(0), "Test Task");
}

TEST_F(MainFrameTest, DeleteTask) {
    frame->inputField->SetValue("Test Task");
    wxCommandEvent evt(wxEVT_BUTTON, frame->addButton->GetId());
    frame->OnAddButtonClicked(evt);

    frame->checkListBox->SetSelection(0);
    wxKeyEvent delEvt(wxEVT_KEY_DOWN);
    delEvt.m_keyCode = WXK_DELETE;
    frame->OnListKeyDown(delEvt);
    EXPECT_EQ(frame->checkListBox->GetCount(), 0);
}

TEST_F(MainFrameTest, MoveTask) {
    frame->inputField->SetValue("Task 1");
    wxCommandEvent evt1(wxEVT_BUTTON, frame->addButton->GetId());
    frame->OnAddButtonClicked(evt1);

    frame->inputField->SetValue("Task 2");
    wxCommandEvent evt2(wxEVT_BUTTON, frame->addButton->GetId());
    frame->OnAddButtonClicked(evt2);

    frame->checkListBox->SetSelection(1);
    wxKeyEvent upEvt(wxEVT_KEY_DOWN);
    upEvt.m_keyCode = WXK_UP;
    frame->OnListKeyDown(upEvt);

    EXPECT_EQ(frame->checkListBox->GetString(0), "Task 2");
    EXPECT_EQ(frame->checkListBox->GetString(1), "Task 1");
}

TEST_F(MainFrameTest, SaveAndLoadTasks) {
    std::vector<Task> tasks = {{"Task 1", false}, {"Task 2", true}};
    SaveTasksToFile(tasks, "test_tasks.txt");
    
    MainFrame* newFrame = new MainFrame("Test Frame");
    newFrame->AddSavedTasks();
    EXPECT_EQ(newFrame->checkListBox->GetCount(), 2);
    EXPECT_EQ(newFrame->checkListBox->GetString(0), "Task 1");
    EXPECT_EQ(newFrame->checkListBox->GetString(1), "Task 2");
    EXPECT_FALSE(newFrame->checkListBox->IsChecked(0));
    EXPECT_TRUE(newFrame->checkListBox->IsChecked(1));
    delete newFrame;
}
