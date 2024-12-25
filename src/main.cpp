#include <Arduino.h>
extern "C" {
  #include "osapi.h"
  #include "os_type.h"
}
#include "wifi_mqtt.h"
#include "sensor.h"
#include "motor.h"
#include "time_sync.h"
#include "speaker.h"

// Bộ đếm thời gian cho động cơ
os_timer_t motorTimer;

// Các chân điều khiển động cơ
#define IN1 D6 
#define IN2 D8
#define IN3 D2
#define IN4 D1

// Loa và các cảm biến
#define SPEAKER D0 //Buzzer
#define FOODSENSOR D5 //Cảm biến hồng ngoại
#define MOTIONSENSOR D7 // Cảm biến chuyển động

// Biến thời gian cho các tác vụ định kỳ
unsigned long current_time;
unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;
unsigned long time4 = 0;
unsigned long time5 = 0;
unsigned long time6 = 0;

// Biến trạng thái
bool hasSetup = false;
bool motionNoti = false;
int totalFood = 0;
int hungry = 0;

// Hàm callback cho bộ đếm thời gian điều khiển động cơ
void motorTimerCallback(void *pArg) {
    if (motorOn) {
        rotateMotor(); // Quay động cơ
    }
}

// Hàm hẹn giờ đa dụng
int Timer(unsigned long *time, int wait) {
    current_time = millis();
    if (current_time - *time > wait) {
        *time = current_time;
        return 1;
    }
    return 0;
}

// Cập nhật trạng thái hệ thống
void updateStatus() {
    if (isAuto && feeding_timer) {
        status = true;
        feeding_timer = false; // Đặt lại bộ hẹn giờ
    }

    if (status && !hasSetup) {
        int revolutions = foodAmount * 2 / 3; // Tính số vòng quay động cơ
        setRotate(revolutions);
        setCalls(5);
        speakerOn = true;
        hasSetup = true;
    }

    if (status && sensor[0]) {
        setCalls(1);
        speakerOn = true;
        status = false;
        motorOn = true;
        hasSetup = false;
        totalFood += foodAmount;
        publishData("chart", String(foodAmount).c_str());
        hungry = 1440; // Đặt lại thời gian thông báo đói
    }
}

void setup() {
    Serial.begin(115200);

    setupSpeaker(SPEAKER);
    setupMotor(IN1, IN2, IN3, IN4);
    setupSensors(MOTIONSENSOR, FOODSENSOR);
    setupWiFi();               // Cấu hình WiFi
    setupMQTT();               // Cấu hình MQTT
    setupTimeSync();           // Đồng bộ thời gian

    // Cấu hình bộ đếm thời gian phần mềm cho động cơ
    os_timer_setfn(&motorTimer, motorTimerCallback, NULL);
    os_timer_arm(&motorTimer, 1, true); // Đặt bộ đếm thời gian kích hoạt mỗi 1ms
}

void loop() {
    handleMQTT(); // Xử lý kết nối MQTT

    if (Timer(&time1, 3000)) {
        if (!mqtt_connected) {
            connect_MQTT();
        }
    }

    if (Timer(&time2, 500)) {
        readSensors();
        if (sensor[0]) {
            motionNoti = true;
        }
    }

    if (Timer(&time3, 400)) {
        updateStatus(); // Cập nhật trạng thái máy cho ăn
    }

    if (Timer(&time4, 991)) {
        ProcessTimerString(mqttMessage); // Xử lý chuỗi hẹn giờ
        checkAndActivateTimers();       // Kích hoạt các bộ hẹn giờ
        if (getSecondsSinceMidnight() < 2) {
            totalFood = 0; // Đặt lại tổng lượng thức ăn vào đầu ngày mới
        }
    }

    if (Timer(&time5, waitSpeaker)) {
        if (speakerOn) {
            callPet(); // Kích hoạt loa để gọi thú cưng
        }
    }

    if (Timer(&time6, 5000)) {
        if (motionNoti) {
            publishData("noti", "1");
            motionNoti = false;
        } else {
            publishData("noti", "0");
        }

        if (sensor[1]) {
            publishData("food", "1");
        } else {
            publishData("food", "0");
        }

        if (totalFood > 0) {
            String message = String("SUM ") + String(totalFood);
            publishData("chart", message.c_str());
        }

        if (hungry > 0) {
            hungry--;
        } else {
            if (motionNoti) {
                publishData("noti", "2"); // Thông báo thú cưng đói
                hungry = 600; // Thông báo lại sau 10 phút
            }
        }
    }

    // Đồng bộ thời gian mỗi 15 phút
    updateTimeSync();
}

// Tổng quan logic:
// - Người dùng có thể cho thú cưng ăn thủ công hoặc theo lịch đã cài đặt.
// - Chế độ tự động có thể bật/tắt bằng tin nhắn MQTT ("ON" hoặc "OFF").
// - Khi đến giờ cho ăn hoặc người dùng cho ăn thủ công, hệ thống sẽ gọi thú cưng và phát thức ăn.
// - Cảm biến phát hiện chuyển động và kiểm tra lượng thức ăn, gửi thông báo cho người dùng.
// - Tóm tắt lượng thức ăn hàng ngày sẽ được gửi định kỳ.
// - Có thể đặt tối đa bốn lịch cho ăn, nếu giờ đã qua, hệ thống sẽ kích hoạt vào ngày hôm sau.
