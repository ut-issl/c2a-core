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
OBCT_STEP_IN_MSEC = 1                                                    # 1 step で何 ms か
OBCT_STEPS_PER_CYCLE = 100                                               # 何 step で 1 cycle か
OBCT_CYCLES_PER_SEC = 1000 // OBCT_STEP_IN_MSEC // OBCT_STEPS_PER_CYCLE  # 1 s で何 cycle か
TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL = 1577836800.0


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_set_time():

    assert "PRM" == wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_TIME, (0xFFFFFFFF,), c2a_enum.Tlm_CODE_HK
    )

    # TL2のテレメループが途切れないように、現在時刻より未来のTIに飛ばす
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    target_ti = tlm_HK["HK.SH.TI"] + 1000

    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_TIME, (target_ti,), c2a_enum.Tlm_CODE_HK
    )
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    assert tlm_HK["HK.SH.TI"] > target_ti
    assert tlm_HK["HK.SH.TI"] < target_ti + 50


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_set_unixtime():

    # unixtime_at_ti0 を current_unixtime とランダムな TI で更新
    current_unixtime = time.time()
    ti = random.randrange(1000)
    step = random.randrange(OBCT_STEPS_PER_CYCLE) # step は OBCT_STEPS_PER_CYCLE 未満 とすること
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_SET_UNIXTIME,
        (current_unixtime, ti, step),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    unixtime_at_ti0 = current_unixtime - (ti / OBCT_CYCLES_PER_SEC) - (step / OBCT_CYCLES_PER_SEC / OBCT_STEPS_PER_CYCLE)
    assert tlm_HK["HK.OBC_TM_UNIXTIME_AT_TI0"] < unixtime_at_ti0 + OBCT_STEP_IN_MSEC / 1000
    assert tlm_HK["HK.OBC_TM_UNIXTIME_AT_TI0"] > unixtime_at_ti0 - OBCT_STEP_IN_MSEC / 1000


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_set_utl_unixtime_epoch():

    # epoch を現在の unixtime に変更
    new_epoch = time.time()
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH, (new_epoch,), c2a_enum.Tlm_CODE_HK
    )

    tlm_MOBC = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_MOBC
    )
    assert tlm_MOBC["MOBC.TM_UTL_UNIXTIME_EPOCH"] == new_epoch

    # epoch をデフォルトに戻す
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH, (TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL,), c2a_enum.Tlm_CODE_HK
    )

    tlm_MOBC = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_MOBC
    )
    assert tlm_MOBC["MOBC.TM_UTL_UNIXTIME_EPOCH"] == TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_utl_cmd():

    # unixtime_at_ti0 > epoch の場合（正常時）
    unixtime_at_ti0 = time.time()
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_SET_UNIXTIME,
        (unixtime_at_ti0, 0, 0),
        c2a_enum.Tlm_CODE_HK,
    )

    _test_utl_cmd(unixtime_at_ti0, TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL)


    # unixtime_at_ti0 < epoch の場合（意図した TI で UTL が打てない）
    unixtime_at_ti0 = TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL - 100
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_SET_UNIXTIME,
        (unixtime_at_ti0, 0, 0),
        c2a_enum.Tlm_CODE_HK,
    )

    _test_utl_cmd(unixtime_at_ti0, TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL)


    # epoch が変わった場合
    new_epoch = time.time()
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH, (new_epoch,), c2a_enum.Tlm_CODE_HK
    )

    unixtime_at_ti0 = time.time() + 100
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_SET_UNIXTIME,
        (unixtime_at_ti0, 0, 0),
        c2a_enum.Tlm_CODE_HK,
    )

    _test_utl_cmd(unixtime_at_ti0, new_epoch)


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_final_check():
    # unixtime_at_ti0 を初期化する
    unixtime_at_ti0 = 0.0
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_SET_UNIXTIME,
        (unixtime_at_ti0, 0, 0),
        c2a_enum.Tlm_CODE_HK,
    )

    # epochをデフォルトに戻す
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH, (TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL,), c2a_enum.Tlm_CODE_HK
    )


def _test_utl_cmd(unixtime_at_ti0, utl_unixtime_epoch):
    # 最初にTL0をクリアしておく
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (0, ),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    unixtime_now = unixtime_at_ti0 + tlm_HK["HK.SH.TI"] / OBCT_CYCLES_PER_SEC

    # NOP を10個, 未来のランダムな unixtime で登録する
    unixtime_of_cmds = generate_random_unixtime(unixtime_now)
    send_utl_nops(unixtime_of_cmds)

    # 重複を削除して時刻順に並べ替え
    unixtime_of_cmds = list(set(unixtime_of_cmds))
    unixtime_of_cmds.sort()


    # TL0 に正しく登録されているか確認
    tlm_TL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )
    assert tlm_TL["TL.LINE_NO"] == 0

    for i, unixtime in enumerate(unixtime_of_cmds):
        tlm_name = "TL.CMD" + str(i) + "_TI"
        ti = calc_ti_from_unixtime(unixtime, unixtime_at_ti0, utl_unixtime_epoch)

        assert tlm_TL[tlm_name] > ti - 1
        assert tlm_TL[tlm_name] < ti + 1

    # 最後にTL0をもう一度クリアする
    assert "SUC" == wings.util.send_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (0, ),
        c2a_enum.Tlm_CODE_HK,
    )


# 未来のランダムな時刻の unixtime を 10 個生成する
def generate_random_unixtime(unixtime_now):
    # TODO: wingsがutl_cmdの時刻引数を0.1秒刻みで受け付けるように改修されたら, 整数縛りをなくす
    unixtime_future = (int)(unixtime_now) + 1000

    return [unixtime_future + random.randrange(100) for i in range(10)]


def send_utl_nops(unixtime_of_cmds):
    for unixtime in unixtime_of_cmds:
        # utl_cmd のwings側の時刻引数は一般の unixtime であることに注意
        wings.util.send_utl_cmd(
            ope, unixtime, c2a_enum.Cmd_CODE_NOP, (),
        )


def calc_ti_from_unixtime(unixtime, unixtime_at_ti0, epoch):
    ti = (unixtime - unixtime_at_ti0) * OBCT_CYCLES_PER_SEC  # 概算値なので小数でもOK

    # utl_unixtime_epoch をデフォルトから変更した場合, wings側とずれが生じる
    # epoch が増えた分だけ, C2A 上では utl_unixtime_at_ti0 が小さくなり, 実行時刻 TI は大きく見積もられる
    ti = ti + (epoch - TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL) * OBCT_CYCLES_PER_SEC

    if unixtime_at_ti0 >= epoch:
        return ti
    else:
        # unixtime_at_ti0 < epoch の場合,
        # utl_unixtime_at_ti0 = 0 として例外処理され大きい値が返り, 実行時刻 TI は小さく見積もられる
        return ti - (epoch - unixtime_at_ti0) * OBCT_CYCLES_PER_SEC


if __name__ == "__main__":
    # test_tmgr_set_time()
    # test_tmgr_set_unixtime()
    # test_tmgr_set_utl_unixtime_epoch()
    # test_tmgr_utl_cmd()
    pass
