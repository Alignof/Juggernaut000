---
listings: True
codeBlockCaptions: True
---

\begin{table}[ht!]
    \centering
    \large
    \begin{tabular}{p{2cm}|p{13cm}} \hline
        装置名          & 赤か、青か        \\\hline
        装置番号        & 000-001           \\\hline
        作問者          & Takana Norimasa   \\\hline
        作問日          & 2021 10/7         \\\hline
        制限時間        & 10m00s            \\\hline
    \end{tabular}
\end{table}

# 作問者より一言
目の前には爆弾！
そして好きな方を切ってくださいと言わんばかり赤と青のコード！  
でも，運任せに切る必要はありません．
落ち着いてよくプログラムを読めばどっちを切るべきか分かるはずです．

# 回路
回路の全体の写真を[@fig:circuit]に示す．

![回路全体の写真](./circuit_001.jpg){#fig:circuit}

また，回路に使用する部品の一覧を[@tbl:parts]に示す．

部品名          個数        データシート
------          ------      ------------
\---            \---        \---

: 回路に使用する部品の一覧 {#tbl:parts}

# ソースコード
[@lst:code]に競技に使用するソースコードを示す．
```{.cpp #lst:code caption="競技に使用するソースコード" title="timer.ino"}
int time_limit = 600;

// giver pin assgin
const uint8_t RED_WIRE = 23;
const uint8_t BLUE_WIRE = 18;

void setup_pin(void) {
	pinMode(RED_WIRE,  INPUT_PULLUP);
	pinMode(BLUE_WIRE, INPUT_PULLUP);
}

void gaming(void *pvParameters) {
	bool flag1 = false;
	bool flag2 = false;

	while(1) {
		flag1 = (digitalRead(RED_WIRE)  == HIGH);
		flag2 = (digitalRead(BLUE_WIRE) == HIGH);
		
		// succeeded
		if(flag1) {
            succeeded();
		}

		// failed
		if(flag2) {
            failed();
		}
	}
}
```
