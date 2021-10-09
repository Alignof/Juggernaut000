---
listings: True
codeBlockCaptions: True
---

\begin{table}[ht!]
    \centering
    \large
    \begin{tabular}{p{2cm}|p{13cm}} \hline
        装置名          & その回路には裏がある \\\hline
        装置番号        & 000-003           \\\hline
        作問者          & Takana Norimasa   \\\hline
        作問日          & 2021 9/29         \\\hline
        制限時間        & 15m00s            \\\hline
    \end{tabular}
\end{table}

# 作問者より一言
作問するときも装置を解くときも大事なのは「慎重であること」です．  
あなたの目の前にあるものは擬似的ではありますが爆弾です．お気をつけて．

> ヒント：データシートを見るのも良いですが，
よく使われてる部品ならインターネットにおいてあるコード例で概略を掴めるかもしれません．
（最も間違ったコードの可能性もありますが．）

# 回路
回路の全体の写真を[@fig:circuit]に示す．

![回路全体の写真](./circuit_003.jpg){#fig:circuit}

また，回路に使用する部品の一覧を[@tbl:parts]に示す．

部品名          個数        データシート
------          ------      ------------
ADXL345         1           [https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf](https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf)

: 回路に使用する部品の一覧 {#tbl:parts}

# ソースコード
[@lst:code]に競技に使用するソースコードを示す．
```{.cpp #lst:code caption="競技に使用するソースコード" title="timer.ino"}
int time_limit = 300;
uint8_t acce_data[6];
const uint8_t DEVICE_ADDRESS = 0x1D;

// giver pin assgin
const uint8_t mySCL  = 22;
const uint8_t mySDA  = 23;

void setup_pin(void) {
	pinMode(mySCL, INPUT_PULLUP);
	pinMode(mySDA, INPUT_PULLUP);
    Serial.begin(115200);

    Wire.begin(mySDA, mySCL);
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(0x31);
	Wire.write(0x0B);
	Wire.endTransmission();
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(0x2d);
	Wire.write(0x08);
	Wire.endTransmission();
}

void getAccelerationData(void) {
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(0x32);
	Wire.endTransmission();

	Wire.requestFrom(DEVICE_ADDRESS, 6);

	for(int i = 0; i < 6; i++) {
		delay(1);
		acce_data[i] = Wire.read();
	}
}

void gaming(void *pvParameters) {
    bool flag1 = false;
    bool flag2 = false;
    bool flag3 = false;
    bool flag4 = false;
    double ax = 0;
    double ay = 0;
    double az = 0;

	while(1) {
        getAccelerationData();
        ax = (int16_t)((acce_data[1] << 8) | acce_data[0]) * 0.0392266;
        ay = (int16_t)((acce_data[3] << 8) | acce_data[2]) * 0.0392266;
        az = (int16_t)((acce_data[5] << 8) | acce_data[4]) * 0.0392266;

        Serial.printf("ax: %g, ay: %g, az: %g\n", ax, ay, az);

        flag1 = ax > 2.0;
        flag2 = ay < -2.0;
        flag3 = az > 9.0;
        flag4 = az > 12.0;
		
		// succeeded
		if(flag1 && flag2 && flag3) {
            succeeded();
		}

		// failed
		if(flag4) {
            failed();
		}
	}
}
```
