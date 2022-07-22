# patch.aul test
## これはなに
https://github.com/ePi5131/patch.aul のテスト版です

AviUtlや拡張編集のバグを直したり機能を追加するプラグイン
黒魔術を一手に引き受けるのが目的

詳細はpatch.aul.txtを読むこと

## ビルドについて

- [CUDA TOOLKIT](https://developer.nvidia.com/cuda-toolkit)を用意してください
 これのOpenCLライブラリを使います 環境変数`CUDA_PATH`を使います

-  `test`フォルダを用意してください
 これは`aviutl.exe`などを置くフォルダで、デバッグ実行するとこのディレクトリの`aviutl.exe`が実行されます

- ビルドすると、`pack`フォルダにリリース用のファイルが集まります

## r41からの変更点
r41.1で追加
- 極座標変換の処理を修正
- 放射ブラーの処理を修正
- ディスプレイスメントマップのGPU処理追加
- ノイズの速度X、変化速度のトラック変化方法が移動無し以外の時に速度Yの値をもとに計算が行われてしまうのを修正
- 右クリック分割で設定ダイアログが更新されないのを修正
- 右クリック削除でテキストの字間行間が変わることがあるのを修正
- 標準描画-拡張描画-パーティクルの切り替え時にトラック変化の設定値(移動フレーム間隔)が0になるのを修正
- トラック変化方式の移動量指定と時間制御の組み合わせでバグるのを修正
- アルファチャンネル有りシーンで合成モード「通常」以外を使用すると、他シーンで表示した時の結果が正しくないのを修正
r41.2で追加
- ＜図形、部分フィルタ、マスク、ディスプレイスメントマップ＞の図形（円、三角、五角、六角、星）の計算を効率化し少しだけ速度アップ
- エフェクトのエイリアスを格納フォルダに保存すると、設定ダイアログの[+]の所に出てこないのを修正
