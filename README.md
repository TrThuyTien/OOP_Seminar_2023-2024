# Seminar OOP

- Đây là project made by OOP 2 Món. Buổi Seminar OOP diễn ra vào ngày 25/05/2024

## Tổng quan về project

- Một project mô phỏng một chiếc thang máy tại UIT sử dụng wxWidget để tạo GUI và được viết bằng ngôn ngữ C++. 


## Cài đặt môi trường và tổng quan về wxWidgets
- Do việc sử dụng wxWidgets để tạo GUI nên cần thêm vào Visual Studio môi trường để có thể sử dụng được: [LINK HƯỚNG DẪN CÀI ĐẶT](https://www.youtube.com/watch?v=ONYW3hBbk-8&list=PLFk1_lkqT8MbVOcwEppCPfjGOGhLvcf9G&index=2&ab_channel=OttoBotCode)

## Giải thích một số dòng lệnh quan trọng trong project

- Chúng ta sẽ tập trung chủ yếu vào 2 file chính Mainframe.cpp và User.cpp (Các thông tin về thuộc tính và hàm xem tại file .h)
- Trước hết chúng ta sẽ xem qua việc xử lý các nút ra sao khi đã có GUI.
```cpp
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
// Hàm auto&btn sẽ tự động tìm kiếm các ID riêng tương ứng đối với từng tầng 
// Nhìn thấy được đối với mỗi nút sẽ đều thao tác với một hàm là OnButtonClick
```
- Tiếp tục là hàm xử lý các sự kiện khi có EVENT nhấn các nút xảy ra. 
```cpp
void MainFrame::OnButtonClick(wxCommandEvent& event)
{
    int id = event.GetId() - ID_Button0; //Lấy ID
 
   //Đoạn dưới đây là xử lý khi nhấn các nút Up mỗi tầng
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

    //Xử lý nút Down mỗi tầng
    else if (id >= 23) {
        id -= 23;

        floorQueue.insert(floorQueue.begin(), id);
    }


     //Xử lý các nút bấm từ tầng G -> Tầng 9
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

     //Xử lý nút BELL
    if (id == 10) {
        btn_bell.button->SetBackgroundColour(*wxRED);
        btn_bell.button->SetForegroundColour(*wxWHITE);
        btn_bell.status = true;
        btn_bell.button->Refresh();
    }

    // Xử lý OpenDoor
    if (id == 11) {
        OpenDoor();
    }

    // Xử lý CloseDoor
    if (id == 12) {
        CloseDoor();
    }
}
```
- Xét khi thang máy có yêu cầu thì thực hiện hàm MoveElevator
```cpp
void MainFrame::OnTimer(wxTimerEvent& event)
{
    if (!floorQueue.empty()) { 
        MoveElevator();
    }
}
```
Hàm di chuyển thang máy, lấy tầng được ưu tiên lên/hay xuống ở hàng đợi queue, lấy giá trị đầu tiên, khi đi tới lập tức bỏ tầng đó ra khỏi hàng đợi
```cpp
void MainFrame::MoveElevator()
{
    int targetFloor;
    if (movingUp) targetFloor = floorQueue.front(); 
    else targetFloor = floorQueue.front();
    if (currentFloor < targetFloor) //Nếu chưa tới tầng mong muốn, thì tiếp tục tăng và di chuyên lên trên cho đến khi đến tầng mong muốn
    {
        currentFloor++;
        movingUp = true;
    }
    else if (currentFloor > targetFloor) //Ngược lại thì cho dừng, không di chuyển nữa
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

        if (human.getWeight() <= 1500 && human.luot == -1) { //Nếu tải trọng của thang máy không vượt quá 1500 và còn lượt vào thang máy thì thực hiện mở cửa thang máy và cập nhật lại cân nặng, chiều cao

            UpdateWeight();
            UpdateUser();
            OpenDoor();
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Thời gian cửa thang máy mở
            CloseDoor();
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Thời gian cửa thang máy mở
            bellLabel->SetLabel("");

        }
        else if (human.luot != -1 && floorQueue.size() > 0) // 
        {
            OpenDoor1();
            std::this_thread::sleep_for(std::chrono::seconds(3)); // Thời gian cửa thang máy mở
            CloseDoor();
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Thời gian cửa thang máy mở
            bellLabel->SetLabel("");
        }




    }
```
- Cập nhật thông tin trên màn hình thang máy khi tất cả mọi người đi ra
```cpp
    UpdateFloorDisplay(); Cập nhật màn hình thang máy
    if (floorQueue.empty() && human.luot == 0) // Nếu như đến một tầng nào đó, thang máy mở cửa và tất cả mọi người đi ra hết, không còn ai trong thang máy thì thông báo
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
