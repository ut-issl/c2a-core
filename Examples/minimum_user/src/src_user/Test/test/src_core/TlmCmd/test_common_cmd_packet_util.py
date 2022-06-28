#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys

import isslwings as wings
import pytest

ROOT_PATH = "../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()


@pytest.mark.real
@pytest.mark.sils
def test_ccp_register_tlc_asap():
    # 登録前に TL_GS をクリア
    clear_tl_gs()

    # GS にセット
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_SET_ID_FOR_TLM,
        (c2a_enum.TLCD_ID_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_TL = get_latest_tl_tlm()
    ti_now = tlm_TL["TL.SH.TI"]

    # 何も無いところに登録
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TEST_CCP_REGISTER_TLC_ASAP,
        (ti_now + 10000,),
        c2a_enum.Tlm_CODE_HK,
    )

    # 確認
    tlm_TL = get_latest_tl_tlm()
    assert tlm_TL["TL.CMD0_TI"] == ti_now + 10000

    # 同時刻に登録
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TEST_CCP_REGISTER_TLC_ASAP,
        (ti_now + 10000,),
        c2a_enum.Tlm_CODE_HK,
    )

    # 確認
    tlm_TL = get_latest_tl_tlm()
    assert tlm_TL["TL.CMD0_TI"] == ti_now + 10000
    assert tlm_TL["TL.CMD1_TI"] == ti_now + 10001

    # 末尾に登録
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TEST_CCP_REGISTER_TLC_ASAP,
        (ti_now + 10002,),
        c2a_enum.Tlm_CODE_HK,
    )

    # 確認
    tlm_TL = get_latest_tl_tlm()
    assert tlm_TL["TL.CMD0_TI"] == ti_now + 10000
    assert tlm_TL["TL.CMD1_TI"] == ti_now + 10001
    assert tlm_TL["TL.CMD2_TI"] == ti_now + 10002

    # 最初と同時刻に登録
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TEST_CCP_REGISTER_TLC_ASAP,
        (ti_now + 10000,),
        c2a_enum.Tlm_CODE_HK,
    )

    # 確認
    tlm_TL = get_latest_tl_tlm()
    assert tlm_TL["TL.CMD0_TI"] == ti_now + 10000
    assert tlm_TL["TL.CMD1_TI"] == ti_now + 10001
    assert tlm_TL["TL.CMD2_TI"] == ti_now + 10002
    assert tlm_TL["TL.CMD3_TI"] == ti_now + 10003

    # 中間時刻に登録
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TEST_CCP_REGISTER_TLC_ASAP,
        (ti_now + 10001,),
        c2a_enum.Tlm_CODE_HK,
    )

    # 確認
    tlm_TL = get_latest_tl_tlm()
    assert tlm_TL["TL.CMD0_TI"] == ti_now + 10000
    assert tlm_TL["TL.CMD1_TI"] == ti_now + 10001
    assert tlm_TL["TL.CMD2_TI"] == ti_now + 10002
    assert tlm_TL["TL.CMD3_TI"] == ti_now + 10003
    assert tlm_TL["TL.CMD4_TI"] == ti_now + 10004

    # 末尾と同時刻に登録
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TEST_CCP_REGISTER_TLC_ASAP,
        (ti_now + 10004,),
        c2a_enum.Tlm_CODE_HK,
    )

    # 確認
    tlm_TL = get_latest_tl_tlm()
    assert tlm_TL["TL.CMD0_TI"] == ti_now + 10000
    assert tlm_TL["TL.CMD1_TI"] == ti_now + 10001
    assert tlm_TL["TL.CMD2_TI"] == ti_now + 10002
    assert tlm_TL["TL.CMD3_TI"] == ti_now + 10003
    assert tlm_TL["TL.CMD4_TI"] == ti_now + 10004
    assert tlm_TL["TL.CMD5_TI"] == ti_now + 10005

    # 最後にクリア
    clear_tl_gs()


@pytest.mark.real
@pytest.mark.sils
def test_ccp_get_raw_param_info():
    # raw なし
    assert "CNT" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TEST_CCP_GET_RAW_PARAM_INFO,
        (0, 0, "0x1234"),
        c2a_enum.Tlm_CODE_HK,
    )
    # FIXME: raw パラメタの長さが 0 で送信できず．．．
    # assert "SUC" == wings.util.send_rt_cmd_and_confirm(
    #     ope,
    #     c2a_enum.Cmd_CODE_TEST_CCP_GET_RAW_PARAM_INFO,
    #     (0, 0, ),
    #     c2a_enum.Tlm_CODE_HK,
    # )

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TEST_CCP_GET_RAW_PARAM_INFO,
        (2, 0x12, "0x1234"),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "CNT" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TEST_CCP_GET_RAW_PARAM_INFO,
        (2, 0x34, "0x1234"),
        c2a_enum.Tlm_CODE_HK,
    )


def clear_tl_gs():
    # TL_GS をクリア
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (c2a_enum.TLCD_ID_FROM_GS,),
        c2a_enum.Tlm_CODE_HK,
    )


def get_latest_tl_tlm():
    return wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TL
    )


if __name__ == "__main__":
    pass
