# WIREDSTG MD

C言語で横スクロールゲームを作ります。

## ソースコード概要

- bios.h BIOS定義
- bios.c 基本的なライブラリ
- boot/rom_head.c ROMヘッダ
- boot/sega.s エントリポイント
  - main.c メインプログラム
    - App.c  アプリケーションプログラム (ここから,TitleかGameが呼ばれる)
      - Title.c タイトル画面
      - Game.c ゲーム本体
        - Ground.c 地面
        - Star.c 星
        - Ship.c ビックバイパー
          - Shot.c 自機弾
        - Enemy.c 敵のコントロール
          - Bullet.c 敵弾
          - EnemyBomb.c 敵の爆発
          - EnemyDee01.c  敵キャラ 砲台
          - EnemyDucker.c 敵キャラ 移動砲台
          - EnemyFans.c   敵キャラ 扇風機みたいなやつ
          - EnemyGarun.c  敵キャラ サインカーブを描いて移動
          - EnemyRugal.c  敵キャラ 寄ってくる
          - EnemyBigCore.c ビックコア
            - EnemyBeam.c ビックコアのビーム

## 各ステップで行うこと

WRIREDSTG MD は以下のステップで段階的に作ることができます:

1. [自機を動かす](https://github.com/hsk/wiredstg_md/compare/v0..v1)
2. [敵機を出現させる](https://github.com/hsk/wiredstg_md/compare/v1..v2)
3. [自機弾を撃つ](https://github.com/hsk/wiredstg_md/compare/v2..v3)
4. [自機弾の当たり判定](https://github.com/hsk/wiredstg_md/compare/v3..v4)
5. [敵弾を撃つ](https://github.com/hsk/wiredstg_md/compare/v4..v5)
6. [敵弾の当たり判定](https://github.com/hsk/wiredstg_md/compare/v5..v6)
7. [敵機の種類を増やす](https://github.com/hsk/wiredstg_md/compare/v6..v7)
8. [ビックコア](https://github.com/hsk/wiredstg_md/compare/v7..v8)
9. [背景生成](https://github.com/hsk/wiredstg_md/compare/v8..v9)
10. [背景と当たり判定](https://github.com/hsk/wiredstg_md/compare/v9..v10)
11. [地上敵](https://github.com/hsk/wiredstg_md/compare/v10..v11)
12. [星を流す](https://github.com/hsk/wiredstg_md/compare/v11..v12)
13. [タイトル画面](https://github.com/hsk/wiredstg_md/compare/v12..v13)
14. [タイトル画面をアニメーション](https://github.com/hsk/wiredstg_md/compare/v13..v14)
15. [スコア](https://github.com/hsk/wiredstg_md/compare/v14..v15)
16. [スピード](https://github.com/hsk/wiredstg_md/compare/v15..v16)
17. [音](https://github.com/hsk/wiredstg_md/compare/v16..v17)

git の diff を見ながら写経してみることでゲーム作りのノウハウが学べるはずです。
