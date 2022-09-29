#pragma section REPRO
/**
 * @file
 * @brief  CCP の汎用 Utility
 */
#include "common_cmd_packet_util.h"
#include "command_analyze.h"
#include "../Library/endian.h"
#include <stddef.h>     // for NULL
#include <string.h>

/**
 * @struct CCP_ParamGenerator
 * @brief  param を構築するための一時領域
 */
typedef struct
{
  CommonCmdPacket packet;   //!< 作成する CCP の param を保存するためのバッファ
  uint8_t  err_flag;        //!< エラーが発生したか
  uint8_t  param_idx;       //!< 今どこまでの param を作ったか
  uint16_t len;             //!< param 長
  CMD_CODE cmd_id;          //!< どのコマンドの param か
} CCP_ParamGenerator;

static CommonCmdPacket CCP_util_packet_;
static CCP_ParamGenerator CCP_param_generator_;


/**
 * @brief NOP cmd の RTC CCP を作る
 * @param[in,out] packet: CCP
 * @return void
 */
void CCP_form_nop_rtc_(CommonCmdPacket* packet);

/**
 * @brief RealTime Command を生成する. CCP_form_* の実体.
 * @note  packet, param は NULL 出ないことが保証されていることを前提とする
 * @param[in,out] packet: CCP
 * @param[in]     cmd_id: CMD_CODE
 * @param[in]     param:  パラメタの先頭アドレス
 * @param[in]     len:    パラメタ長
 * @return void
 */
static void CCP_form_rtc_(CommonCmdPacket* packet, CMD_CODE cmd_id, const uint8_t* param, uint16_t len);

/**
 * @brief CCP パラメタ開始位置に対する n 番目の引数の offset を計算する
 * @param[in]  cmd_id: CMD_CODE
 * @param[in]  n: N番目の引数 （0起算）
 * @param[out] offset: offset
 * @return CCP_UTIL_ACK
 */
static CCP_UTIL_ACK CCP_calc_param_offset_(CMD_CODE cmd_id, uint8_t n, uint16_t* offset);

/**
 * @brief  Param Generator でエラーを発生した際に呼ぶエラー保存関数
 * @param void
 * @retval CCP_UTIL_ACK_PARAM_ERR
 */
static CCP_UTIL_ACK CCP_raise_err_at_param_generator_(void);

/**
 * @brief  Param Generator で n byte のパラメタを登録
 * @note   Param Generator の使い方は CCP_reset_param_for_packet の doxygen コメントを参照のこと
 * @param[in] param: 登録するパラメタ
 * @param[in] byte: パラメタのサイズ
 * @return CCP_UTIL_ACK
 */
static CCP_UTIL_ACK CCP_prepare_param_for_packet_(void* param, uint8_t byte);


CCP_CmdRet CCP_make_cmd_ret(CCP_EXEC_STS exec_sts, uint32_t err_code)
{
  CCP_CmdRet ret;
  ret.exec_sts = exec_sts;
  ret.err_code = err_code;
  return ret;
}


CCP_CmdRet CCP_make_cmd_ret_without_err_code(CCP_EXEC_STS exec_sts)
{
  return CCP_make_cmd_ret(exec_sts, 0);
}


void CCP_form_nop_rtc_(CommonCmdPacket* packet)
{
  CCP_form_rtc(packet, Cmd_CODE_NOP, NULL, 0);
}


void CCP_form_app_cmd(CommonCmdPacket* packet, cycle_t ti, AR_APP_ID id)
{
  // FIXME: この uint32_t は環境依存なので，依存しないように直す
  //        適切に直すことで， CCP_form_tlc の返り値をみなくて良くなるはず．
  //        Cmd_AM_EXECUTE_APP の引数取得部分と同時に直すべきだが，パラメタサイズは CmdDB から取得可能なはず．
  const uint8_t* param;
  uint16_t len;

  CCP_init_param_for_packet(Cmd_CODE_AM_EXECUTE_APP);
  CCP_prepare_uint32_param_for_packet(id);
  CCP_get_prepared_param_for_packet(&param, &len);

  CCP_form_tlc(packet, ti, Cmd_CODE_AM_EXECUTE_APP, param, len);
}


CCP_UTIL_ACK CCP_form_rtc(CommonCmdPacket* packet, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (packet == NULL)
  {
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  if (param == NULL && len != 0)
  {
    CCP_form_nop_rtc_(packet);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  if (CA_ckeck_cmd_param_len(cmd_id, len) != CA_ACK_OK)
  {
    CCP_form_nop_rtc_(packet);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  CCP_form_rtc_(packet, cmd_id, param, len);

  return CCP_UTIL_ACK_OK;
}


CCP_UTIL_ACK CCP_form_tlc(CommonCmdPacket* packet, cycle_t ti, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (packet == NULL)
  {
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  if (param == NULL && len != 0)
  {
    CCP_form_nop_rtc_(packet);
    CCP_convert_rtc_to_tlc(packet, ti);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  if (CA_ckeck_cmd_param_len(cmd_id, len) != CA_ACK_OK)
  {
    CCP_form_nop_rtc_(packet);
    CCP_convert_rtc_to_tlc(packet, ti);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  CCP_form_rtc_(packet, cmd_id, param, len);
  CCP_convert_rtc_to_tlc(packet, ti);

  return CCP_UTIL_ACK_OK;
}


CCP_UTIL_ACK CCP_form_rtc_to_other_obc(CommonCmdPacket* packet, APID apid, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (packet == NULL)
  {
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  if (param == NULL && len != 0)
  {
    CCP_form_nop_rtc_(packet);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  // NOTE: 他の OBC のコマンドは cmd_table に保存されていないので param チェックできない

  CCP_form_rtc_(packet, cmd_id, param, len);
  CCP_set_apid(packet, apid);

  return CCP_UTIL_ACK_OK;
}


CCP_UTIL_ACK CCP_form_tlc_to_other_obc(CommonCmdPacket* packet, cycle_t ti, APID apid, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (packet == NULL)
  {
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  if (param == NULL && len != 0)
  {
    CCP_form_nop_rtc_(packet);
    CCP_convert_rtc_to_tlc(packet, ti);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  // NOTE: 他の OBC のコマンドは cmd_table に保存されていないので param チェックできない

  CCP_form_rtc_(packet, cmd_id, param, len);
  CCP_convert_rtc_to_tlc(packet, ti);
  CCP_set_apid(packet, apid);

  return CCP_UTIL_ACK_OK;
}


CCP_UTIL_ACK CCP_form_block_deploy_cmd(CommonCmdPacket* packet, TLCD_ID tl_no, bct_id_t block_no)
{
  const uint8_t* param;
  uint16_t len;

  if (packet == NULL)
  {
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  if ((tl_no >= TLCD_ID_MAX) || (block_no >= BCT_MAX_BLOCKS))
  {
    // タイムラインのline番号、ブロックコマンド番号が範囲外の場合異常判定
    CCP_form_nop_rtc_(packet);
    return CCP_UTIL_ACK_PARAM_ERR;
  }

  CCP_init_param_for_packet(Cmd_CODE_TLCD_DEPLOY_BLOCK);
  CCP_prepare_uint8_param_for_packet(tl_no);
  CCP_prepare_bct_id_param_for_packet(block_no);
  CCP_get_prepared_param_for_packet(&param, &len);

  return CCP_form_rtc(packet, Cmd_CODE_TLCD_DEPLOY_BLOCK, param, len);
}


static void CCP_form_rtc_(CommonCmdPacket* packet, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  CCP_set_common_hdr(packet);
  CCP_set_id(packet, cmd_id);
  CCP_set_exec_type(packet, CCP_EXEC_TYPE_RT);
  CCP_set_dest_type(packet, CCP_DEST_TYPE_TO_ME);
  CCP_set_ti(packet, 0);  // RTの場合、TIは0固定
  CCP_set_param(packet, param, len);
}


void CCP_convert_rtc_to_tlc(CommonCmdPacket* packet, cycle_t ti)
{
  if (packet == NULL) return;
  CCP_set_exec_type(packet, CCP_EXEC_TYPE_TL_FROM_GS);  // TL なので，一旦仮で入れる
  CCP_set_ti(packet, ti);
}


PH_ACK CCP_register_rtc(CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  if (CCP_form_rtc(&CCP_util_packet_, cmd_id, param, len) != CCP_UTIL_ACK_OK)
  {
    return PH_ACK_INVALID_PACKET;
  }

  return PH_analyze_cmd_packet(&CCP_util_packet_);
}


PH_ACK CCP_register_tlc(cycle_t ti, TLCD_ID tlcd_id, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  CCP_EXEC_TYPE type = CCP_get_exec_type_from_tlcd_id(tlcd_id);

  if (type == CCP_EXEC_UNKNOWN)
  {
    return PH_ACK_INVALID_PACKET;
  }

  if (CCP_form_tlc(&CCP_util_packet_, ti, cmd_id, param, len) != CCP_UTIL_ACK_OK)
  {
    return PH_ACK_INVALID_PACKET;
  }

  CCP_set_exec_type(&CCP_util_packet_, type);
  return PH_analyze_cmd_packet(&CCP_util_packet_);
}


PH_ACK CCP_register_tlc_asap(cycle_t ti, TLCD_ID tlcd_id, CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  CCP_EXEC_TYPE type = CCP_get_exec_type_from_tlcd_id(tlcd_id);
  const PacketList* pl = PH_get_packet_list_from_exec_type(type);

  if (pl == NULL) return PH_ACK_INVALID_PACKET;
  if (PL_is_full(pl)) return PH_ACK_PL_LIST_FULL;

  if (!PL_is_empty(pl))
  {
    cycle_t head = CCP_get_ti((const CommonCmdPacket*)(PL_get_head(pl)->packet));
    cycle_t tail = CCP_get_ti((const CommonCmdPacket*)(PL_get_tail(pl)->packet));

    if (head <= ti && ti <= tail)
    {
      uint16_t i;
      uint16_t active_nodes = PL_count_active_nodes(pl);
      const PL_Node* current = PL_get_head(pl);

      for (i = 0; i < active_nodes; ++i)
      {
        cycle_t current_ti;

        if (current == NULL) break;
        current_ti = CCP_get_ti((const CommonCmdPacket*)(current->packet));
        if (current_ti < ti)
        {
          // do nothing
        }
        else if (current_ti == ti)
        {
          ++ti;
        }
        else
        {
          break;
        }

        current = current->next;
      }
    }
  }

  if (CCP_form_tlc(&CCP_util_packet_, ti, cmd_id, param, len) != CCP_UTIL_ACK_OK)
  {
    return PH_ACK_INVALID_PACKET;
  }
  CCP_set_exec_type(&CCP_util_packet_, type);
  return PH_analyze_cmd_packet(&CCP_util_packet_);
}


CCP_CmdRet CCP_form_and_exec_rtc(CMD_CODE cmd_id, const uint8_t* param, uint16_t len)
{
  CCP_UTIL_ACK ret = CCP_form_rtc(&CCP_util_packet_, cmd_id, param, len);
  if (ret != CCP_UTIL_ACK_OK)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_PACKET_FMT_ERR);
  }

  return PH_dispatch_command(&CCP_util_packet_);
}


CCP_CmdRet CCP_form_and_exec_block_deploy_cmd(TLCD_ID tl_no, bct_id_t block_no)
{
  CCP_UTIL_ACK ret = CCP_form_block_deploy_cmd(&CCP_util_packet_, tl_no, block_no);
  if (ret != CCP_UTIL_ACK_OK)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_PACKET_FMT_ERR);
  }

  return PH_dispatch_command(&CCP_util_packet_);
}


CCP_EXEC_TYPE CCP_get_exec_type_from_tlcd_id(TLCD_ID tlcd_id)
{
  switch (tlcd_id)
  {
  case TLCD_ID_FROM_GS:
    return CCP_EXEC_TYPE_TL_FROM_GS;

  case TLCD_ID_DEPLOY_BC:
    return CCP_EXEC_TYPE_TL_DEPLOY_BC;

  case TLCD_ID_DEPLOY_TLM:
    return CCP_EXEC_TYPE_TL_DEPLOY_TLM;

#ifdef TLCD_ENABLE_MISSION_TL
  case TLCD_ID_FROM_GS_FOR_MISSION:
    return CCP_EXEC_TYPE_TL_FOR_MISSION;
#endif

  default:
    return CCP_EXEC_TYPE_UNKNOWN;
  }
}


uint8_t* CCP_get_1byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n)
{
  static uint8_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 1;
  CCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}


uint16_t* CCP_get_2byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n)
{
  static uint16_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 2;
  CCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}


uint32_t* CCP_get_4byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n)
{
  static uint32_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 4;
  CCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}


uint64_t* CCP_get_8byte_param_from_packet(const CommonCmdPacket* packet, uint8_t n)
{
  static uint64_t ret;
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  uint8_t param_size = 8;
  CCP_UTIL_ACK ack;

  ret = 0;

  ack = CCP_calc_param_offset_(cmd_id, n, &offset);
  if (ack != CCP_UTIL_ACK_OK) return &ret;

  if (CA_get_cmd_param_size(cmd_id, n) != param_size) return &ret;

  endian_memcpy(&ret, CCP_get_param_head(packet) + offset, (size_t)param_size);
  return &ret;
}


uint16_t CCP_get_raw_param_from_packet(const CommonCmdPacket* packet, void* dest, uint16_t max_copy_len)
{
  uint16_t offset = 0;
  CMD_CODE cmd_id = CCP_get_id(packet);
  CCP_UTIL_ACK ack;
  int32_t copy_len;

  if (!CA_has_raw_param(cmd_id)) return 0;

  ack = CCP_calc_param_offset_(cmd_id, (uint8_t)(CA_get_cmd_param_num(cmd_id) - 1), &offset);
  if (ack != CCP_UTIL_ACK_OK) return 0;

  copy_len = CCP_get_param_len(packet) - offset;
  if (copy_len < 0) return 0;
  if (max_copy_len != 0 && copy_len > max_copy_len)
  {
    copy_len = max_copy_len;
  }

  memcpy(dest,  CCP_get_param_head(packet) + offset, (size_t)copy_len);
  return (uint16_t)copy_len;
}


const uint8_t* CCP_get_raw_param_head(const CommonCmdPacket* packet)
{
  uint16_t offset;
  CMD_CODE cmd_id = CCP_get_id(packet);

  if (!CA_has_raw_param(cmd_id)) return NULL;

  offset = CA_get_cmd_param_min_len(cmd_id);
  return CCP_get_param_head(packet) + offset;
}


uint16_t CCP_get_raw_param_len(const CommonCmdPacket* packet)
{
  int32_t len;
  CMD_CODE cmd_id = CCP_get_id(packet);

  if (!CA_has_raw_param(cmd_id)) return 0;

  len = CCP_get_param_len(packet) - CA_get_cmd_param_min_len(cmd_id);
  return (len < 0) ? 0 : (uint16_t)len;
}


static CCP_UTIL_ACK CCP_calc_param_offset_(CMD_CODE cmd_id, uint8_t n, uint16_t* offset)
{
  uint8_t i;
  if (cmd_id >= CA_MAX_CMDS) return CCP_UTIL_ACK_PARAM_ERR;
  if (CA_get_cmd_param_num(cmd_id) <= n) return CCP_UTIL_ACK_PARAM_ERR;

  *offset = 0;
  for (i = 0; i < n; ++i)
  {
    (*offset) += CA_get_cmd_param_size(cmd_id, i);
  }
  return CCP_UTIL_ACK_OK;
}


void CCP_init_param_for_packet(CMD_CODE cmd_id)
{
  CCP_param_generator_.err_flag = 0;
  CCP_param_generator_.param_idx = 0;
  CCP_param_generator_.len = 0;
  CCP_param_generator_.cmd_id = cmd_id;

  if (cmd_id >= CA_MAX_CMDS) CCP_raise_err_at_param_generator_();
}


CCP_UTIL_ACK CCP_get_prepared_param_for_packet(const uint8_t** param_head, uint16_t* len)
{
  CCP_ParamGenerator* p_pg = &CCP_param_generator_;
  *len = 0;
  *param_head = CCP_get_param_head(&(p_pg->packet));

  if (p_pg->err_flag) return CCP_UTIL_ACK_PARAM_ERR;
  if (CA_get_cmd_param_num(p_pg->cmd_id) != p_pg->param_idx) return CCP_UTIL_ACK_PARAM_ERR;

  *len = p_pg->len;
  return CCP_UTIL_ACK_OK;
}


CCP_UTIL_ACK CCP_prepare_uint8_param_for_packet(uint8_t param)
{
  return CCP_prepare_param_for_packet_(&param, 1);
}


CCP_UTIL_ACK CCP_prepare_int8_param_for_packet(int8_t param)
{
  return CCP_prepare_param_for_packet_(&param, 1);
}


CCP_UTIL_ACK CCP_prepare_uint16_param_for_packet(uint16_t param)
{
  return CCP_prepare_param_for_packet_(&param, 2);
}


CCP_UTIL_ACK CCP_prepare_int16_param_for_packet(int16_t param)
{
  return CCP_prepare_param_for_packet_(&param, 2);
}


CCP_UTIL_ACK CCP_prepare_uint32_param_for_packet(uint32_t param)
{
  return CCP_prepare_param_for_packet_(&param, 4);
}


CCP_UTIL_ACK CCP_prepare_int32_param_for_packet(int32_t param)
{
  return CCP_prepare_param_for_packet_(&param, 4);
}


CCP_UTIL_ACK CCP_prepare_uint64_param_for_packet(uint64_t param)
{
  return CCP_prepare_param_for_packet_(&param, 8);
}


CCP_UTIL_ACK CCP_prepare_int64_param_for_packet(int64_t param)
{
  return CCP_prepare_param_for_packet_(&param, 8);
}


CCP_UTIL_ACK CCP_prepare_float_param_for_packet(float param)
{
  return CCP_prepare_param_for_packet_(&param, 4);
}


CCP_UTIL_ACK CCP_prepare_double_param_for_packet(double param)
{
  return CCP_prepare_param_for_packet_(&param, 8);
}


CCP_UTIL_ACK CCP_prepare_bct_id_param_for_packet(bct_id_t param)
{
  return CCP_prepare_param_for_packet_(&param, SIZE_OF_BCT_ID_T);
}


CCP_UTIL_ACK CCP_prepare_raw_param_for_packet(const uint8_t* param, uint16_t len)
{
  CCP_ParamGenerator* p_pg = &CCP_param_generator_;
  uint16_t offset = CA_get_cmd_param_min_len(p_pg->cmd_id);

  if (p_pg->err_flag) return CCP_raise_err_at_param_generator_();
  if (!CA_has_raw_param(p_pg->cmd_id)) return CCP_raise_err_at_param_generator_();
  if (CA_get_cmd_param_num(p_pg->cmd_id) != p_pg->param_idx + 1)
  {
    return CCP_raise_err_at_param_generator_();
  }
  if (CCP_get_max_param_len() < offset + len) return CCP_raise_err_at_param_generator_();
  if (p_pg->len != offset) return CCP_raise_err_at_param_generator_();

  memcpy((void*)(CCP_get_param_head(&(p_pg->packet)) + offset), param, len);   // const_cast
  p_pg->len += len;
  p_pg->param_idx++;
  return CCP_UTIL_ACK_OK;
}


static CCP_UTIL_ACK CCP_raise_err_at_param_generator_(void)
{
  CCP_param_generator_.err_flag = 1;
  return CCP_UTIL_ACK_PARAM_ERR;
}


static CCP_UTIL_ACK CCP_prepare_param_for_packet_(void* param, uint8_t byte)
{
  CCP_ParamGenerator* p_pg = &CCP_param_generator_;
  uint16_t offset = 0;

  if (p_pg->err_flag) return CCP_raise_err_at_param_generator_();
  if (CA_get_cmd_param_size(p_pg->cmd_id, p_pg->param_idx) != byte)
  {
    return CCP_raise_err_at_param_generator_();
  }
  if (CCP_calc_param_offset_(p_pg->cmd_id, p_pg->param_idx, &offset) != CCP_UTIL_ACK_OK)
  {
    return CCP_raise_err_at_param_generator_();
  }
  if (p_pg->len != offset) return CCP_raise_err_at_param_generator_();

  endian_memcpy((void*)(CCP_get_param_head(&(p_pg->packet)) + offset), param, byte);   // const_cast
  p_pg->len += byte;
  p_pg->param_idx++;
  return CCP_UTIL_ACK_OK;
}

#pragma section
