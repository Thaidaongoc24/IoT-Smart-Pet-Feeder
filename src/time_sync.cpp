#include "time_sync.h"
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>



// Biến trạng thái của động cơ
bool feeding_timer = false;
int locks[4] = {0, 0, 0, 0};

struct Timer {
    unsigned long startTime; // Thời điểm kích hoạt (giây từ đầu ngày)
    bool isActive;           // Trạng thái của hẹn giờ (đang bật hay tắt)
};

Timer feedingTimers[4]; // Tối đa 4 hẹn giờ cho động cơ

// Cấu hình NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 900000); // UTC+7, cập nhật 15 phút một lần

void setupTimeSync() {
    timeClient.begin(); // Khởi động NTP Client
    timeClient.update(); // Đồng bộ thời gian ngay khi khởi tạo
}

void updateTimeSync() {
    timeClient.update(); // Cập nhật thời gian từ máy chủ NTP
}

// Lấy thời gian hiện tại ở dạng HH:MM:SS
String getCurrentTime() {
    return timeClient.getFormattedTime();
}

// Lấy số giây kể từ đầu ngày
unsigned long getSecondsSinceMidnight() {
    unsigned long epochTime = timeClient.getEpochTime(); // Lấy thời gian epoch (giây từ 01/01/1970)
    return epochTime % 86400; // Lấy phần dư của số giây trong ngày (1 ngày = 86400 giây)
}

void initializeTimers() {
    // Khởi tạo tất cả các hẹn giờ là không hoạt động
    for (int pump = 0; pump < 4; pump++) {
        for (int timer = 0; timer < 4; timer++) {
            feedingTimers[timer].startTime = 0;
            feedingTimers[timer].isActive = false;
        }
    }
}



// Thiết lập hẹn giờ cho động cơ
void SetFeedingTimer(int timerIndex, unsigned long startTimeInSeconds) {
    if (timerIndex < 0 || timerIndex > 3) {
        // Serial.println("Chỉ số hẹn giờ không hợp lệ!");
        return;
    }
    feedingTimers[timerIndex].startTime = startTimeInSeconds;
    feedingTimers[timerIndex].isActive = true;
}

void ProcessTimerString(String& input) {
    // Kiểm tra độ dài chuỗi đầu vào
    if (input.length() < 2) {
        input = ""; // Xóa chuỗi sau khi xử lý
        return;
    }

    // Lấy số đầu tiên (timerIndex)
    int timerIndex = input[0] - '0'; // Số đầu tiên

    // Kiểm tra tính hợp lệ của timerIndex
    if (timerIndex < 0 || timerIndex > 3) {
        // Serial.println("Chỉ số hẹn giờ không hợp lệ!");
        input = ""; // Xóa chuỗi sau khi xử lý
        return;
    }

    // Lấy phần còn lại sau khoảng trắng
    String remaining = input.substring(1);
    remaining.trim(); // Loại bỏ khoảng trắng thừa

    if (remaining == "off") {
        // Nếu chuỗi là "off", tắt bộ hẹn giờ
        feedingTimers[timerIndex].isActive = false;
        // Serial.println("Đã tắt hẹn giờ cho hẹn giờ " + String(timerIndex));
    } else {
        // Nếu chuỗi là số, chuyển đổi và gọi SetFeedingTimer
        unsigned long startTimeInSeconds = remaining.toInt();
        if (startTimeInSeconds > 0) {
            SetFeedingTimer(timerIndex, startTimeInSeconds);
            // Serial.println("Đã đặt hẹn giờ cho hẹn giờ " + String(timerIndex) + " vào " + String(startTimeInSeconds) + " giây.");
        } else {
            // Serial.println("Thời gian không hợp lệ!");
        }
    }

    // Xóa chuỗi sau khi xử lý
    input = "";
}


void checkAndActivateTimers() {
    unsigned long currentSeconds = getSecondsSinceMidnight();

    for (int timer = 0; timer < 4; timer++) {
        if (locks[timer]>0){
            locks[timer]--;
        }
        if (feedingTimers[timer].isActive &&
            feedingTimers[timer].startTime + 20 > currentSeconds && // Không được lớn hơn quá 1 phút. 1 phút phòng lệch giờ so với thực tế
            currentSeconds >= feedingTimers[timer].startTime) {
            
            if (locks[timer]<=0){
                // Kích hoạt động cơ
                feeding_timer = true;
                locks[timer] = 70;
                // nếu không muốn hẹn giờ hàng ngày thì bỏ comment cái này đi
                // feedingTimers[timer].isActive = false; // Vô hiệu hóa hẹn giờ sau khi kích hoạt
            }
        }
    }
}