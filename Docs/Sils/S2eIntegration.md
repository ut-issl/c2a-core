# S2E Integration
C2A 標準 SILS として， S2E (FIXME: 公開後にリンクを貼る) がある．

これは，シミュレータである S2E 内部で，フライトソフトウェアである C2A を動かすことが可能である．  
詳細は S2E ドキュメント (TBA) を参照されたいが， S2E 側の `CMakeLists.txt` で `set(BUILD_C2A_AS_CXX ON)` する必要がある．（[経緯](https://github.com/ut-issl/c2a-core/pull/35)）  

現行では， S2E 側の `CMakeLists.txt` は次のようになる．
```
  set(BUILD_C2A_AS_CXX ON)
  add_subdirectory(${C2A_DIR} C2A_CORE)
```
