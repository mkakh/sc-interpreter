# sc-interpreter

## 概要
Small C言語のインタプリタです．

## 補足
教科書に載っている命令に加えて2つ命令を追加しました．

|命令番号|命令|意味|動作|
|:-:|:-:|:-:|:-:|
|24|INP|input|sp++; printf("> "); scanf("%d", &stack[sp]);|
|25|OUT|output|printf("%d\n", stack[sp]);|
|26|OCH|output character| printf("%c", stack[sp]);|
