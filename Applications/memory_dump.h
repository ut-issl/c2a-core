#ifndef MEMORY_DUMP_H_
#define MEMORY_DUMP_H_

#include "../System/ApplicationManager/app_info.h"
#include "../TlmCmd/common_cmd_packet.h"

#define MEM_TLM_ID      (0xff)
#define MEM_DUMP_WIDTH  (CTCP_MAX_LEN - 18)     // テレメパケット最大長 - ヘッダ長
#define MEM_MAX_SPAN    (0x00800000)            // 最大ダンプ幅は16MB(ADUサイズ制約)

typedef enum
{
  MEM_SUCCESS,
  MEM_NO_DATA
} MEM_ACK;

typedef struct
{
  uint32_t begin;
  uint32_t end;
  uint32_t adu_size;
  uint16_t adu_seq;
  uint8_t  adu_counter;
  uint32_t dest;
  uint32_t rp;
} MemoryDump;

extern const MemoryDump* const memory_dump;

AppInfo MEM_create_app(void);


// 2018/08/24
// 自分の解釈をコメントとして追加
CCP_CmdRet Cmd_MEM_SET_REGION(const CommonCmdPacket* packet);

// FIXME: CTCP 大改修が終わったら直す
// https://github.com/ut-issl/c2a-core/pull/217
#if 0
CCP_CmdRet Cmd_MEM_DUMP_REGION_SEQ(const CommonCmdPacket* packet);
// 1パケットに入り切らない場合は，最初のADU分割された最初のパケットのみダンプ
// もう一度送ると，その次のパケットがダンプ
// 最後はちゃんと止まる

CCP_CmdRet Cmd_MEM_DUMP_REGION_RND(const CommonCmdPacket* packet);
// ADU分割された場合，その途中のパケットからダンプ

CCP_CmdRet Cmd_MEM_DUMP_SINGLE(const CommonCmdPacket* packet);
// アドレスを指定して，ダンプ？
// Cmd_MEM_SET_REGION は無視？
#endif

CCP_CmdRet Cmd_MEM_LOAD(const CommonCmdPacket* packet);
// MEMにアップリンクして書き込み

CCP_CmdRet Cmd_MEM_SET_DESTINATION(const CommonCmdPacket* packet);
// Cmd_MEM_COPY_REGION_SEQのコピー先を指定

CCP_CmdRet Cmd_MEM_COPY_REGION_SEQ(const CommonCmdPacket* packet);
// destにrpを指定幅だけコピーしていく
// これもCmd_MEM_DUMP_REGION_SEQと同様に，何度も繰り返し発行して使う．

#endif
