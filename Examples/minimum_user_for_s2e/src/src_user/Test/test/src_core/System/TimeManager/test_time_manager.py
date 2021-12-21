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

    current_unixtime = time.time()

    ti = 100
    step = 10000
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
    assert (
        tlm_HK["HK.OBC_TM_UNIXTIME_AT_TI0"]
        > current_unixtime - (ti / 10) - (step / 1000) - 0.1
    )
    assert (
        tlm_HK["HK.OBC_TM_UNIXTIME_AT_TI0"]
        < current_unixtime - (ti / 10) - (step / 1000) + 0.1
    )


if __name__ == "__main__":
    pass
    # test_tmgr_set_time()
    # test_tmgr_set_unixtime()
