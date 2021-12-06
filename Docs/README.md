# C2A Documents

## 本ドキュメントの役割
これは， C2A についてのドキュメントです．

C2A に関する説明の棲み分けは，次のようになっています．

- C2A のコードとそのコメント
	- マスター情報
	- 変数や構造体，関数などの定義などを Doxygen 形式で記述
- C2A リファレンス: https://github.com/ut-issl/c2a-reference
	- C2A のコードから自動生成されるもの (Doxygen)
	- 変数や構造体の定義，関数の相関図なども閲覧可能
- 本ドキュメント
	- マスター情報はコードとそのコメントなため，それを補足する情報など
		- C2A 全体の説明
		- 設計思想
		- 規則など
		- 操作方法や設定方法などの手順
		- などなど
	- したがって，マスター情報の二重管理を避けるため，本ドキュメントには，変数定義の説明などは記載しないこと！

以上より，本ドキュメントと合わせてコードを見ることを推奨する．  
（@ドキュメント記入者: 各ドキュメントから関連コードにリンクが張ってある状態が望ましい．）


## 目次

1. General Information
	1. [Overview](./General/Overview.md)
	1. [Coding Rule](./General/CodingRule.md)
1. Application Layer
	1. [Overview](./Application/Overview.md)
	1. How to add a application
1. Core Layer
	1. Overview
	1. [Tips](./Core/Tips.md)
1. Driver
	1. [Overview](./Driver/Overview.md)
1. SILS
	1. Overview
	1. [S2E Integration](./Sils/S2eIntegration.md)
1. Tips
	1. [Parameter Settings](./Tips/ParameterSettings.md)
1. Tools
	1. [Overview](./Tools/Overview.md)
