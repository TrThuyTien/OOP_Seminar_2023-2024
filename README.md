# Seminar OOP BHT Đoàn khoa MMT&TT

- Đây là project thuộc BHT Đoàn khoa MMT&TT. Buổi Seminar OOP diễn ra vào ngày 25/05/2024

## Tổng quan về project

- Một project mô phỏng một chiếc thang máy tại UIT sử dụng wxWidget để tạo GUI và được viết bằng ngôn ngữ C++. 


## Cài đặt môi trường và tổng quan về wxWidgets
- Do việc sử dụng wxWidgets để tạo GUI nên cần thêm vào Visual Studio môi trường để có thể sử dụng được: [LINK HƯỚNG DẪN CÀI ĐẶT](https://www.youtube.com/watch?v=ONYW3hBbk-8&list=PLFk1_lkqT8MbVOcwEppCPfjGOGhLvcf9G&index=2&ab_channel=OttoBotCode)



```python
import foobar

# returns 'words'
foobar.pluralize('word')

# returns 'geese'
foobar.pluralize('goose')

# returns 'phenomenon'
foobar.singularize('phenomena')
```

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
# Hàm auto&btn sẽ tự động tìm kiếm các ID riêng tương ứng đối với từng tầng 
# Nhìn thấy được đối với mỗi nút sẽ đều thao tác với một hàm là OnButtonClick
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
