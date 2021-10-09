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
お花です．綺麗ですね．

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
```
