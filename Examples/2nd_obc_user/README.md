# C2A 2ND OBC USER SAMPLE for CORE DEVELOPMENT

## 概要
C2A 間通信の開発のために， [`Examples/minimum_user`](/Examples/minimum_user) とセットで実行し，それと通信し合うことを目的とした 2nd OBC (非 MOBC) の User Sample.

## 注意
- 2nd OBC という名前だが，コード上は AOBC を想定している．（APID など）
- あくまで C2A 間通信開発のためなので，最小限の Tlm, Cmd などしか登録されてないなど，大幅に省略している．
- したがって，C2A の User のサンプルとしては， [`Examples/minimum_user`](/Examples/minimum_user) を参照するほうが望ましい．
- 一方で， MOBC との通信部分や， 2nd OBC としての振る舞いについてはこちらを参照すること．
- C2A 間通信については， [`Docs/Driver/Communication with Components`](/Docs/Driver/communication_with_components.md) も参照すること．
