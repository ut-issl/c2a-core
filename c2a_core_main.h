#ifndef C2A_CORE_MAIN_H_
#define C2A_CORE_MAIN_H_

void C2A_core_init(void);
void C2A_core_main(void);

// C2A の バージョンは Semantic Versioning を採用する
// これらの番号は，リリース時に手動で合わせる
// 詳細: Docs/General/release.md
#define C2A_CORE_VER_MAJOR (3)
#define C2A_CORE_VER_MINOR (9)
#define C2A_CORE_VER_PATCH (0)
#define C2A_CORE_VER_PRE   ("beta.6")

#endif
