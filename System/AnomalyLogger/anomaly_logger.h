#ifndef ANOMALY_LOGGER_H_
#define ANOMALY_LOGGER_H_

#include <stddef.h> // for size_t

#include "../TimeManager/obc_time.h"
#include "../../TlmCmd/common_tlm_cmd_packet.h"
#include <src_user/Settings/AnomalyLogger/anomaly_group.h>

#define AL_TLM_PAGE_SIZE (32)                                 //!< アノマリロガーのログテーブルの1テレメトリパケット(=1ページ)に格納されるログ数（ページネーション用）
#define AL_TLM_PAGE_MAX  (4)                                  //!< アノマリロガーのログテーブルのページ数（ページネーション用）
#define AL_RECORD_MAX (AL_TLM_PAGE_SIZE * AL_TLM_PAGE_MAX)    //!< 最大何個のアノマリを記録できるか
// [TODO] 2018/12/10
// AL_FULL のままでいいのか？
// FLASHに書いていって，消してく？
// 定期的に全部おろして，clearするのもありかもしれない
// アノマリレコーダーの方は，runlengthで圧縮せずに，tiとともに保存したい

// anomaly_logger_params.h にて，以下の定数の再定義をする
//   - AL_TLM_PAGE_SIZE
//   - AL_TLM_PAGE_MAX
//   - AL_RECORD_MAX
// また
//   AL_DISALBE_AT_C2A_CORE
// を定義することで， C2A CORE での AL_add_anomaly を抑制できる．
// AL から Event Logger への移行が済んでいるものに対しては，定義を推奨する．
// なお，これを定義しても， AL, AH 自体の機能は引き続き利用可能である．
#include <src_user/Settings/System/anomaly_logger_params.h>

typedef enum
{
  AL_CORE_GROUP_ANOMALY_LOGGER,
  AL_CORE_GROUP_MODE_MANAGER,
  AL_CORE_GROUP_TASK_MANAGER,
  AL_CORE_GROUP_TASK_DISPATCHER,
  AL_CORE_GROUP_APP_MANAGER,
  AL_CORE_GROUP_TLCD,
  AL_CORE_GROUP_MAX
} AL_CORE_GROUP;

typedef enum
{
  AL_SUCCESS,
  AL_NEARLY_FULL,
  AL_FULL
} AL_ACK;

// AL_add_anomalyの返り値
enum
{
  AL_ADD_SUCCESS,         // 正常終了
  AL_ADD_ERR_INVALID,     // エラー
  AL_ADD_DISABLE_LOGGING  // ロギング無効化（棄却）
};

typedef struct
{
  uint32_t group;
  uint32_t local;
} AL_AnomalyCode;

typedef struct
{
  ObcTime time;
  AL_AnomalyCode code;
  size_t run_length;
} AL_AnomalyRecord;

typedef struct
{
  size_t counter;           // AL_add_anomaly された回数
  size_t header;            // 次にaddされると書き込まれる位置．ARはリングバッファではなく，Fullでとまる．
  AL_AnomalyRecord records[AL_RECORD_MAX];
  uint8_t page_no;
  uint8_t is_logging_enable[AL_GROUP_MAX / 8];    // bit単位で情報を保持する
  uint16_t threshold_of_nearly_full;
} AnomalyLogger;

extern const AnomalyLogger* const anomaly_logger;

const AL_AnomalyRecord* AL_get_record(size_t pos);

const AL_AnomalyRecord* AL_get_latest_record(void);

void AL_initialize(void);

int AL_add_anomaly(uint32_t group, uint32_t local);

int AL_enable_logging(uint32_t group);

int AL_disable_logging(uint32_t group);

int  AL_is_logging_enable(uint32_t group);

void AL_clear(void);

CCP_EXEC_STS Cmd_AL_ADD_ANOMALY(const CTCP* packet);

CCP_EXEC_STS Cmd_AL_CLEAR_LIST(const CTCP* packet);

CCP_EXEC_STS Cmd_AL_SET_PAGE_FOR_TLM(const CTCP* packet);

CCP_EXEC_STS Cmd_AL_INIT_LOGGING_ENA_FLAG(const CTCP* packet);

CCP_EXEC_STS Cmd_AL_ENABLE_LOGGING(const CTCP* packet);

CCP_EXEC_STS Cmd_AL_DISABLE_LOGGING(const CTCP* packet);

CCP_EXEC_STS Cmd_AL_SET_THRES_OF_NEARLY_FULL(const CTCP* packet);

#endif
