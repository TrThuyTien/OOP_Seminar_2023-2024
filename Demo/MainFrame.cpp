#include "MainFrame.h"
#include <queue>
#include <deque>
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

enum {
    ID_Timer = 1,
    ID_Button0,
    ID_Button1,
    ID_Button2,
    ID_Button3,
    ID_Button4,
    ID_Button5,
    ID_Button6,
    ID_Button7,
    ID_Button8,
    ID_Button9,
    ID_ButtonBell,
    ID_ButtonOpen,
    ID_ButtonClose,
    ID_ButtonUp0,
    ID_ButtonUp1,
    ID_ButtonUp2,
    ID_ButtonUp3,
    ID_ButtonUp4,
    ID_ButtonUp5,
    ID_ButtonUp6,
    ID_ButtonUp7,
    ID_ButtonUp8,
    ID_ButtonUp9,
    ID_ButtonDown0,
    ID_ButtonDown1,
    ID_ButtonDown2,
    ID_ButtonDown3,
    ID_ButtonDown4,
    ID_ButtonDown5,
    ID_ButtonDown6,
    ID_ButtonDown7,
    ID_ButtonDown8,
    ID_ButtonDown9
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(ID_Button0, MainFrame::OnButtonClick)
EVT_BUTTON(ID_Button1, MainFrame::OnButtonClick)
EVT_BUTTON(ID_Button2, MainFrame::OnButtonClick)
EVT_BUTTON(ID_Button3, MainFrame::OnButtonClick)
EVT_BUTTON(ID_Button4, MainFrame::OnButtonClick)
EVT_BUTTON(ID_Button5, MainFrame::OnButtonClick)
EVT_BUTTON(ID_Button6, MainFrame::OnButtonClick)
EVT_BUTTON(ID_Button7, MainFrame::OnButtonClick)
EVT_BUTTON(ID_Button8, MainFrame::OnButtonClick)
EVT_BUTTON(ID_Button9, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonBell, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonOpen, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonClose, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp0, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown0, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp1, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown1, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp2, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown2, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp3, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown3, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp4, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown4, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp5, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown5, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp6, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown6, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp7, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown7, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp8, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown8, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonUp9, MainFrame::OnButtonClick)
EVT_BUTTON(ID_ButtonDown9, MainFrame::OnButtonClick)

EVT_TIMER(ID_Timer, MainFrame::OnTimer)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(300, 600)),
currentFloor(0), movingUp(true)
{
    CreateControls();
    BindEvents();

    timer = new wxTimer(this, ID_Timer);
    timer->Start(1000);
}

// Hàm tạo màn hình hiển thị các nút

void MainFrame::CreateControls()
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    floorDisplay = new wxStaticText(panel, wxID_ANY, "Floor: G", wxPoint(110, 20), wxSize(80, 20), wxALIGN_CENTER);
    floorDisplay->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // Outside
    // Tạo nút bấm
    for (int i = 0; i < 10; ++i) {
        Button btn;
        if (i % 2 == 0) {
            if (i == 0) btn.button = new wxButton(panel, ID_Button0 + i, "G", wxPoint(100, 60 + i * 30), wxSize(50, 40));
            else btn.button = new wxButton(panel, ID_Button0 + i, std::to_string(i), wxPoint(100, 60 + i * 30), wxSize(50, 40));
        }
        else btn.button = new wxButton(panel, ID_Button0 + i, std::to_string(i), wxPoint(200, 60 + (i - 1) * 30), wxSize(50, 40));
        btn.status = false;
        floorButtons.push_back(btn);
    }

    // Tạo nút chuông
    btn_bell.button = new wxButton(panel, ID_ButtonBell, "BELL", wxPoint(300, 180), wxSize(70, 70));
    btn_bell.status = false;
    btn_bell.button->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    btn_bell.button->SetForegroundColour(*wxRED);

    // Tạo nút mở
    btn_Open.button = new wxButton(panel, ID_ButtonOpen, "<||>", wxPoint(100, 400), wxSize(50, 40));
    btn_Open.button->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    // Tạo nút đóng
    btn_Close.button = new wxButton(panel, ID_ButtonClose, ">||<", wxPoint(200, 400), wxSize(50, 40));
    btn_Close.button->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    bellLabel = new wxStaticText(panel, wxID_ANY, "", wxPoint(300, 50), wxSize(200, -1));

    for (int i = 0; i < 10; ++i) {
        Button Up;
        if (i == 0)
        {
            outsideLabel = new wxStaticText(panel, wxID_ANY, "Floor: G", wxPoint(475, 50 + i * 60), wxSize(200, -1));
        }
        else
        {
            if (i & 1)
                outsideLabel = new wxStaticText(panel, wxID_ANY, "Floor: " + to_string(i), wxPoint(600, 45 + (i - 1) * 60), wxSize(50, 15));
            else
                outsideLabel = new wxStaticText(panel, wxID_ANY, "Floor: " + to_string(i), wxPoint(500, 45 + i * 60), wxSize(50, 15));
        }
        if (i == 0)
        {
            btn_Up.button = new wxButton(panel, ID_ButtonUp0 + i, "Up", wxPoint(475, 70 + i * 60), wxSize(50, 40));
            Up.status = false;

        }
        if (i != 0 and i != 9)
        {
            if (i % 2 == 0)
            {
                btn_Up.button = new wxButton(panel, ID_ButtonUp0 + i, "Up", wxPoint(450, 60 + i * 60), wxSize(50, 40));
                Up.status = false;
                Button Down;
                btn_Down.button = new wxButton(panel, ID_ButtonDown0 + i, "Down", wxPoint(500, 60 + i * 60), wxSize(50, 40));
                Down.status = false;
            }
            else
            {
                btn_Up.button = new wxButton(panel, ID_ButtonUp0 + i, "Up", wxPoint(600, 65 + (i - 1) * 60), wxSize(50, 40));
                Up.status = false;
                Button Down;
                btn_Down.button = new wxButton(panel, ID_ButtonDown0 + i, "Down", wxPoint(650, 65 + (i - 1) * 60), wxSize(50, 40));
                Down.status = false;
            }
        }
        if (i == 9)
        {
            Button Down;
            btn_Down.button = new wxButton(panel, ID_ButtonDown0 + i, "Down", wxPoint(625, 60 + (i - 1) * 60), wxSize(50, 40));
            Down.status = false;
        }
    }

    // Tạo dòng chữ hiển thị số cân hiện tại
    weightDisplay = new wxStaticText(panel, wxID_ANY, "Weight: 0", wxPoint(500, 20), wxSize(80, 20), wxALIGN_CENTER);
    userDisplay = new wxStaticText(panel, wxID_ANY, "User: 0", wxPoint(600, 20), wxSize(80, 20), wxALIGN_CENTER);
}

// Hàm này nghe sự kiện khi click chuột
void MainFrame::BindEvents()
{
    for (auto& btn : floorButtons) {
        btn.button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
    }
    btn_bell.button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
    btn_Open.button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
    btn_Close.button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
    btn_Up.button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
    btn_Down.button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);


}

// Khi có sự kiện click chuột thì hàm này xử lý các tầng

void MainFrame::OnButtonClick(wxCommandEvent& event)
{
    int id = event.GetId() - ID_Button0;
    if (id >= 13 && id <= 22) {
        id -= 13;
        if (movingUp) {
            auto insertPos = std::find_if(floorQueue.begin(), floorQueue.end(), [id](int floor) {
                return floor > id;
                });
            floorQueue.insert(insertPos, id);
        }
        else {
            floorQueue.insert(floorQueue.end(), id);
        }
    }

    else if (id >= 23) {
        id -= 23;

        floorQueue.insert(floorQueue.begin(), id);
    }

    else if (id >= 0 && id <= 9) {
        dem++;
        human.luot = 0;
        human.luot++;
        // Kiểm tra xem tầng đã được bấm có trong hàng đợi không
        auto it = std::find(floorQueue.begin(), floorQueue.end(), id);
        if (it != floorQueue.end()) {
            // Nếu đã có trong hàng đợi, loại bỏ nó ra
            floorQueue.erase(it);
        }
        // Chèn lại tầng vào hàng đợi ở vị trí thích hợp dựa trên hướng di chuyển
        if (movingUp) {
            auto insertPos = std::find_if(floorQueue.begin(), floorQueue.end(), [id](int floor) {
                return floor > id;
                });
            floorQueue.insert(insertPos, id);
            human.luot = human.luot - 1;
        }
        else {
            auto insertPos = std::find_if(floorQueue.begin(), floorQueue.end(), [id](int floor) {
                return floor < id;
                });
            floorQueue.insert(insertPos, id);
            human.luot = human.luot - 1;
        }
        // Cập nhật màu sắc và trạng thái của nút bấm
        floorButtons[id].button->SetBackgroundColour(*wxRED);
        floorButtons[id].status = true;
        floorButtons[id].button->Refresh();
    }
    if (id == 10) {
        btn_bell.button->SetBackgroundColour(*wxRED);
        btn_bell.button->SetForegroundColour(*wxWHITE);
        btn_bell.status = true;
        btn_bell.button->Refresh();
    }
    if (id == 11) {
        OpenDoor();
    }
    if (id == 12) {
        CloseDoor();
    }
}

// Xét khi thang máy có yêu cầu thì thực hiện hàm MoveElevator
void MainFrame::OnTimer(wxTimerEvent& event)
{
    if (!floorQueue.empty()) {
        MoveElevator();
    }
}

// Hàm di chuyển thang máy, lấy tầng được ưu tiên lên/hay xuống ở hàng đợi queue, lấy giá trị đầu tiên, khi đi tới lập tức bỏ tầng đó ra khỏi hàng đợi
void MainFrame::MoveElevator()
{
    int targetFloor;
    if (movingUp) targetFloor = floorQueue.front();
    else targetFloor = floorQueue.front();
    if (currentFloor < targetFloor)
    {
        currentFloor++;
        movingUp = true;
    }
    else if (currentFloor > targetFloor)
    {
        currentFloor--;
        movingUp = false;
    }
    else
    {
        if (movingUp) floorQueue.pop_front();
        else floorQueue.pop_front();
        floorButtons[currentFloor].button->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        floorButtons[currentFloor].status = false;
        floorButtons[currentFloor].button->Refresh();
        currentWeight = human.getWeight();

        if (human.getWeight() <= 1500 && human.luot == -1) {

            UpdateWeight();
            UpdateUser();
            OpenDoor();
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Time the door stays open
            CloseDoor();
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Time the door stays open
            bellLabel->SetLabel("");

        }
        else if (human.luot != -1 && floorQueue.size() > 0)
        {
            OpenDoor1();
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Time the door st\ays open
            CloseDoor();
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Time the door stays open
            bellLabel->SetLabel("");
        }




    }
    UpdateFloorDisplay();
    if (floorQueue.empty() && human.luot == 0)
    {
        human.setWeight(0);
        UpdateWeight();

        human.luot = -1;
        human.soluong = 0;
        UpdateUser();
        UpdateLabel("Door Open");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        UpdateLabel("Door Closed, all people go out");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        UpdateLabel("");

    }


}

// Hàm này update tầng hiện tại
void MainFrame::UpdateFloorDisplay()
{
    if (currentFloor == 0) floorDisplay->SetLabel("Floor: G");
    else floorDisplay->SetLabel("Floor: " + std::to_string(currentFloor));
    floorDisplay->Refresh();
}

void MainFrame::UpdateWeight() {
    weightDisplay->SetLabel("Weight: " + std::to_string(human.getWeight()));
    weightDisplay->Refresh();
}

void MainFrame::UpdateUser()
{
    userDisplay->SetLabel("User: " + std::to_string(human.getUser()));
    userDisplay->Refresh();
}

void MainFrame::OpenDoor()
{
    doorOpen = true;
    human.addUser();
    human.getWeightrandom();

    UpdateWeight();
    UpdateUser();
    UpdateLabel("Door Open");
}

void MainFrame::OpenDoor1()
{
    doorOpen = true;
    dem++;
    if (dem >= 1)
    {
        human.subUser();
        human.truWeight();
        dem = 0;
    }
    UpdateWeight();
    UpdateUser();
    UpdateLabel("Door Open");

}

void MainFrame::CloseDoor()
{
    doorOpen = false;
    UpdateWeight();
    UpdateUser();
    UpdateLabel("Door Closed, Someone go out");
}

void MainFrame::CloseDoor1()
{
    doorOpen = false;

    UpdateLabel("Door Closed, Someone go out");
}

void MainFrame::SetLabel(wxStaticText* label)
{
    bellLabel = label;
}

void MainFrame::UpdateLabel(const wxString& text)
{
    if (bellLabel) {
        bellLabel->SetLabel(text);
        bellLabel->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    }
}