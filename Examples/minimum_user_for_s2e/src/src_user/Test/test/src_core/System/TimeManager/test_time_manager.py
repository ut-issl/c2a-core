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
    # TLテレメを TL_gs の ページ0 にセット
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

    # ===== 通常時 =====
    check_utl_cmd_with(TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL, 1.0)

    # ===== 過去の時刻を指定した場合 =====
    # コマンドが登録されず TLC_PAST_TIME のエラーが出ることを確認する
    # generate_and_receive_tlm を使うとそれが成功して CMD_ACK = SUCCESS と上書きされてしまうので, utl で3秒後に generate_tlm を発火させる
    wings.util.send_utl_cmd(
        ope,
        time.time() + 3,
        c2a_enum.Cmd_CODE_GENERATE_TLM,
        (0x40, c2a_enum.Tlm_CODE_GS, 1),
    )
    wings.util.send_utl_cmd(
        ope,
        time.time() - 100000,
        c2a_enum.Cmd_CODE_NOP,
        (),
    )
    time.sleep(3)
    tlm_GS = ope.get_latest_tlm(c2a_enum.Tlm_CODE_GS)[0]
    assert tlm_GS["GS.CCSDS.RX.CMD_ACK"] == "TLC_PAST_TIME"

    # ===== CYCLES_PER_SEC を補正した場合 =====
    # 0.7 <= set_value <= 1.3 でランダムに補正倍率をセット
    set_value = random.uniform(0.7, 1.3)
    check_utl_cmd_with(TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL, set_value)

    # ===== epoch を変えた場合 =====
    new_epoch = time.time() - 86400 * 30  # 例えば30日前に変更
    check_utl_cmd_with(new_epoch, 1.0)

    # ===== epoch を変えて CYCLES_PER_SEC も補正した場合 =====
    set_value = random.uniform(0.7, 1.3)
    check_utl_cmd_with(new_epoch, set_value)


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

    # TL_gs をクリア
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (c2a_enum.TLCD_ID_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )


def check_utl_cmd_with(utl_unixtime_epoch, cycle_correction):
    # utl_unixtime_epoch, cycle_correction を設定する
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_SET_UTL_UNIXTIME_EPOCH,
        (utl_unixtime_epoch,),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_SET_CYCLE_CORRECTION, (cycle_correction,), c2a_enum.Tlm_CODE_HK
    )

    # unixtime_info を現在の unixtime と ti で同期する
    # 必ず Cmd_CODE_TMGR_SET_CYCLE_CORRECTION のあとに送ること
    unixtime_now = time.time()
    tlm_HK = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)[0]
    ti_now = tlm_HK["HK.SH.TI"]
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TMGR_UPDATE_UNIXTIME,
        (unixtime_now, ti_now, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    unixtime_at_ti0 = unixtime_now - ti_now / (OBCT_CYCLES_PER_SEC * cycle_correction)

    # 最初にTL_gsをクリアしておく
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (c2a_enum.TLCD_ID_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )

    # NOP を10個, 未来のランダムな unixtime で登録する
    unixtime_of_cmds = generate_random_unixtime(10)
    send_utl_nops(unixtime_of_cmds)

    # TL_gs に正しく登録されているか確認
    tlm_TL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )
    assert tlm_TL["TL.LINE_NO"] == c2a_enum.TLCD_ID_FROM_GS
    assert tlm_TL["TL.PAGE_NO"] == 0

    for i, unixtime in enumerate(unixtime_of_cmds):
        tlm_name = "TL.CMD" + str(i) + "_TI"
        ti = calc_ti_from_unixtime(unixtime, unixtime_at_ti0, utl_unixtime_epoch, cycle_correction)

        assert tlm_TL[tlm_name] > ti - 1, {"registered_unixtime": unixtime, "tlm_name": tlm_name}
        assert tlm_TL[tlm_name] < ti + 1, {"registered_unixtime": unixtime, "tlm_name": tlm_name}


# 未来のランダムな時刻の unixtime を num 個生成する
# WINGS に合わせて0.1秒精度で指定する
def generate_random_unixtime(num):
    unixtime_future = int(time.time() + 1000)
    unixtimes = [
        unixtime_future + random.randrange(0, 1000, 2) * 0.1 for i in range(num)
    ]  # 少なくとも0.2秒間隔をあける

    # 重複を削除して時刻順に並べ替え
    unixtimes = list(set(unixtimes))
    unixtimes.sort()
    return unixtimes


def send_utl_nops(unixtime_of_cmds):
    for unixtime in unixtime_of_cmds:
        # utl_cmd のwings側の時刻引数は一般の unixtime であることに注意
        wings.util.send_utl_cmd(
            ope,
            unixtime,
            c2a_enum.Cmd_CODE_NOP,
            (),
        )


# unixtime を ti に変換する（小数精度）
def calc_ti_from_unixtime(unixtime, unixtime_at_ti0, epoch, cycle_correction):
    precise_cycles_per_sec = OBCT_CYCLES_PER_SEC * cycle_correction

    ti = (unixtime - unixtime_at_ti0) * precise_cycles_per_sec

    # utl_unixtime_epoch をデフォルトから変更した場合, wings側とずれが生じる
    # epoch が増えた分だけ, C2A 上では unixtime が大きくなり, 実行時刻 TI は大きく見積もられる
    return ti + (epoch - TMGR_DEFAULT_UNIXTIME_EPOCH_FOR_UTL) * precise_cycles_per_sec


if __name__ == "__main__":
    # test_tmgr_set_time()
    # test_tmgr_set_unixtime()
    # test_tmgr_set_utl_unixtime_epoch()
    test_tmgr_utl_cmd()
    test_tmgr_final_check()
    pass
