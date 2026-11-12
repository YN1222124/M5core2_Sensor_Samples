#include <M5Unified.h>
#include <M5UnitENV.h>

// ENV III Unit
static SHT3X sht30;
static QMP6988 qmp;

// 描画ちらつき防止用スプライトバッファ
static M5Canvas canvas(&M5.Display);

static bool sht30_ready = false;
static bool qmp_ready   = false;

void setup() {
    auto cfg = M5.config();
    cfg.internal_imu = false; // 必要に応じて設定
    M5.begin(cfg);

    M5.Display.setRotation(0);
    M5.Display.setBrightness(128);

    // M5Core2 Port.A のI2Cピン設定 (SDA: 32, SCL: 33)
    // 外部I2Cにはデフォルトの 'Wire' を使用
    Wire.begin(32, 33, 400000UL);

    // センサーの初期化 (Wire, I2Cアドレス, SDA, SCL, 周波数)
    // SHT30 デフォルトアドレス: 0x44
    if (sht30.begin(&Wire, SHT3X_I2C_ADDR, 32, 33, 400000UL)) {
        sht30_ready = true;
    }

    // QMP6988 デフォルトアドレス: 0x70
    if (qmp.begin(&Wire, QMP6988_SLAVE_ADDRESS_L, 32, 33, 400000UL)) {
        qmp_ready = true;
    }

    // スプライト領域の確保 (320x240)
    canvas.setColorDepth(8);
    canvas.createSprite(M5.Display.width(), M5.Display.height());
    canvas.setTextWrap(false);
}

void loop() {
    M5.update();

    float temp_sht = 0.0f;
    float humidity = 0.0f;
    float pressure = 0.0f;

    // SHT30 から温湿度を取得
    if (sht30_ready && sht30.update()) {
        temp_sht = sht30.cTemp;
        humidity = sht30.humidity;
    }

    // QMP6988 から気圧を取得
    if (qmp_ready && qmp.update()) {
        pressure = qmp.pressure / 100.0f; // Pa -> hPa
    }

    // スプライト描画
    canvas.fillScreen(BLACK);

    // ヘッダーバー
    canvas.fillRect(0, 0, 320, 32, NAVY);
    canvas.setTextDatum(top_center);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(WHITE);
    canvas.drawString("M5Core2 ENV III Monitor", 160, 8);

    if (!sht30_ready || !qmp_ready) {
        canvas.setTextDatum(top_left);
        canvas.setFont(&fonts::Font2);
        canvas.setTextColor(RED);
        if (!sht30_ready) canvas.drawString("ERR: SHT30 not detected", 15, 45);
        if (!qmp_ready)   canvas.drawString("ERR: QMP6988 not detected", 15, 65);
    }

    // 各計測値の表示枠
    canvas.setTextDatum(top_left);

    // 温度
    canvas.drawRoundRect(10, 40, 300, 56, 4, DARKGREY);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(CYAN);
    canvas.drawString("Temperature", 20, 46);
    canvas.setFont(&fonts::Font4);
    canvas.setTextColor(WHITE);
    canvas.drawFloat(temp_sht, 1, 140, 56);
    canvas.drawString("C", 230, 56);

    // 湿度
    canvas.drawRoundRect(10, 102, 300, 56, 4, DARKGREY);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(GREEN);
    canvas.drawString("Humidity", 20, 108);
    canvas.setFont(&fonts::Font4);
    canvas.setTextColor(WHITE);
    canvas.drawFloat(humidity, 1, 140, 118);
    canvas.drawString("%", 230, 118);

    // 気圧
    canvas.drawRoundRect(10, 164, 300, 56, 4, DARKGREY);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(ORANGE);
    canvas.drawString("Pressure", 20, 170);
    canvas.setFont(&fonts::Font4);
    canvas.setTextColor(WHITE);
    canvas.drawFloat(pressure, 1, 120, 180);
    canvas.drawString("hPa", 240, 180);

    // 画面へ一括転送
    canvas.pushSprite(0, 0);

    delay(500);
}