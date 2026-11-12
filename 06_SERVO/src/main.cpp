#include <M5Unified.h>
#include <ESP32Servo.h>

#define SERVO_PIN      26  // Port.B: G26 (Port.Aを使う場合は 32)
#define STEP_INTERVAL  15  // 1度動かす間隔 [ms] (リセットする場合は 20〜30 に増やす)

static Servo myservo;
static M5Canvas canvas(&M5.Display);

static int current_angle = 90;
static int target_angle  = 90;
static unsigned long last_step_ms = 0;

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Display.setRotation(3);
    M5.Display.setBrightness(100); // 消費電流抑制のため輝度を少し控えめに設定

    canvas.setColorDepth(8);
    canvas.createSprite(M5.Display.width(), M5.Display.height());
    canvas.setRotation(1);
    canvas.setTextWrap(false);

    // 起動直後の電源安定待ち
    delay(200);

    // サーボ初期化 (500us - 2400us)
    ESP32PWM::allocateTimer(0);
    myservo.setPeriodHertz(50);
    myservo.attach(SERVO_PIN, 500, 2400);
    myservo.write(current_angle);
}

void loop() {
    M5.update();

    // タッチボタンで目標角度を設定
    if (M5.BtnA.wasPressed()) {
        target_angle = 5;
    } else if (M5.BtnB.wasPressed()) {
        target_angle = 90;
    } else if (M5.BtnC.wasPressed()) {
        target_angle = 175;
    }

    // 段階的に1度ずつ目標角度へ移動
    unsigned long now = millis();
    if (current_angle != target_angle && (now - last_step_ms >= STEP_INTERVAL)) {
        last_step_ms = now;
        if (current_angle < target_angle) {
            current_angle++;
        } else {
            current_angle--;
        }
        myservo.write(current_angle);
    }

    // 画面描画
    canvas.fillScreen(BLACK);
    canvas.fillRect(0, 0, 320, 32, NAVY);
    canvas.setTextDatum(top_center);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(WHITE);
    canvas.drawString("M5Core2 Servo Controller (Smooth)", 160, 8);

    // 目標角度・現在角度の表示
    canvas.drawRoundRect(10, 42, 300, 138, 4, DARKGREY);

    char buf[32];
    canvas.setTextDatum(top_left);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(CYAN);
    snprintf(buf, sizeof(buf), "Target : %3d deg", target_angle);
    canvas.drawString(buf, 22, 52);

    canvas.setTextDatum(middle_center);
    canvas.setFont(&fonts::Font6);
    canvas.setTextColor(WHITE);
    snprintf(buf, sizeof(buf), "%d", current_angle);
    canvas.drawString(buf, 145, 108);

    canvas.setFont(&fonts::Font4);
    canvas.drawString("deg", 235, 115);

    // 角度インジケータバー (0 - 180 deg)
    int bar_w = map(current_angle, 0, 180, 0, 280);
    canvas.drawRect(20, 152, 280, 14, DARKGREY);
    canvas.fillRect(20, 152, bar_w, 14, (current_angle == target_angle) ? GREEN : ORANGE);

    // タッチボタンガイド
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(YELLOW);
    canvas.drawString("[BtnA: 5 deg]  [BtnB: 90 deg]  [BtnC: 175 deg]", 160, 202);

    canvas.pushSprite(0, 0);
    delay(5);
}