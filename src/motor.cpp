#include "motor.h"
#include <Arduino.h>

// Số bước trong một chu kỳ
const int stepsPerRevolution = 4096; // 64 bước x tỷ số truyền 1:64

// Trình tự điều khiển động cơ bước (half-step)
int stepSequence[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1},
};

// Mảng lưu các chân điều khiển động cơ
int MT[4];
bool motorOn = false; // Trạng thái động cơ

// Biến trạng thái
int currentStep = 0; // Bước hiện tại của động cơ
int totalSteps = 0;  // Tổng số bước cần thực hiện

// Cấu hình động cơ bước
void setupMotor(int IN1, int IN2, int IN3, int IN4) {
    MT[0] = IN1;
    MT[1] = IN2;
    MT[2] = IN3;
    MT[3] = IN4;

    for (int i = 0; i < 4; i++) {
        pinMode(MT[i], OUTPUT);
        digitalWrite(MT[i], LOW); // Tắt động cơ ban đầu
    }
}

// Hàm điều khiển một bước của động cơ
void stepMotor(int step) {
    digitalWrite(MT[0], stepSequence[step][0]);
    digitalWrite(MT[1], stepSequence[step][1]);
    digitalWrite(MT[2], stepSequence[step][2]);
    digitalWrite(MT[3], stepSequence[step][3]);
}

// Đặt số vòng quay cho động cơ (tương ứng với lượng thức ăn)
void setRotate(float revolutions) {
    totalSteps = static_cast<int>(revolutions * stepsPerRevolution); // Tính tổng số bước từ số vòng quay
    motorOn = true; // Bật trạng thái động cơ
}

// Hàm quay động cơ theo số vòng quay đã đặt
void rotateMotor() {
    if (totalSteps > 0) {
        currentStep = (currentStep + 1) % 8; // Tiến đến bước tiếp theo
        stepMotor(currentStep);
        totalSteps--;
    } else {
        motorOn = false; // Tắt động cơ khi hoàn thành
        totalSteps = 0;

        // Đảm bảo động cơ không tiêu tốn năng lượng khi không hoạt động
        for (int i = 0; i < 4; i++) {
            digitalWrite(MT[i], LOW);
        }
    }
}
