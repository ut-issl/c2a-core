# S2E Integration
C2A 標準 SILS として， S2E (FIXME: 公開後にリンクを貼る) というシミュレータがある．

これにより，フライトソフトウェアである C2A を S2E 内部で動かすことが可能である．  
詳細はS2Eドキュメント (TBA) を参照のこと．

現行の S2E では C2A を C++ としてビルドしなければならないため，その際は `BUILD_C2A_AS_CXX` というオプションを使用してビルドする．
（[オプションの追加経緯](https://github.com/ut-issl/c2a-core/pull/35)）

具体的には， S2E 側の `CMakeLists.txt` を次のようにする．
```
  set(BUILD_C2A_AS_CXX ON)
  add_subdirectory(${C2A_DIR} C2A_CORE)
```
