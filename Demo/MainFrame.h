#pragma once

#include <wx/wx.h>
#include <wx/timer.h>
#include <vector>
#include <queue>
#include <deque>
#include "User.h"


// status là thể hiện tầng đó có được yêu cầu hay ko, true là có, false là không
struct Button {
    wxButton* button;
    bool status;
};

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
    int dem = -1;
private:
    void OpenDoor1();
    void CreateControls();
    void BindEvents();

    void OnButtonClick(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);

    void UpdateFloorDisplay();
    void UpdateWeight();
    void UpdateUser();
    void MoveElevator();
    void OverWeight();
    void OpenDoor();
    void CloseDoor();
    void SetLabel(wxStaticText* label);
    void UpdateLabel(const wxString& text);

    std::vector<Button> floorButtons;
    Button btn_bell;
    Button btn_Open;
    Button btn_Close;
    Button btn_Up;
    Button btn_Down;
    bool doorOpen;
    wxStaticText* floorDisplay;
    wxStaticText* outsideLabel;
    wxStaticText* bellLabel;
    wxStaticText* weightDisplay;
    wxStaticText* overWeight;
    wxStaticText* userDisplay;
    wxTimer* timer;
    std::deque<int> floorQueue;
    int currentFloor;
    int currentWeight;
    int currentUser;
    bool movingUp;
    void CloseDoor1();

    User human;

    wxDECLARE_EVENT_TABLE();
};
