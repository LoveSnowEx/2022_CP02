HW03
===

姓名: 郭泰維
學號: 40847031S

## Build

使用 ```make``` 來編譯，```make clean``` 來清除產生出的 .o 和執行檔


## Run

使用 ```./<目標名稱>``` 來執行
例如: ```./hw0301```


### hw0301

1. 不允許輸入空字串，如果直接輸入 enter 會視為尚未輸入，直到讀到非空字串才會開始查詢

### hw0303

1. 僅允許輸入 24 bits 的 bmp 檔
2. center 可輸入任意整數座標，須符合格式(x,y)，否則會報錯
3. radius 請輸入非負整數，否則會報錯

### hw0304

1. 僅允許輸入 24 bits 的 bmp 檔

### hw0305

1. 僅允許輸入 24 bits 的 bmp 檔
2. 如果圖片寬高不足 chunck 的大小，會自動補白
3. 如果 m, n 大於寬高，chunck 的大小會是 0，到最後會拿到只有黑色邊界的圖，也就是全黑
4. puzzle 請依照題目輸入，錯誤檢查如下
	1. m, n, k 不是正整數
	2. format error: 輸入了非整數、空行、連續空白、行尾空白
	3. out of bound: 輸入了小於0或超出M*N的數字
	4. repeat chunck: 輸入了重複的數字
	5. different part too much: 多達三處以上與先前 state 不同處
	6. invlaid move: 非法移動 
