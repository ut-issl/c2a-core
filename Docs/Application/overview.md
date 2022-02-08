# Overview of Application

## Application
- Application とは，以下の4つに分類される．
  - Core
  - User Defined
  - Middleware
  - Driver Instance
- Application は，以下の要素によって構成される．
  - Application 実体
    - 内部状態を保存する AppInfo 構造体
    - 初期化関数 (initializer)
    - 実行関数 (executor)
  - コマンド
  - テレメトリ

## Application実体
### 内部状態を保存する AppInfo構造体
- App 全体の情報を集約する構造体． global に const ポインタとして公開され，他 App や Tlm 生成部から read される．

### 初期化関数 (initializer)
- AM 初期化時に実行される．

### 実行関数 (executor)
- Task List から定期実行される．


## コマンド
- 処理の最小単位
- 以下のような処理をする
  - アプリケーションの一部処理を単独で行う
  - アプリケーションの内部状態を更新する


## テレメトリ
- アプリケーションの内部状態を送信するデータを規定する．
