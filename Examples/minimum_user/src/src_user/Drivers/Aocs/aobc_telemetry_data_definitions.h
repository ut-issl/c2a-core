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
    struct
    {
      uint32_t counter;
      uint8_t queued;
      uint32_t last_exec_time;
      uint16_t last_exec_id;
      int32_t last_exec_sts;
      uint32_t last_err_time;
      uint16_t last_err_id;
      int32_t last_err_sts;
      uint32_t err_counter;
      uint8_t soe_flag;
      uint8_t lockout_flag;
      uint32_t next_time;
      uint16_t next_id;
    } tlc_gs;
    struct
    {
      uint32_t counter;
      uint8_t queued;
      uint32_t last_exec_time;
      uint16_t last_exec_id;
      int32_t last_exec_sts;
      uint32_t last_err_time;
      uint16_t last_err_id;
      int32_t last_err_sts;
      uint32_t err_counter;
      uint8_t soe_flag;
      uint8_t lockout_flag;
      uint32_t next_time;
      uint16_t next_id;
    } tlc_bc;
    struct
    {
      uint32_t counter;
      uint8_t queued;
      uint32_t last_exec_time;
      uint16_t last_exec_id;
      int32_t last_exec_sts;
      uint32_t last_err_time;
      uint16_t last_err_id;
      int32_t last_err_sts;
      uint32_t err_counter;
      uint8_t soe_flag;
      uint8_t lockout_flag;
      uint32_t next_time;
      uint16_t next_id;
    } tlc_tlm;
    uint8_t bct_blk_ptr;
    uint8_t bct_cmd_ptr;
    uint32_t bct_regd_time;
    uint16_t bct_regd_id;
    uint32_t ms_tlm_counter;
    uint8_t ms_tlm_queued;
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
    uint32_t obc_gs_cmd_counter;
    uint32_t obc_gs_cmd_last_exec_time;
    uint16_t obc_gs_cmd_last_exec_id;
    int8_t obc_gs_cmd_last_exec_sts;
    uint32_t obc_gs_cmd_last_err_time;
    uint16_t obc_gs_cmd_last_err_id;
    int8_t obc_gs_cmd_last_err_sts;
    uint32_t obc_rt_cmd_counter;
    uint32_t obc_rt_cmd_last_exec_time;
    uint16_t obc_rt_cmd_last_exec_id;
    int32_t obc_rt_cmd_last_exec_sts;
    uint32_t obc_rt_cmd_last_err_time;
    uint16_t obc_rt_cmd_last_err_id;
    int32_t obc_rt_cmd_last_err_sts;
    struct
    {
      uint32_t counter;
      uint8_t queued;
      uint32_t last_exec_time;
      uint16_t last_exec_id;
      int8_t last_exec_sts;
      uint32_t last_err_time;
      uint16_t last_err_id;
      int8_t last_err_sts;
      uint32_t next_time;
      uint16_t next_id;
      uint8_t soe_flag;
      uint8_t lockout_flag;
    } obc_tlc_gs;
    struct
    {
      uint32_t counter;
      uint8_t queued;
      uint8_t soe_flag;
      uint8_t lockout_flag;
    } obc_tlc_bc;
    struct
    {
      uint32_t counter;
      uint8_t queued;
      uint8_t soe_flag;
      uint8_t lockout_flag;
    } obc_tlc_tlm;
    struct
    {
      uint8_t is_enable;
      uint8_t is_clear_enable;
    } wdt;
    uint8_t dummy_data0;
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
        uint32_t note;
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
    uint8_t obc_gs_cmd_err_counter;
    uint32_t git_rev_core;
    uint32_t git_rev_user;
    uint32_t dummy_data1;
    uint32_t dummy_data2;
    uint32_t dummy_data3;
    uint32_t dummy_data4;
    uint32_t dummy_data5;
    uint32_t dummy_data6;
    uint32_t dummy_data7;
    uint32_t dummy_data8;
    uint32_t dummy_data9;
    uint32_t dummy_data10;
    uint32_t dummy_data11;
    uint32_t dummy_data12;
    uint32_t dummy_data13;
    uint32_t dummy_data14;
    uint32_t dummy_data15;
    uint32_t dummy_data16;
    uint32_t dummy_data17;
    uint32_t dummy_data18;
    uint32_t dummy_data19;
    uint32_t dummy_data20;
    uint32_t dummy_data21;
    uint32_t dummy_data22;
    uint32_t dummy_data23;
    uint32_t dummy_data24;
    uint32_t dummy_data25;
    uint32_t dummy_data26;
    uint32_t dummy_data27;
    uint32_t dummy_data28;
    uint32_t dummy_data29;
    uint32_t dummy_data30;
    uint32_t dummy_data31;
    uint32_t dummy_data32;
    uint32_t dummy_data33;
    uint32_t dummy_data34;
    uint32_t dummy_data35;
    uint32_t dummy_data36;
    uint32_t dummy_data37;
    uint32_t dummy_data38;
    uint32_t dummy_data39;
    uint32_t dummy_data40;
    uint32_t dummy_data41;
    uint32_t dummy_data42;
    uint32_t dummy_data43;
    uint32_t dummy_data44;
    uint32_t dummy_data45;
    uint32_t dummy_data46;
    uint32_t dummy_data47;
    uint32_t dummy_data48;
    uint32_t dummy_data49;
    uint32_t dummy_data50;
    uint32_t dummy_data51;
    uint32_t dummy_data52;
    uint32_t dummy_data53;
    uint32_t dummy_data54;
    uint32_t dummy_data55;
    uint32_t dummy_data56;
    uint32_t dummy_data57;
    uint32_t dummy_data58;
    uint32_t dummy_data59;
    uint32_t dummy_data60;
    uint32_t dummy_data61;
  } aobc_hk;
} AOBC_TlmData;

#endif
