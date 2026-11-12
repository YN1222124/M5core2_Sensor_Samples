// Port.A: G32、LED数 10個の設定
#include <M5Unified.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   32  // Port.A: 32 (Port.Bを使う場合は 26)
#define LED_COUNT 10  // 光らせたいLEDの数

static Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
static M5Canvas canvas(&M5.Display);

static uint8_t mode = 0;
static uint16_t hue = 0;

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);

    M5.Display.setRotation(3);
    M5.Display.setBrightness(128);

    strip.begin();
    strip.setBrightness(40); // 眩しすぎ・消費電流過多防止のため控えめに設定
    strip.show();

    canvas.setColorDepth(8);
    canvas.createSprite(M5.Display.width(), M5.Display.height());
    canvas.setRotation(1);
    canvas.setTextWrap(false);
}

void loop() {
    M5.update();

    // タッチボタン(BtnA)で点灯モード切替
    if (M5.BtnA.wasPressed()) {
        mode = (mode + 1) % 4;
    }

    const char* mode_name = "";
    switch (mode) {
        case 0: // 虹色アニメーション
            mode_name = "Rainbow Cycle";
            for (int i = 0; i < LED_COUNT; i++) {
                uint16_t pixelHue = hue + (i * 65536L / LED_COUNT);
                strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
            }
            hue += 512;
            break;
        case 1:
            mode_name = "Solid Red";
            strip.fill(strip.Color(255, 0, 0));
            break;
        case 2:
            mode_name = "Solid Green";
            strip.fill(strip.Color(0, 255, 0));
            break;
        case 3:
            mode_name = "OFF";
            strip.clear();
            break;
    }
    strip.show();

    // 画面描画
    canvas.fillScreen(BLACK);
    canvas.fillRect(0, 0, 320, 32, NAVY);
    canvas.setTextDatum(top_center);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(WHITE);
    canvas.drawString("M5Core2 NeoPixel Monitor", 160, 8);

    canvas.drawRoundRect(10, 50, 300, 110, 4, DARKGREY);
    canvas.setTextDatum(middle_center);
    canvas.setFont(&fonts::Font2);
    canvas.setTextColor(CYAN);
    canvas.drawString("Current Mode (Press BtnA)", 160, 75);

    canvas.setFont(&fonts::Font4);
    canvas.setTextColor(WHITE);
    canvas.drawString(mode_name, 160, 115);

    canvas.pushSprite(0, 0);
    delay(20);
}