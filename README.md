# M5core2_Sensor_Samples

M5Stack Core2 と各種センサ・アクチュエータユニットを **`M5Unified`** + **`PlatformIO`** で制御するためのサンプルコード集です。  
演習などのベースコードとして利用できるよう、すべてのサンプルで画面描画（`M5Canvas` スプライトによるちらつき防止処理）とピン設定を統一しています。

## Notes / Development

本リポジトリのサンプルコードおよびドキュメントの作成には生成AI（Gemini 3.8 flash）を補助的に利用していますが、すべてのプログラムは M5Core2 実機にて動作確認および人間による修正・検証を行っています。

---

## サンプル一覧と接続ポート

| ディレクトリ | 対象ユニット / センサ | 接続ポート | 使用 GPIO | 依存ライブラリ |
| :--- | :--- | :--- | :--- | :--- |
| **`01_ENV3`** | ENV III Unit (温湿度 SHT30 + 気圧 QMP6988) | **Port.A** (赤) | SDA: `G32`, SCL: `G33` | `M5UnitENV @ 1.4.0` |
| **`02_LED`** | NeoPixel LED Bar / HEX Unit | **Port.A** (赤) | DATA: `G32` | `Adafruit NeoPixel @ 1.15.5` |
| **`03_ToF`** | ToF Unit (測距センサ VL53L0X) | **Port.A** (赤) | SDA: `G32`, SCL: `G33` | `M5UnitUnified`<br>`M5Unit-TOF @ 0.3.0` |
| **`04_LIGHT`** | Light Unit (光センサ / フォトレジスタ) | **Port.B** (黒) | Analog: `G36`, Digital: `G26` | `M5Unified` のみ（標準ADC/GPIO） |
| **`05_IMU`** | 内蔵 6軸IMU (加速度・ジャイロ MPU6886) | 内蔵 | 内部 I2C (`Wire1`) | `M5Unified` のみ |
| **`06_SERVO`** | PWM サーボモータ (SG90 等) | **Port.B** (黒) | PWM: `G26` | `ESP32Servo @ 3.2.1` |

> **ハードウェア接続の補足:**
> * サーボモータ（`06_SERVO`）を Grove ポート経由で駆動する場合、急激な角度変更による突入電流で M5Core2 の 5V 電源が電圧降下しリセットすることがあります。サンプルコードでは 1 度ずつ段階的に移動（スイープ制御）させることで負荷を軽減しています。

---

## 開発環境のセットアップとビルド手順

### 1. 必要なソフトウェア
* Visual Studio Code
* VS Code 拡張機能: **PlatformIO IDE**

### 2. プロジェクトの開き方（ワークスペース利用）
本リポジトリには全サンプルをまとめた VS Code ワークスペースファイル（`*.code-workspace`）が含まれています。

1. リポジトリをクローンまたは ZIP ダウンロードして展開します。
   ``` bash
   git clone https://github.com/YN1222124/M5core2_Sensor_Samples.git
   ```
1. VS Code を開き、メニューの 「ファイル (File)」 > 「ファイルからワークスペースを開く... (Open Workspace from File...)」 からリポジトリ直下の .code-workspace ファイルを選択します。

2. 画面下部ステータスバーの PlatformIO プロジェクトスイッチャー（フォルダアイコン） をクリックし、ビルドしたいサンプル（例: 01_ENV3）を選択します。

3. M5Core2 を USB-C ケーブルで PC に接続し、ステータスバーの Upload（→ アイコン） をクリックするとビルドと書き込みが実行されます。

## ディレクトリ構成
``` text
M5Core2-SensorKit-Samples/
├── M5Core2-Sensor-Samples.code-workspace  # VS Code マルチプロジェクト設定
├── README.md
├── LICENSE                                   # MIT License
├── .gitignore
├── 01_ENV3/                                  # 温湿度・気圧センサ
│   ├── platformio.ini
│   └── src/main.cpp
├── 02_LED/                                   # NeoPixel LED 制御
├── 03_ToF/                                   # ToF 距離センサ
├── 04_LIGHT/                                 # 光センサ (アナログ/デジタル)
├── 05_IMU/                                   # 本体内蔵 6軸IMU
└── 06_SERVO/                                 # サーボモータ (スイープ制御)
```

## License
This project is licensed under the MIT License.
