# Release

## リリース
現在， `main` ブランチがリリースブランチとなっている．

リリースの手順は以下のようにする．

1. [c2a_core_main.h](https://github.com/ut-issl/c2a-core/blob/develop/c2a_core_main.h) 内の `C2A_CORE_VER_*` をインクリメントする PR (Pull Request) を発行し， `develop` ブランチへマージする．
	- この後リリースを控えるので，念の為すべてのテストを再度回す．
	- PR 名は `Update version (v3.4.0)` のようにする．
	- 例: TBA
1. バージョン上げ PR がマージされたら，直ちに（他の PR を止め） `develop` から `main` に PR を発行し，すべてのテストを回し，マージする．
	- PR 名は以下のようにする．
		- `Update main (v3.4.0) on 2021-12-31`
		- `Update main (v3.4.0) on 2021-12-31 - ほげほげ`
	- PR のディスクリプションは [これ](https://github.com/ut-issl/c2a-core/pull/151) のように書く．
	- これまでの PR 一覧: https://github.com/ut-issl/c2a-core/pulls?q=is%3Apr+Update+main
1. [tag](https://github.com/ut-issl/c2a-core/tags) を打ち， [release](https://github.com/ut-issl/c2a-core/releases) を発行する．
	- tag 名は `v3.4.0` のようにする．
	- relase 名は `v3.4.0` や `v3.3.1 初版` のようにする．
	- release には以下を含める．
		- Release Note として簡潔な更新差分の箇条書き
		- `main` にマージしたときの PR のリンク

これを，だいたい以下のような粒度で行う．

- 最後のリリースからおおよそ１ヶ月程度が経過した場合（更新が少ない場合を除く）
- まとまった機能実装が一段落した場合
- [Tools/Overview](../Tools/overview.md) の互換性が失われた場合

ただし，あるまとまった機能の実装の PR が複数に分割され，一部がまだマージされていない場合はリリースを控えること．


## バージョニング
[Semantic Versioning](https://semver.org) を採用する．


## C2A Tools との互換性について
[Tools/Overview](../Tools/overview.md) の Tool に後方互換性のない更新があった場合は，新しいバージョンをリリースする．  
このとき，以下に注意する．

- C2A と同様の規則で Relase を発行する．
- release には以下を含める．
	- Release Note として簡潔な更新差分の箇条書き
	- 対応する最小 C2A Core バージョン
		- この Tool の更新が入ったときの C2A Core バージョン
	- この Tool に適合させたときの C2A Core の PR へのリンク

例: TBA

