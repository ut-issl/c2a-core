/**
 * @file git_revision.h
 * @brief git revisionをコードに埋め込むための準備をする
 */
#ifndef GIT_REVISION_H_
#define GIT_REVISION_H_

#include <src_user/Library/stdint.h>

#include <src_user/Settings/git_revision_config.h> //!< 設定ないしcommit hashを読み込む

/*! @brief コンパイルオプションないしuserの設定で定義されたcommit hashを読み込んで埋め込みます
 * git revisionの埋め込みは，以下の定数を通して行われます(経緯: #82)．
 *   GIT_REVISION_C2A_CORE          C2A coreのcommit hash(文字列)
 *   GIT_REVISION_C2A_CORE_SHORT    C2A coreのcommit hash short(0xから始まる数値)
 *   GIT_REVISION_C2A_USER          C2A userのcommit hash(文字列)
 *   GIT_REVISION_C2A_USER_SHORT    C2A userのcommit hash short(0xから始まる数値)
 * これらの定数は，Cコンパイラの引数からdefine(-Dなど)してもよいですし，
 * src_user/Settings/git_revision_config.hでdefineしてもよいです．
 */

/**
 * @note CMakeを使う場合はコマンドラインオプションからのdefineを推奨します．
 *   この場合，src_user/Settings/git_revision_config.hでC2A_GIT_REVISION_FROM_OPTIONをdefineしてください(他には何も要りません)．
 */

/**
 * @note コマンドラインオプションからのdefineが開発環境の都合などにより困難な場合は，
 *   src_user/Script/Git/revision.sh/batをビルドパイプライン上で実行するようにします．
 *   このスクリプトは上記の定数群をdefineしたsrc_user/Settings/git_revision_config.hを自動生成します．
 *   この場合，src_user/Settings/git_revision_config.hはcommit・ビルドの度に変更されるため，gitignoreしておいた方がよいでしょう．
 */

/**
 * @note ビルドパイプラインをうまく組めないなど，上記２つの手段が使えない，かつ，revisionを埋め込まなくてもよい場合は，
 *   src_user/Settings/git_revision_config.hでC2A_GIT_REVISION_IGNOREをdefineしてください．
 *   こうすると，0が埋め込まれます．
 */

#ifdef C2A_GIT_REVISION_IGNORE
// commit hashを埋め込まない場合

#define GIT_REVISION_C2A_CORE         "0000000000000000000000000000000000000000"
#define GIT_REVISION_C2A_CORE_SHORT   0x0000000
#define GIT_REVISION_C2A_USER         "0000000000000000000000000000000000000000"
#define GIT_REVISION_C2A_USER_SHORT   0x0000000

#else

// commit hashを埋め込むはずなのにsrc_user/Settings/git_revision_config.hで設定されていない
#if !defined(C2A_GIT_REVISION_FROM_OPTION) && !defined(C2A_GIT_REVISION_GENERATED)
#error "please specify git revision config in src_user/Settings/git_revision_config.h"
#endif // error

#endif // C2A_GIT_REVISION_IGNORE

// これをc2a_core_main.cで埋め込む
extern const char GIT_REV_CORE[41];
extern const uint32_t GIT_REV_CORE_SHORT;
extern const char GIT_REV_USER[41];
extern const uint32_t GIT_REV_USER_SHORT;

#endif // GIT_REVISION_H_
