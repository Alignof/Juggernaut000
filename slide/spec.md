---
listings: True
codeBlockCaptions: True
---

\begin{table}[ht!]
    \centering
    \large
    \begin{tabular}{p{2cm}|p{13cm}} \hline
        装置名          &         \\\hline
        装置番号        & 000-005           \\\hline
        作問者          & Takana Norimasa   \\\hline
        作問日          & 2021 10/13         \\\hline
        制限時間        & 15m00s            \\\hline
    \end{tabular}
\end{table}

# 作問者より一言

# 回路
回路の全体の写真を[@fig:circuit]に示す．

![回路全体の写真](./circuit_005.jpg){#fig:circuit}

また，回路に使用する部品の一覧を[@tbl:parts]に示す．

部品名           個数        データシート
------           ------      ------------
ADXL345          1           [https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf](https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf)
タクトスイッチ   3           [https://akizukidenshi.com/download/ds/switronic/1273HIM-160G-G.pdf](https://akizukidenshi.com/download/ds/switronic/1273HIM-160G-G.pdf)
スライドスイッチ 1           [https://akizukidenshi.com/download/ds/jinbeili/AKIZUKI-SS-12D00G3.pdf](https://akizukidenshi.com/download/ds/jinbeili/AKIZUKI-SS-12D00G3.pdf)
2N2222A          1           [https://datasheetspdf.com/pdf-file/1129136/SEMTECH/2N2222A/1](https://datasheetspdf.com/pdf-file/1129136/SEMTECH/2N2222A/1)   
2SA1015          1           [https://akizukidenshi.com/download/2sa1015-gr.pdf](https://akizukidenshi.com/download/2sa1015-gr.pdf)   

: 回路に使用する部品の一覧 {#tbl:parts}

# ソースコード
[@lst:code]に競技に使用するソースコードを示す．
```{.cpp #lst:code caption="競技に使用するソースコード" title="timer.ino"}
```
