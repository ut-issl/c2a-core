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


@pytest.mark.sils
@pytest.mark.real
def test_tlcd_set_flag():
    # TL_ID_MAX 以上の引数ではコマンドが通らないことを確認
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG, (c2a_enum.TL_ID_MAX, 0), c2a_enum.Tlm_CODE_HK
    )
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG, (c2a_enum.TL_ID_MAX, 0), c2a_enum.Tlm_CODE_HK
    )

    # flag が 0 or 1 以外ではコマンドが通らないことを確認
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG,
        (c2a_enum.TL_ID_FROM_GS, 2),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG,
        (c2a_enum.TL_ID_FROM_GS, 2),
        c2a_enum.Tlm_CODE_HK,
    )

    # SOE, LOUT ともに Flag を立てる
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG,
        (c2a_enum.TL_ID_FROM_GS, 1),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG,
        (c2a_enum.TL_ID_FROM_GS, 1),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    assert tlm_HK["HK.OBC_TL0_CMD_SOE_FLAG"] == "ENA"
    assert tlm_HK["HK.OBC_TL0_CMD_LOCKOUT_FLAG"] == "YES"

    # SOE, LOUT ともに Flag を降ろす
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_SOE_FLAG,
        (c2a_enum.TL_ID_FROM_GS, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LOUT_FLAG,
        (c2a_enum.TL_ID_FROM_GS, 0),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    assert tlm_HK["HK.OBC_TL0_CMD_SOE_FLAG"] == "DIS"
    assert tlm_HK["HK.OBC_TL0_CMD_LOCKOUT_FLAG"] == "NO"


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_tl0_and_tl3():

    clear_tl0_and_tl3()

    # page_no をゼロにセットしておく
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_PAGE_FOR_TLM,
        (0,),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_TL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )
    assert tlm_TL["TL.PAGE_NO"] == 0
    ti_now = tlm_TL["TL.SH.TI"]

    # tl コマンドをランダムなTIで登録
    ti_of_tl0_cmds = generate_random_ti(ti_now, 5)
    send_tl_nops(ti_of_tl0_cmds)

    # tl_mis コマンドをランダムなTIで登録
    ti_of_tl3_cmds = generate_random_ti(ti_now, 5)
    send_tl_mis_nops(ti_of_tl3_cmds)

    # TL0,3 に正しいTIで登録されているかチェック
    check_registered_tl_cmds(ti_of_tl0_cmds, c2a_enum.TL_ID_FROM_GS)
    check_registered_tl_cmds(ti_of_tl3_cmds, c2a_enum.TL_ID_FROM_GS_FOR_MISSION)

    # Cmd_TLCD_CLEAR_TIMELINE_AT が正しく動作するかチェック
    test_tlcd_clear_timeline_at(ti_of_tl0_cmds, c2a_enum.TL_ID_FROM_GS)

    # Cmd_TLCD_CLEAR_ALL_TIMELINE で正しくクリアされるか（ゼロにリセットされているか）チェック
    clear_tl0_and_tl3()
    ti_zeros = [0] * 5
    check_registered_tl_cmds(ti_zeros, c2a_enum.TL_ID_FROM_GS)
    check_registered_tl_cmds(ti_zeros, c2a_enum.TL_ID_FROM_GS_FOR_MISSION)


# pytest からではなく test_tmgr_tl0_and_tl3 から呼ばれることに注意
def test_tlcd_clear_timeline_at(ti_of_cmds, line_no):
    target_ti = ti_of_cmds[0]
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_TIMELINE_AT,
        (line_no, target_ti),
        c2a_enum.Tlm_CODE_HK,
    )

    check_registered_tl_cmds(ti_of_cmds[1:-1], line_no)


def clear_tl0_and_tl3():
    # TL0をクリア
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (c2a_enum.TL_ID_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )
    # TL3をクリア
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (c2a_enum.TL_ID_FROM_GS_FOR_MISSION,),
        c2a_enum.Tlm_CODE_HK,
    )


# 未来のランダムな時刻の ti を num 個生成する
def generate_random_ti(ti_now, num):
    tis = [ti_now + 10000 + random.randrange(1000) for i in range(num)]
    # 重複を削除して時刻順に並べ替え
    tis = list(set(tis))
    tis.sort()
    return tis


# 指定された TI で NOP を tl_cmd として送る
def send_tl_nops(ti_of_cmds):
    for ti in ti_of_cmds:
        wings.util.send_tl_cmd(
            ope,
            ti,
            c2a_enum.Cmd_CODE_NOP,
            (),
        )


# 指定された TI で NOP を tl_mis_cmd として送る
def send_tl_mis_nops(ti_of_cmds):
    for ti in ti_of_cmds:
        wings.util.send_tl_mis_cmd(
            ope,
            ti,
            c2a_enum.Cmd_CODE_NOP,
            (),
        )


# 指定された TI で登録されているか TL テレメでチェックする関数
def check_registered_tl_cmds(ti_of_tl_cmds, line_no):
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LINE_NO_FOR_TIMELINE_TLM,
        (line_no,),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_TL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )
    assert tlm_TL["TL.LINE_NO"] == line_no

    for i, ti in enumerate(ti_of_tl_cmds):
        tlm_name = "TL.CMD" + str(i) + "_TI"
        assert tlm_TL[tlm_name] == ti


if __name__ == "__main__":
    # test_tmgr_tl0_and_tl3()
    pass
