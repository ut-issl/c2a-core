# Coding Rule

## 概要
### コーディング規約とは
なぜ，コーディング規約というものがあるのでしょうか？
あまりSW開発の経験がない人にとっては，馴染みがなかったり，レビュー時にたくさん指摘されて煩わしいもの，と思うかもしれません．

例えば，HWの開発のことを考えてみましょう．
クリーンルームに入って作業をしようとしたときに，ドライバなどの工具がいつも同じ場所に保管してあると楽ですよね．
ネジを保管してるボックスを開いたら，インチネジとメートルネジが混ざって入っていたらどうでしょう？
事故の原因になりますよね．

コーディング規約とは，このような概念をSW開発にも適用したもの，と考えると，（多少語弊はあるかもしれませんが，）あながち間違ってはいないと思います．
一貫性をもったコードにして，筆者以外の開発者が理解するようにするだけではなく，適切なルールを設定することで，致命的なミスを防ぐことができます．


### 資料など
コード規約や読みやすいコードについては，『[リーダブルコード ―より良いコードを書くためのシンプルで実践的なテクニック](https://www.amazon.co.jp/dp/4873115655)』などを読むといいと思います．

コーディングなど，SW開発の思想についてなどは，個人的には『[UNIXという考え方―その設計思想と哲学](https://www.amazon.co.jp/dp/4274064069)』を読むのがおすすめです．

リーダブルコードは研究室にも転がってますし，両方とも鈴本が持ってますので，気軽に声をかけてください．
どちらも薄いので，すぐ読めると思います．

また，他のコード規約を読んでみるのもいいと思います．
C/C++のコード規約はたくさんありますが，例えば『[Google C++ スタイルガイド 日本語全訳](https://ttsuki.github.io/styleguide/cppguide.ja.html)』は比較的モダンでいいかもしれないです．


Googleの規約が話に上がったので，少しだけ規約について大事なことを引用しておきます．
少しでもコード規約についての雰囲気が伝われば幸いです．


> スタイルルールを重くしすぎないこと
> 
> 個々のスタイルルールは「所属するすべてのエンジニアが覚えなくてはならない」ことを正当化できるくらいに価値のあるものでなければなりません。 ルールの価値は「仮にそのルールが無かったら書かれたかもしれないコード」との比較で測定されます。このため、<b>仮に「非常に有害な慣行に対するルール」があったとしても、「そもそもそんなルールがなくとも普通はしないよね」という場合、そのルールの価値は低いと考えます</b>。 この原則は、どちらかというと、本スタイルガイドに書かれていないルールについて説明しています。例えば、gotoの濫用はこれに続く多くの原則に違反しますが、すでにそれが行われること自体がレアケースになっています。このため、本ガイドではgotoの濫用については取り扱いません。

.

> 常識を大切にし、一貫性を持ってください。
> 
> <b>コードを編集する前には、1～2分、コード全体を眺める時間をとって、そのコードの持つスタイルを判別してください</b>。 ifの周りにスペースがあったら、あなたもそのようにします。 コメントがアスタリスクで囲まれた箱になっていたら、あなたのコメントもアスタリスクで囲って箱にしましょう。
> 
> スタイルガイドラインを持つことの要点は、コーディングにおける共通のボキャブラリを持つということです。これによって、みんなが、どう話すかという話し方ではなく、何を話すかという話の内容に集中できるようになるのです。 我々はここでグローバルなスタイルルールを説明しています。これによって、みんなが、共通のボキャブラリを持つことができます。 しかし、ローカルなスタイルもまた重要なものです。 もし、あなたがファイルに追加したコードが、その周囲の既存のコードと大きく違って見えてしまうと、その境目によって読者のリズムを崩してしまいます。 これを避けるよう努めましょう。

ともに大事な考え方だと思っています．
「値を設定する関数名は `set_hoge` にする」とか，「TLに使われるBCのID定義は `BC_TL_HOGE` にする」とか，これまでの慣習によって決まってきてる数多ことや，
「配列サイズを超えてポインタ経由で操作しない」といった細かなことまでをすべて明文化することはとても難しいことで，コストもかかります．
こういったことは，議論が起き，必要性が増してきたり，ああこれは規約化した方が混乱なくいいよね，という話が出てくれば，その都度規約化していけばいいと思っています．


規約にがちがちになって，開発が進まないのは元も子もありません．
最初はすべての規約が守れないかもしれません．
でもそれでいいのです．それがマージされるのを防ぐために，フォーマッタといったツールや，レビュー体制があるのです．
開発経験を繰り返し，徐々に規約が守れるようになり，そして，徐々に，（C2Aのみならず何かの）SW開発プロジェクトそれぞれの雰囲気にあったコーディングができるようになっていけばいいのです．

最後に一つ，gitのtipsを．
git blameを使うことで，該当ファイルの各行がいつ変更されたか簡単にわかります．（マージされたPRへも飛べます．PRに飛べれば，関連する議論やissueが追えます．）
コードの雰囲気を掴むときなど，そのコードがいつどのタイミングでどういう文脈で更新されたかなどを見るときにきっと重宝しますよ．


### 規約のスコープについて
本規約は，基本的には命名規則など，表面的なことを重点的に扱います．
これは，C/C++のチュートリアルでも解説ドキュメントでもありません．
それについては，別途学習してください．

また，安全なコードを書くための指針といった規則（気をつけること）は，IPAが出している『[SEC BOOKS：ESCR Ver. 3.0：【改訂版】組込みソフトウェア開発向け コーディング作法ガイド［C言語版］ESCR Ver.3.0](https://www.ipa.go.jp/sec/publish/tn18-004.html)』を参照してください．
この中で，どれが大切で，どれはC2Aでは採用しない，などと言ったドキュメントは，近いうちにまとめます．


### ルールを満たしていない既存のコードなどについて
明らかな違反については，徐々に直していくべきです．
あるファイルを変更しようとして，それが規約違反であるならば，一緒にまわりも直してあげるといいと思います．
そうすることで，そのSWが全体的に改善されていくだけでなく，追加したコードと周りとの一貫性も強化されます．

明確な規約がないものについては，現在のコードの一貫性を保つためにも，一旦この規約から外れることがむしろ良い場合もあります．
局所的な一貫性を重視する，という考え方もあることを忘れないでください．

ただし，このようなものは，長期的にみるとあまり良いことではないので，適切な管理者に相談してください．


### その他
コード規約は完全ではありません．

規約策定者には自明と思われていても，新入生にはそうではないかもしれません．
そういった場合は，より明確で詳細な規約にすべきかもしれません．

規約自体が時が流れ不適切になっている場合もあります．
規約策定時には考慮されなかったケースがでてくることもあります．
そういった場合には，規約自体のアップデートが必要かもしれません．

何かあれば，誰でもいい（CDHや各リポジトリのMaintainerなど）ので相談してください．


### 凡例
本規約では，以下の凡例を用います．
カスケード方式で上書きされます．つまり，節で[M]でも項で[W]となっていた場合，[W]によって上書きされます．

- [M] : MUST．絶対守る．
- [W] : WANT．守ることを推奨する．
- [F] : 整備しようとしているフォーマッタによって，自動整形される．
- [C++] : C++にも関する．



## 基本 [M]
### スタイルについて
先頭大文字の `CamelCase`，小文字の `snake_case`，大文字の `SNAKE_CASE` などのスタイルがるが，原則として，この形式を優先する．
つまり，MOBC Utilityなどは `MOBCUtility` ではなく `MobcUtility` となるし，iPhoneも `Iphone` とする．


### ディレクトリ名・ファイル名
- ディレクトリ名は先頭大文字の `CamelCase`
- ファイル名は小文字の `snake_case`
	- C言語では，ファイル名がユニークではないといけない処理系があるため，[後述する各論](#個別箇所についての命名など-m)にて必要な接頭辞をつけること．


### スタイル [F]
- インデントは空白2つを単位とする．
	- いかなる場合もtabは許可しない．
- スタイルは[Allman style](https://en.wikipedia.org/wiki/Indent_style#Allman_style)に倣う．
```cpp
while (x == y)
{
  something();
  somethingelse();
}
finalthing();
```


### 文字コード
- 文字コードは基本的に UTF-8 で統一する．
	- 一部のバッチファイルなど， Shift-JIS のほうが圧倒的に楽なものについては例外を認める．
	- User側で Shift-JIS などを要求する場合 (HEW など) は， User 側のビルドワークフローで変換するなどで対応する．
- 改行コードはCR+LFで統一する．



## 命名 [M]
### 接頭辞
C言語は名前空間を切ることができず，各種命名がglobal空間で衝突する可能性がある．
そのために（たとえstaticなものだとしても）接頭辞を大文字で `PREFIX_` とつけるようにすることが基本である．
詳細は各規則を参照すること．

なお，接頭辞は， `DRIVER_SUPER_` のように複数単語になるのも認める．


### 変数名・関数名
- 小文字の `snake_case`
- 関数の基本は，`動詞+目的語`
- なお，コマンド，テレメトリとなる関数はこの限りではない．
- staticなものは接尾辞 `_` をつける．
- priavteな気持ちで設計したメンバは接尾辞 `_` をつける．
	- C言語なのでprivateという概念は存在しないが，気持ちとして．
- ハンガリアン記法は禁止．
	- ポインタを表す `p_` は，原則禁止だが，特別見にくいなど理由があれば許可する．

例外：

- コマンドは `Cmd_${PREFIX}_SNAKE_CASE`
- テレメトリは `Tlm_${TLM_NAME}_`
- 定数は `kCamelCase`
	- constがついたらすべて定数というわけではない．
	- constexprあるいはconstとして宣言され，プログラムの始めから終わりまで値が変わらない変数のことで，関数のconstをつけた引数などは該当しない．

例：
```cpp
uint32_t head_block;
static int FLASH_dump_region_rnd_(const CTCP* packet);
int Cmd_APP_DR_SET_PARAMS(const CTCP* packet);
static int Tlm_MOBC_(unsigned char* contents, int max_len);
extern const uint32_t FLASH_kMeiseiBlockBegin;
```


### 型名
- 基本的に先頭大文字の `CamelCase`
- 構造体名，Class名は先頭大文字の `CamelCase` [C++]
- enum名は例外的に大文字の `SNAKE_CASE` [C++]
	- enumについては，複雑なため，[enumの項](#enum)を参照すること．
	- C++本格移行時に修正する．
- 基本型に `typedef` を使用する場合は `snake_case_t`

例：
```cpp
typedef struct
{
  uint32_t              adc_setting;                //!< ADCの設定値
  uint16_t              adc_value[ADC_CH_MAX];      //!< ADCの値保持用（テレメ用）
  ADC_SCAN_STATUS_CODE  scan_status;                //!< scan status
} AdcUtility;

typedef unsigned char uint8_t;
typedef uint32_t flash_block_t;
```


### プリプロセッサディレクティブ
- 全て大文字の `SNAKE_CASE`

注意：

- なお，プリプロセッサディレクティブはインデントさせない．
- `#define` はstaticではないので，接尾辞 `_` は不要．



## 個別箇所についての命名など [M]
### Applications/UserDefined
- ファイル名とAppInfo構造体名を一致させる（スタイルを除く）
- AppInfo構造体名とそのインスタンス名を一致させる（スタイルを除く）
- 接頭辞は， `APP_HOGE` で `HOGE` 部分はケースバイケース．

例：

Applications/UserDefined/data_recorder.c
```cpp
static DataRecorder data_recorder_;
const DataRecorder* const data_recorder = &data_recorder_;


AppInfo APP_DR_create_app(void)
{
  return AI_create_app_info("data_recorder", NULL, APP_DR_update_);
}
```

Applications/UserDefined/data_recorder.h
```cpp
#ifndef DATA_RECORDER_H_
#define DATA_RECORDER_H_

#define APP_DR_MAX_PARTITIONS (16)

typedef struct
{
  uint8_t hoge;
} APP_DR_Partition;

typedef struct
{
  APP_DR_Partition partitions[APP_DR_MAX_PARTITIONS];
} DataRecorder;

int Cmd_APP_DR_SET_PARAMS(const CTCP* packet);
```


### Applications/DriverInstances
- ファイル名は `di_${IFやデバイス名}`
- Driver構造体名とそのインスタンス名を一致させる（スタイルを除く）
	- 特定のドライバ構造体のインスタンスが複数ある場合は，配列にまとめる．
	- 単一の場合は，スカラ形式を推奨するが，要素1の配列にしても良い．
		- その場合，そのC2A内ではすべてのドライバ構造体のインスタンスは配列にすること．
- 接頭辞は `DI_${IFやデバイス名}`

例：
```cpp
ファイル名
di_pcdu.c/h
di_xtx.c/h

接頭辞
DI_PCDU_
DI_XTX_

インスタンス名
static PCDU_Driver pcdu_driver_;
const PCDU_Driver* const pcdu_driver = &pcdu_driver_;
static XTX_Driver xtx_driver_;
const XTX_Driver* const xtx_driver = &xtx_driver_;
```

複数インスタンス例：
```cpp
typedef enum
{
  DI_RM3100_IDX_ON_AOBC = 0,
  DI_RM3100_IDX_ON_HOGE,
  DI_RM3100_IDX_ON_FUGA,
  DI_RM3100_IDX_MAX,
} DI_RM3100_IDX;

RM3100_Driver rm3100_driver[RM3100_IDX_MAX];
```


### Applications/Middleware
- ファイル名は `mw_${IFやデバイス名}`
- 接頭辞は `MW_${IFやデバイス名}`

例：
```cpp
ファイル名
mw_ccsds.c/h
mw_flash.c/h

接頭辞
MW_CCSDS_
MW_FLASH_

インスタンス名
static MwFlash mw_flash_;
const MwFlash* const mw_flash = &mw_flash_;
```


### Applications その他
`AR_APP_ID` enumの接頭辞は，
- `AR_NOP`: NOP
- `AR_APP_HOGE`: UserDefined
- `AR_DI_HOGE`: DI
- `AR_WM_HOGE`: MW

とし，基本的には `HOGE` は ファイル名にする．

つまり， 
- Applications/UserDefined/data_recorder.h → `AR_APP_DATA_RECORDER`
- Applications/DriverInstances/di_pcdu.h → `AR_DI_PCDU`

となる．


### Drivers
- ファイル名は `${IFやデバイス名}`
- 接頭辞は `${IFやデバイス名}`

例：
```
ファイル名
pcdu.c/h
xtx.c/h

接頭辞
PCDU_
XTX_
```

[Driver/Overview](../Driver/overview.md) も参照のこと．


### IfWrapper
基本的な命名は， `${IFやデバイス名}` とする．

このディレクトリは大きく分けて以下の２つのケースがある．（その限りではない）

1. `uart.h` で宣言したopen関数などを，`uart_issl6u_mobc.c` , `uart_sils.c` などで定義する
2. `uart.h` で宣言したopen関数などを，`uart_common.c` で定義し，さらにHW依存部を小さく集約し，各フォルダに配置する（宣言は `uart_common.h`）


1.のケース
```
IfWrapper/
  uart.h
  ./Common/
  ./Issl6uMobc/
    uart_issl6u_mobc.c        <- 名前空間： UART
    ./HogeChipset/
      uart_hoge_chipset.cpp   <- 名前空間： UART_HOGE_CHIPSET
  ./Sils/
    uart_sils.c               <- 名前空間： UART
```

2.のケース
```
IfWrapper/
  flash.h
  ./Common/
    flash_common.c            <- 名前空間： FLASH
    flash_common.h            <- より細かく切り出したHW依存の関数を宣言
    flash_hoge.c              <- 名前空間： FLASH_HOGE（不都合があったら考える）
  ./Issl6uMobc/
    flash_issl6u_mobc.c       <- 名前空間： FLASH
    ./HogeChipset/
      flash_hoge_chipset.cpp  <- 名前空間： FLASH_HOGE_CHIPSET
  ./Sils/
    flash_sils.c              <- 名前空間： FLASH
```

HWを示すファイル名は，再利用性を意識し，決定する．
例えば，Arduino互換マイコンを使っているのであれば `Arduino` でよく，ペリフェラルなどが特殊な一品物などを使っている場合は，`Issl6uMobc` のように，衛星名＋ボード名でよい．



### その他の場所のファイル名
C言語におけるファイル名のユニーク性を担保するために，以下のファイル名prefixをつける．
名前空間を切るために用いる接頭辞（変数名・関数名につけるもの）とは異なることに注意すること．

```
Settings/Modes/TaskLists
-> tl_

Settings/Modes/TaskLists/Elements
-> tl_elem_
旧 Settings/Modes/TaskLists/Composition

Settings/Modes/Transitions
-> sl_

Settings/AnomalyHandlerRules
-> ahr_

Settings/AnomalyLogger
-> al_

TlmCmd/NormalBlockCommandDefinition
-> nbc_
```



## 細かな記法 [M]
### 宣言
複数変数の単一行宣言は，同じ型のみ許す．
```cpp
uint8_t i, j, k;
```
ただし，以下のような初期値あり宣言は禁止する．
```cpp
uint8_t i, j, k = 0;
```
なお，ポインタの複数宣言は誤解を招くのでいかなる時も禁止する．
```cpp
uint8_t *p, i;
```

ポインタの宣言は，`*` を型名につける．
```cpp
uint8_t* p;    // OK
uint8_t *p;    // NG
```


### コードブロック
#### 単一行ブロック
特に`if`について．

単一行`if`は基本的には非推奨．
ただし，assertionなどで`if`が連続する場合などは，単一行の`if`を許可する．

このとき，
```cpp
if (hoge) fuga;
```
と，`{}`はなくてよい．

一方で
```cpp
if (hoge)
      fugafuga;
```
はバグを生みやすいので禁止．

なお，`while`や`for`，`else`の伴う`if`は，単一行は禁止．


#### switch [F]
switchのインデントは以下の流派を採用する．
```cpp
switch (ch)
{
case PORT_CH_RS422_MOBC_REPRO:
  RS422_reopen(hoge);
  break;
default:
  // NOT REACHED
  break;
}
```


#### その他
- ロジック的に確実に到達しない部分にコードを書かない．
- 書き忘れであるのか，意図しているのかがわかりにくいため，以下のルールを設ける．
	- `else if`の伴う`if`において，`else`ブロックのないものは禁止
		- `else`ブロックを作り， `// NOT REACHED` などのコメントを残す
	- 同様に`default`ブロックのない`switch`も禁止


### 改行など [W, F]
みやすさのために，１行は100文字（TBD）に制限する．

また，改行に伴う演算子は後置とする．
```cpp
if (hogehoge ||
    fugafuga ||
    piyopiyo)
{
  var = hoge +
        fuga +
        piyo;
```

また，入れ子は少ないほうが好ましい．
目安としてインデントが５個を超えたときは改善を考える．
そのためには，以下のようなことが効果的である．

- 処理を関数化して切り出し，見通しを良くする．これは説明的なコードになることにもつながる．
- アサーションなどは関数冒頭でさっさとreturnする．エラー処理もはやくreturnする．
- そもそももっとシンプルなロジックにならないか考える．



### include [C++]
includeガードは `${ファイル名大文字SNAKE_CASE}_H_` とする．

`hoge.c` の順番は，以下．[F]
```cpp
hoge.h
<空行>
C システムヘッダ
C++ システムヘッダ
<空行>
ほかのライブラリの.hファイル
プロジェクトの.hファイル
```

なお，これはフォーマッタの整備時に変わる可能性がある！


### 略語
基本的には略語を禁止する．
変数名が長くなっても良いことにする．

使っていい略語はホワイトリストで管理し，現状以下とする．（適宜追加）
```
tlm, cmd
min, max
num, id, idx
ok, err
コンポ名（mobcやxtxなど）
名前空間になるもの
```



## 安全のために [M]
### 関数の引数関連
- 受け取ったら原則必ずassertionを行う．
	- 引数が想定範囲内のものであるか？（配列の長さ，値の範囲など）
	- 範囲外であれば，適切に処理する．
	- 関数呼び出しは入れ子になっていくが，一貫したassertionを行うこと．返り値なども一貫したものにすること．
		- enumで定義するとか，エラー情報格納構造体をやり取りするとか．最近のコード（2021年以降）を参考にすること．
- 参照渡しで呼び出し元からみて変更されないことを保障すべきものはconst修飾子を付けておくこと．
- 関数内で変更しない引数はconst修飾子を付けておくこと．[W]
	- 基本型の値渡しにおいては，必ずしもconstを付ける必要はない．


### 関数の戻り値関連
- 返り値は必ずassertionを行う．
	- 全体として一貫した管理がなされている関数群ではこの限りではない．
- 使わない返り値はvoidにキャストして，以後使わないことを明示する．
- 返り値としてboolを使うことは非推奨 [W]
	- なにがtrue/falseなのかは一意ではないことが多いため．
	- 代わりにenumを使い，その名前が説明的であることが望ましい．
- 値を返すものにエラーコードを混ぜない [W]
	- get_address の返り値が正だとaddressで負だとエラーコード，といったものは一貫性がないので禁止する．
	- 例えば，一連の関数群の返り値はエラーコードに統一し，値はポインタで返すことで対応する．


## 各機能について [M]
### enum
enumは中身を含めてすべてグローバル名前空間に漏れる．
そのため命名を長くし，全て大文字の `SNAKE_CASE` で書くこと．

enumの型名は
```
HOGE_CODE
HOGE_STATUS
HOGE_ERRORS
HOGE_MODE
HOGE_ACK
HOGE_ID
```

などを推奨する．

enumのメンバは，型名の末の単語を消し or 消さずに，内容を続ける．

例：
```cpp
typedef enum
{
  FLASH_SUCCESS,
  FLASH_NO_DATA,
  FLASH_DESTINATION_INSIDE_REGION,
  FLASH_API_BAD_BLOCK,
  FLASH_API_TIMEOUT,
  FLASH_API_NOT_INITIALIZED,
  FLASH_API_ERR,
  FLASH_UNKNOWN
} FLASH_ACK;

typedef enum
{
  DRIVER_SUPER_REC_STATUS_FINDING_HEADER,
  DRIVER_SUPER_REC_STATUS_RECEIVING_HEADER,
  DRIVER_SUPER_REC_STATUS_RECEIVING_DATA,
  DRIVER_SUPER_REC_STATUS_RECEIVING_FOOTER,
  DRIVER_SUPER_REC_STATUS_FIXED_FRAME,
  DRIVER_SUPER_REC_STATUS_DISABLE,
  DRIVER_SUPER_REC_STATUS_ERR
} DRIVER_SUPER_REC_STATUS;
```

enumはintのように使ってはいけない．

- `+=` や `++` は使用してはいけない．例えば `HOGEHOGE` という型の `hogehoge` という変数名のenumをインクリメントしたいときは，`hogehoge++;` ではなく `hogehoge = (HOGEHOGE)(hogehoge + 1);` と書くこと．
- 返り値がenumの関数は，必ず同じ型のenumを返すこと．よくある例として，ある関数から呼ばれた別の関数の返り値（異なるenum）をそのまま返してはいけない．
- enumにintを代入してはいけない．代入する場合はCのキャストを明示的に行うこと．たとえば `HOGEHOGE hogehoge = (HOGEHOGE)0` など．(とはいえ，enumで宣言した以上は可能な限りenumを代入する．)

基本的に，定義時にコメントで型を指定すること．

- enumをテレメで落とすときや，その他のエラーコードなどと共用する場合など，型サイズを想定する場合がありうる．
- コメントで型サイズを明記すること．例えば以下．
```cpp
/**
 * @enum   DRIVER_SUPER_ERR_CODE
 * @brief  DriverSuperの汎用エラーコード
 * @note   uint8_tを想定
 */
typedef enum
{
  DRIVER_SUPER_ERR_CODE_OK   = 0,     // 返り値は継承先やIF先で上書きされるため，OKは0であることが必須
  DRIVER_SUPER_ERR_CODE_ERR  = 1
} DRIVER_SUPER_ERR_CODE;
```

例外：

コマンドとテレメトリのCODEは以下のようにする．
```cpp
typedef enum
{
  Cmd_CODE_NOP = 0x0000,
  Cmd_CODE_TMGR_SET_TIME = 0x0001,
  Cmd_CODE_MAX
} CMD_CODE;

typedef enum
{
  Tlm_CODE_MOBC = 0x00,
  Tlm_CODE_DR = 0x0c,
  TLM_CODE_MAX
} TLM_CODE;
```


### `#define` の使用について
- `.h` 内においての定義は，他でも利用されるものに限る．
- `.c` 内での定義は， `static const` 変数に置き換えられないか考えること．
	- `static const` であればメモリ上に展開されるため，memloadによる再プロが可能．
	- 逆に言うと，それによってメモリ配置やサイズが変わるものなど，再プロ出来ないものは `#define` が望ましい．



## コミット時，マージ時に守るべきこと [M]
- PR発行時には，最低限，実機開発環境でビルドできることは確認する．
	- 一度作業ブランチに最新developをマージし，その状態でビルドできることを確認するのがベスト．
- AppRegistryへの追加などをマージして良いのは，実機試験を行った後に限る．
- 不要なincludeはビルド時間を不必要に増大させるので，消すこと．
- gitのお作法については，git関連資料を参照すること．
	- branch方針や，merge方針，Pull Request，issueなど．
	- 適当にwebにあるものや，研究室wiki，git講習会資料などを参照．
- そのマージリクエストのスコープを意識すること．
	- スコープ外の修正が入ってませんか？ PRを分割しましょう． [W]



## 互換性を保ったコーディングについて [M]
搭載ソフトウェアは，SILS/HILSでの実行のため，HEWなどのマイコンIDEとS2E環境などのSILS/HILS環境の両方でコンパイルできる必要がある．
基本的なガードはCIにあるが，以下のことに留意すること．


### 全般
- ハードウェアに依存する書き方をしないこと．
	- 通信や特定のデバイス利用に関しては，IfWrapperフォルダ直下の関数を用いれば問題ない．
	- もちろん，マイコンのレジスタの操作などを直接書くとパソコンでは動かせなくなる．
- エンディアンの違い
	- ほとんどのパソコンがリトルエンディアンなのに対し，いくつかのマイコンはビッグエンディアンである．
		- 当然バイトオーダーは異なるし，ビットフィールドの並びも異なる．
		- ドライバなどで，バイト列をintやfloatに変換するところなどは要注意．
	- エンディアンに依存しないコードを書くか，以下のような定義を適切に使うこと．また，`endian_memcpy` などといった共用関数を積極的に利用すること．
- `SILS_FW` と `IS_LITTLE_ENDIAN` を混同して使わないこと
```cpp
#ifndef SILS_FW
// #define SILS_FW //ここでdefineする！
#endif

#ifdef SILS_FW
#define IS_LITTLE_ENDIAN // SILS環境により変更する
#endif
```


### サイズ指定の型の利用について
- `uint8_t` などのサイズ指定の方を積極的に用いる．
- `size_t` は， `sizeof` 演算子に関わる変数以外の使用は禁止する．
- ジェネラルなIFとしての関数引数・返り値としての `int` や，`for` でのindexとしての `int` はよい．


### その他
- コメント
	- `// hoge` のように，`//` の後には空白を入れること．
- ファイル末尾には必ず空行を入れること．
	- Unexpected EOFのエラーを防ぐため．


### VC++コンパイラに関する注意
- コメントについて
	- VC++コンパイラでは，下記のコメントはエラーになるので使用しない．
		- `//**********************************`
		- `/*` が閉じていないと解釈されるためと思われる．
		- 過去衛星のコードでは多用されていたため，暫定的に全て `// ******` に置換した．
- C++ビルドを行う上でHEWコンパイラではビルドが通るが注意しなければならない点をまとめる．
	- constポインタをconstではないポインタに代入してはいけない．
		- これが発生するときは，設計が間違っているか，constで良い変数をconstにし忘れている．
			- 本来設計が間違っていれば修正をすべきだが，どうしてもの場合はCのキャストでconstを外し，`const_cast` といったコメントを入れる．
				- 例) `PL_Node *pos = (PL_Node*)PL_get_head(&(tl_cmd_list[line_no])); // const_cast`
	- C++の予約語は変数や関数名に使用してはいけない
		- 過去にあった例は `virtual` や `new` といった変数．
- コンパイラのワーニングについて
	- 警告レベルを3以上に設定し，そのワーニングはすべてエラーとして扱うことにする．
- その他Windowsについて
	- `#pragma once` の利用は禁止．


### HEWコンパイラに関する注意
- コンパイラバージョンの違い
	- 最新のVC++がC99に準拠しているのに対し，HEWのコンパイラはC89準拠なので，古い書き方を強制される．
	- 例えば：
		- 関数内変数の宣言は，全て関数の頭で行わなければならない．
			- `for (int i = 0; i < 10; i++)` みたいなのは `i` の宣言部でコンパイルエラーになる
		- include忘れで関数定義がなくてもコンパイルが通ってしまう．
- コンパイラのワーニングについて
	- もっとも強いコンパイラオプションにし，すべてエラーとして扱うことにする．



## その他細々
### 説明的なコード，書くべきでないコメント [W]
[W]としているが，これについては，各位意識しながらコーディングすること．

明らかなことをわざわざコメントしてはならない．
コメントは，そのコードがなぜそれをしているのかといった理由や背景，制約について書くべきである．

コードとはそもそもそれ自体が説明的であるように，変数名や関数名を設定すべきである．
たとえば，マジックナンバーが嫌われるのは，その数字が何を意味するのかがコメントなしでは判断できないからである．
その数字を変数名にいれ，名前をつけることで，コメントに頼ることなくわかりやすいコードとなる．

心持ちとしては，「コメントを書くのがめんどくさいので書かなくて良いんだ」ではなく，
「（処理内容を説明する）コメントが不要になるように，労力を払って説明的なコードを書く」という意識が大事となる．

例えば，以下のようなコメントは避けるべきである．
このように関数名や返り値などを設定することで，コメントが不要になっている．
```cpp
// Select port
port = select_port_(ch);
if (port == NULL) return ERR_CODE_CH_ERR;

// Read mode
ret = read_mode_(port, device_address, size);
if (ret != ERR_CODE_OK) return ret;
```

同様の理由で，次のようなコードやコメントは避けるべきである．
```cpp
// ADC生値をdegreeに変換
temp_degc = ADC_get_value(PORT_CH_ADC_TEMPERATURE) / 2.0f - 273.0f;

// Reopen port
port->endMaster();
ret = init(port);
```

これらは以下のようにすると，コード自体が説明的になる．
```cpp
temp = convert_adc_value_to_degree(ADC_get_value(PORT_CH_ADC_TEMPERATURE));

ret = reopen_port(port);
```

```cpp
float convert_adc_value_to_degree(uint16_t adc_val)
{
  return adc_val / 2.0f - 273.0f;
}

int reopen_port(int port)
{
  port->endMaster();
  return init(port);
}
```


### インライン関数 [M]
禁止とする．

memloadなどによる部分的な再プロをやりやすくするため．
パフォーマンスなどの問題が発生した場合はその都度議論する．


### その他細かなこと [W]
- 予約語の前後は空白をいれる．`(),{}` などの外側も空白を入れる．二項演算子の両側にも空白をいれる． [M,F]
- 見やすく書く．
	- 一行を長くしすぎない．
	- 適切に中間変数を置くことで，処理を追いやすくする．
		- 変数には変数名がつくので，わかりやすくなる．
	- など
- 意味のないものは書かない．逆に書いたものには意味がある．
	- 意味のない修正はgit logのノイズになることに留意すること．またマージ時に不要なコンフリクトを生じさせることにもつながる．
	- 不要な空白をいれない．[M]
		- 特に目につくのは行末や空行のインデント．
	- 不要な文字列やコードを書かない．[M]
		- `;` などが不要な場所にあったとしてもコンパイルは通るが，意味がないのできちんと消すこと．
	- 意味もなく多数（３行程度以上）の空行を入れない．空行の行数には意味があること．[M]
		- ひとまとまりの処理を１行空行をいれ，さらに大きな区切りや関数区切りに２行の空行をいれる，など．
- そのコードが担うべきスコープを意識すること．
	- その処理は本当にそこでやるべきですか？


### メタコメント [M]
あとで一括して grep できるように，メタコメントを定義する．

- `TODO` を含む文字列: ToDo
- `FIXME` を含む文字列: 今後修正する箇所（必須な箇所）

例えば，以下のように使う．

```cpp
  // [TODO] 現在は，シンプル化のためにヘッダサイズ0の場合は未実装（詳細はヘッダファイル参照）
  // FIXME: CRC未対応なので，追加する
```

`TODO` を含む，であるので， `[TODO]` でも `TODO:` でも問わない．


### Doxygenコメント [M]
- TBA などを参照のこと．
- その他は，最近コミットされたものを参考にすること．
- 方針としては， `.h` として公開される関数は必ず `.h` の関数定義に記入すること．
	- 自明なgetter/setterはその限りではない．なお，このとき，getter/setterの対象となるメンバの宣言に，きちんとしたコメントが書いてあることが必須．
- その他のstatic関数類も，重要なもの，複雑なものは書くこと（基本的には書いてほしい）．

### フォーマッタ
- clang formatterあたりを使用予定．
- [詳細](https://github.com/ut-issl/c2a-core/issues/41)
