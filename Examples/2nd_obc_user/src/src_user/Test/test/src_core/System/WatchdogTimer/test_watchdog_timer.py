#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys

import isslwings as wings
import pytest

ROOT_PATH = "../../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()


@pytest.mark.sils
def test_wdt_at_sils():
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    assert tlm_HK["HK.WDT.IS_ENABLE"] == "ENA"
    assert tlm_HK["HK.WDT.IS_CLEAR_ENABLE"] == "ENA"

    wings.util.send_cmd_and_confirm(ope, c2a_enum.Cmd_CODE_WDT_DISABLE, (), c2a_enum.Tlm_CODE_HK)
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    assert tlm_HK["HK.WDT.IS_ENABLE"] == "DIS"
    assert tlm_HK["HK.WDT.IS_CLEAR_ENABLE"] == "ENA"

    wings.util.send_cmd_and_confirm(ope, c2a_enum.Cmd_CODE_WDT_ENABLE, (), c2a_enum.Tlm_CODE_HK)
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    assert tlm_HK["HK.WDT.IS_ENABLE"] == "ENA"
    assert tlm_HK["HK.WDT.IS_CLEAR_ENABLE"] == "ENA"

    wings.util.send_cmd_and_confirm(ope, c2a_enum.Cmd_CODE_WDT_STOP_CLEAR, (), c2a_enum.Tlm_CODE_HK)
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    assert tlm_HK["HK.WDT.IS_ENABLE"] == "ENA"
    assert tlm_HK["HK.WDT.IS_CLEAR_ENABLE"] == "DIS"
    # 本当はここでWDTに引っかかるはずだが，SILSなので何も起きない

    wings.util.send_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_WDT_START_CLEAR, (), c2a_enum.Tlm_CODE_HK
    )
    tlm_HK = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
    )
    assert tlm_HK["HK.WDT.IS_ENABLE"] == "ENA"
    assert tlm_HK["HK.WDT.IS_CLEAR_ENABLE"] == "ENA"


@pytest.mark.real
def test_wdt_at_real():
    pass
    # FIXME: 実機用も作る
