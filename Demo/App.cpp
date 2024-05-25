#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* mainFrame = new MainFrame("C++ GUI");
    mainFrame->SetClientSize(800, 700);
    mainFrame->SetBackgroundColour(wxColor(246, 241, 147));
    mainFrame->Show();
    return true;
}