---
listings: True
codeBlockCaptions: True
---

\begin{table}[ht!]
    \centering
    \large
    \begin{tabular}{p{2cm}|p{13cm}} \hline
        装置名          & slide        \\\hline
        装置番号        & 000-005           \\\hline
        作問者          & Takana Norimasa   \\\hline
        作問日          & 2021 10/13         \\\hline
        制限時間        & 15m00s            \\\hline
    \end{tabular}
\end{table}

# 作問者より一言
Juggernaut#000の最終問題です！ここまでお付き合いいただきありがとうございました！
最終問題に相応しい難易度にしましたので是非チャレンジしてみてください．
やや複雑ですが，落ち着いて回路を読み解けば解除にたどり着くはずです．
頑張って！

# 回路
回路の全体の写真を[@fig:circuit]に示す．

![回路全体の写真](./circuit_005.jpg){#fig:circuit}

最終問題かつ配線が複雑で写真やカメラでは上手く回路の様子が確認できないことが
予測されるため配線図を提供する．
配線図を[@fig:wiring]に示す．

![配線図](./circuit_005_wiring.png){#fig:wiring}

また，回路に使用する部品の一覧を[@tbl:parts]に示す．

部品名           個数        データシート
------           ------      ------------
ADXL345          1           [https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf](https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf)
タクトスイッチ   3           [https://akizukidenshi.com/download/ds/switronic/1273HIM-160G-G.pdf](https://akizukidenshi.com/download/ds/switronic/1273HIM-160G-G.pdf)
スライドスイッチ 1           [https://akizukidenshi.com/download/ds/jinbeili/AKIZUKI-SS-12D00G3.pdf](https://akizukidenshi.com/download/ds/jinbeili/AKIZUKI-SS-12D00G3.pdf)
2N2222A          1           [https://datasheetspdf.com/pdf-file/1129136/SEMTECH/2N2222A/1](https://datasheetspdf.com/pdf-file/1129136/SEMTECH/2N2222A/1)   
2SA1015          1           [https://akizukidenshi.com/download/2sa1015-gr.pdf](https://akizukidenshi.com/download/2sa1015-gr.pdf)   

: 回路に使用する部品の一覧 {#tbl:parts}

\newpage 

# ソースコード
[@lst:code]に競技に使用するソースコードを示す．
```{.cpp #lst:code caption="競技に使用するソースコード" title="timer.ino"}
int time_limit = 900;
uint8_t acce_data[6];
const uint8_t DEVICE_ADDRESS = 0x1D;

// giver pin assgin
const uint8_t mySCL  = 22;
const uint8_t mySDA  = 23;
const uint8_t trans_l = 16;
const uint8_t trans_r = 17;
const uint8_t slide_sw = 15;
const uint8_t gray_button = 2;
const uint8_t flat_button = 4;
const uint8_t black_button = 21;

void setup_pin(void) {
	pinMode(mySCL, INPUT_PULLUP);
	pinMode(mySDA, INPUT_PULLUP);
	pinMode(slide_sw, INPUT_PULLUP);
	pinMode(gray_button, INPUT_PULLUP);
	pinMode(flat_button, INPUT_PULLUP);
	pinMode(black_button, INPUT_PULLUP);
	pinMode(trans_r, OUTPUT);
	pinMode(trans_l, OUTPUT);
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
    double az = 0;

	while(1) {
        if (digitalRead(slide_sw) == HIGH) {
            digitalWrite(trans_l, LOW);
            digitalWrite(trans_r, HIGH);

            if (digitalRead(flat_button) == LOW) {
                flag1 = true;
            }
        } else {
            digitalWrite(trans_r, LOW);
            digitalWrite(trans_l, HIGH);

            if (flag1 && digitalRead(black_button) == LOW) {
                flag2 = true;
            }

            if (flag1 && flag2 && digitalRead(black_button) == HIGH &&
                digitalRead(gray_button) == LOW) {
                flag3 = true;
            }
        }

        getAccelerationData();
        az = (int16_t)((acce_data[5] << 8) | acce_data[4]) * 0.0392266;
        flag4 = az > 12.0;
        Serial.printf("az: %g\n", az);

		// succeeded
		if(flag3) {
            succeeded();
		}

		// failed
		if(flag4) {
            failed();
		}
	}
}
```
