take(@elgraiv_take)

・タイトル
なにかの暫定版

・レンダラ名
1hRenderer

・作品コンセプト
とりあえず"人間がいる部屋"

・技術的アピールポイント
GIはPPM(実装がかなり怪しい)
BVHはメディアンベースの適当実装
skeleton-basedでblobby-likeなimplicit surfaceを使った髪の毛

・ビルドの設定
とりあえずデフォルトのReleaseビルドから
プラットフォーム:x64
最適化:実行速度の最大化
OpenMPのサポート:はい(/openmp)
インクルードディレクトリ:incを追加

・その他
texturesの中はBRDFとテクスチャに使った画像
実行結果はPPM形式で出力されたものを縮小してpngに変換したもの
BRDFのテーブルは手描き