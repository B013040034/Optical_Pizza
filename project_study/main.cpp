/*
目的：main.cpp是整個專案的介面，在之中所做的事情應當只有計算處理需求時間，呼叫處理光纖需求的方法(function)，以及給定該方法所需求
的實驗環境參數。
引用標頭：除了standard io之外，main.cpp應該只引用vone.hpp和<chrono>，chrono用來處理計時。

在每一個檔案(*.cpp或者是*.hpp)中都應該要有這開頭的註解，用來表示這個檔案的目的以及它引用了那些標頭(header)檔案。
如果你要編寫註解，請使用全形的逗號(，快捷鍵是ctrl + ,)和句號(。快捷鍵是ctrl + .)，這樣比較好閱讀。
如果你要在註解之中使用比較艱澀難懂的中文名詞，請在中文名詞之後用括號加上英文名稱，例如說虛擬(virtual)光學(optical)網路(network)
嵌入(embedding)。因為雖然中文比較好寫，但英文名詞比較好懂。

TODO:: 目前的想法是先將整個程式的架構建立起來，這會用許多空的檔案和TODO來完成。加入TODO註解非常重要，因為它可以讓我們知道有哪些事
情需要做但是還沒做。TODO註解和其他程式碼用空行隔開，這樣方便看到。

TODO:: 編寫英文註解。等到事情都做完時最好將所有註解寫出英文版本，因為有很低的機率中文字會變成亂碼無法閱讀，不過這件事情可以留到後面
再做。
*/
/*
TODO:: Write some English comments here.
*/
#include <iostream>
#include <chrono>
#include "vone.h"

// TODO:: 定義一些常數，實驗環境之類的鬼東西。

int main()
{
	/* TODO::
	定義Capacity和LRC的eumeration。
	問題：因為被enum的東西在main和在vone中是一模一樣的，是否要在兩邊各自宣告一次?還是說有更優雅的寫法？
	還有這個玩意要放在main裡面嗎？
	*/

	/* TODO::
	定義以下兩種優先順序的eumeration。
	先路徑再時間最後頻譜
	先路徑再頻譜最後時間
	*/

	// TODO:: 開始計時。

	// 問題：vone.hpp中需要定義常數嗎？還是說為了介面使用上的方便，所有常數都在main中定義，並且輸入到vone中？
	// 目前的想法是會變更的常數放在main中，而不太會變更的(像是頻譜槽數量)則定義在vone.hpp中。
	vone();

	int h;
	std::cin >> h;
	// TODO:: 結束計時。

	return 0;
}