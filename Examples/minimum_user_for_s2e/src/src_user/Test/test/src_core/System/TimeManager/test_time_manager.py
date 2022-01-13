#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import time
import random

import isslwings as wings
import pytest

ROOT_PATH = "../../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()


# C2Aでのdefine値
TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL = 1577836800.0


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_set_time():

    ret = wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_TIME, (0xFFFFFFFF,), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "PRM"

    # TL2のテレメループが途切れないように、現在時刻より未来のTIに飛ばす
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    target_ti = tlm_HK["HK.SH.TI"] + 1000

    ret = wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_TIME, (target_ti,), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    assert tlm_HK["HK.SH.TI"] > target_ti
    assert tlm_HK["HK.SH.TI"] < target_ti + 50


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_set_unixtime():

    #=====unixtime_at_ti0 が正しく更新できるか確認=====
    current_unixtime = time.time()
    ti = 1000
    step = 60
    ret = wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_SET_UNIXTIME,
        (current_unixtime, ti, step),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"

    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    unixtime_at_ti0 = current_unixtime - (ti / 10) - (step / 1000)
    assert tlm_HK["HK.OBC_TM_UNIXTIME_AT_TI0"] ==  unixtime_at_ti0


    #=====UTL_cmd が正しく打てるか確認=====
    # 最初にTL0をクリアしておく
    ret = wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (0, ),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"

    # NOP を10個, 未来のランダムな時刻で登録する
    # TODO: wingsがutl_cmdの時刻引数を0.1秒刻みで受け付けるように改修されたら, tiの10の倍数縛りをなくす
    ti_60_sec_after = (tlm_HK["HK.SH.TI"] // 10) * 10 + 600
    ti_of_cmds = [ti_60_sec_after + random.randrange(100) * 10 for i in range(10)]
    unixtime_at_ti0 = (int)(unixtime_at_ti0)
    for ti in ti_of_cmds:
        ret = wings.util.send_utl_cmd(
            ope, unixtime_at_ti0 + ti//10, c2a_enum.Cmd_CODE_NOP, (),
        )
    # 重複を削除して時刻順に並べ替え
    ti_of_cmds = list(set(ti_of_cmds))
    ti_of_cmds.sort()
    time.sleep(2)

    # TL0 に正しく登録されているか確認
    tlm_TL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )
    assert tlm_TL["TL.LINE_NO"] == 0
    for i, ti in enumerate(ti_of_cmds):
        tlm_name = "TL.CMD" + str(i) + "_TI"
        assert tlm_TL[tlm_name] <= ti # FIXME: 0.1秒刻みになったら修正する
        assert tlm_TL[tlm_name] > ti - 10

    # 最後にTL0をもう一度クリアする
    ret = wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (0, ),
        c2a_enum.Tlm_CODE_HK,
    )
    assert ret == "SUC"


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_set_utl_unixtime_epoch():

    epoch_set = time.time() # 現在のunixtimeをepochに設定する

    ret = wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH, (epoch_set,), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"

    tlm_MOBC = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_MOBC
    )
    assert tlm_MOBC["MOBC.TM_UTL_UNIXTIME_EPOCH"] == epoch_set

    # epochをデフォルトに戻す
    ret = wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH, (TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL,), c2a_enum.Tlm_CODE_HK
    )
    assert ret == "SUC"

    tlm_MOBC = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_MOBC
    )
    assert tlm_MOBC["MOBC.TM_UTL_UNIXTIME_EPOCH"] == TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL


if __name__ == "__main__":
    # test_tmgr_set_time()
    test_tmgr_set_unixtime()
    # test_tmgr_set_utl_unixtime_epoch()
    pass
