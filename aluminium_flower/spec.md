---
listings: True
codeBlockCaptions: True
---

\begin{table}[ht!]
    \centering
    \large
    \begin{tabular}{p{2cm}|p{13cm}} \hline
        装置名          & aluminium flower \\\hline
        装置番号        & 000-004           \\\hline
        作問者          & Takana Norimasa   \\\hline
        作問日          & 2021 10/1         \\\hline
        制限時間        & 15m00s            \\\hline
    \end{tabular}
\end{table}

# 作問者より一言
光り輝くアルミの花．
綺麗だけど，作るのはなかなか大変．

# 回路
回路の全体の写真を[@fig:circuit]に示す．

![回路全体の写真](./circuit_004.jpg){#fig:circuit}

また，回路に使用する部品の一覧を[@tbl:parts]に示す．

部品名          個数        データシート
------          ------      ------------
OSTAMA5B31A     1           [https://akizukidenshi.com/download/ds/optosupply/OSTAMA5B31A.pdf](https://akizukidenshi.com/download/ds/optosupply/OSTAMA5B31A.pdf)

: 回路に使用する部品の一覧 {#tbl:parts}

# ソースコード
[@lst:code]に競技に使用するソースコードを示す．
```{.cpp #lst:code caption="競技に使用するソースコード" title="timer.ino"}
typedef enum {
    LED_R = 1,
    LED_G,
    LED_B,
} Color;

int time_limit = 900;
const uint8_t threshold = 23;

// giver pin assgin
const uint8_t touch0 = 4;
const uint8_t touch1 = 13;
const uint8_t touch2 = 2;
const uint8_t touch3 = 15;
const uint8_t touch4 = 14;
const uint8_t touch5 = 33;
const uint8_t led_red   = 21;
const uint8_t led_green = 22;
const uint8_t led_blue  = 23;

void setup_pin(void) {
    Serial.begin(115200);
    ledcAttachPin(led_red,  LED_R);
    ledcAttachPin(led_green,LED_G);
    ledcAttachPin(led_blue, LED_B);
    ledcSetup(LED_R, 12000, 8);
    ledcSetup(LED_G, 12000, 8);
    ledcSetup(LED_B, 12000, 8);
}

void gaming(void *pvParameters) {
    bool flag1 = false;
    bool flag2 = false;
    bool flag3 = false;
    bool flag4 = false;
    uint8_t red = 170;
    uint8_t green = 100;
    uint8_t blue = 50;

	while(1) {
        red     += touchRead(touch0) < threshold ? 10 : 0;
        red     -= touchRead(touch1) < threshold ? 10 : 0;
        green   += touchRead(touch2) < threshold ? 10 : 0;
        green   -= touchRead(touch3) < threshold ? 10 : 0;
        blue    += touchRead(touch4) < threshold ? 10 : 0;
        blue    -= touchRead(touch5) < threshold ? 10 : 0;
        delay(200);

        ledcWrite(LED_R, 255 - red);
        ledcWrite(LED_G, 255 - green);
        ledcWrite(LED_B, 255 - blue);

        Serial.printf("red: %d, green: %d, blue: %d\n", red, green, blue);

        flag1 = green > 200;
        flag2 = red < 100;
        flag3 = blue < 50;
        flag4 = red > 230;
		
		// succeeded
		if(flag1 && flag2 && flag3) {
            succeeded(); }

		// failed
		if(flag4) {
            failed();
		}
	}
}
```
