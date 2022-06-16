#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import time

import isslwings as wings
import pytest

ROOT_PATH = "../../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()


# コードと整合をとる
EL_TLOG_TLM_PAGE_SIZE = 28
EL_TLOG_TLM_PAGE_MAX_HIGH = 4
EL_TLOG_TLM_PAGE_MAX_MIDDLE = 4
EL_TLOG_TLM_PAGE_MAX_LOW = 4
EL_CLOG_TLM_PAGE_SIZE = 18
EL_CLOG_TLM_PAGE_MAX_HIGH = 4
EL_CLOG_TLM_PAGE_MAX_MIDDLE = 4
EL_CLOG_TLM_PAGE_MAX_LOW = 4

EL_TLOG_LOG_SIZE_MAX_EL = 16
EL_CLOG_LOG_SIZE_MAX_EL = 8

EL_ERROR_LEVEL_HIGH = 0
EL_ERROR_LEVEL_MIDDLE = 1
EL_ERROR_LEVEL_LOW = 2
EL_ERROR_LEVEL_EL = 3
EL_ERROR_LEVEL_EH = 4
EL_ERROR_LEVEL_MAX = 5
EL_ERROR_LEVEL_TLM_DICT = ["HIGH", "MIDDLE", "LOW", "EL"]

EL_CORE_GROUP_NULL = c2a_enum.EL_CORE_GROUP_NULL
EL_CORE_GROUP_EVENT_LOGGER = c2a_enum.EL_CORE_GROUP_EVENT_LOGGER
EL_CORE_GROUP_EL_DROP_CLOG1 = c2a_enum.EL_CORE_GROUP_EL_DROP_CLOG1
EL_CORE_GROUP_EL_DROP_CLOG2 = c2a_enum.EL_CORE_GROUP_EL_DROP_CLOG2
EL_GROUP_TEST = c2a_enum.EL_GROUP_TEST
EL_GROUP_TEST1 = c2a_enum.EL_GROUP_TEST1
EL_GROUP_MAX = c2a_enum.EL_GROUP_MAX

EL_EVENT_LOCAL_TLOG_HIGH_HALF_FULL = 1
EL_EVENT_LOCAL_TLOG_HIGH_FULL = 2


# 更新が頻繁にあるので，グローバルで
# もっといい方法ない？
g_el_tlm = {}
g_count_total = 0
g_counts = []
g_tlm_info = {}
g_latest_event = {}
g_tlog_em_tlm = {}
g_tlog_count_total = 0
g_tlog_count = 0
g_tlog_tlm_info = {}
g_tlog_capacity = 0
g_tlog_wp = 0
g_tlog_is_enable_overwrite = 0
g_tlog_is_overflow = 0
g_clog_em_tlm = {}
g_clog_count_total = 0
g_clog_count = 0
g_clog_tlm_info = {}
g_clog_capacity = 0

g_el_tlm_pre = g_el_tlm
g_count_total_pre = g_count_total
g_counts_pre = g_counts
g_tlm_info_pre = g_tlm_info
g_latest_event_pre = g_latest_event
g_tlog_em_tlm_pre = g_tlog_em_tlm
g_tlog_count_total_pre = g_tlog_count_total
g_tlog_count_pre = g_tlog_count
g_tlog_tlm_info_pre = g_tlog_tlm_info
g_tlog_capacity_pre = g_tlog_capacity
g_tlog_wp_pre = g_tlog_wp
g_tlog_is_enable_overwrite_pre = g_tlog_is_enable_overwrite
g_tlog_is_overflow_pre = g_tlog_is_overflow
g_clog_em_tlm_pre = g_clog_em_tlm
g_clog_count_total_pre = g_clog_count_total
g_clog_count_pre = g_clog_count
g_clog_tlm_info_pre = g_clog_tlm_info
g_clog_capacity_pre = g_clog_capacity


# 各種設定の確認など
@pytest.mark.real
@pytest.mark.sils
def test_event_logger_init_check():
    update_all_tlm()
    initialize_el()
    update_all_tlm()
    assert g_count_total == 0
    for err_level in range(EL_ERROR_LEVEL_MAX):
        assert g_counts[err_level] == 0

    # high だけ確認
    assert g_tlog_capacity == EL_TLOG_TLM_PAGE_SIZE * EL_TLOG_TLM_PAGE_MAX_HIGH
    assert g_clog_capacity == EL_CLOG_TLM_PAGE_SIZE * EL_CLOG_TLM_PAGE_MAX_HIGH


@pytest.mark.real
@pytest.mark.sils
def test_event_logger_set_params():
    # TODO: 範囲外アクセスがある EL_ERROR_LEVEL_EL, EL_ERROR_LEVEL_EH のテレメもおろしておくこと
    assert change_tlog_tlm_page(0, EL_ERROR_LEVEL_HIGH) == "SUC"
    assert check_tlog_tlm_page(0, EL_ERROR_LEVEL_HIGH) == 1
    assert change_tlog_tlm_page(1, EL_ERROR_LEVEL_MIDDLE) == "SUC"
    assert check_tlog_tlm_page(1, EL_ERROR_LEVEL_MIDDLE) == 1
    assert change_tlog_tlm_page(1, EL_ERROR_LEVEL_LOW) == "SUC"
    assert check_tlog_tlm_page(1, EL_ERROR_LEVEL_LOW) == 1
    assert change_tlog_tlm_page(0, EL_ERROR_LEVEL_EL) == "SUC"
    assert check_tlog_tlm_page(0, EL_ERROR_LEVEL_EL) == 1

    assert change_tlog_tlm_page(1, EL_ERROR_LEVEL_EL) == "PRM"
    assert change_tlog_tlm_page(EL_TLOG_TLM_PAGE_MAX_LOW, EL_ERROR_LEVEL_LOW) == "PRM"

    assert change_clog_tlm_page(0, EL_ERROR_LEVEL_HIGH) == "SUC"
    assert check_clog_tlm_page(0, EL_ERROR_LEVEL_HIGH) == 1
    assert change_clog_tlm_page(1, EL_ERROR_LEVEL_MIDDLE) == "SUC"
    assert check_clog_tlm_page(1, EL_ERROR_LEVEL_MIDDLE) == 1
    assert change_clog_tlm_page(1, EL_ERROR_LEVEL_LOW) == "SUC"
    assert check_clog_tlm_page(1, EL_ERROR_LEVEL_LOW) == 1
    assert change_clog_tlm_page(0, EL_ERROR_LEVEL_EL) == "SUC"
    assert check_clog_tlm_page(0, EL_ERROR_LEVEL_EL) == 1

    assert change_clog_tlm_page(1, EL_ERROR_LEVEL_EL) == "PRM"
    assert change_clog_tlm_page(EL_CLOG_TLM_PAGE_MAX_LOW, EL_ERROR_LEVEL_LOW) == "PRM"


# 基本的になイベント発行のロジックの確認
# ほぼEL_ERROR_LEVEL_HIGH で行う
@pytest.mark.real
@pytest.mark.sils
def test_event_logger_record_event():
    initialize_el()
    change_tlog_tlm_page(0, EL_ERROR_LEVEL_HIGH)
    change_clog_tlm_page(0, EL_ERROR_LEVEL_HIGH)
    update_all_tlm()

    # ### Cmd_EL_RECORD_EVENT のアサーション
    local0 = 1
    local1 = 5
    note0 = 2
    note1 = 3

    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_MAX, note0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "PRM"
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_EL, note0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "PRM"
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_MAX, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "PRM"

    update_all_tlm()

    assert g_count_total_pre == g_count_total
    for err_level in range(EL_ERROR_LEVEL_MAX):
        assert g_counts_pre[err_level] == g_counts[err_level]

    # ### Cmd_EL_RECORD_EVENT での登録
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"

    # カウンタが上がってるか
    update_all_tlm()
    assert g_count_total_pre + 1 == g_count_total
    for err_level in range(EL_ERROR_LEVEL_MAX):
        if err_level == EL_ERROR_LEVEL_HIGH:
            assert g_counts_pre[err_level] + 1 == g_counts[err_level]
        else:
            assert g_counts_pre[err_level] == g_counts[err_level]

    assert_latest_log(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0)
    assert_latest_tlog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0)
    assert assert_latest_clog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0) == 1

    # 初回のdelta時間は 0
    assert g_clog_em_tlm["EL_CLOG.CLOGS.LOG0.DELTA_RECORD_TIME.TOTAL_CYCLE"] == 0
    assert g_clog_em_tlm["EL_CLOG.CLOGS.LOG0.DELTA_RECORD_TIME.STEP"] == 0
    last_log_time_cycle = g_clog_em_tlm["EL_CLOG.CLOGS.LOG0.EVENT.TIME.TOTAL_CYCLE"]
    last_log_time_step = g_clog_em_tlm["EL_CLOG.CLOGS.LOG0.EVENT.TIME.STEP"]

    # 同じイベント（note はわざと変えている）
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note1),
        c2a_enum.Tlm_CODE_HK,
    )

    assert_latest_log(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note1)
    assert_latest_tlog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note1)
    assert assert_latest_clog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note1) == 2
    assert_tlog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0, 0)

    diff_cycle = g_clog_em_tlm["EL_CLOG.CLOGS.LOG0.EVENT.TIME.TOTAL_CYCLE"] - last_log_time_cycle
    diff_step = g_clog_em_tlm["EL_CLOG.CLOGS.LOG0.EVENT.TIME.STEP"] - last_log_time_step

    if diff_step < 0:
        diff_step += 100
        diff_cycle -= 1

    assert g_clog_em_tlm["EL_CLOG.CLOGS.LOG0.DELTA_RECORD_TIME.TOTAL_CYCLE"] == diff_cycle
    assert g_clog_em_tlm["EL_CLOG.CLOGS.LOG0.DELTA_RECORD_TIME.STEP"] == diff_step

    # ### CLog のずらし
    # 違うイベント
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local1, EL_ERROR_LEVEL_HIGH, note1),
        c2a_enum.Tlm_CODE_HK,
    )

    assert_latest_tlog(EL_GROUP_TEST, local1, EL_ERROR_LEVEL_HIGH, note1)
    assert_tlog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0, 0)
    assert_tlog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note1, 1)
    assert assert_latest_clog(EL_GROUP_TEST, local1, EL_ERROR_LEVEL_HIGH, note1) == 1
    assert assert_clog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note1, 1) == 2

    # 最初と同じイベント
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )

    assert_tlog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0, 0)
    assert_tlog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note1, 1)
    assert_tlog(EL_GROUP_TEST, local1, EL_ERROR_LEVEL_HIGH, note1, 2)
    assert_latest_tlog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0)
    assert assert_latest_clog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0) == 3
    assert assert_clog(EL_GROUP_TEST, local1, EL_ERROR_LEVEL_HIGH, note1, 1) == 1


# log clear
@pytest.mark.real
@pytest.mark.sils
def test_event_logger_clear_log():
    initialize_el()

    local0 = 1
    local1 = 5
    note0 = 2
    note1 = 3

    # ### Cmd_EL_RECORD_EVENT での登録
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert_latest_log(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0)

    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local1, EL_ERROR_LEVEL_LOW, note1),
        c2a_enum.Tlm_CODE_HK,
    )
    assert_latest_log(EL_GROUP_TEST, local1, EL_ERROR_LEVEL_LOW, note1)

    assert g_count_total != 0
    assert g_counts[EL_ERROR_LEVEL_HIGH] != 0
    assert g_counts[EL_ERROR_LEVEL_LOW] != 0
    assert_tlog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0, 0)
    assert_tlog(EL_GROUP_TEST, local1, EL_ERROR_LEVEL_LOW, note1, 0)
    assert assert_clog(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0, 0) == 1
    assert assert_clog(EL_GROUP_TEST, local1, EL_ERROR_LEVEL_LOW, note1, 0) == 1

    # ### 全消去
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_CLEAR_LOG_ALL, (), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"

    update_el_tlm()
    assert g_count_total == 0
    assert g_counts[EL_ERROR_LEVEL_HIGH] == 0
    assert g_counts[EL_ERROR_LEVEL_LOW] == 0

    assert_latest_log(EL_CORE_GROUP_NULL, 0, EL_ERROR_LEVEL_HIGH, 0)
    assert_tlog(EL_CORE_GROUP_NULL, 0, EL_ERROR_LEVEL_HIGH, 0, 0)
    assert_tlog(EL_CORE_GROUP_NULL, 0, EL_ERROR_LEVEL_LOW, 0, 0)
    assert assert_clog(EL_CORE_GROUP_NULL, 0, EL_ERROR_LEVEL_HIGH, 0, 0) == 0
    assert assert_clog(EL_CORE_GROUP_NULL, 0, EL_ERROR_LEVEL_LOW, 0, 0) == 0

    # ### Cmd_EL_RECORD_EVENT での登録
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert_latest_log(EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0)

    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local1, EL_ERROR_LEVEL_LOW, note1),
        c2a_enum.Tlm_CODE_HK,
    )
    assert_latest_log(EL_GROUP_TEST, local1, EL_ERROR_LEVEL_LOW, note1)

    # ### エラーレベル消去
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_CLEAR_LOG_BY_ERR_LEVEL,
        (EL_ERROR_LEVEL_MAX,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "PRM"
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_CLEAR_LOG_BY_ERR_LEVEL,
        (EL_ERROR_LEVEL_HIGH,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_CLEAR_LOG_BY_ERR_LEVEL,
        (EL_ERROR_LEVEL_MAX,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "PRM"
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_CLEAR_LOG_BY_ERR_LEVEL,
        (EL_ERROR_LEVEL_LOW,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"

    update_el_tlm()
    # これは消えない
    assert g_count_total != 0
    assert g_counts[EL_ERROR_LEVEL_HIGH] != 0
    assert g_counts[EL_ERROR_LEVEL_LOW] != 0

    assert_latest_log(EL_GROUP_TEST, local1, EL_ERROR_LEVEL_LOW, note1)  # これは消えない
    assert_tlog(EL_CORE_GROUP_NULL, 0, EL_ERROR_LEVEL_HIGH, 0, 0)
    assert_tlog(EL_CORE_GROUP_NULL, 0, EL_ERROR_LEVEL_LOW, 0, 0)
    assert assert_clog(EL_CORE_GROUP_NULL, 0, EL_ERROR_LEVEL_HIGH, 0, 0) == 0
    assert assert_clog(EL_CORE_GROUP_NULL, 0, EL_ERROR_LEVEL_LOW, 0, 0) == 0

    # 本当はTLogを消したらCLogも消えてないことを書くにするとかやりたいが，面倒なので．．．

    # ### 統計情報削除
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_CLEAR_STATISTICS, (), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"

    update_el_tlm()
    assert g_count_total == 0
    assert g_counts[EL_ERROR_LEVEL_HIGH] == 0
    assert g_counts[EL_ERROR_LEVEL_LOW] == 0


# TLogの溢れ
@pytest.mark.real
@pytest.mark.sils
def test_event_logger_tlog_overflow():
    initialize_el()

    # ### 設定コマンドのアサーション確認
    change_tlog_tlm_page(0, EL_ERROR_LEVEL_HIGH)

    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_DISABLE_TLOG_OVERWRITE_ALL, (), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"
    update_el_tlog_tlm()
    assert g_tlog_is_enable_overwrite == 0
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_ENABLE_TLOG_OVERWRITE_ALL, (), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"
    update_el_tlog_tlm()
    assert g_tlog_is_enable_overwrite == 1

    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_DISABLE_TLOG_OVERWRITE,
        (EL_ERROR_LEVEL_MAX,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "PRM"
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_DISABLE_TLOG_OVERWRITE,
        (EL_ERROR_LEVEL_LOW,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"
    update_el_tlog_tlm()
    assert g_tlog_is_enable_overwrite == 1
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_DISABLE_TLOG_OVERWRITE,
        (EL_ERROR_LEVEL_HIGH,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"
    update_el_tlog_tlm()
    assert g_tlog_is_enable_overwrite == 0
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_ENABLE_TLOG_OVERWRITE,
        (EL_ERROR_LEVEL_HIGH,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"
    update_el_tlog_tlm()
    assert g_tlog_is_enable_overwrite == 1

    # ### 上書き不可にする
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_DISABLE_TLOG_OVERWRITE,
        (EL_ERROR_LEVEL_HIGH,),
        c2a_enum.Tlm_CODE_HK,
    )
    update_el_tlog_tlm()
    assert g_tlog_is_enable_overwrite == 0

    # ### 半分埋める
    local0 = 1
    # local1 = 5
    note0 = 2
    # note1 = 3

    change_tlog_tlm_page(0, EL_ERROR_LEVEL_HIGH)
    update_el_tlog_tlm()
    assert g_tlog_wp == 0
    for i in range(g_tlog_wp, g_tlog_capacity // 2 - 1):
        ope.send_rt_cmd(
            c2a_enum.Cmd_CODE_EL_RECORD_EVENT, (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0)
        )
        time.sleep(0.4)

    # 念の為チェック
    update_el_tlm()
    update_el_tlog_tlm()
    assert g_count_total == g_tlog_capacity // 2 - 1
    assert g_counts[EL_ERROR_LEVEL_HIGH] == g_tlog_capacity // 2 - 1
    assert g_tlog_wp == g_tlog_capacity // 2 - 1

    # ### 半分まで来た EL インベント確認
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )

    update_el_tlm()
    update_el_tlog_tlm()

    assert g_counts[EL_ERROR_LEVEL_HIGH] == g_tlog_capacity // 2
    assert g_counts[EL_ERROR_LEVEL_EL] == 1
    assert g_count_total == g_counts[EL_ERROR_LEVEL_HIGH] + g_counts[EL_ERROR_LEVEL_EL]

    assert_latest_tlog(
        EL_CORE_GROUP_EVENT_LOGGER, EL_EVENT_LOCAL_TLOG_HIGH_HALF_FULL, EL_ERROR_LEVEL_EL, 0
    )
    assert (
        assert_latest_clog(
            EL_CORE_GROUP_EVENT_LOGGER, EL_EVENT_LOCAL_TLOG_HIGH_HALF_FULL, EL_ERROR_LEVEL_EL, 0
        )
        == 1
    )

    assert g_tlog_is_overflow == 0

    # ### 残１まで埋める
    change_tlog_tlm_page(0, EL_ERROR_LEVEL_HIGH)
    update_el_tlog_tlm()
    for i in range(g_tlog_wp, g_tlog_capacity - 1):
        ope.send_rt_cmd(
            c2a_enum.Cmd_CODE_EL_RECORD_EVENT, (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0)
        )
        time.sleep(0.4)

    # 念の為チェック
    update_el_tlm()
    update_el_tlog_tlm()
    assert g_counts[EL_ERROR_LEVEL_HIGH] == g_tlog_capacity - 1
    assert g_tlog_wp == g_tlog_capacity - 1
    assert g_tlog_is_overflow == 0

    # ### 溢れチェック
    # ラスト１
    change_tlog_tlm_page(0, EL_ERROR_LEVEL_HIGH)
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )

    update_el_tlm()
    update_el_tlog_tlm()

    assert g_counts[EL_ERROR_LEVEL_HIGH] == g_tlog_capacity
    assert g_counts[EL_ERROR_LEVEL_EL] == 2
    assert g_count_total == g_counts[EL_ERROR_LEVEL_HIGH] + g_counts[EL_ERROR_LEVEL_EL]
    assert g_tlog_wp == 0
    assert g_tlog_is_overflow == 1

    assert_latest_tlog(
        EL_CORE_GROUP_EVENT_LOGGER, EL_EVENT_LOCAL_TLOG_HIGH_FULL, EL_ERROR_LEVEL_EL, 0
    )
    assert (
        assert_latest_clog(
            EL_CORE_GROUP_EVENT_LOGGER, EL_EVENT_LOCAL_TLOG_HIGH_FULL, EL_ERROR_LEVEL_EL, 0
        )
        == 1
    )

    # ### 溢れた場合のwpの位置など
    change_tlog_tlm_page(0, EL_ERROR_LEVEL_HIGH)
    update_el_tlm()
    update_el_tlog_tlm()

    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )

    update_el_tlm()
    update_el_tlog_tlm()

    assert g_count_total_pre + 1 == g_count_total
    assert g_counts_pre[EL_ERROR_LEVEL_HIGH] + 1 == g_counts[EL_ERROR_LEVEL_HIGH]
    assert g_tlog_wp == 0

    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )

    update_el_tlm()
    update_el_tlog_tlm()

    assert g_count_total_pre + 1 == g_count_total
    assert g_counts_pre[EL_ERROR_LEVEL_HIGH] + 1 == g_counts[EL_ERROR_LEVEL_HIGH]
    assert g_tlog_wp == 0

    # ### 追記OKに
    change_tlog_tlm_page(0, EL_ERROR_LEVEL_HIGH)
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_ENABLE_TLOG_OVERWRITE,
        (EL_ERROR_LEVEL_HIGH,),
        c2a_enum.Tlm_CODE_HK,
    )

    update_el_tlm()
    update_el_tlog_tlm()
    assert g_tlog_is_enable_overwrite == 1

    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )

    update_el_tlm()
    update_el_tlog_tlm()

    assert g_count_total_pre + 1 == g_count_total
    assert g_counts_pre[EL_ERROR_LEVEL_HIGH] + 1 == g_counts[EL_ERROR_LEVEL_HIGH]
    assert g_tlog_wp_pre + 1 == g_tlog_wp


# CLogの溢れ
@pytest.mark.real
@pytest.mark.sils
def test_event_logger_clog_overflow():
    initialize_el()
    update_el_tlm()
    update_el_clog_tlm()

    # ### すべて埋める
    local0 = 1
    # local1 = 5
    note0 = 2
    # note1 = 3

    change_clog_tlm_page(0, EL_ERROR_LEVEL_HIGH)
    update_el_clog_tlm()
    for i in range(0, g_clog_capacity):
        ope.send_rt_cmd(
            c2a_enum.Cmd_CODE_EL_RECORD_EVENT, (EL_GROUP_TEST, i, EL_ERROR_LEVEL_HIGH, note0)
        )
        time.sleep(0.4)

    update_el_tlm()
    update_el_clog_tlm()

    assert g_counts[EL_ERROR_LEVEL_HIGH] == g_clog_capacity
    # assert g_counts[EL_ERROR_LEVEL_EL] == 0           # TLog の Full event があるかもしれないので．
    assert g_count_total == g_counts[EL_ERROR_LEVEL_HIGH] + g_counts[EL_ERROR_LEVEL_EL]

    # ### 溢れチェック
    wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST1, local0, EL_ERROR_LEVEL_HIGH, note0),
        c2a_enum.Tlm_CODE_HK,
    )

    update_el_tlm()
    update_el_clog_tlm()

    assert g_counts[EL_ERROR_LEVEL_HIGH] == g_clog_capacity + 1
    assert g_counts_pre[EL_ERROR_LEVEL_EL] + 2 == g_counts[EL_ERROR_LEVEL_EL]
    assert g_count_total == g_counts[EL_ERROR_LEVEL_HIGH] + g_counts[EL_ERROR_LEVEL_EL]

    # assert_tlog(
    #     EL_CORE_GROUP_EL_DROP_CLOG1, EL_GROUP_TEST, EL_ERROR_LEVEL_EL, 0, 0
    # )  # TLog の Full event があるかもしれないので．
    # assert_tlog(
    #     EL_CORE_GROUP_EL_DROP_CLOG2, 0, EL_ERROR_LEVEL_EL, 1, 1
    # )  # TLog の Full event があるかもしれないので．
    assert assert_clog(EL_CORE_GROUP_EL_DROP_CLOG1, EL_GROUP_TEST, EL_ERROR_LEVEL_EL, 0, 1) == 1
    assert assert_clog(EL_CORE_GROUP_EL_DROP_CLOG2, 0, EL_ERROR_LEVEL_EL, 1, 0) == 1


# ロギングの有効・無効化
@pytest.mark.real
@pytest.mark.sils
def test_event_logger_logging_setting():
    initialize_el()

    # ### 切り替え
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_DISABLE_LOGGING_ALL, (), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"
    update_el_tlm()
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST)] == 0
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST1)] == 0

    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_ENABLE_LOGGING_ALL, (), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"
    update_el_tlm()
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST)] == 1
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST1)] == 1

    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_DISABLE_LOGGING, (EL_GROUP_MAX,), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "PRM"
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_DISABLE_LOGGING, (EL_GROUP_TEST,), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"
    update_el_tlm()
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST)] == 0
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST1)] == 1

    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_ENABLE_LOGGING, (EL_GROUP_MAX,), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "PRM"
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_ENABLE_LOGGING, (EL_GROUP_TEST,), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"
    update_el_tlm()
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST)] == 1
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST1)] == 1

    # ### 無効化されたイベント記録
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_DISABLE_LOGGING, (EL_GROUP_TEST,), c2a_enum.Tlm_CODE_HK
    )

    update_el_tlm()
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST, 0, EL_ERROR_LEVEL_HIGH, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "CNT"
    assert g_count_total_pre == g_count_total
    assert g_counts_pre[EL_ERROR_LEVEL_HIGH] == g_counts[EL_ERROR_LEVEL_HIGH]

    # ### リセット
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_INIT_LOGGING_SETTINGS, (), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"
    update_el_tlm()
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST)] == 1
    assert g_el_tlm["EL.IS_LOGGING_ENABLE" + str(EL_GROUP_TEST1)] == 1


# 最後のお掃除
@pytest.mark.real
@pytest.mark.sils
def test_event_logger_final_check():
    update_all_tlm()

    # ### 初期化
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_INIT, (), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"


def initialize_el():
    # ### 初期化
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_INIT, (), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"
    # ### 不正引数のチェックをするtestでEL登録されないように無効化
    ret = wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_DISABLE_LOGGING,
        (c2a_enum.EL_CORE_GROUP_CDIS_EXEC_ERR,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"


# 難しいので time はみない
def assert_latest_log(group, local, err_level, note):
    update_el_tlm()

    assert g_latest_event["group"] == group
    assert g_latest_event["local"] == local
    assert g_latest_event["err_level"] == EL_ERROR_LEVEL_TLM_DICT[err_level]
    # assert g_latest_event["total_cycle"] == total_cycle
    # assert g_latest_event["step"       ] == step
    assert g_latest_event["note"] == note


def assert_latest_tlog(group, local, err_level, note):
    if check_tlog_tlm_page(g_tlog_tlm_info["page_no"], err_level) != 1:
        change_tlog_tlm_page(g_tlog_tlm_info["page_no"], err_level)
    update_el_tlog_tlm()

    wp = g_tlog_wp
    newest_log_idx = wp - 1
    if newest_log_idx < 0:
        newest_log_idx = g_tlog_capacity - 1

    assert_tlog(group, local, err_level, note, newest_log_idx)


# idx: TLogのログ番号
# 難しいので time はみない
def assert_tlog(group, local, err_level, note, idx):
    page_no = idx // EL_TLOG_TLM_PAGE_SIZE

    if check_tlog_tlm_page(page_no, err_level) != 1:
        change_tlog_tlm_page(page_no, err_level)
    update_el_tlog_tlm()

    tlm_idx = idx % EL_TLOG_TLM_PAGE_SIZE
    assert g_tlog_em_tlm["EL_TLOG.TLOGS.EVENTS" + str(tlm_idx) + ".GROUP"] == group
    assert g_tlog_em_tlm["EL_TLOG.TLOGS.EVENTS" + str(tlm_idx) + ".LOCAL"] == local
    assert g_tlog_em_tlm["EL_TLOG.TLOGS.EVENTS" + str(tlm_idx) + ".NOTE"] == note


def assert_latest_clog(group, local, err_level, note):
    return assert_clog(group, local, err_level, note, 0)


# idx: CLogのログ番号
# 難しいので time はみない
def assert_clog(group, local, err_level, note, idx):
    page_no = idx // EL_CLOG_TLM_PAGE_SIZE

    if check_clog_tlm_page(page_no, err_level) != 1:
        change_clog_tlm_page(page_no, err_level)
    update_el_clog_tlm()

    assert g_clog_em_tlm["EL_CLOG.CLOGS.LOG" + str(idx) + ".EVENT.GROUP"] == group
    assert g_clog_em_tlm["EL_CLOG.CLOGS.LOG" + str(idx) + ".EVENT.LOCAL"] == local
    assert g_clog_em_tlm["EL_CLOG.CLOGS.LOG" + str(idx) + ".EVENT.NOTE"] == note

    return g_clog_em_tlm["EL_CLOG.CLOGS.LOG" + str(idx) + ".COUNT"]


def change_tlog_tlm_page(page_no, err_level):
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_TLOG_SET_PAGE_FOR_TLM, (page_no, err_level), c2a_enum.Tlm_CODE_HK
    )
    return ret


# OK -> 1, NG -> 0
def check_tlog_tlm_page(page_no, err_level):
    update_el_tlog_tlm()
    if g_tlog_tlm_info["page_no"] != page_no:
        return 0
    if g_tlog_tlm_info["err_level"] != EL_ERROR_LEVEL_TLM_DICT[err_level]:
        return 0
    return 1


def change_clog_tlm_page(page_no, err_level):
    ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_CLOG_SET_PAGE_FOR_TLM, (page_no, err_level), c2a_enum.Tlm_CODE_HK
    )
    return ret


# OK -> 1, NG -> 0
def check_clog_tlm_page(page_no, err_level):
    update_el_clog_tlm()
    if g_clog_tlm_info["page_no"] != page_no:
        return 0
    if g_clog_tlm_info["err_level"] != EL_ERROR_LEVEL_TLM_DICT[err_level]:
        return 0
    return 1


def update_all_tlm():
    update_el_tlm()
    update_el_tlog_tlm()
    update_el_clog_tlm()


def update_el_tlm():
    global g_el_tlm, g_count_total, g_counts, g_tlm_info, g_latest_event
    global g_el_tlm_pre, g_count_total_pre, g_counts_pre, g_tlm_info_pre, g_latest_event_pre

    g_el_tlm_pre, g_count_total_pre, g_counts_pre, g_tlm_info_pre, g_latest_event_pre = (
        g_el_tlm,
        g_count_total,
        g_counts,
        g_tlm_info,
        g_latest_event,
    )

    el_tlm = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EL
    )

    g_el_tlm = el_tlm
    g_count_total = el_tlm["EL.STATISTICS.RECORD_COUNTER_TOTAL"]
    g_counts = [
        el_tlm["EL.STATISTICS.RECORD_COUNTERS_HIGH"],
        el_tlm["EL.STATISTICS.RECORD_COUNTERS_MIDDLE"],
        el_tlm["EL.STATISTICS.RECORD_COUNTERS_LOW"],
        el_tlm["EL.STATISTICS.RECORD_COUNTERS_EL"],
        el_tlm["EL.STATISTICS.RECORD_COUNTERS_EH"],
    ]
    g_tlm_info = {
        "tlog": {
            "page_no": el_tlm["EL.TLM_INFO.TLOG.PAGE_NO"],
            "err_level": el_tlm["EL.TLM_INFO.TLOG.ERR_LEVEL"],
        },
        "clog": {
            "page_no": el_tlm["EL.TLM_INFO.CLOG.PAGE_NO"],
            "err_level": el_tlm["EL.TLM_INFO.CLOG.ERR_LEVEL"],
        },
    }
    g_latest_event = {
        "group": el_tlm["EL.LATEST_EVENT.GROUP"],
        "local": el_tlm["EL.LATEST_EVENT.LOCAL"],
        "err_level": el_tlm["EL.LATEST_EVENT.ERR_LEVEL"],
        "total_cycle": el_tlm["EL.LATEST_EVENT.TIME.TOTAL_CYCLE"],
        "step": el_tlm["EL.LATEST_EVENT.TIME.STEP"],
        "note": el_tlm["EL.LATEST_EVENT.NOTE"],
    }


def update_el_tlog_tlm():
    global g_tlog_em_tlm, g_tlog_count_total, g_tlog_count, g_tlog_tlm_info
    global g_tlog_capacity, g_tlog_wp
    global g_tlog_is_enable_overwrite, g_tlog_is_overflow
    global g_tlog_em_tlm_pre, g_tlog_count_total_pre, g_tlog_count_pre, g_tlog_tlm_info_pre
    global g_tlog_capacity_pre, g_tlog_wp_pre
    global g_tlog_is_enable_overwrite_pre, g_tlog_is_overflow_pre

    (
        g_tlog_em_tlm_pre,
        g_tlog_count_total_pre,
        g_tlog_count_pre,
        g_tlog_tlm_info_pre,
        g_tlog_capacity_pre,
        g_tlog_wp_pre,
        g_tlog_is_enable_overwrite_pre,
        g_tlog_is_overflow_pre,
    ) = (
        g_tlog_em_tlm,
        g_tlog_count_total,
        g_tlog_count,
        g_tlog_tlm_info,
        g_tlog_capacity,
        g_tlog_wp,
        g_tlog_is_enable_overwrite,
        g_tlog_is_overflow,
    )

    el_tlog_tlm = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EL_TLOG
    )

    g_tlog_em_tlm = el_tlog_tlm
    g_tlog_count_total = el_tlog_tlm["EL_TLOG.STATISTICS.RECORD_COUNTER_TOTAL"]
    g_tlog_count = el_tlog_tlm["EL_TLOG.STATISTICS.RECORD_COUNTERS_ERR_LEVEL"]
    g_tlog_tlm_info = {
        "page_no": el_tlog_tlm["EL_TLOG.TLM_INFO.TLOG.PAGE_NO"],
        "err_level": el_tlog_tlm["EL_TLOG.TLM_INFO.TLOG.ERR_LEVEL"],
    }
    g_tlog_capacity = el_tlog_tlm["EL_TLOG.TLOGS.LOG_CAPACITY"]
    g_tlog_wp = el_tlog_tlm["EL_TLOG.TLOGS.LOG_WP"]
    g_tlog_is_enable_overwrite = el_tlog_tlm["EL_TLOG.TLOGS.IS_ENABLE_OVERWRITE"]
    g_tlog_is_overflow = el_tlog_tlm["EL_TLOG.TLOGS.IS_TABLE_OVERFLOW"]


def update_el_clog_tlm():
    global g_clog_em_tlm, g_clog_count_total, g_clog_count
    global g_clog_tlm_info, g_clog_capacity
    global g_clog_em_tlm_pre, g_clog_count_total_pre, g_clog_count_pre
    global g_clog_tlm_info_pre, g_clog_capacity_pre

    (
        g_clog_em_tlm_pre,
        g_clog_count_total_pre,
        g_clog_count_pre,
        g_clog_tlm_info_pre,
        g_clog_capacity_pre,
    ) = (
        g_clog_em_tlm,
        g_clog_count_total,
        g_clog_count,
        g_clog_tlm_info,
        g_clog_capacity,
    )

    el_clog_tlm = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EL_CLOG
    )

    g_clog_em_tlm = el_clog_tlm
    g_clog_count_total = el_clog_tlm["EL_CLOG.STATISTICS.RECORD_COUNTER_TOTAL"]
    g_clog_count = el_clog_tlm["EL_CLOG.STATISTICS.RECORD_COUNTERS_ERR_LEVEL"]
    g_clog_tlm_info = {
        "page_no": el_clog_tlm["EL_CLOG.TLM_INFO.CLOG.PAGE_NO"],
        "err_level": el_clog_tlm["EL_CLOG.TLM_INFO.CLOG.ERR_LEVEL"],
    }
    g_clog_capacity = el_clog_tlm["EL_CLOG.CLOGS.LOG_CAPACITY"]


if __name__ == "__main__":
    pass
