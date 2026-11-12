#include <M5Unified.h>

static M5Canvas canvas(&M5.Display);

void setup() {
    auto cfg = M5.config();
    cfg.internal_imu = true; // 内蔵IMUを有効化
    M5.begin(cfg);

    M5.Display.setRotation(3);
    M5.Display.setBrightness(128);

    canvas.setColorDepth(8);
    canvas.createSprite(M5.Display.width(), M5.Display.height());
    canvas.setRotation(1);
    canvas.setTextWrap(false);
}

void loop() {
    M5.update();

    float ax = 0.0f, ay = 0.0f, az = 0.0f;
    float gx = 0.0f, gy = 0.0f, gz = 0.0f;

    if (M5.Imu.isEnabled()) {
        M5.Imu.getAccel(&ax, &ay, &az);
        M5.Imu.getGyro(&gx, &gy, &gz);
    }

    canvas.fillScreen(BLACK);
    canvas.fillRect(0, 0, 320, 32, NAVY);
    canvas.setTextDatum(top_center);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(WHITE);
    canvas.drawString("M5Core2 Internal IMU Monitor", 160, 8);

    char buf[64];
    canvas.setTextDatum(top_left);

    // 加速度 (G)
    canvas.drawRoundRect(10, 42, 300, 88, 4, DARKGREY);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(CYAN);
    canvas.drawString("Accelerometer [G]", 20, 48);

    canvas.setFont(&fonts::Font4);
    canvas.setTextColor(WHITE);
    snprintf(buf, sizeof(buf), "X:%6.2f  Y:%6.2f", ax, ay);
    canvas.drawString(buf, 20, 72);
    snprintf(buf, sizeof(buf), "Z:%6.2f", az);
    canvas.drawString(buf, 20, 100);

    // ジャイロ (deg/s)
    canvas.drawRoundRect(10, 138, 300, 88, 4, DARKGREY);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(ORANGE);
    canvas.drawString("Gyroscope [deg/s]", 20, 144);

    canvas.setFont(&fonts::Font4);
    canvas.setTextColor(WHITE);
    snprintf(buf, sizeof(buf), "X:%6.1f  Y:%6.1f", gx, gy);
    canvas.drawString(buf, 20, 168);
    snprintf(buf, sizeof(buf), "Z:%6.1f", gz);
    canvas.drawString(buf, 20, 196);

    canvas.pushSprite(0, 0);
    delay(50);
}