#include <gtest/gtest.h>
#include <wx/app.h>
#include <wx/frame.h>
#include "App.h"
#include "MainFrame.h"

// оголошуємо клас тесту як wxApp для wxWidgets
class TestApp : public App
{
public:
    virtual bool OnInit() 
{
        // ініціалізуємо
        if (!App::OnInit())
            return false;

        //  перевіряємо чи створюється вікно
        mainFrame = wxDynamicCast(wxTheApp->GetTopWindow(), MainFrame);
        return true;
    }

    MainFrame* mainFrame = nullptr;
};

// оголошуємо тестовий клас для wxWidgets
wxDECLARE_APP(TestApp);
wxIMPLEMENT_APP_NO_MAIN(TestApp);

class AppTest : public ::testing::Test 
{
protected:
    virtual void SetUp() 
{
        // ініціалізуємо wxWidgets
        int argc = 0;
        char** argv = nullptr;
        wxEntryStart(argc, argv);
        wxTheApp->CallOnInit();
    }

    virtual void TearDown() 
{
        wxTheApp->OnExit();
        wxEntryCleanup();
    }
};

TEST_F(AppTest, MainFrameCreation) 
{
    TestApp* app = wxDynamicCast(wxTheApp, TestApp);
    ASSERT_NE(app, nullptr);
    ASSERT_NE(app->mainFrame, nullptr);

    EXPECT_TRUE(app->mainFrame->IsShown());
    EXPECT_EQ(app->mainFrame->GetTitle(), "notty");
    EXPECT_EQ(app->mainFrame->GetSize().GetWidth(), 800);
    EXPECT_EQ(app->mainFrame->GetSize().GetHeight(), 600);
}
