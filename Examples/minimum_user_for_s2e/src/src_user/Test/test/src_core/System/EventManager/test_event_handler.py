#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import time
import copy

import isslwings as wings
import pytest

ROOT_PATH = "../../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()

# コードと整合をとる
EH_RULE_MAX = 20 * 8
EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES = 4
EH_MAX_MULTI_LEVEL_NUM_DEFAULT = 4

EH_RULE_TEST0 = c2a_enum.EH_RULE_TEST0
EH_RULE_TEST1 = c2a_enum.EH_RULE_TEST1
EH_RULE_TEST2 = c2a_enum.EH_RULE_TEST2
EH_RULE_TEST3 = c2a_enum.EH_RULE_TEST3
EH_RULE_TEST4 = c2a_enum.EH_RULE_TEST4
EH_RULE_TEST5 = c2a_enum.EH_RULE_TEST5
EH_RULE_TEST6 = c2a_enum.EH_RULE_TEST6
EH_RULE_TEST7 = c2a_enum.EH_RULE_TEST7

EL_GROUP_TEST_EH = c2a_enum.EL_GROUP_TEST_EH
EL_GROUP_TEST_EH_RESPOND = c2a_enum.EL_GROUP_TEST_EH_RESPOND
EL_GROUP_TEST_EH1 = c2a_enum.EL_GROUP_TEST_EH1
EL_LOCAL_TEST0 = 0
EL_LOCAL_TEST1 = 1
EL_LOCAL_TEST2 = 2
EL_LOCAL_TEST3 = 3
EL_LOCAL_TEST4 = 4

BC_TEST_EH_RESPOND = c2a_enum.BC_TEST_EH_RESPOND
TEST_EH_EL_LOCAL_ID_RESPOND = 0

EL_ERROR_LEVEL_HIGH = 0
EL_ERROR_LEVEL_MIDDLE = 1
EL_ERROR_LEVEL_LOW = 2
EL_ERROR_LEVEL_EL = 3
EL_ERROR_LEVEL_EH = 4

EH_RESPONSE_CONDITION_SINGLE = 0
EH_RESPONSE_CONDITION_CONTINUOUS = 1
EH_RESPONSE_CONDITION_CUMULATIVE = 2

EH_REGISTER_ACK_OK = 0
EH_REGISTER_ACK_ERR_FULL = 1
EH_REGISTER_ACK_ERR_RULE_OVERWRITE = 2
EH_REGISTER_ACK_ERR_DUPLICATE_FULL = 3
EH_REGISTER_ACK_ILLEGAL_RULE_ID = 4
EH_REGISTER_ACK_ILLEGAL_GROUP = 5
EH_REGISTER_ACK_ILLEGAL_ERROR_LEVEL = 6
EH_REGISTER_ACK_ILLEGAL_MATCH_FLAG = 7
EH_REGISTER_ACK_ILLEGAL_CONDITION_TYPE = 8
EH_REGISTER_ACK_ILLEGAL_COUNT_THRESHOLD = 9
EH_REGISTER_ACK_ILLEGAL_BCT_ID = 10
EH_REGISTER_ACK_ILLEGAL_ACTIVE_FLAG = 11
EH_REGISTER_ACK_ILLEGAL_MULTI_LEVEL = 12
EH_REGISTER_ACK_UNKNOWN_ERR = 13

conv_to_err_level = {0: "HIGH", 1: "MIDDLE", 2: "LOW", 3: "EL", 4: "EH"}
conv_to_match_flag = {0: "NO", 1: "YES"}
conv_to_active = {0: "INACTIVE", 1: "ACTIVE"}
conv_to_condition_type = {0: "SINGLE", 1: "CONTINUOUS", 2: "CUMULATIVE"}

# TODO: カウンタのクリアを入れたら足す
# TODO: EH対応EL発行を入れたら，色々変えないとダメ


# 各種設定の確認など
@pytest.mark.real
@pytest.mark.sils
def test_event_handler_init_check():
    print("")
    print("test_event_handler_init_check")

    # EHが勝手に動くのを阻止
    disable_eh_exec()

    # 初期化
    init_el_and_eh()

    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.EL_EVENT_COUNTER.COUNTER_TOTAL"] == 0
    assert tlm_EH["EH.EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_HIGH"] == 0
    assert tlm_EH["EH.EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_MIDDLE"] == 0
    assert tlm_EH["EH.EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_LOW"] == 0
    assert tlm_EH["EH.EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_EL"] == 0
    assert tlm_EH["EH.EL_EVENT_COUNTER.COUNTERS.EL_ERROR_LEVEL_EH"] == 0
    assert tlm_EH["EH.TLM_INFO.RULE.PAGE_NO"] == 0
    assert tlm_EH["EH.TLM_INFO.RULE.TARGET_RULE_ID"] == 0
    assert tlm_EH["EH.TLM_INFO.RULE_SORTED_INDEX.PAGE_NO"] == 0
    assert tlm_EH["EH.TLM_INFO.LOG.PAGE_NO"] == 0
    assert tlm_EH["EH.EXEC_SETTINGS.MAX_RESPONSE_NUM"] == 8
    assert tlm_EH["EH.EXEC_SETTINGS.MAX_CHECK_EVENT_NUM"] == 64
    assert tlm_EH["EH.EXEC_SETTINGS.MAX_MULTI_LEVEL_NUM"] == 4


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_check_tlm_settings():
    print("")
    print("test_event_handler_check_tlm_settings")

    init_el_and_eh()

    print("Cmd_EH_SET_PAGE_OF_RULE_TABLE_FOR_TLM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_SET_PAGE_OF_RULE_TABLE_FOR_TLM, (2,), c2a_enum.Tlm_CODE_HK
    )

    print("Cmd_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM, (3,), c2a_enum.Tlm_CODE_HK
    )

    print("Cmd_EH_SET_PAGE_OF_LOG_TABLE_FOR_TLM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_SET_PAGE_OF_LOG_TABLE_FOR_TLM, (1,), c2a_enum.Tlm_CODE_HK
    )

    # 本当は１個ずつアサーションするべきだが，手抜き
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.TLM_INFO.RULE.PAGE_NO"] == 2
    assert tlm_EH["EH.TLM_INFO.RULE_SORTED_INDEX.PAGE_NO"] == 3
    assert tlm_EH["EH.TLM_INFO.LOG.PAGE_NO"] == 1


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_check_ah_exec_settings():
    print("")
    print("test_event_handler_check_ah_exec_settings")

    init_el_and_eh()

    print("Cmd_EH_SET_MAX_RESPONSE_NUM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_SET_MAX_RESPONSE_NUM, (1,), c2a_enum.Tlm_CODE_HK
    )

    print("Cmd_EH_SET_MAX_CHECK_EVENT_NUM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_SET_MAX_CHECK_EVENT_NUM, (3,), c2a_enum.Tlm_CODE_HK
    )

    print("Cmd_EH_SET_MAX_MULTI_LEVEL_NUM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_SET_MAX_MULTI_LEVEL_NUM, (5,), c2a_enum.Tlm_CODE_HK
    )

    # 本当は１個ずつアサーションするべきだが，手抜き
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.EXEC_SETTINGS.MAX_RESPONSE_NUM"] == 1
    assert tlm_EH["EH.EXEC_SETTINGS.MAX_CHECK_EVENT_NUM"] == 3
    assert tlm_EH["EH.EXEC_SETTINGS.MAX_MULTI_LEVEL_NUM"] == 5


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_load_rule():
    print("")
    print("test_event_handler_load_rule")

    init_el_and_eh()

    # デフォルトのルールが登録されているか？
    check_default_rules()
    check_default_rule_indexes()

    # 全ルール消し
    print("Cmd_EH_CLEAR_ALL_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_CLEAR_ALL_RULE, (), c2a_enum.Tlm_CODE_HK
    )
    check_rules_cleared()
    check_rule_indexes_cleared()

    # デフォルトルールのロード
    print("Cmd_EH_LOAD_DEFAULT_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_LOAD_DEFAULT_RULE, (), c2a_enum.Tlm_CODE_HK
    )
    check_default_rules()
    check_default_rule_indexes()


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_delete_rule():
    print("")
    print("test_event_handler_delete_rule")

    init_el_and_eh()

    download_eh_index_tlm()  # 目視チェック用

    # ルールを消す
    print("Cmd_EH_DELETE_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST2,), c2a_enum.Tlm_CODE_HK
    )
    download_eh_index_tlm()  # 目視チェック用
    # FIXME: 自動化したい
    print("!!!!! 目視チェック：EH_RULE_TEST2 が EH_INDEX から消されつつ，ソートされているか確認 !!!!!")
    check_rule(
        "EH_RULE_TEST2 cleared", EH_RULE_TEST2, {"group": 0, "local": 0, "is_active": "INACTIVE"}
    )

    # 同じルールを消してエラー
    assert "CNT" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST2,), c2a_enum.Tlm_CODE_HK
    )


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_register_rule():
    print("")
    print("test_event_handler_register_rule")

    # 初期化
    init_el_and_eh()
    # 既存のルールを全部消す
    print("Cmd_EH_CLEAR_ALL_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_CLEAR_ALL_RULE, (), c2a_enum.Tlm_CODE_HK
    )

    # テンプレ
    settings = {
        "event": {"group": EL_GROUP_TEST_EH, "local": 0, "err_level": EL_ERROR_LEVEL_LOW},
        "condition": {
            "type": EH_RESPONSE_CONDITION_SINGLE,
            "count_threshold": 0,
            "time_threshold_ms": 0,
        },
        "should_match_err_level": 1,
        "deploy_bct_id": BC_TEST_EH_RESPOND,
        "is_active": 1,
    }

    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST0, settings)
    check_reg_from_cmd_eh_rule_param(EH_RULE_TEST0, settings)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"
    assert reg_ack == EH_REGISTER_ACK_OK

    # 重複登録
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "CNT"
    assert reg_ack == EH_REGISTER_ACK_ERR_RULE_OVERWRITE

    # 不正な group
    settings_invalid = copy.deepcopy(settings)
    settings_invalid["event"]["group"] = 0xFFFFFFFF
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST1, settings_invalid)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "PRM"
    assert reg_ack == EH_REGISTER_ACK_ILLEGAL_GROUP

    # 不正な type
    settings_invalid = copy.deepcopy(settings)
    settings_invalid["condition"]["type"] = 0xFF
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST1, settings_invalid)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "PRM"
    assert reg_ack == EH_REGISTER_ACK_ILLEGAL_CONDITION_TYPE

    # 不正な多段
    settings_invalid = copy.deepcopy(settings)
    settings_invalid["event"]["group"] = c2a_enum.EL_CORE_GROUP_EVENT_LOGGER
    settings_invalid["event"]["err_level"] = EL_ERROR_LEVEL_EH
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST1, settings_invalid)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "PRM"
    assert reg_ack == EH_REGISTER_ACK_ILLEGAL_MULTI_LEVEL

    settings_invalid = copy.deepcopy(settings)
    settings_invalid["event"]["group"] = c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    settings_invalid["event"]["err_level"] = EL_ERROR_LEVEL_HIGH
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST1, settings_invalid)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "PRM"
    assert reg_ack == EH_REGISTER_ACK_ILLEGAL_MULTI_LEVEL

    # 一旦リセット
    print("Cmd_EH_CLEAR_ALL_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_CLEAR_ALL_RULE, (), c2a_enum.Tlm_CODE_HK
    )

    # 多段 EH 対応の段数が多すぎる不正
    print("EH_MAX_MULTI_LEVEL_NUM_DEFAULT check")
    # まず Lv.1 登録
    mutli_level_settings = copy.deepcopy(settings)
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST0, mutli_level_settings)
    check_reg_from_cmd_eh_rule_param(EH_RULE_TEST0, mutli_level_settings)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"
    assert reg_ack == EH_REGISTER_ACK_OK

    # まず Lv.2 ~ Lv. EH_MAX_MULTI_LEVEL_NUM_DEFAULT 登録
    mutli_level_settings = copy.deepcopy(settings)
    mutli_level_settings["event"]["group"] = c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    mutli_level_settings["event"]["err_level"] = EL_ERROR_LEVEL_EH
    for i in range(1, EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES):
        mutli_level_settings["event"]["local"] = EH_RULE_TEST0 + (i - 1)
        set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST0 + i, mutli_level_settings)
        check_reg_from_cmd_eh_rule_param(EH_RULE_TEST0 + i, mutli_level_settings)
        (cmd_ret, reg_ack) = register_rule()
        assert reg_ack == EH_REGISTER_ACK_OK
        assert cmd_ret == "SUC"

    mutli_level_settings["event"]["local"] = EH_RULE_TEST0 + (
        EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES - 1
    )
    set_param_of_reg_from_cmd_eh_rule(
        EH_RULE_TEST0 + EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES, mutli_level_settings
    )
    check_reg_from_cmd_eh_rule_param(
        EH_RULE_TEST0 + EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES, mutli_level_settings
    )
    (cmd_ret, reg_ack) = register_rule()
    assert reg_ack == EH_REGISTER_ACK_ILLEGAL_MULTI_LEVEL
    assert cmd_ret == "PRM"

    # 一旦リセット
    print("Cmd_EH_CLEAR_ALL_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_CLEAR_ALL_RULE, (), c2a_enum.Tlm_CODE_HK
    )

    # 最大重複数チェック（後ろに登録なしバージョン）
    for i in range(EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES):
        print("DUPLICATE check: " + str(i))
        set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST0 + i, settings)
        check_reg_from_cmd_eh_rule_param(EH_RULE_TEST0 + i, settings)
        (cmd_ret, reg_ack) = register_rule()
        assert reg_ack == EH_REGISTER_ACK_OK
        assert cmd_ret == "SUC"

    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST0 + EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES, settings)
    check_reg_from_cmd_eh_rule_param(EH_RULE_TEST0 + EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES, settings)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "CNT"
    assert reg_ack == EH_REGISTER_ACK_ERR_DUPLICATE_FULL

    # 一旦リセット
    print("Cmd_EH_CLEAR_ALL_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_CLEAR_ALL_RULE, (), c2a_enum.Tlm_CODE_HK
    )

    # 最大重複数チェック（後ろに登録ありバージョン）
    settings_later_group = copy.deepcopy(settings)
    settings_later_group["event"]["group"] = EL_GROUP_TEST_EH + 1
    set_param_of_reg_from_cmd_eh_rule(
        EH_RULE_TEST0 + EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES + 1, settings_later_group
    )
    check_reg_from_cmd_eh_rule_param(
        EH_RULE_TEST0 + EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES + 1, settings_later_group
    )
    (cmd_ret, reg_ack) = register_rule()
    assert reg_ack == EH_REGISTER_ACK_OK
    assert cmd_ret == "SUC"

    for i in range(EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES):
        print("DUPLICATE check: " + str(i))
        set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST0 + i, settings)
        check_reg_from_cmd_eh_rule_param(EH_RULE_TEST0 + i, settings)
        (cmd_ret, reg_ack) = register_rule()
        assert reg_ack == EH_REGISTER_ACK_OK
        assert cmd_ret == "SUC"

    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST0 + EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES, settings)
    check_reg_from_cmd_eh_rule_param(EH_RULE_TEST0 + EH_MAX_RULE_NUM_OF_EL_ID_DUPLICATES, settings)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "CNT"
    assert reg_ack == EH_REGISTER_ACK_ERR_DUPLICATE_FULL


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_check_index():
    print("")
    print("test_event_handler_check_index")

    # ごちゃまぜ順番でindexがただしいか

    # 初期化
    init_el_and_eh()
    # 既存のルールを全部消す
    print("Cmd_EH_CLEAR_ALL_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_CLEAR_ALL_RULE, (), c2a_enum.Tlm_CODE_HK
    )

    # テンプレ
    settings = {
        "event": {"group": EL_GROUP_TEST_EH, "local": 0, "err_level": EL_ERROR_LEVEL_LOW},
        "condition": {
            "type": EH_RESPONSE_CONDITION_SINGLE,
            "count_threshold": 0,
            "time_threshold_ms": 0,
        },
        "should_match_err_level": 1,
        "deploy_bct_id": BC_TEST_EH_RESPOND,
        "is_active": 1,
    }

    # 次の順番のindexを作れるか？
    # group, local, duplicate_id, rule_id
    # -----------------------------------
    # EL_GROUP_TEST_EH,  0, 0, EH_RULE_TEST0
    # EL_GROUP_TEST_EH,  1, 0, EH_RULE_TEST1
    # EL_GROUP_TEST_EH,  1, 1, EH_RULE_TEST2
    # EL_GROUP_TEST_EH,  1, 2, EH_RULE_TEST3
    # EL_GROUP_TEST_EH,  2, 0, EH_RULE_TEST4
    # EL_GROUP_TEST_EH1, 0, 0, EH_RULE_TEST5
    # EL_GROUP_TEST_EH1, 0, 1, EH_RULE_TEST6
    # EL_GROUP_TEST_EH1, 1, 0, EH_RULE_TEST7

    # 順番はわざとバラバラ
    settings_tmp = copy.deepcopy(settings)
    settings_tmp["event"]["group"] = EL_GROUP_TEST_EH
    settings_tmp["event"]["local"] = 1
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST1, settings_tmp)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"

    settings_tmp = copy.deepcopy(settings)
    settings_tmp["event"]["group"] = EL_GROUP_TEST_EH1
    settings_tmp["event"]["local"] = 0
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST5, settings_tmp)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"

    settings_tmp = copy.deepcopy(settings)
    settings_tmp["event"]["group"] = EL_GROUP_TEST_EH
    settings_tmp["event"]["local"] = 1
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST2, settings_tmp)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"

    settings_tmp = copy.deepcopy(settings)
    settings_tmp["event"]["group"] = EL_GROUP_TEST_EH
    settings_tmp["event"]["local"] = 0
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST0, settings_tmp)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"

    settings_tmp = copy.deepcopy(settings)
    settings_tmp["event"]["group"] = EL_GROUP_TEST_EH
    settings_tmp["event"]["local"] = 2
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST4, settings_tmp)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"

    settings_tmp = copy.deepcopy(settings)
    settings_tmp["event"]["group"] = EL_GROUP_TEST_EH1
    settings_tmp["event"]["local"] = 1
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST7, settings_tmp)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"

    settings_tmp = copy.deepcopy(settings)
    settings_tmp["event"]["group"] = EL_GROUP_TEST_EH
    settings_tmp["event"]["local"] = 1
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST3, settings_tmp)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"

    settings_tmp = copy.deepcopy(settings)
    settings_tmp["event"]["group"] = EL_GROUP_TEST_EH1
    settings_tmp["event"]["local"] = 0
    set_param_of_reg_from_cmd_eh_rule(EH_RULE_TEST6, settings_tmp)
    (cmd_ret, reg_ack) = register_rule()
    assert cmd_ret == "SUC"

    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "TEST0",
        0,
        {"group": EL_GROUP_TEST_EH, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST0},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST1",
        1,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST1},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST2",
        2,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 1, "rule_id": EH_RULE_TEST2},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST3",
        3,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 2, "rule_id": EH_RULE_TEST3},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST4",
        4,
        {"group": EL_GROUP_TEST_EH, "local": 2, "duplicate_id": 0, "rule_id": EH_RULE_TEST4},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST5",
        5,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST5},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST6",
        6,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 1, "rule_id": EH_RULE_TEST6},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST7",
        7,
        {"group": EL_GROUP_TEST_EH1, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST7},
        tlm_EH_INDEX,
    )

    # いい感じに消していく
    # 重複真ん中
    print("Cmd_EH_DELETE_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST2,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "TEST0",
        0,
        {"group": EL_GROUP_TEST_EH, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST0},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST1",
        1,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST1},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST3",
        2,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 1, "rule_id": EH_RULE_TEST3},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST4",
        3,
        {"group": EL_GROUP_TEST_EH, "local": 2, "duplicate_id": 0, "rule_id": EH_RULE_TEST4},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST5",
        4,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST5},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST6",
        5,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 1, "rule_id": EH_RULE_TEST6},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST7",
        6,
        {"group": EL_GROUP_TEST_EH1, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST7},
        tlm_EH_INDEX,
    )

    # 重複先頭
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST1,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "TEST0",
        0,
        {"group": EL_GROUP_TEST_EH, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST0},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST3",
        1,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST3},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST4",
        2,
        {"group": EL_GROUP_TEST_EH, "local": 2, "duplicate_id": 0, "rule_id": EH_RULE_TEST4},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST5",
        3,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST5},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST6",
        4,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 1, "rule_id": EH_RULE_TEST6},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST7",
        5,
        {"group": EL_GROUP_TEST_EH1, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST7},
        tlm_EH_INDEX,
    )

    # 重複後ろ
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST6,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "TEST0",
        0,
        {"group": EL_GROUP_TEST_EH, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST0},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST3",
        1,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST3},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST4",
        2,
        {"group": EL_GROUP_TEST_EH, "local": 2, "duplicate_id": 0, "rule_id": EH_RULE_TEST4},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST5",
        3,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST5},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST7",
        4,
        {"group": EL_GROUP_TEST_EH1, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST7},
        tlm_EH_INDEX,
    )

    # 先頭
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST0,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "TEST3",
        0,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST3},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST4",
        1,
        {"group": EL_GROUP_TEST_EH, "local": 2, "duplicate_id": 0, "rule_id": EH_RULE_TEST4},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST5",
        2,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST5},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST7",
        3,
        {"group": EL_GROUP_TEST_EH1, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST7},
        tlm_EH_INDEX,
    )

    # 後ろ
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST7,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "TEST3",
        0,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST3},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST4",
        1,
        {"group": EL_GROUP_TEST_EH, "local": 2, "duplicate_id": 0, "rule_id": EH_RULE_TEST4},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST5",
        2,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST5},
        tlm_EH_INDEX,
    )

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST4,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "TEST3",
        0,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST3},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST5",
        1,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST5},
        tlm_EH_INDEX,
    )

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST3,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "TEST5",
        0,
        {"group": EL_GROUP_TEST_EH1, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST5},
        tlm_EH_INDEX,
    )

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_DELETE_RULE, (EH_RULE_TEST5,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_indexes_cleared()


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_check_counter():
    print("")
    print("test_event_handler_check_counter")

    # 初期化
    init_el_and_eh()

    # テレメ設定
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM,
        (EH_RULE_TEST0,),
        c2a_enum.Tlm_CODE_HK,
    )

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_CLEAR_RULE_COUNTER, (EH_RULE_TEST0,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.TARTGET_RULE.COUNTER"] == 0

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_SET_RULE_COUNTER, (EH_RULE_TEST0, 10), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.TARTGET_RULE.COUNTER"] == 10

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_CLEAR_RULE_COUNTER, (EH_RULE_TEST0,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.TARTGET_RULE.COUNTER"] == 0


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_clear_counter_by_event():
    print("")
    print("test_event_handler_clear_counter_by_event")

    # 初期化
    init_el_and_eh()
    enable_eh_exec()

    # テレメ設定
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM,
        (EH_RULE_TEST3,),
        c2a_enum.Tlm_CODE_HK,
    )

    # +2
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 3, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 3, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.TARTGET_RULE.COUNTER"] == 2

    # エラーレベル違い
    print("Cmd_EH_CLEAR_RULE_COUNTER_BY_EVENT")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_CLEAR_RULE_COUNTER_BY_EVENT,
        (EL_GROUP_TEST_EH, 3, EL_ERROR_LEVEL_HIGH),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.TARTGET_RULE.COUNTER"] == 2

    # クリアされるはず
    print("Cmd_EH_CLEAR_RULE_COUNTER_BY_EVENT")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_CLEAR_RULE_COUNTER_BY_EVENT,
        (EL_GROUP_TEST_EH, 3, EL_ERROR_LEVEL_LOW),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.TARTGET_RULE.COUNTER"] == 0


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_respond_single():
    print("")
    print("test_event_handler_respond_single")

    # 初期化
    init_el_and_eh()

    # should_match_err_level == 1 の
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_HIGH, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "not_responded"

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"

    # inactivateされてしまうので動かないはず
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "not_responded"

    # activate
    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "INACTIVE"}
    )
    print("Cmd_EH_ACTIVATE_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST0,), c2a_enum.Tlm_CODE_HK
    )
    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "ACTIVE"}
    )

    # これで再度動く
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"

    # should_match_err_level == 0 の
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 1, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST1,), c2a_enum.Tlm_CODE_HK
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 1, EL_ERROR_LEVEL_HIGH, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST1,), c2a_enum.Tlm_CODE_HK
    )

    # inactivate
    check_rule(
        "TEST1", EH_RULE_TEST1, {"group": EL_GROUP_TEST_EH, "local": 1, "is_active": "ACTIVE"}
    )
    print("Cmd_EH_INACTIVATE_RULE")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_INACTIVATE_RULE, (EH_RULE_TEST1,), c2a_enum.Tlm_CODE_HK
    )
    check_rule(
        "TEST1", EH_RULE_TEST1, {"group": EL_GROUP_TEST_EH, "local": 1, "is_active": "INACTIVE"}
    )

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 1, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "not_responded"


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_respond_single_multiple():
    print("")
    print("test_event_handler_respond_single_multiple")

    # 初期化
    init_el_and_eh()

    # 現在のカウンタを保存
    tlm_EH = download_eh_tlm()
    counter = tlm_EH["EH.LOG_TABLE.RESPOND_COUNTER"]

    # テスト用にアクティベーション
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST4,), c2a_enum.Tlm_CODE_HK
    )
    check_rule(
        "TEST4", EH_RULE_TEST4, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "ACTIVE"}
    )

    # 発火
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"  # 少なくとも１つは動いた

    # カウンタの増加が +2 であることを確認
    tlm_EH = download_eh_tlm()
    assert counter + 2 == tlm_EH["EH.LOG_TABLE.RESPOND_COUNTER"]

    # ログもチェック
    tlm_EH_LOG = download_eh_log_tlm()
    check_log(0, EH_RULE_TEST4, tlm_EH_LOG)
    check_log(1, EH_RULE_TEST0, tlm_EH_LOG)


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_respond_continuous():
    print("")
    print("test_event_handler_respond_continuous")

    # 初期化
    init_el_and_eh()
    enable_eh_exec()

    # EL テレメで見れるように
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM,
        (EH_RULE_TEST2,),
        c2a_enum.Tlm_CODE_HK,
    )

    download_eh_log_tlm()

    for i in range(3):
        assert check_respend_eh() == "not_responded"
        assert "SUC" == wings.util.send_rt_cmd_and_confirm(
            ope,
            c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
            (EL_GROUP_TEST_EH, 2, EL_ERROR_LEVEL_LOW, 0),
            c2a_enum.Tlm_CODE_HK,
        )
        time.sleep(28)
    assert check_respend_eh() == "not_responded"
    download_eh_log_tlm()

    for i in range(3):
        assert check_respend_eh() == "not_responded"
        assert "SUC" == wings.util.send_rt_cmd_and_confirm(
            ope,
            c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
            (EL_GROUP_TEST_EH, 2, EL_ERROR_LEVEL_LOW, 0),
            c2a_enum.Tlm_CODE_HK,
        )
    assert check_respend_eh() == "responded"
    download_eh_log_tlm()

    disable_eh_exec()


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_respond_cumulative():
    print("")
    print("test_event_handler_respond_cumulative")

    # 初期化
    init_el_and_eh()
    enable_eh_exec()

    # EL テレメで見れるように
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM,
        (EH_RULE_TEST3,),
        c2a_enum.Tlm_CODE_HK,
    )

    for i in range(3):
        assert check_respend_eh() == "not_responded"
        assert "SUC" == wings.util.send_rt_cmd_and_confirm(
            ope,
            c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
            (EL_GROUP_TEST_EH, 3, EL_ERROR_LEVEL_LOW, 0),
            c2a_enum.Tlm_CODE_HK,
        )
    assert check_respend_eh() == "responded"

    disable_eh_exec()


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_activate_and_inactivate_rule_for_multi_level():
    print("")
    print("test_event_handler_activate_and_inactivate_rule_for_multi_level")

    # 初期化
    init_el_and_eh()

    # テスト用にアクティベーション
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST5,), c2a_enum.Tlm_CODE_HK
    )
    check_rule(
        "TEST5",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "ACTIVE",
        },
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST6,), c2a_enum.Tlm_CODE_HK
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "ACTIVE",
        },
    )

    # inactivate Lv.1 to Lv.3
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL,
        (EH_RULE_TEST6,),
        c2a_enum.Tlm_CODE_HK,
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "INACTIVE",
        },
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "INACTIVE",
        },
    )
    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "INACTIVE"}
    )

    # activate Lv.1 to Lv.3
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL,
        (EH_RULE_TEST6,),
        c2a_enum.Tlm_CODE_HK,
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "ACTIVE",
        },
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "ACTIVE",
        },
    )
    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "ACTIVE"}
    )

    # inactivate Lv.1 to Lv.2
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL,
        (EH_RULE_TEST5,),
        c2a_enum.Tlm_CODE_HK,
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "ACTIVE",
        },
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "INACTIVE",
        },
    )
    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "INACTIVE"}
    )

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_INACTIVATE_RULE, (EH_RULE_TEST6,), c2a_enum.Tlm_CODE_HK
    )

    # activate Lv.1 to Lv.2
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL,
        (EH_RULE_TEST5,),
        c2a_enum.Tlm_CODE_HK,
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "INACTIVE",
        },
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "ACTIVE",
        },
    )
    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "ACTIVE"}
    )

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST6,), c2a_enum.Tlm_CODE_HK
    )

    # inactivate only Lv.1
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_INACTIVATE_RULE_FOR_MULTI_LEVEL,
        (EH_RULE_TEST0,),
        c2a_enum.Tlm_CODE_HK,
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "ACTIVE",
        },
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "ACTIVE",
        },
    )
    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "INACTIVE"}
    )

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_INACTIVATE_RULE, (EH_RULE_TEST6,), c2a_enum.Tlm_CODE_HK
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_INACTIVATE_RULE, (EH_RULE_TEST5,), c2a_enum.Tlm_CODE_HK
    )

    # activate only Lv.1
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL,
        (EH_RULE_TEST0,),
        c2a_enum.Tlm_CODE_HK,
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "INACTIVE",
        },
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "INACTIVE",
        },
    )
    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "ACTIVE"}
    )


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_respond_multi_level():
    print("")
    print("test_event_handler_respond_multi_level")

    # 初期化
    init_el_and_eh()

    # テスト用にアクティベーション
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST5,), c2a_enum.Tlm_CODE_HK
    )
    check_rule(
        "TEST5",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "ACTIVE",
        },
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST6,), c2a_enum.Tlm_CODE_HK
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "ACTIVE",
        },
    )

    # Lv.1 発火
    print("### Lv.1 ###")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.LOG_TABLE.RESPOND_COUNTER"] == 1
    tlm_EH_LOG = download_eh_log_tlm()
    check_log(0, EH_RULE_TEST0, tlm_EH_LOG)
    tlm_EL = download_el_tlm()
    assert tlm_EL["EL.TLOGS.EH.EVENTS0.GROUP"] == c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    assert tlm_EL["EL.TLOGS.EH.EVENTS0.LOCAL"] == EH_RULE_TEST0

    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "INACTIVE"}
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST0,), c2a_enum.Tlm_CODE_HK
    )

    # Lv.2 発火 (Lv.1 はキャンセル)
    print("### Lv.2 ###")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.LOG_TABLE.RESPOND_COUNTER"] == 2
    tlm_EH_LOG = download_eh_log_tlm()
    check_log(0, EH_RULE_TEST5, tlm_EH_LOG)
    check_log(1, EH_RULE_TEST0, tlm_EH_LOG)
    tlm_EL = download_el_tlm()
    assert (
        tlm_EL["EL.TLOGS.EH.EVENTS0.GROUP"]
        == c2a_enum.EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE
    )
    assert tlm_EL["EL.TLOGS.EH.EVENTS0.LOCAL"] == EH_RULE_TEST0
    assert tlm_EL["EL.TLOGS.EH.EVENTS1.GROUP"] == c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    assert tlm_EL["EL.TLOGS.EH.EVENTS1.LOCAL"] == EH_RULE_TEST5
    assert tlm_EL["EL.TLOGS.EH.EVENTS2.GROUP"] == c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    assert tlm_EL["EL.TLOGS.EH.EVENTS2.LOCAL"] == EH_RULE_TEST0
    assert tlm_EL["EL.TLOGS.EH.EVENTS3.GROUP"] == c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    assert tlm_EL["EL.TLOGS.EH.EVENTS3.LOCAL"] == EH_RULE_TEST0

    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "INACTIVE"}
    )
    check_rule(
        "TEST0",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "INACTIVE",
        },
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE_FOR_MULTI_LEVEL,
        (EH_RULE_TEST5,),
        c2a_enum.Tlm_CODE_HK,
    )

    # Lv.1 発火
    print("### Lv.1 ###")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.LOG_TABLE.RESPOND_COUNTER"] == 3
    tlm_EH_LOG = download_eh_log_tlm()
    check_log(0, EH_RULE_TEST0, tlm_EH_LOG)
    check_log(1, EH_RULE_TEST5, tlm_EH_LOG)
    check_log(2, EH_RULE_TEST0, tlm_EH_LOG)
    tlm_EL = download_el_tlm()
    assert tlm_EL["EL.TLOGS.EH.EVENTS0.GROUP"] == c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    assert tlm_EL["EL.TLOGS.EH.EVENTS0.LOCAL"] == EH_RULE_TEST0
    assert (
        tlm_EL["EL.TLOGS.EH.EVENTS1.GROUP"]
        == c2a_enum.EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE
    )
    assert tlm_EL["EL.TLOGS.EH.EVENTS1.LOCAL"] == EH_RULE_TEST0
    assert tlm_EL["EL.TLOGS.EH.EVENTS2.GROUP"] == c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    assert tlm_EL["EL.TLOGS.EH.EVENTS2.LOCAL"] == EH_RULE_TEST5
    assert tlm_EL["EL.TLOGS.EH.EVENTS3.GROUP"] == c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    assert tlm_EL["EL.TLOGS.EH.EVENTS3.LOCAL"] == EH_RULE_TEST0

    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "INACTIVE"}
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST0,), c2a_enum.Tlm_CODE_HK
    )

    # Lv.3 発火 (Lv.1,2 はキャンセル)
    print("### Lv.3 ###")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.LOG_TABLE.RESPOND_COUNTER"] == 4
    tlm_EH_LOG = download_eh_log_tlm()
    check_log(0, EH_RULE_TEST6, tlm_EH_LOG)
    check_log(1, EH_RULE_TEST0, tlm_EH_LOG)
    check_log(2, EH_RULE_TEST5, tlm_EH_LOG)
    check_log(3, EH_RULE_TEST0, tlm_EH_LOG)
    tlm_EL = download_el_tlm()
    assert (
        tlm_EL["EL.TLOGS.EH.EVENTS0.GROUP"]
        == c2a_enum.EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE
    )
    assert tlm_EL["EL.TLOGS.EH.EVENTS0.LOCAL"] == EH_RULE_TEST0
    assert (
        tlm_EL["EL.TLOGS.EH.EVENTS1.GROUP"]
        == c2a_enum.EL_CORE_GROUP_EH_RESPOND_WITH_HIGHER_LEVEL_RULE
    )
    assert tlm_EL["EL.TLOGS.EH.EVENTS1.LOCAL"] == EH_RULE_TEST5
    assert tlm_EL["EL.TLOGS.EH.EVENTS2.GROUP"] == c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    assert tlm_EL["EL.TLOGS.EH.EVENTS2.LOCAL"] == EH_RULE_TEST6
    assert tlm_EL["EL.TLOGS.EH.EVENTS3.GROUP"] == c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE
    assert tlm_EL["EL.TLOGS.EH.EVENTS3.LOCAL"] == EH_RULE_TEST5

    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "INACTIVE"}
    )
    check_rule(
        "TEST0",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "INACTIVE",
        },
    )
    check_rule(
        "TEST0",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "INACTIVE",
        },
    )


@pytest.mark.real
@pytest.mark.sils
def test_event_handler_responded_log():
    print("")
    print("test_event_handler_responded_log")

    # 初期化
    init_el_and_eh()
    enable_eh_exec()

    tlm_EH_LOG = download_eh_log_tlm()
    check_log(0, EH_RULE_MAX, tlm_EH_LOG)
    check_log(1, EH_RULE_MAX, tlm_EH_LOG)
    check_log(2, EH_RULE_MAX, tlm_EH_LOG)

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 0, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST0,), c2a_enum.Tlm_CODE_HK
    )

    tlm_EH_LOG = download_eh_log_tlm()
    check_log(0, EH_RULE_TEST0, tlm_EH_LOG)
    check_log(1, EH_RULE_MAX, tlm_EH_LOG)
    check_log(2, EH_RULE_MAX, tlm_EH_LOG)

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_RECORD_EVENT,
        (EL_GROUP_TEST_EH, 1, EL_ERROR_LEVEL_LOW, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    exec_eh()
    assert check_respend_eh() == "responded"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_ACTIVATE_RULE, (EH_RULE_TEST1,), c2a_enum.Tlm_CODE_HK
    )

    tlm_EH_LOG = download_eh_log_tlm()
    check_log(0, EH_RULE_TEST1, tlm_EH_LOG)
    check_log(1, EH_RULE_TEST0, tlm_EH_LOG)
    check_log(2, EH_RULE_MAX, tlm_EH_LOG)

    print("Cmd_EH_CLEAR_LOG")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_CLEAR_LOG, (), c2a_enum.Tlm_CODE_HK
    )

    tlm_EH_LOG = download_eh_log_tlm()
    check_log(0, EH_RULE_MAX, tlm_EH_LOG)
    check_log(1, EH_RULE_MAX, tlm_EH_LOG)
    check_log(2, EH_RULE_MAX, tlm_EH_LOG)

    disable_eh_exec()


# 最後のお掃除
@pytest.mark.real
@pytest.mark.sils
def test_event_handler_final_check():
    print("")
    print("test_event_handler_final_check")

    # 初期化
    init_el_and_eh()

    # EHが動けるように
    enable_eh_exec()


def init_el_and_eh():
    print("init_el_and_eh")
    # EL初期化
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_INIT, (), c2a_enum.Tlm_CODE_HK
    )
    # EH初期化
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_INIT, (), c2a_enum.Tlm_CODE_HK
    )


def download_eh_tlm():
    return wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EH
    )


def download_eh_rule_tlm():
    return wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EH_RULE
    )


def download_eh_log_tlm():
    return wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EH_LOG
    )


def download_eh_index_tlm():
    return wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EH_INDEX
    )


def download_el_tlm():
    return wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EL
    )


def check_rule(name, rule_id, settings):
    print("check_rule: " + name)
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_SET_TARGET_ID_OF_RULE_TABLE_FOR_TLM,
        (rule_id,),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.TLM_INFO.RULE.TARGET_RULE_ID"] == rule_id
    assert tlm_EH["EH.TARTGET_RULE.SETTINGS.EVENT.GROUP"] == settings["group"]
    assert tlm_EH["EH.TARTGET_RULE.SETTINGS.EVENT.LOCAL"] == settings["local"]
    assert tlm_EH["EH.TARTGET_RULE.SETTINGS.IS_ACTIVE"] == settings["is_active"]


def check_log(index, id, tlm_EH_LOG):
    print("check_log: " + str(index) + ", id: " + str(id))
    assert tlm_EH_LOG["EH_LOG.LOG" + str(index) + ".RULE_ID"] == id


def check_rule_index(name, id, settings, tlm_EH_INDEX):
    print("check_rule_index: " + name + ", id: " + str(id))
    assert tlm_EH_INDEX["EH_INDEX.IDX" + str(id) + ".GROUP"] == settings["group"]
    assert tlm_EH_INDEX["EH_INDEX.IDX" + str(id) + ".LOCAL"] == settings["local"]
    assert tlm_EH_INDEX["EH_INDEX.IDX" + str(id) + ".DUPLICATE_ID"] == settings["duplicate_id"]
    assert tlm_EH_INDEX["EH_INDEX.IDX" + str(id) + ".RULE_ID"] == settings["rule_id"]


def check_default_rules():
    print("check_default_rules")
    check_rule(
        "TEST0", EH_RULE_TEST0, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "ACTIVE"}
    )
    check_rule(
        "TEST1", EH_RULE_TEST1, {"group": EL_GROUP_TEST_EH, "local": 1, "is_active": "ACTIVE"}
    )
    check_rule(
        "TEST2", EH_RULE_TEST2, {"group": EL_GROUP_TEST_EH, "local": 2, "is_active": "ACTIVE"}
    )
    check_rule(
        "TEST3", EH_RULE_TEST3, {"group": EL_GROUP_TEST_EH, "local": 3, "is_active": "ACTIVE"}
    )
    check_rule(
        "TEST4", EH_RULE_TEST4, {"group": EL_GROUP_TEST_EH, "local": 0, "is_active": "INACTIVE"}
    )
    check_rule(
        "TEST5",
        EH_RULE_TEST5,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "is_active": "INACTIVE",
        },
    )
    check_rule(
        "TEST6",
        EH_RULE_TEST6,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "is_active": "INACTIVE",
        },
    )


def check_rules_cleared():
    print("check_default_rule_cleared")
    check_rule("TEST0", EH_RULE_TEST0, {"group": 0, "local": 0, "is_active": "INACTIVE"})
    check_rule("TEST1", EH_RULE_TEST1, {"group": 0, "local": 0, "is_active": "INACTIVE"})
    check_rule("TEST2", EH_RULE_TEST2, {"group": 0, "local": 0, "is_active": "INACTIVE"})
    check_rule("TEST3", EH_RULE_TEST3, {"group": 0, "local": 0, "is_active": "INACTIVE"})
    check_rule("TEST4", EH_RULE_TEST3, {"group": 0, "local": 0, "is_active": "INACTIVE"})
    check_rule("TEST5", EH_RULE_TEST3, {"group": 0, "local": 0, "is_active": "INACTIVE"})
    check_rule("TEST6", EH_RULE_TEST3, {"group": 0, "local": 0, "is_active": "INACTIVE"})
    check_rule("TEST7", EH_RULE_TEST3, {"group": 0, "local": 0, "is_active": "INACTIVE"})


def check_default_rule_indexes():
    # FIXME: user側でルールが追加すると，テストが通らなくなる！
    print("check_default_rule_indexes")

    print("Cmd_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM, (0,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "TEST0",
        2,
        {"group": EL_GROUP_TEST_EH, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_TEST0},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST1",
        4,
        {"group": EL_GROUP_TEST_EH, "local": 1, "duplicate_id": 0, "rule_id": EH_RULE_TEST1},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST2",
        5,
        {"group": EL_GROUP_TEST_EH, "local": 2, "duplicate_id": 0, "rule_id": EH_RULE_TEST2},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST3",
        6,
        {"group": EL_GROUP_TEST_EH, "local": 3, "duplicate_id": 0, "rule_id": EH_RULE_TEST3},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST4",
        3,
        {"group": EL_GROUP_TEST_EH, "local": 0, "duplicate_id": 1, "rule_id": EH_RULE_TEST4},
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST5",
        0,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST0,
            "duplicate_id": 0,
            "rule_id": EH_RULE_TEST5,
        },
        tlm_EH_INDEX,
    )
    check_rule_index(
        "TEST6",
        1,
        {
            "group": c2a_enum.EL_CORE_GROUP_EH_MATCH_RULE,
            "local": EH_RULE_TEST5,
            "duplicate_id": 0,
            "rule_id": EH_RULE_TEST6,
        },
        tlm_EH_INDEX,
    )


def check_rule_indexes_cleared():
    print("check_rule_indexes_cleared")

    print("Cmd_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_SET_PAGE_OF_RULE_SORTED_IDX_FOR_TLM, (0,), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH_INDEX = download_eh_index_tlm()
    check_rule_index(
        "0", 0, {"group": 0, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_MAX}, tlm_EH_INDEX
    )
    check_rule_index(
        "1", 1, {"group": 0, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_MAX}, tlm_EH_INDEX
    )
    check_rule_index(
        "2", 2, {"group": 0, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_MAX}, tlm_EH_INDEX
    )
    check_rule_index(
        "3", 3, {"group": 0, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_MAX}, tlm_EH_INDEX
    )
    check_rule_index(
        "4", 4, {"group": 0, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_MAX}, tlm_EH_INDEX
    )
    check_rule_index(
        "5", 5, {"group": 0, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_MAX}, tlm_EH_INDEX
    )
    check_rule_index(
        "6", 6, {"group": 0, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_MAX}, tlm_EH_INDEX
    )
    check_rule_index(
        "7", 7, {"group": 0, "local": 0, "duplicate_id": 0, "rule_id": EH_RULE_MAX}, tlm_EH_INDEX
    )


def set_param_of_reg_from_cmd_eh_rule(rule_id, settings):
    print("set_param_of_reg_from_cmd_eh_rule")

    print("Cmd_EH_SET_REGISTER_RULE_EVENT_PARAM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_SET_REGISTER_RULE_EVENT_PARAM,
        (
            rule_id,
            settings["event"]["group"],
            settings["event"]["local"],
            settings["event"]["err_level"],
            settings["should_match_err_level"],
            settings["deploy_bct_id"],
        ),
        c2a_enum.Tlm_CODE_HK,
    )
    print("Cmd_EH_SET_REGISTER_RULE_CONDITION_PARAM")
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EH_SET_REGISTER_RULE_CONDITION_PARAM,
        (
            settings["condition"]["type"],
            settings["condition"]["count_threshold"],
            settings["condition"]["time_threshold_ms"],
            settings["is_active"],
        ),
        c2a_enum.Tlm_CODE_HK,
    )


def check_reg_from_cmd_eh_rule_param(rule_id, settings):
    print("check_reg_from_cmd_eh_rule_param")

    tlm_EH = download_eh_tlm()

    assert tlm_EH["EH.REG_FROM_CMD.RULE_ID"] == rule_id
    assert tlm_EH["EH.REG_FROM_CMD.SETTINGS.EVENT.GROUP"] == settings["event"]["group"]
    assert tlm_EH["EH.REG_FROM_CMD.SETTINGS.EVENT.LOCAL"] == settings["event"]["local"]
    assert (
        tlm_EH["EH.REG_FROM_CMD.SETTINGS.EVENT.ERR_LEVEL"]
        == conv_to_err_level[settings["event"]["err_level"]]
    )
    assert (
        tlm_EH["EH.REG_FROM_CMD.SETTINGS.SHOULD_MATCH_ERR_LEVEL"]
        == conv_to_match_flag[settings["should_match_err_level"]]
    )
    assert tlm_EH["EH.REG_FROM_CMD.SETTINGS.IS_ACTIVE"] == conv_to_active[settings["is_active"]]
    assert (
        tlm_EH["EH.REG_FROM_CMD.SETTINGS.CONDITION.TYPE"]
        == conv_to_condition_type[settings["condition"]["type"]]
    )
    assert (
        tlm_EH["EH.REG_FROM_CMD.SETTINGS.CONDITION.COUNT_THRESHOLD"]
        == settings["condition"]["count_threshold"]
    )
    assert (
        tlm_EH["EH.REG_FROM_CMD.SETTINGS.CONDITION.TIME_THRESHOLD_MS"]
        == settings["condition"]["time_threshold_ms"]
    )
    assert tlm_EH["EH.REG_FROM_CMD.SETTINGS.DEPLOY_BCT_ID"] == settings["deploy_bct_id"]


def register_rule():
    print("register_rule")

    print("Cmd_EH_REGISTER_RULE")
    cmd_ret = wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EH_REGISTER_RULE, (), c2a_enum.Tlm_CODE_HK
    )

    tlm_EH = download_eh_tlm()

    return (cmd_ret, tlm_EH["EH.REG_FROM_CMD.REGISTER_ACK"])


def check_respend_eh():
    print("check_respend_eh")
    time.sleep(1)

    # # WINGS で目視するため
    # download_eh_tlm()

    (group, local, err_level) = get_latest_event()
    if group == EL_GROUP_TEST_EH_RESPOND and local == 0 and err_level == "LOW":
        return "responded"
    else:
        return "not_responded"


def get_latest_event():
    print("check_latest_event")

    tlm_EL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EL
    )
    return (
        tlm_EL["EL.LATEST_EVENT.GROUP"],
        tlm_EL["EL.LATEST_EVENT.LOCAL"],
        tlm_EL["EL.LATEST_EVENT.ERR_LEVEL"],
    )


def exec_eh():
    print("exec_eh")
    wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EVENT_UTIL_EXEC_EH, (), c2a_enum.Tlm_CODE_HK
    )


def enable_eh_exec():
    print("enable_eh_exec")

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EVENT_UTIL_ENABLE_EH_EXEC, (), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.EVENT_UTIL.IS_ENABLED_EH_EXECUTION"] == "ENABLE"


def disable_eh_exec():
    print("disable_eh_exec")

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EVENT_UTIL_DISABLE_EH_EXEC, (), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH = download_eh_tlm()
    assert tlm_EH["EH.EVENT_UTIL.IS_ENABLED_EH_EXECUTION"] == "DISABLE"


if __name__ == "__main__":
    pass
