#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# 前提
# - AOBC -> MOBC に 1 Hz で AOBC_HK が流れていること

import os
import sys
import time

import isslwings as wings
import pytest

ROOT_PATH = "../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
mobc_c2a_enum = c2a_enum_utils.get_mobc_c2a_enum()
ope = wings_utils.get_wings_operation()

SECOND_OBC = "AOBC"
Tlm_CODE_2ND_OBC_HK = c2a_enum.Tlm_CODE_AOBC_HK
TLM_APID_2ND_OBC = c2a_enum.APID_AOBC_TLM
USE_BCT_ID = 100

# NOP だと id が 0x00 なのでちがうのを
USE_DUMMY_CMD_ID = c2a_enum.Cmd_CODE_EH_CLEAR_LOG

g_mobc_gsc_cnt = 0
g_second_obc_gsc_cnt = 0
g_second_obc_rtc_cnt = 0


@pytest.mark.real
@pytest.mark.sils
def test_c2a_comm_gsc():
    set_cmd_counter()

    send_2nd_obc_gsc_directly(USE_DUMMY_CMD_ID, ())
    check_gsc_ack(USE_DUMMY_CMD_ID, "SUC")

    send_2nd_obc_gsc_via_mobc(USE_DUMMY_CMD_ID, ())
    check_rtc_ack(USE_DUMMY_CMD_ID, "SUC")


@pytest.mark.real
@pytest.mark.sils
def test_c2a_comm_tlc():
    global g_second_obc_rtc_cnt
    set_cmd_counter()

    wait_sec = 15
    wait_ti = 10 * wait_sec

    second_obc_ti = get_2nd_obc_ti()

    send_2nd_obc_gsc_directly(c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE, (0,))
    send_2nd_obc_tlc_directly(second_obc_ti + wait_ti, USE_DUMMY_CMD_ID, ())
    check_tlc_ack(second_obc_ti + wait_ti, USE_DUMMY_CMD_ID)
    send_2nd_obc_gsc_directly(c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE, (0,))

    mobc_ti = get_mobc_ti()

    send_mobc_gsc(mobc_c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE, (0,))
    send_2nd_obc_tlc_via_mobc(mobc_ti + wait_ti, USE_DUMMY_CMD_ID, ())
    check_mobc_tlc_ack(mobc_ti + wait_ti, USE_DUMMY_CMD_ID)

    # MOBC TL の発火の確認
    time.sleep(wait_sec)
    g_second_obc_rtc_cnt += 1  # TL 経由で1つ実行
    check_rtc_ack(USE_DUMMY_CMD_ID, "SUC")


@pytest.mark.real
@pytest.mark.sils
def test_c2a_comm_blc():
    global g_second_obc_rtc_cnt
    set_cmd_counter()

    send_2nd_obc_gsc_directly(c2a_enum.Cmd_CODE_BCT_CLEAR_BLOCK, (USE_BCT_ID,))
    send_2nd_obc_bct_directly(1, USE_DUMMY_CMD_ID, ())
    ckeck_bct_ack(1, USE_DUMMY_CMD_ID, USE_BCT_ID)

    send_2nd_obc_gsc_directly(c2a_enum.Cmd_CODE_BCT_CLEAR_BLOCK, (USE_BCT_ID,))
    send_mobc_gsc(mobc_c2a_enum.Cmd_CODE_BCT_CLEAR_BLOCK, (USE_BCT_ID,))
    send_2nd_obc_bct_via_mobc(1, USE_DUMMY_CMD_ID, ())
    ckeck_mobc_bct_ack(1, USE_DUMMY_CMD_ID, USE_BCT_ID)

    # MOBC BL の発火の確認
    send_mobc_gsc(mobc_c2a_enum.Cmd_CODE_BCE_ACTIVATE_BLOCK, ())
    send_mobc_gsc(
        mobc_c2a_enum.Cmd_CODE_TLCD_DEPLOY_BLOCK, (mobc_c2a_enum.TLCD_ID_FROM_GS, USE_BCT_ID)
    )
    time.sleep(2)
    g_second_obc_rtc_cnt += 1  # TL 経由で1つ実行
    check_rtc_ack(USE_DUMMY_CMD_ID, "SUC")


@pytest.mark.real
@pytest.mark.sils
def test_c2a_comm_tlm_buffer():
    send_mobc_gsc(mobc_c2a_enum.Cmd_CODE_AM_INITIALIZE_APP, (mobc_c2a_enum.AR_DI_AOBC,))

    # MOBC の tlm buffer が空なのでエラー
    assert "CNT" == wings.util.send_rt_cmd_and_confirm(
        ope,
        mobc_c2a_enum.Cmd_CODE_TG_FORWARD_AS_MS_TLM,
        (TLM_APID_2ND_OBC, c2a_enum.Tlm_CODE_AOBC_AOBC),
        mobc_c2a_enum.Tlm_CODE_HK,
    )
    send_2nd_obc_gsc_directly(c2a_enum.Cmd_CODE_TG_GENERATE_MS_TLM, (c2a_enum.Tlm_CODE_AOBC_AOBC,))
    time.sleep(2)
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        mobc_c2a_enum.Cmd_CODE_TG_FORWARD_AS_MS_TLM,
        (TLM_APID_2ND_OBC, c2a_enum.Tlm_CODE_AOBC_AOBC),
        mobc_c2a_enum.Tlm_CODE_HK,
    )


def set_cmd_counter():
    global g_mobc_gsc_cnt
    global g_second_obc_gsc_cnt
    global g_second_obc_rtc_cnt

    g_mobc_gsc_cnt = get_mobc_gsc_counter()
    g_second_obc_gsc_cnt = get_2nd_obc_gsc_counter()
    g_second_obc_rtc_cnt = get_2nd_obc_rtc_counter()


def send_mobc_gsc(cmd_id, params):
    global g_mobc_gsc_cnt
    ope.send_rt_cmd(cmd_id, params)
    g_mobc_gsc_cnt += 1


def send_2nd_obc_gsc_directly(cmd_id, params):
    global g_second_obc_gsc_cnt
    ope.send_rt_cmd(cmd_id, params, SECOND_OBC, False)
    g_second_obc_gsc_cnt += 1


def send_2nd_obc_gsc_via_mobc(cmd_id, params):
    global g_mobc_gsc_cnt
    global g_second_obc_rtc_cnt
    ope.send_rt_cmd(cmd_id, params, SECOND_OBC, True)
    g_second_obc_rtc_cnt += 1
    g_mobc_gsc_cnt += 1  # MOBC キュー を経由


def send_2nd_obc_tlc_directly(ti, cmd_id, params):
    ope.send_tl_cmd(ti, cmd_id, params, SECOND_OBC, False)


def send_2nd_obc_tlc_via_mobc(ti, cmd_id, params):
    ope.send_tl_cmd(ti, cmd_id, params, SECOND_OBC, True)


def send_2nd_obc_bct_directly(ti, cmd_id, params):
    ope.send_bl_cmd(ti, cmd_id, params, SECOND_OBC, False)


def send_2nd_obc_bct_via_mobc(ti, cmd_id, params):
    ope.send_bl_cmd(ti, cmd_id, params, SECOND_OBC, True)


def check_gsc_ack(exec_cmd, sts):
    wait_for_2nd_obc_hk_tlm()
    assert g_mobc_gsc_cnt == get_mobc_gsc_counter()
    tlm_2ND_HK = get_2nd_obc_hk_tlm()
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.GS_CMD.COUNTER"] == g_second_obc_gsc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.RT_CMD.COUNTER"] == g_second_obc_rtc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.GS_CMD.LAST_EXEC.ID"] == exec_cmd
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.GS_CMD.LAST_EXEC.EXEC_STS"] == sts


def check_rtc_ack(exec_cmd, sts):
    wait_for_2nd_obc_hk_tlm()
    assert g_mobc_gsc_cnt == get_mobc_gsc_counter()
    tlm_2ND_HK = get_2nd_obc_hk_tlm()
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.GS_CMD.COUNTER"] == g_second_obc_gsc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.RT_CMD.COUNTER"] == g_second_obc_rtc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.RT_CMD.LAST_EXEC.ID"] == exec_cmd
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.RT_CMD.LAST_EXEC.EXEC_STS"] == sts


def check_tlc_ack(ti, exec_cmd):
    wait_for_2nd_obc_hk_tlm()
    assert g_mobc_gsc_cnt == get_mobc_gsc_counter()
    tlm_2ND_HK = get_2nd_obc_hk_tlm()
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.GS_CMD.COUNTER"] == g_second_obc_gsc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.RT_CMD.COUNTER"] == g_second_obc_rtc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.TLC_GS.QUEUED"] == 1
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.TLC_GS.NEXT_TIME"] == ti
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.TLC_GS.NEXT_ID"] == exec_cmd


def check_mobc_tlc_ack(ti, exec_cmd):
    wait_for_2nd_obc_hk_tlm()
    assert g_mobc_gsc_cnt == get_mobc_gsc_counter()
    tlm_HK = get_mobc_hk_tlm()
    assert tlm_HK["HK.OBC.TLC_GS.QUEUED"] == 1
    assert tlm_HK["HK.OBC.TLC_GS.NEXT_TIME"] == ti
    assert tlm_HK["HK.OBC.TLC_GS.NEXT_ID"] == exec_cmd
    tlm_2ND_HK = get_2nd_obc_hk_tlm()
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.GS_CMD.COUNTER"] == g_second_obc_gsc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.RT_CMD.COUNTER"] == g_second_obc_rtc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.TLC_GS.QUEUED"] == 0


def ckeck_bct_ack(ti, exec_cmd, bct_id):
    wait_for_2nd_obc_hk_tlm()
    assert g_mobc_gsc_cnt == get_mobc_gsc_counter()
    tlm_2ND_HK = get_2nd_obc_hk_tlm()
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.GS_CMD.COUNTER"] == g_second_obc_gsc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.RT_CMD.COUNTER"] == g_second_obc_rtc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.BCT_BLK_PTR"] == bct_id
    assert tlm_2ND_HK[SECOND_OBC + "_HK.BCT_CMD_PTR"] == 1
    assert tlm_2ND_HK[SECOND_OBC + "_HK.BCT_REGD_TIME"] == ti
    assert tlm_2ND_HK[SECOND_OBC + "_HK.BCT_REGD_ID"] == exec_cmd


def ckeck_mobc_bct_ack(ti, exec_cmd, bct_id):
    global g_mobc_gsc_cnt
    wait_for_2nd_obc_hk_tlm()
    assert g_mobc_gsc_cnt == get_mobc_gsc_counter()

    g_mobc_gsc_cnt += 1
    tlm_MOBC = wings.util.generate_and_receive_tlm(
        ope, mobc_c2a_enum.Cmd_CODE_TG_GENERATE_MS_TLM, mobc_c2a_enum.Tlm_CODE_MOBC
    )
    assert tlm_MOBC["MOBC.BCT_BLK_PTR"] == bct_id
    assert tlm_MOBC["MOBC.BCT_CMD_PTR"] == 1
    assert tlm_MOBC["MOBC.BCT_REGD_TIME"] == ti
    assert tlm_MOBC["MOBC.BCT_REGD_ID"] == exec_cmd

    tlm_2ND_HK = get_2nd_obc_hk_tlm()
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.GS_CMD.COUNTER"] == g_second_obc_gsc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.OBC.RT_CMD.COUNTER"] == g_second_obc_rtc_cnt
    assert tlm_2ND_HK[SECOND_OBC + "_HK.BCT_BLK_PTR"] == bct_id
    assert tlm_2ND_HK[SECOND_OBC + "_HK.BCT_CMD_PTR"] == 0


def get_mobc_hk_tlm():
    time.sleep(1)
    tlm_HK, _ = ope.get_latest_tlm(mobc_c2a_enum.Tlm_CODE_HK)
    return tlm_HK


def get_2nd_obc_hk_tlm():
    global g_mobc_gsc_cnt
    g_mobc_gsc_cnt += 1

    tlm_2ND_HK = wings.util.forward_and_receive_tlm(
        ope, mobc_c2a_enum.Cmd_CODE_TG_FORWARD_TLM, TLM_APID_2ND_OBC, Tlm_CODE_2ND_OBC_HK
    )
    return tlm_2ND_HK


def get_mobc_ti():
    return get_mobc_hk_tlm()["HK.SH.TI"]


def get_2nd_obc_ti():
    return get_2nd_obc_hk_tlm()[SECOND_OBC + "_HK.SH.TI"]


def get_mobc_gsc_counter():
    return get_mobc_hk_tlm()["HK.OBC.GS_CMD.COUNTER"]


def get_2nd_obc_gsc_counter():
    return get_2nd_obc_hk_tlm()[SECOND_OBC + "_HK.OBC.GS_CMD.COUNTER"]


def get_2nd_obc_rtc_counter():
    return get_2nd_obc_hk_tlm()[SECOND_OBC + "_HK.OBC.RT_CMD.COUNTER"]


def wait_for_2nd_obc_hk_tlm():
    time.sleep(1.5)


if __name__ == "__main__":
    pass
