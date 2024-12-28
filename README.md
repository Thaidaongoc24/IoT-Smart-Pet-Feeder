# IoT Smart Pet Feeder

## 🐾 Mô tả dự án

Hệ thống **IoT Smart Pet Feeder** giúp bạn tự động hóa việc cho thú cưng ăn, đảm bảo chúng được chăm sóc đúng giờ và đầy đủ. Sử dụng công nghệ IoT, hệ thống cho phép bạn quản lý lịch trình cho ăn, theo dõi hoạt động qua ứng dụng và điều khiển từ xa thông qua MQTT.

---

## 📚 Đặt vấn đề

Việc chăm sóc thú cưng đòi hỏi sự kiên nhẫn và thời gian, đặc biệt là khi bạn có lịch trình bận rộn. Bỏ lỡ bữa ăn hoặc cho ăn không đúng giờ có thể ảnh hưởng đến sức khỏe của chúng. **IoT Smart Pet Feeder** được phát triển để giải quyết vấn đề này bằng cách cung cấp một giải pháp tự động và dễ dàng quản lý qua Internet.

---
## 🌟 Tính năng chính

- **Tự động hóa:** Lên lịch và thực hiện việc cho ăn tự động theo giờ định sẵn.
- **Điều khiển từ xa:** Quản lý qua Dashboard Node-RED **trên trực tiếp internet mà không phải localhost**.
- **Theo dõi:** Gửi thông báo và hiển thị trạng thái thiết bị, lượng thức ăn còn hay hết.
- **Phát hiện chuyển động:** Xác định khi thú cưng đến gần thiết bị.
- **Cảm biến hết thức ăn:** Gửi cảnh báo khi thức ăn trong thiết bị hết.
- **Tùy chỉnh:** Điều chỉnh khẩu phần ăn phù hợp với từng loại thú cưng.
- **Thiết lập WIFI:** Cho phép thiết lập lại kết nối WIFI khi mất kết nối hoặc kết nối lần đầu từ giao diện người dùng mà không cần phải nạp lại code.

---
## 🔧 Sơ đồ hệ thống

![Hình 1](https://github.com/user-attachments/assets/7dcd48c7-1a83-4ef8-9ced-e5b2e1b9362a)

![IoT Cuối kỳ drawio](https://github.com/user-attachments/assets/75d7d6ad-598a-42d2-a314-8813c8f87721)


### **Phần cứng:**
  1. **ESP8266 NodeMCU**
     - **Chức năng:** Điều khiển toàn bộ hệ thống và giao tiếp với MQTT qua Wi-Fi.
     - **Ưu điểm:**
       - Tích hợp Wi-Fi, dễ dàng giao tiếp với các hệ thống IoT.
       - Giá thành rẻ *(rẻ hơn ESP32)*, phù hợp với các dự án DIY.
       - Có nhiều GPIO để kết nối với các cảm biến và module.
  
  2. **Động cơ bước 28BYJ-48 kèm Driver ULN2003**
     - **Chức năng:** Điều khiển việc đóng/mở cửa phân phối thức ăn.
     - **Ưu điểm:**
       - Hoạt động chính xác, dễ dàng điều khiển vị trí dừng.
       - Bền bỉ, phù hợp cho các thiết bị cần độ ổn định cao.
       - Đi kèm module driver ULN2003, giúp đơn giản hóa việc kết nối.
  
  3. **Cảm biến chuyển động SR602 (PIR)**
     - **Chức năng:** Phát hiện sự hiện diện của thú cưng gần hệ thống.
     - **Ưu điểm:**
       - Phạm vi phát hiện rộng, độ nhạy cao.
       - Tiêu thụ năng lượng thấp, phù hợp cho các hệ thống IoT.
  
  4. **Cảm biến hồng ngoại phát hiện mức thức ăn**
     - **Chức năng:** Kiểm tra xem liệu thiết bị còn thức ăn hay không.
     - **Ưu điểm:**
       - Hoạt động ổn định với chi phí thấp.
       - Đơn giản trong việc lắp đặt và tích hợp vào hệ thống.
  
  5. **Loa phát âm thanh buzzer**
     - **Chức năng:** Gọi thú cưng đến khi thức ăn được phân phối.
     - **Ưu điểm:**
       - Nhỏ gọn, dễ tích hợp.
       - Tạo âm thanh rõ ràng để thu hút sự chú ý của thú cưng.
  
  6. **Nguồn DC 5V**
     - **Chức năng:** Cung cấp nguồn điện cho toàn bộ hệ thống.
     - **Ưu điểm:**
       - Ổn định, đáp ứng đủ nhu cầu cho các thành phần của hệ thống.
       - Phổ biến, dễ tìm kiếm trên thị trường.
---

## 📋 Cài đặt
### **Cấu hình HiveMQ:**
   - Đăng ký tài khoản trên HiveMQ và tạo MQTT broker.

### **Cấu hình Node-RED:**
   #### Node-red flow:
![image](https://github.com/user-attachments/assets/f2054c4b-0220-42ec-a691-3539164e77ec)

  #### Node-red dashboard:

![image](https://github.com/user-attachments/assets/cc987c0b-f811-4ca5-b6e8-900c17cc34f0)

### **Cài đặt VPS của AWS để chạy Node-RED online:**
- Có thể truy cập Node-RED qua internet tại địa chỉ: http://13.211.141.181/ (với username là: iothus ; password là: test) để cài đặt FLOW
- Truy cập Dashboard tại địa chỉ; http://13.211.141.181/ui để điều khiển Pet Feeder

Tải và lưu file khoá .pem.
---
### **Thiết kế 3D vỏ sản phẩm và in 3D:**
  #### Sử dụng phần mềm Fusion360 để thiết kế và chỉnh sửa 3D vỏ nhựa máy chia thức ăn.
  <img src="https://github.com/user-attachments/assets/e2aadc0b-d4b1-48f4-824c-9c4572de8ecd" alt="Hình2" style="width:50%;"/>
    
(Mô hình 3D trên được tham khảo tại https://www.thingiverse.com/thing:4826741 và có chỉnh sửa lại cho phù hợp)
  #### Sử dụng máy in 3D Adventurer 3 của Bộ môn Vật lý Vô tuyến tại phòng 216T1 để in sản phẩm:
<div style="display: flex; justify-content: space-around; align-items: center;">
  <img src="https://github.com/user-attachments/assets/4722b124-6102-4569-b5bb-fdd0734ac458" alt="in1" style="width:30%;"/>
  <img src="https://github.com/user-attachments/assets/09b625db-04c1-4013-9cf3-0b999e962f60" alt="in2" style="width:40%;"/>
</div>


## 🚀 Kết quả

### Hình ảnh và video

- **Hình ảnh:**

- **Video:**


### Kết quả đạt được

- 

