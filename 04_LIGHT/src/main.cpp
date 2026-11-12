#include <M5Unified.h>

// M5Core2 Port.B ピン定義
#define LIGHT_ANALOG_PIN  36 // Port.B IN  (ADC1_CH0: 入力専用ピン)
#define LIGHT_DIGITAL_PIN 26 // Port.B OUT (本体可変抵抗の閾値判定)

static M5Canvas canvas(&M5.Display);

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Display.setRotation(1);
    M5.Display.setBrightness(128);

    // ピンモード設定 (G36は入力専用ピンのためプルアップ不可)
    pinMode(LIGHT_ANALOG_PIN, INPUT);
    pinMode(LIGHT_DIGITAL_PIN, INPUT);
    analogReadResolution(12); // 12bit (0 - 4095)

    canvas.setColorDepth(8);
    canvas.createSprite(M5.Display.width(), M5.Display.height());
    canvas.setRotation(1);
    canvas.setTextWrap(false);
}

void loop() {
    M5.update();

    // アナログ値 (0: 非常に明るい 〜 4095: 暗い)
    uint16_t raw_val = analogRead(LIGHT_ANALOG_PIN);
    // デジタル値 (閾値より明るいと LOW, 暗いと HIGH)
    int dig_val = digitalRead(LIGHT_DIGITAL_PIN);

    // 直感的に見やすいよう明るさ(%)へ反転変換 (暗い=0%, 明るい=100%)
    int brightness_pct = map(constrain(raw_val, 0, 4095), 4095, 0, 0, 100);

    canvas.fillScreen(BLACK);
    canvas.fillRect(0, 0, 320, 32, NAVY);
    canvas.setTextDatum(top_center);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(WHITE);
    canvas.drawString("M5Core2 Light Unit (Port.B)", 160, 8);

    // アナログ照度・ADC値表示
    canvas.drawRoundRect(10, 42, 300, 115, 4, DARKGREY);
    canvas.setTextDatum(top_left);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(CYAN);
    canvas.drawString("Brightness / Raw ADC (G36)", 20, 50);

    char buf[32];
    snprintf(buf, sizeof(buf), "%3d %%", brightness_pct);
    canvas.setFont(&fonts::Font6);
    canvas.setTextColor(WHITE);
    canvas.drawString(buf, 25, 76);

    snprintf(buf, sizeof(buf), "ADC: %4d", raw_val);
    canvas.setFont(&fonts::Font4);
    canvas.setTextColor(LIGHTGREY);
    canvas.drawString(buf, 175, 92);

    // バーグラフ表示
    int bar_w = map(brightness_pct, 0, 100, 0, 280);
    canvas.fillRect(20, 134, bar_w, 12, YELLOW);

    // デジタル閾値ステータス表示 (基板上の青いポテンショメータで閾値調整)
    canvas.drawRoundRect(10, 165, 300, 62, 4, DARKGREY);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(ORANGE);
    canvas.drawString("Comparator Status (G26)", 20, 173);

    canvas.setFont(&fonts::Font4);
    if (dig_val == LOW) {
        canvas.setTextColor(GREEN);
        canvas.drawString("BRIGHT (LOW)", 20, 196);
    } else {
        canvas.setTextColor(DARKGREY);
        canvas.drawString("DARK (HIGH)", 20, 196);
    }

    canvas.pushSprite(0, 0);
    delay(50);
}