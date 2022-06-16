# SILS MOCKUP
- C2A Core 開発のために， Core 単体で C89 ビルドするための IF Wrapper Mockup
    - Core 開発では特定のハードウェアを仮定できないので， SILS を用いるが，現在のデフォルトの SILS である S2E は， C++ ビルドを行う．
    - それでは， C89 の Core のコンパイルエラー，ワーニングのチェックができないため，非環境依存の Mockup を用いて， C89 ビルドする環境を用意したものがこれ．
    - 検証したいコードは `src_user/IfWrapper/Sils` には存在しないはずなので，ビルドのみで実行しないのであれば，これで良い．
- 参考: https://github.com/ut-issl/c2a-core/issues/27
