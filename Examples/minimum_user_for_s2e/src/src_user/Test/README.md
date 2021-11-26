# C2A PYTEST
- CIなどで自動でテストできるようにしたpytest．
- C2Aは現状関数レベルのテストはなく，コマンド送信とテレメ結果で判断する，結合テストのみしかない．

## 環境
- python3 系列と以下のライブラリ
  - [python-wings-interface](https://github.com/ut-issl/python-wings-interface)
  - [c2a-enum-loader](https://github.com/ut-issl/c2a-enum-loader)
- C2A実行環境（特定のボードでもSILSでも可）
- WINGS

## 構成
### フォルダ構成
- C2Aと揃える．
- ファイル名は， `test_${c2a_src_filename}.py`．例えば次のようなもの．
  - './test/src_core/Applications/test_nop.py'
  - './test/src_user/Applications/UserDefined/test_tlm_mem_dump.py'

### 関数名
`test_hoge` という関数を定義すれば，それが実行される．

### デコレーター
- 実機実行のものは `@pytest.mark.real`
- SILS実行のものは `@pytest.mark.sils`
- 両方のものは両方のデコレータをつける．

### 注意
- テストと仕様が対応するように書くこと．
- 失敗ケースも正しく失敗されるようにアサーションを書くこと．
- HKが0.5Hzで降りてくるのは前提として良い．

## 実行
```
pytest -m real -v ./test/  # 実機の場合
pytest -m sils -v ./test/  # SILSの場合
# --capture=no オプションをつけると，printなども表示される
# 簡易表示にするには -v を取り除く
```
これにて，すべてのファイルに対してテストが走る．

個別ファイルをテストしたい場合は，以下などでOK．
```
pytest -m real -v ./test/src_user/Applications/UserDefined/test_tlm_mem_dump.py

or 

cd ./test/src_user/Applications/UserDefined/
pytest -m real -v test_tlm_mem_dump.py
```
