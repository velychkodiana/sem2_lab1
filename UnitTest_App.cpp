#include <gtest/gtest.h>
#include <wx/app.h>
#include <wx/frame.h>
#include "App.h"
#include "MainFrame.h"

// Макрос, що оголошує наш клас тесту як wxApp для wxWidgets
class TestApp : public App {
public:
    virtual bool OnInit() {
        // Викликаємо базовий метод для ініціалізації
        if (!App::OnInit())
            return false;

        // Тестовий код для перевірки створеного вікна
        mainFrame = wxDynamicCast(wxTheApp->GetTopWindow(), MainFrame);
        return true;
    }

    MainFrame* mainFrame = nullptr;
};

// Оголошуємо тестовий клас для wxWidgets
wxDECLARE_APP(TestApp);
wxIMPLEMENT_APP_NO_MAIN(TestApp);

class AppTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        // Ініціалізація wxWidgets
        int argc = 0;
        char** argv = nullptr;
        wxEntryStart(argc, argv);
        wxTheApp->CallOnInit();
    }

    virtual void TearDown() {
        wxTheApp->OnExit();
        wxEntryCleanup();
    }
};

TEST_F(AppTest, MainFrameCreation) {
    TestApp* app = wxDynamicCast(wxTheApp, TestApp);
    ASSERT_NE(app, nullptr);
    ASSERT_NE(app->mainFrame, nullptr);

    EXPECT_TRUE(app->mainFrame->IsShown());
    EXPECT_EQ(app->mainFrame->GetTitle(), "notty");
    EXPECT_EQ(app->mainFrame->GetSize().GetWidth(), 800);
    EXPECT_EQ(app->mainFrame->GetSize().GetHeight(), 600);
}
