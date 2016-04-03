# Dx11ANDPhysx
Simple directx 11 and physx demo.
# If the text is garbled, please convert encoding to UTF-8.
このプログラムはDirectX11を使ってPhysXの剛体の簡単な実装です。

実行ファイル：x64/Release/PhysXTest.exe

*実行環境等を含めた実行方法:

動作確認：Windows 10　Visual Studio 2015 community

開発言語：C++  directx 11 

外部ライブラリ：PhysX SDK

必要な環境：DirectX SDK をインストール済み

実行方法："Dx11ANDPysx.sln"　Visual Studio 2015で開けたら、F5を押す。

操作方法：

左マウスクリック+ドラッグ　　　視点を回転

w,a,s,dボタン　　　　　　　カメラの移動　　　　　　　　　　

pボタン                   一時停止                

*プログラムを作成する上で苦労した箇所は？
カメラのデバッグ、PhysXの導入

*力をいれて作った部分で、「プログラム上」で特に注意してみてもらいたい箇所は？
MYAPP.h、DX11の初期化をクラスして、MYAPPを継承して、必要なバーチャル関数をリライトするだけでいいです。
ジオメトリーデータの生成(GeometryData.h)

*参考にしたソースファイルがあるなら、どの様なところを参考にしましたか？またその部分のファイル名を書いてください
DXのsamples、dx11のフレームワーク、アプリケーションとシェーダの交渉仕方に参考。
PhysXのSnippets、PhysXの初期化、シミュレーションに参考。

───
このプロジェクトはhttps://github.com/duoshengyu/Dx11ANDPhysxにも見えます。