# cgc

https://github.com/user-attachments/assets/dfa1e78d-5812-4a6a-89a8-e6f0cbcfd3bf


openglで描画するためのプログラム

yukselの.hairファイルを読み込める

使い方

glfw,glm,eigen3をインストール

最新の機能使ってないけど一応全部最新verで

macならbrew installで全部入る。

brew install glfw

brew install glm

brew install eigen

髪を読み込むためにyukselのホームページから.hairファイルをダウンロードする必要がある。

Thank you Cem Yuksel!

hairについて

シミュレーションのコードは不完全。これから色々実装する

ray-tracingリポジトリからgltfを読み込む機能を追加したい。誰かしてくれ。

あとgltfよりobjのほうがいいかもしれない。

To do

髪のシミュレーション

gltfもしくはobjの読み込み

compute shaderの実装

# memo
位置と速度を更新

レファレンスフレームを更新

アダプティブフレームを計算

力を計算
