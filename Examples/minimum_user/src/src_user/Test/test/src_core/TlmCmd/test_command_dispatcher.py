#!/usr/bin/env python3
# -*- coding: utf-8 -*-

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
ope = wings_utils.get_wings_operation()

# C2Aでのdefine値
AM_TLM_PAGE_MAX = 4


@pytest.mark.sils
@pytest.mark.real
def test_cdis_exec_err():

    # === ELの初期化
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_INIT, (), c2a_enum.Tlm_CODE_HK
    )
    tlm_EL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EL
    )
    ti_now = tlm_EL["EL.SH.TI"]

    # === 以下の4つを無効パラメタでコマンド登録する
    # RT Cmd_CODE_TMGR_UPDATE_UNIXTIME
    # RT Cmd_CODE_AM_SET_PAGE_FOR_TLM
    # TL Cmd_CODE_TMGR_UPDATE_UNIXTIME
    # TL Cmd_CODE_AM_SET_PAGE_FOR_TLM
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TMGR_UPDATE_UNIXTIME, (-10, 0, 0), c2a_enum.Tlm_CODE_HK
    )
    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_AM_SET_PAGE_FOR_TLM,
        (AM_TLM_PAGE_MAX + 100,),
        c2a_enum.Tlm_CODE_HK,
    )
    wings.util.send_tl_cmd(ope, ti_now + 50, c2a_enum.Cmd_CODE_TMGR_UPDATE_UNIXTIME, (-10, 0, 0))
    wings.util.send_tl_cmd(
        ope, ti_now + 55, c2a_enum.Cmd_CODE_AM_SET_PAGE_FOR_TLM, (AM_TLM_PAGE_MAX + 100,)
    )
    time.sleep(5)

    # === ELのチェック
    tlm_EL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EL
    )
    # GS_cmd_dispatcher
    assert tlm_EL["EL.TLOGS.LOW.EVENTS3.GROUP"] == c2a_enum.EL_CORE_GROUP_CDIS_EXEC_ERR
    assert tlm_EL["EL.TLOGS.LOW.EVENTS2.GROUP"] == c2a_enum.EL_CORE_GROUP_CDIS_EXEC_ERR
    assert tlm_EL["EL.TLOGS.LOW.EVENTS3.LOCAL"] == tlm_EL["EL.TLOGS.LOW.EVENTS2.LOCAL"]
    note = (c2a_enum.Cmd_CODE_TMGR_UPDATE_UNIXTIME << 16) | c2a_enum.CCP_EXEC_ILLEGAL_PARAMETER
    assert tlm_EL["EL.TLOGS.LOW.EVENTS3.NOTE"] == note
    note = (c2a_enum.Cmd_CODE_AM_SET_PAGE_FOR_TLM << 16) | c2a_enum.CCP_EXEC_ILLEGAL_PARAMETER
    assert tlm_EL["EL.TLOGS.LOW.EVENTS2.NOTE"] == note
    # TL_cmd_dispatcher
    assert tlm_EL["EL.TLOGS.LOW.EVENTS1.GROUP"] == c2a_enum.EL_CORE_GROUP_CDIS_EXEC_ERR
    assert tlm_EL["EL.TLOGS.LOW.EVENTS0.GROUP"] == c2a_enum.EL_CORE_GROUP_CDIS_EXEC_ERR
    assert tlm_EL["EL.TLOGS.LOW.EVENTS1.LOCAL"] == tlm_EL["EL.TLOGS.LOW.EVENTS0.LOCAL"]
    note = (c2a_enum.Cmd_CODE_TMGR_UPDATE_UNIXTIME << 16) | c2a_enum.CCP_EXEC_ILLEGAL_PARAMETER
    assert tlm_EL["EL.TLOGS.LOW.EVENTS1.NOTE"] == note
    note = (c2a_enum.Cmd_CODE_AM_SET_PAGE_FOR_TLM << 16) | c2a_enum.CCP_EXEC_ILLEGAL_PARAMETER
    assert tlm_EL["EL.TLOGS.LOW.EVENTS0.NOTE"] == note
    # GSとTLをlocalで区別できているか
    assert tlm_EL["EL.TLOGS.LOW.EVENTS1.LOCAL"] != tlm_EL["EL.TLOGS.LOW.EVENTS3.LOCAL"]

    # === 最後にもう一度初期化
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_INIT, (), c2a_enum.Tlm_CODE_HK
    )


if __name__ == "__main__":
    test_cdis_exec_err()
    pass
