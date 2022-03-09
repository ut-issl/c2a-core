/**
 * @file
 * @brief  バッファリングされているテレメをパースしてMOBC内でかんたんに利用できるようにするためのテレメデータ構造体定義
 * @note   このコードは自動生成されています！
 */
#ifndef AOBC_TELEMETRY_DATA_DEFINITIONS_H_
#define AOBC_TELEMETRY_DATA_DEFINITIONS_H_

typedef struct
{
  struct
  {
    struct
    {
      uint16_t ver;
      uint16_t type;
      uint16_t sh_flag;
      uint16_t apid;
      uint16_t seq_flag;
      uint16_t seq_count;
      uint16_t packet_len;
    } ph;
    struct
    {
      uint8_t ver;
      uint32_t ti;
      uint8_t tlm_id;
      double global_time;
      uint32_t on_board_subnet_time;
      uint8_t dest_flags;
      uint8_t dr_partition;
    } sh;
    uint32_t tm_mode_time;
    double tm_utl_unixtime_epoch;
    double tm_cycles_per_sec_fix_ratio;
    uint8_t mm_sts;
    uint8_t mm_opsmode;
    uint8_t mm_opsmode_prev;
    uint8_t tdsp_current_id;
    uint32_t tdsp_cmd_last_err_time_master;
    uint8_t tdsp_cmd_last_err_time_step;
    uint16_t tdsp_cmd_last_err_id;
    int32_t tdsp_cmd_last_err_sts;
    int32_t gs_ret_from_if_rx;
    uint8_t tctf_last_recv_ack;
    uint8_t tctf_validate_status;
    uint32_t tctf_last_recv_time;
    uint8_t tctf_farm_pw;
    uint8_t tcp_last_recv_ack;
    uint8_t tlm_tx_port_type;
    uint32_t gs_cmd_counter;
    uint32_t gs_cmd_last_exec_time;
    uint16_t gs_cmd_last_exec_id;
    int32_t gs_cmd_last_exec_sts;
    uint32_t gs_cmd_last_err_time;
    uint16_t gs_cmd_last_err_id;
    int32_t gs_cmd_last_err_sts;
    uint32_t gs_cmd_err_counter;
    uint32_t rt_cmd_counter;
    uint32_t rt_cmd_last_exec_time;
    uint16_t rt_cmd_last_exec_id;
    int32_t rt_cmd_last_exec_sts;
    uint32_t rt_cmd_last_err_time;
    uint16_t rt_cmd_last_err_id;
    int32_t rt_cmd_last_err_sts;
    uint32_t rt_cmd_err_counter;
    uint32_t tl0_cmd_counter;
    uint8_t tl0_cmd_queued;
    uint32_t tl0_cmd_last_exec_time;
    uint16_t tl0_cmd_last_exec_id;
    int32_t tl0_cmd_last_exec_sts;
    uint32_t tl0_cmd_last_err_time;
    uint16_t tl0_cmd_last_err_id;
    int32_t tl0_cmd_last_err_sts;
    uint32_t tl0_cmd_err_counter;
    uint8_t tl0_cmd_soe_flag;
    uint8_t tl0_cmd_lockout_flag;
    uint32_t tl0_next_time;
    uint16_t tl0_next_id;
    uint32_t tl1_cmd_counter;
    uint8_t tl1_cmd_queued;
    uint32_t tl1_cmd_last_exec_time;
    uint16_t tl1_cmd_last_exec_id;
    int32_t tl1_cmd_last_exec_sts;
    uint32_t tl1_cmd_last_err_time;
    uint16_t tl1_cmd_last_err_id;
    int32_t tl1_cmd_last_err_sts;
    uint32_t tl1_cmd_err_counter;
    uint8_t tl1_cmd_soe_flag;
    uint8_t tl1_cmd_lockout_flag;
    uint32_t tl1_next_time;
    uint16_t tl1_next_id;
    uint32_t tl2_cmd_counter;
    uint8_t tl2_cmd_queued;
    uint32_t tl2_cmd_last_exec_time;
    uint16_t tl2_cmd_last_exec_id;
    int32_t tl2_cmd_last_exec_sts;
    uint32_t tl2_cmd_last_err_time;
    uint16_t tl2_cmd_last_err_id;
    int32_t tl2_cmd_last_err_sts;
    uint32_t tl2_cmd_err_counter;
    uint8_t tl2_cmd_soe_flag;
    uint8_t tl2_cmd_lockout_flag;
    uint32_t tl2_next_time;
    uint16_t tl2_next_id;
    uint8_t bct_blk_ptr;
    uint8_t bct_cmd_ptr;
    uint32_t bct_regd_time;
    uint16_t bct_regd_id;
    uint8_t ms_buffer_available;
    uint32_t ms_t2m_flush_interval;
    uint32_t rp_t2m_flush_interval;
    uint32_t ms_tlm_counter;
    uint8_t ms_tlm_queued;
    uint32_t st_tlm_counter;
    uint8_t st_tlm_queued;
    uint32_t rp_tlm_counter;
    uint8_t rp_tlm_queued;
    uint32_t tci_tx_cycle;
    uint32_t tci_tx_bitrate;
    uint8_t tci_vcid;
    uint32_t tci_vcdu_counter;
  } aobc_aobc;
  struct
  {
    struct
    {
      uint16_t ver;
      uint16_t type;
      uint16_t sh_flag;
      uint16_t apid;
      uint16_t seq_flag;
      uint16_t seq_count;
      uint16_t packet_len;
    } ph;
    struct
    {
      uint8_t ver;
      uint32_t ti;
      uint8_t tlm_id;
      double global_time;
      uint32_t on_board_subnet_time;
      uint8_t dest_flags;
      uint8_t dr_partition;
    } sh;
    uint32_t obc_tm_mode_time;
    double obc_tm_unixtime_at_ti0;
    uint8_t obc_mm_opsmode;
    uint8_t obc_mm_sts;
    uint8_t obc_mm_opsmode_prev;
    uint16_t obc_tdsp_current_id;
    uint8_t obc_tctf_last_recv_ack;
    uint32_t obc_tctf_last_recv_time;
    uint8_t obc_tcp_last_recv_ack;
    uint32_t obc_gs_cmd_counter;
    uint32_t obc_gs_cmd_last_exec_time;
    uint16_t obc_gs_cmd_last_exec_id;
    int8_t obc_gs_cmd_last_exec_sts;
    uint32_t obc_gs_cmd_last_err_time;
    uint16_t obc_gs_cmd_last_err_id;
    int8_t obc_gs_cmd_last_err_sts;
    uint32_t obc_tl0_cmd_counter;
    uint8_t obc_tl0_cmd_queued;
    uint32_t obc_tl0_cmd_last_exec_time;
    uint16_t obc_tl0_cmd_last_exec_id;
    int8_t obc_tl0_cmd_last_exec_sts;
    uint32_t obc_tl0_cmd_last_err_time;
    uint16_t obc_tl0_cmd_last_err_id;
    int8_t obc_tl0_cmd_last_err_sts;
    uint32_t obc_tl0_next_time;
    uint16_t obc_tl0_next_id;
    uint8_t obc_tl0_cmd_soe_flag;
    uint8_t obc_tl0_cmd_lockout_flag;
    uint8_t obc_tl1_cmd_soe_flag;
    uint8_t obc_tl1_cmd_lockout_flag;
    uint8_t obc_tl2_cmd_soe_flag;
    uint8_t obc_tl2_cmd_lockout_flag;
    struct
    {
      uint8_t is_enable;
      uint8_t is_clear_enable;
    } wdt;
    uint16_t obc_bct_blk_ptr;
    uint8_t obc_bct_cmd_ptr;
    uint32_t obc_bct_regd_time;
    uint16_t obc_bct_regd_id;
    struct
    {
      struct
      {
        uint32_t record_counter_total;
        uint16_t record_counters_high;
        uint16_t record_counters_middle;
        uint16_t record_counters_low;
        uint16_t record_counters_el;
        uint16_t record_counters_eh;
      } statistics;
      struct
      {
        uint8_t group;
        uint32_t local;
        uint8_t err_level;
        struct
        {
          uint32_t total_cycle;
          uint8_t step;
        } time;
        uint16_t note;
      } latest_event;
      struct
      {
        uint8_t group;
        uint32_t local;
        struct
        {
          uint32_t total_cycle;
          uint8_t step;
        } time;
        uint16_t note;
      } latest_high_event;
    } el;
    struct
    {
      struct
      {
        uint32_t respond_counter;
      } log_table;
    } eh;
    uint32_t anomaly_mobc_al_counter;
    uint32_t anomaly_mobc_al_latest_time_master;
    uint8_t anomaly_mobc_al_latest_time_step;
    uint8_t anomaly_mobc_al_latest_code_group;
    uint32_t anomaly_mobc_al_latest_code_local;
    uint8_t anomaly_mobc_al_latest_run_length;
    uint8_t anomaly_mobc_al_header;
    uint32_t obc_tl1_cmd_counter;
    uint32_t obc_tl2_cmd_counter;
    uint8_t obc_tl1_cmd_queued;
    uint8_t obc_tl2_cmd_queued;
    uint8_t obc_ah_action_counter;
    uint8_t obc_ah_latest_id;
    uint32_t obc_ah_respond_at;
    uint8_t obc_gs_cmd_err_counter;
    struct
    {
      struct
      {
        uint16_t cmd_code;
        uint8_t status;
        uint16_t exec_counter;
        int8_t last_exec_sts;
        struct
        {
          uint32_t total_cycle;
        } last_exec_time;
      } exec_logs0;
      struct
      {
        uint16_t cmd_code;
        uint8_t status;
        uint16_t exec_counter;
        int8_t last_exec_sts;
        struct
        {
          uint32_t total_cycle;
        } last_exec_time;
      } exec_logs1;
    } dcu;
    uint32_t git_rev_core;
    uint32_t git_rev_user;
  } aobc_hk;
} AOBC_TlmData;

#endif
