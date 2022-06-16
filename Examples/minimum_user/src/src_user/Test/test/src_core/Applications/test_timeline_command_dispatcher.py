#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import random

import isslwings as wings
import pytest

ROOT_PATH = "../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()

TL_TLM_PAGE_MAX = 8


@pytest.mark.sils
@pytest.mark.real
def test_tlcd_set_flag():
    # TLCD_ID_MAX 以上の引数ではコマンドが通らないことを確認
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG, (c2a_enum.TLCD_ID_MAX, 0), c2a_enum.Tlm_CODE_HK
    )
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG, (c2a_enum.TLCD_ID_MAX, 0), c2a_enum.Tlm_CODE_HK
    )

    # flag が 0 or 1 以外ではコマンドが通らないことを確認
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS, 2),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS, 2),
        c2a_enum.Tlm_CODE_HK,
    )

    # TL_GS の SOE flag チェック
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS, 1),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_HK, rec_time = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
    assert tlm_HK["HK.OBC_TLC_GS.SOE_FLAG"] == "ENA"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_HK, rec_time = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
    assert tlm_HK["HK.OBC_TLC_GS.SOE_FLAG"] == "DIS"

    # TL_GS の LOUT flag チェック
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS, 1),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_HK, rec_time = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
    assert tlm_HK["HK.OBC_TLC_GS.LOCKOUT_FLAG"] == "YES"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_HK, rec_time = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
    assert tlm_HK["HK.OBC_TLC_GS.LOCKOUT_FLAG"] == "NO"

    # TL_MIS の SOE flag チェック
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS_FOR_MISSION, 1),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_HK, rec_time = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
    assert tlm_HK["HK.OBC_TLC_MIS.SOE_FLAG"] == "ENA"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS_FOR_MISSION, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_HK, rec_time = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
    assert tlm_HK["HK.OBC_TLC_MIS.SOE_FLAG"] == "DIS"

    # TL_MIS の LOUT flag チェック
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS_FOR_MISSION, 1),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_HK, rec_time = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
    assert tlm_HK["HK.OBC_TLC_MIS.LOCKOUT_FLAG"] == "YES"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG,
        (c2a_enum.TLCD_ID_FROM_GS_FOR_MISSION, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_HK, rec_time = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
    assert tlm_HK["HK.OBC_TLC_MIS.LOCKOUT_FLAG"] == "NO"


@pytest.mark.sils
@pytest.mark.real
def test_tlcd_set_id_and_page_for_tlm():
    # id と page_no を適当な値にセット
    target_id = 1
    target_page = 2
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_ID_FOR_TLM,
        (target_id,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_PAGE_FOR_TLM,
        (target_page,),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_TL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )
    assert tlm_TL["TL.LINE_NO"] == target_id
    assert tlm_TL["TL.PAGE_NO"] == target_page

    # 不正引数が弾かれるかチェック
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_ID_FOR_TLM,
        (c2a_enum.TLCD_ID_MAX,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_PAGE_FOR_TLM,
        (TL_TLM_PAGE_MAX,),
        c2a_enum.Tlm_CODE_HK,
    )

    reset_id_and_page()


@pytest.mark.sils
@pytest.mark.real
def test_tlcd_send_and_clear_tl():

    clear_tl_gs_and_tl_mis()

    tlm_TL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )
    ti_now = tlm_TL["TL.SH.TI"]

    # このテストでは代表として Cmd_TLCD_CLEAR_TIMELINE_AT を登録して動作確認しているが, 特に意味はない
    target_cmd_id = c2a_enum.Cmd_CODE_TLCD_CLEAR_TIMELINE_AT

    # TL コマンドをランダムなTIで登録
    tis_tl_gs = generate_random_ti(ti_now, 5)
    params_tl_gs = (1, 123456)  # params は適当
    send_tl_cmds(tis_tl_gs, target_cmd_id, params_tl_gs)

    # TL_MIS コマンドをランダムなTIで登録
    tis_tl_mis = generate_random_ti(ti_now, 5)
    params_tl_mis = (2, 654321)  # params は適当
    send_tl_mis_cmds(tis_tl_mis, target_cmd_id, params_tl_mis)

    # TimeLine に正しく登録されているかチェック
    check_registered_tl_cmds(c2a_enum.TLCD_ID_FROM_GS, tis_tl_gs, target_cmd_id, params_tl_gs)
    check_registered_tl_cmds(
        c2a_enum.TLCD_ID_FROM_GS_FOR_MISSION,
        tis_tl_mis,
        target_cmd_id,
        params_tl_mis,
    )

    # Cmd_TLCD_CLEAR_TIMELINE_AT が正しく動作するかチェック
    clear_ti = tis_tl_gs[0]
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_TIMELINE_AT,
        (c2a_enum.TLCD_ID_FROM_GS, clear_ti),
        c2a_enum.Tlm_CODE_HK,
    )
    check_registered_tl_cmds(
        c2a_enum.TLCD_ID_FROM_GS,
        tis_tl_gs[1:],
        target_cmd_id,
        params_tl_gs,
    )

    # Cmd_TLCD_CLEAR_ALL_TIMELINE で正しくクリアされるか（全てゼロにリセットされるか）チェック
    clear_tl_gs_and_tl_mis()
    ti_zeros = [0] * 5
    check_registered_tl_cmds(c2a_enum.TLCD_ID_FROM_GS, ti_zeros, 0, (0, 0))
    check_registered_tl_cmds(c2a_enum.TLCD_ID_FROM_GS_FOR_MISSION, ti_zeros, 0, (0, 0))


@pytest.mark.sils
@pytest.mark.real
# 最後のお掃除
def test_tlcd_final_check():
    reset_id_and_page()
    clear_tl_gs_and_tl_mis()


# 未来のランダムな時刻の ti を num 個生成する
def generate_random_ti(ti_now, num):
    tis = [ti_now + 10000 + random.randrange(1000) for i in range(num)]
    # 重複を削除して時刻順に並べ替え
    tis = list(set(tis))
    tis.sort()
    return tis


def send_tl_cmds(tis, cmd_id, params):
    for ti in tis:
        wings.util.send_tl_cmd(ope, ti, cmd_id, params)


def send_tl_mis_cmds(tis, cmd_id, params):
    for ti in tis:
        wings.util.send_tl_mis_cmd(ope, ti, cmd_id, params)


# 指定された TI, cmd_id, params で登録されているか TL テレメでチェックする
# params のチェックは Cmd_TLCD_CLEAR_TIMELINE_AT を前提としており, パラメタ構成が違うコマンドだとエラーが出ることに注意
def check_registered_tl_cmds(line_no, tis, cmd_id, params):
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_ID_FOR_TLM,
        (line_no,),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_TL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )
    assert tlm_TL["TL.LINE_NO"] == line_no

    for i, ti in enumerate(tis):
        str_id = "TL.CMD" + str(i) + "_ID"
        str_ti = "TL.CMD" + str(i) + "_TI"
        str_param = "TL.CMD" + str(i) + "_PARAM"
        param0 = tlm_TL[str_param + "0"]  # uint8_t (例: 0x12)
        param1 = (
            tlm_TL[str_param + "1"]
            + tlm_TL[str_param + "2"][2:]
            + tlm_TL[str_param + "3"][2:]
            + tlm_TL[str_param + "4"][2:]
        )  # uint32_t (例: 0x12341234)

        # cmd_id, params は16進数に直して比較
        assert tlm_TL[str_id] == "{:#06x}".format(cmd_id)
        assert tlm_TL[str_ti] == ti
        assert param0 == "{:#04x}".format(params[0])
        assert param1 == "{:#010x}".format(params[1])


def reset_id_and_page():
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_ID_FOR_TLM,
        (c2a_enum.TLCD_ID_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_PAGE_FOR_TLM,
        (0,),
        c2a_enum.Tlm_CODE_HK,
    )


def clear_tl_gs_and_tl_mis():
    # TL_gs をクリア
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (c2a_enum.TLCD_ID_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )
    # TL_mis をクリア
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (c2a_enum.TLCD_ID_FROM_GS_FOR_MISSION,),
        c2a_enum.Tlm_CODE_HK,
    )


if __name__ == "__main__":
    # test_tlcd_send_and_clear_tl()
    pass
