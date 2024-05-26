# Seminar OOP

- Đây là project made by OOP 2 Món. Buổi Seminar OOP diễn ra vào ngày 25/05/2024

## Tổng quan về project

- Một project ứng dụng Lập trình hướng đối tượng kết hợp framework wxWidget để mô phỏng một chiếc thang máy tại UIT. 

## Tính năng
- Bảng điều khiển (các nút bấm) của thang máy
- Nút chuông dùng để cảnh báo
- Mở cửa, đóng cửa
- Hiện thị số người và cân nặng hiện tại của thang máy


## Cài đặt 
### Yêu cầu hệ thống
- Ngôn ngữ: C++
- Phần mềm code: Visual Code
- Framework: [wxWidget](https://www.youtube.com/watch?v=ONYW3hBbk-8&list=PLFk1_lkqT8MbVOcwEppCPfjGOGhLvcf9G&index=2&ab_channel=OttoBotCode)

### Xây dựng giao diện (GUI)
- Tạo khung cho dự án (ở file App.h và App.cpp)
```cpp
bool App::OnInit()
{
    MainFrame* mainFrame = new MainFrame("C++ GUI");
    mainFrame->SetClientSize(800, 700);
    mainFrame->SetBackgroundColour(wxColor(246, 241, 147));
    mainFrame->Show();
    return true;
}
```

- Tạo các nút bấm (ví dụ tạo 9 nút bấm nằm ở trong thang máy, các nút còn lại tương tự)
```cpp
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
```
### Giải thích một số dòng lệnh quan trọng trong project

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
- Hàm di chuyển thang máy, lấy tầng được ưu tiên lên/hay xuống ở hàng đợi queue, lấy giá trị đầu tiên, khi đi tới lập tức bỏ tầng đó ra khỏi hàng đợi
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
```

- Tạo đối tượng User -> để kiểm soát cân nặng và số người đi vào
```cpp
class User
{
private:
	int Weight;
	int Height;
public:
	int soluong; // Số lượng người
	int luot;
	User();
	int getWeight();
	int getWeightrandom(); // Hàm lấy tổng số cân nặng khi có một nhóm người vào thang máy
	void truWeight(); // Trừ cân nặng khi có người đi ra
	void setWeight(int weight);
	void subUser(); // Trừ số người khi có người đi ra
	void addUser(); // Cộng thêm số người khi có người đi vào thang máy
	int getUser();
	void setUser();
};
```
- Tạo ngẫu nhiên số người khi nhấn nút inside (phần này có thể thay thế bằng dữ liệu thực tế nếu có) 
```cpp
void User::addUser()
{
	this->soluong += 1 + rand() % 10;
}
```
- Trừ số lượng ngẫu nhiên User khi tới 1 tới 1 tầng
```cpp
void User::subUser()
{
	int currentUser = this->soluong - 1;
	if (currentUser > 0)
		this->soluong -= (rand() % currentUser) + 1;
}
```
- Lấy cân nặng random ứng với số lượng tạo ra
```cpp
int User::getWeightrandom()
{
	int preWeight = this->Weight;
	this->Weight += (rand() % 70 + 50) * soluong;
	return preWeight;
}
```
- Trừ cân nặng ứng với từng user đi ra 
```cpp
void User::truWeight()
{
	int newWeight = this->Weight;
	int moi = (rand() & 70 + 50) * soluong;
	if (newWeight - moi > 50)
	{
		this->Weight -= moi;
	}
	else 
	{
		this->Weight = 50;
	}

}
```

# License
MIT License

Copyright (c) 2024 TrThuyTien

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

# Liên hệ
- Author: Phạm Thái Bảo - mail: 21520156@gm.uit.edu.vn
- Co-Author: Đỗ Quang Trung - mail: 23521673@gm.uit.edu.vn
- Co-Author: Trần Thị Thùy Tiên - mail: 23521588@gm.uit.edu.vn
- Co-Author: Nguyễn Lê Hạ My - mail: 23520964@gm.uit.edu.vn
