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
OBCT_STEP_IN_MSEC = 1  # 1 step で何 ms か
OBCT_STEPS_PER_CYCLE = 100  # 何 step で 1 cycle か
OBCT_CYCLES_PER_SEC = 1000 // OBCT_STEP_IN_MSEC // OBCT_STEPS_PER_CYCLE  # 1 s で何 cycle か
TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL = 1577836800.0
TL_ID_DEPLOY_FROM_GS = 0

TL_DFAULT_PAGE_NO = 0


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_set_time():

    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_TIME, (0xFFFFFFFF,), c2a_enum.Tlm_CODE_HK
    )

    # TL2のテレメループが途切れないように、現在時刻より未来のTIに飛ばす
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    target_ti = tlm_HK["HK.SH.TI"] + 1000

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
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
    step = random.randrange(OBCT_STEPS_PER_CYCLE)  # step は OBCT_STEPS_PER_CYCLE 未満 とすること
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_UPDATE_UNIXTIME,
        (current_unixtime, ti, step),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    unixtime_at_ti0 = (
        current_unixtime
        - (ti / OBCT_CYCLES_PER_SEC)
        - (step / OBCT_CYCLES_PER_SEC / OBCT_STEPS_PER_CYCLE)
    )
    assert tlm_HK["HK.OBC_TM_UNIXTIME_AT_TI0"] < unixtime_at_ti0 + OBCT_STEP_IN_MSEC / 1000
    assert tlm_HK["HK.OBC_TM_UNIXTIME_AT_TI0"] > unixtime_at_ti0 - OBCT_STEP_IN_MSEC / 1000

    # unixtime_at_ti0 < ti ではコマンドが通らないことを確認
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_UPDATE_UNIXTIME,
        (0, ti, step),
        c2a_enum.Tlm_CODE_HK,
    )


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_set_utl_unixtime_epoch():

    # 負の値ではコマンドが通らないことを確認
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH, (-10.0,), c2a_enum.Tlm_CODE_HK
    )

    # epoch を現在の unixtime に変更
    new_epoch = time.time()
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH, (new_epoch,), c2a_enum.Tlm_CODE_HK
    )

    tlm_MOBC = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_MOBC
    )
    assert tlm_MOBC["MOBC.TM_UTL_UNIXTIME_EPOCH"] == new_epoch


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_set_and_reset_cycle_correction():

    # 負の値ではコマンドが通らないことを確認
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_CYCLE_CORRECTION, (-0.5,), c2a_enum.Tlm_CODE_HK
    )

    # SET コマンド
    set_value = 1.1
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_CYCLE_CORRECTION, (set_value,), c2a_enum.Tlm_CODE_HK
    )

    tlm_MOBC = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_MOBC
    )
    assert tlm_MOBC["MOBC.TM_CYCLES_PER_SEC_FIX_RATIO"] == set_value

    # RESET コマンド
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_RESET_CYCLE_CORRECTION, (), c2a_enum.Tlm_CODE_HK
    )

    tlm_MOBC = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_MOBC
    )
    assert tlm_MOBC["MOBC.TM_CYCLES_PER_SEC_FIX_RATIO"] == 1.0


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_utl_cmd():

    # ===== 実行unixtime > unixtime_at_ti0 の場合 =====
    unixtime_at_ti0 = time.time()
    test_utl_cmd_ten_times(unixtime_at_ti0, TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL, 1.0)

    # ===== 実行unixtime < unixtime_at_ti0 の場合 =====
    # TODO: TL0に登録されないことを確認する

    # ===== CYCLES_PER_SEC を補正した場合 =====
    # 0.5 <= set_value <= 2.0 でランダムに補正倍率をセットする
    set_value = random.uniform(0.5, 2.0)
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_CYCLE_CORRECTION, (set_value,), c2a_enum.Tlm_CODE_HK
    )
    test_utl_cmd_ten_times(unixtime_at_ti0, TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL, set_value)

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_RESET_CYCLE_CORRECTION, (), c2a_enum.Tlm_CODE_HK
    )

    # ===== epoch が変わった場合 =====
    new_epoch = time.time()
    test_utl_cmd_ten_times(unixtime_at_ti0, new_epoch, 1.0)

    # ===== epoch を変えて CYCLES_PER_SEC も補正した場合 =====
    set_value = random.uniform(0.5, 2.0)
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_CYCLE_CORRECTION, (set_value,), c2a_enum.Tlm_CODE_HK
    )
    test_utl_cmd_ten_times(unixtime_at_ti0, new_epoch, set_value)


@pytest.mark.sils
@pytest.mark.real
def test_tmgr_final_check():
    # unixtime_info_ を初期化する
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_CLEAR_UNIXTIME_INFO,
        (),
        c2a_enum.Tlm_CODE_HK,
    )


def test_utl_cmd_ten_times(unixtime_at_ti0, utl_unixtime_epoch, cycle_correction):
    # unixtime_at_ti0 と utl_unixtime_epoch を設定する
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_UPDATE_UNIXTIME,
        (unixtime_at_ti0, 0, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH,
        (utl_unixtime_epoch,),
        c2a_enum.Tlm_CODE_HK,
    )

    # 最初にTL0をクリアしておく
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (TL_ID_DEPLOY_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    unixtime_now = unixtime_at_ti0 + tlm_HK["HK.SH.TI"] / OBCT_CYCLES_PER_SEC

    # NOP を10個, 未来のランダムな unixtime で登録する
    unixtime_of_cmds = generate_random_unixtime(unixtime_now, 10)
    send_utl_nops(unixtime_of_cmds)

    # 重複を削除して時刻順に並べ替え
    unixtime_of_cmds = list(set(unixtime_of_cmds))
    unixtime_of_cmds.sort()

    # TL0 に正しく登録されているか確認
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_LINE_NO_FOR_TIMELINE_TLM,
        (TL_ID_DEPLOY_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_PAGE_FOR_TLM,
        (TL_DFAULT_PAGE_NO,),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_TL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )
    assert tlm_TL["TL.LINE_NO"] == TL_ID_DEPLOY_FROM_GS
    assert tlm_TL["TL.PAGE_NO"] == TL_DFAULT_PAGE_NO

    for i, unixtime in enumerate(unixtime_of_cmds):
        tlm_name = "TL.CMD" + str(i) + "_TI"
        ti = calc_ti_from_unixtime(unixtime, unixtime_at_ti0, utl_unixtime_epoch, cycle_correction)

        assert tlm_TL[tlm_name] > ti - 1
        assert tlm_TL[tlm_name] < ti + 1

    # 最後にTL0をもう一度クリアする
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (TL_ID_DEPLOY_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )


# 未来のランダムな時刻の unixtime を num 個生成する
def generate_random_unixtime(unixtime_now, num):
    # TODO: wingsがutl_cmdの時刻引数を0.1秒刻みで受け付けるように改修されたら, 整数縛りをなくす
    unixtime_future = (int)(unixtime_now) + 1000

    return [unixtime_future + random.randrange(100) for i in range(num)]


def send_utl_nops(unixtime_of_cmds):
    for unixtime in unixtime_of_cmds:
        # utl_cmd のwings側の時刻引数は一般の unixtime であることに注意
        wings.util.send_utl_cmd(
            ope,
            unixtime,
            c2a_enum.Cmd_CODE_NOP,
            (),
        )


def calc_ti_from_unixtime(unixtime, unixtime_at_ti0, epoch, cycle_correction):
    precise_cycles_per_sec = OBCT_CYCLES_PER_SEC * cycle_correction

    ti = (unixtime - unixtime_at_ti0) * precise_cycles_per_sec

    # utl_unixtime_epoch をデフォルトから変更した場合, wings側とずれが生じる
    # epoch が増えた分だけ, C2A 上では unixtime が大きくなり, 実行時刻 TI は大きく見積もられる
    return ti + (epoch - TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL) * precise_cycles_per_sec


if __name__ == "__main__":
    test_tmgr_final_check()
    # test_tmgr_set_time()
    # test_tmgr_set_unixtime()
    # test_tmgr_set_utl_unixtime_epoch()
    # test_tmgr_utl_cmd()
    pass
