# Release

## リリース
現在， `main` ブランチがリリースブランチとなっている．  
リリースには，本 Release と Pre-release の２つを設ける．

- 本 Release
    - まとまった機能更新のリリース．
    - バージョンを上げる．
    - `v3.4.0` → `v3.5.0` など
- Pre-release
    - 主に， [Tools/Overview](../Tools/overview.md) などで非互換なアップデートが入った場合のリリース．
    - `beta` をリリースする．
        - `v3.4.0` → `v3.5.0-beta.0` など

リリースの手順は以下のようにする．

### 本 Release

1. [c2a_core_main.h](https://github.com/ut-issl/c2a-core/blob/develop/c2a_core_main.h) 内の `C2A_CORE_VER_*` をインクリメントする PR (Pull Request) を発行し， `develop` ブランチへマージする．
    - この後リリースを控えるので，念の為すべてのテストを再度回す．
    - `#define C2A_CORE_VER_PRE` は `("")` とする．
    - PR 名は `Update version (v3.4.0)` のようにする．
    - 例: https://github.com/ut-issl/c2a-core/pulls?q=is%3Apr+Update+version
1. バージョン上げ PR がマージされたら，直ちに（他の PR を止め） `develop` から `main` に PR を発行し，すべてのテストを回し，マージする．
    - PR 名は以下のようにする．
        - `Update main (v3.4.0) on 2021-12-31`
        - `Update main (v3.4.0) on 2021-12-31 - ほげほげ`
    - PR のディスクリプションは [これ](https://github.com/ut-issl/c2a-core/pull/151) のように書く．
    - 例: https://github.com/ut-issl/c2a-core/pulls?q=is%3Apr+Update+main
1. [tag](https://github.com/ut-issl/c2a-core/tags) を打ち， [release](https://github.com/ut-issl/c2a-core/releases) を発行する．
    - tag 名は `v3.4.0` のようにする．
    - release 名は `v3.4.0` や `v3.3.1 初版` のようにする．
    - release には以下を含める．
        - Release Note として簡潔な更新差分の箇条書き
        - `main` にマージしたときの PR のリンク


これを，だいたい以下のような粒度で行う．

- 最後のリリースからおおよそ１ヶ月程度が経過した場合（更新が少ない場合を除く）
- まとまった機能実装が一段落した場合

ただし，あるまとまった機能の実装の PR が複数に分割され，一部がまだマージされていない場合はリリースを控えること．

### Pre-release
1.  [Tools/Overview](../Tools/overview.md) などに非互換なアップデートが含まれる PR を発行する．このとき， Pre-release PR となる．
    - これは， `feature` ブランチから `develop` ブランチへの PR になるはず．
    - 対応する Tools の PR のリンクを貼る．
    - `#define C2A_CORE_VER_PRE` に `("beta.0")` などをセットする．
    - 本 Release 後最初の Pre-release の場合， `C2A_CORE_VER_*` をインクリメントする．
    - PR 名は以下のようにする．
        - `Pre Release (v3.5.0-beta.0): 通常のPRのタイトル`
    - 例: https://github.com/ut-issl/c2a-core/pulls?q=is%3Apr+Pre+Release
1. [tag](https://github.com/ut-issl/c2a-core/tags) を打ち， [release](https://github.com/ut-issl/c2a-core/releases) を Pre-release として発行する．
    - tag 名は `v3.5.0-beta.0` のようにする．
    - release 名は `v3.5.0-beta.0` のようにする．
    - release には以下を含める．
        - 非互換となった Tools の新しい バージョン (Release) へのリンク
        - `develop` にマージしたときの PR のリンク
1. Bug fix や 大きな機能更新などで，本 Release 前に User サイドで最新の Core が必要になった際にも， Pre-release を行うことができる．


## バージョニング
[Semantic Versioning](https://semver.org) を採用する．


## C2A Tools との互換性について
少なくとも， [Tools/Overview](../Tools/overview.md) の Tool に後方互換性のない更新があった場合は，新しいバージョンをリリースする．  
Tool のリリースには，以下に注意する．

- C2A と同様の規則で Relase を発行する．
- release には以下を含める．
    - Release Note として簡潔な更新差分の箇条書き
    - 対応する最小 C2A Core バージョン
      - [c2a-tlm-cmd-code-generator](https://github.com/ut-issl/c2a-tlm-cmd-code-generator) については，依存関係が逆なので除外
    - この Tool に適合させたときの C2A Core の PR へのリンク

例:

- https://github.com/ut-issl/c2a-tlm-cmd-code-generator/releases
- https://github.com/ut-issl/python-wings-interface/releases
- https://github.com/ut-issl/c2a-enum-loader/releases
- https://github.com/ut-issl/tlm-cmd-db/releases
