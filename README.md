# Core of Command Centric Architecture

## C2A
- Command Centric Architecture
- OBC 搭載フライトソフトウェアフレームワーク

### C2A core
- 各 C2A で共通利用される C2A の中核部のコード
- 基本的には，各々の C2A user repository （サンプル： TBW） にて， git submodule で参照される．
- core 開発のための最小限の user 部は `Examples/` にあり，これを用いてビルドし，検証する．

### 開発のためのセットアップ
1. clone 後， setup.bat or setup.sh を実行

### テスト
- 特定のuserを仮定しないと，各種パラメタが確定しないため， `Examples/minimum_user_for_s2e` でテストする．
- https://github.com/ut-issl/c2a-core/tree/develop/Examples/minimum_user_for_s2e/src/src_user/Test 参照．

### ブランチ
- `main`: リリース版
- `develop`: 概ね検証された最新版（beta機能含む）
- `feature/*` : 開発ブランチ
- `hotfix/*` : 重大バグ修正用ブランチ

## ドキュメント
- ドキュメント:  https://github.com/ut-issl/c2a-core/tree/develop/Docs
- リファレンス:  https://github.com/ut-issl/c2a-reference

## 関連リポジトリ
- https://github.com/ut-issl/tlm-cmd-db
- https://github.com/ut-issl/c2a-tlm-cmd-code-generator
- https://github.com/ut-issl/python-wings-interface
- https://github.com/ut-issl/c2a-enum-loader

## 質問，問い合わせ，その他なんでも
お気軽に [Discussions](https://github.com/ut-issl/c2a-core/discussions) に投稿してください．
