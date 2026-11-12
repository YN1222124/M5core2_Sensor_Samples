#include <M5Unified.h>
#include <M5UnitUnified.h>
#include <M5UnitUnifiedTOF.h>

static m5::unit::UnitUnified Units;
static m5::unit::UnitToF unitToF; // VL53L0X (ToF Unit)

static M5Canvas canvas(&M5.Display);
static bool tof_ready = false;

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Display.setRotation(3);
    M5.Display.setBrightness(128);

    // M5Core2 Port.A (SDA: 32, SCL: 33) のI2Cピン番号を取得して初期化
    auto pin_num_sda = M5.getPin(m5::pin_name_t::port_a_sda);
    auto pin_num_scl = M5.getPin(m5::pin_name_t::port_a_scl);
    Wire.begin(pin_num_sda, pin_num_scl, 400000U);

    // UnitUnified に ToF Unit を登録して開始
    if (Units.add(unitToF, Wire) && Units.begin()) {
        tof_ready = true;
    }

    canvas.setColorDepth(8);
    canvas.createSprite(M5.Display.width(), M5.Display.height());
    canvas.setRotation(1);
    canvas.setTextWrap(false);
}

void loop() {
    M5.update();
    Units.update();

    uint16_t dist_mm = 0;
    bool valid = false;

    if (tof_ready && unitToF.updated()) {
        // 最新計測値（mm）の取得
        int16_t range = unitToF.range();
        if (range >= 0 && range < 2000) {
            dist_mm = static_cast<uint16_t>(range);
            valid = true;
        }
    }

    canvas.fillScreen(BLACK);
    canvas.fillRect(0, 0, 320, 32, NAVY);
    canvas.setTextDatum(top_center);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(WHITE);
    canvas.drawString("M5Core2 ToF Distance Monitor", 160, 8);

    if (!tof_ready) {
        canvas.setTextColor(RED);
        canvas.drawString("ERR: ToF Unit not detected", 160, 100);
    } else {
        canvas.drawRoundRect(10, 45, 300, 130, 4, DARKGREY);
        canvas.setTextDatum(top_left);
        canvas.setFont(&fonts::Font2);
        canvas.setTextColor(CYAN);
        canvas.drawString("Distance", 20, 55);

        canvas.setTextDatum(middle_center);
        canvas.setFont(&fonts::Font6);
        if (valid) {
            canvas.setTextColor(WHITE);
            char buf[16];
            snprintf(buf, sizeof(buf), "%4d", dist_mm);
            canvas.drawString(buf, 145, 105);
            canvas.setFont(&fonts::Font4);
            canvas.drawString("mm", 255, 112);

            // バーグラフ表示 (最大1000mm基準)
            int bar_w = map(constrain(dist_mm, 0, 1000), 0, 1000, 0, 280);
            canvas.fillRect(20, 148, bar_w, 14, GREEN);
        } else {
            canvas.setFont(&fonts::Font4);
            canvas.setTextColor(YELLOW);
            canvas.drawString("Out of Range", 160, 105);
        }
    }

    canvas.pushSprite(0, 0);
    delay(50);
}