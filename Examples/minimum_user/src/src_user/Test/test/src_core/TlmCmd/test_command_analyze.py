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

# コードと整合をとる
CA_TLM_PAGE_SIZE = 32
CA_TLM_PAGE_MAX = 48
CA_MAX_CMDS = CA_TLM_PAGE_SIZE * CA_TLM_PAGE_MAX
CA_MAX_CMD_PARAM_NUM = 6

# 既存のコマンドIDと被らないように！！
TEST_CMD_ID = CA_MAX_CMDS - CA_TLM_PAGE_SIZE


@pytest.mark.real
@pytest.mark.sils
def test_command_analyze_set_page():
    tlm_CA = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_CA
    )
    assert tlm_CA["CA.PAGE_NO"] == 0

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_CA_SET_PAGE_FOR_TLM, (1,), c2a_enum.Tlm_CODE_HK
    )
    tlm_CA = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_CA
    )
    assert tlm_CA["CA.PAGE_NO"] == 1

    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_CA_SET_PAGE_FOR_TLM, (CA_TLM_PAGE_MAX,), c2a_enum.Tlm_CODE_HK
    )


@pytest.mark.real
@pytest.mark.sils
def test_command_analyze_add_cmd():
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_CA_SET_PAGE_FOR_TLM, (CA_TLM_PAGE_MAX - 1,), c2a_enum.Tlm_CODE_HK
    )

    # これから上書きするので，NULL，つまり使われてないものでないとNG
    tlm_CA = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_CA
    )
    assert tlm_CA["CA.CMD0.FUNC"] == "0x00000000"

    test_cmd_adr = "0x12345678"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_CA_REGISTER_CMD,
        (TEST_CMD_ID, test_cmd_adr, "0x1234f0"),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_CA = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_CA
    )
    assert tlm_CA["CA.CMD0.FUNC"] == test_cmd_adr
    assert tlm_CA["CA.CMD0.PARAM0_SIZE"] == "1BYTE"
    assert tlm_CA["CA.CMD0.PARAM1_SIZE"] == "2BYTE"
    assert tlm_CA["CA.CMD0.PARAM2_SIZE"] == "4BYTE"
    assert tlm_CA["CA.CMD0.PARAM3_SIZE"] == "8BYTE"
    assert tlm_CA["CA.CMD0.PARAM4_SIZE"] == "RAW"
    assert tlm_CA["CA.CMD0.PARAM5_SIZE"] == "NONE"

    # 危ないので戻す
    test_cmd_adr = "0x00000000"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_CA_REGISTER_CMD,
        (TEST_CMD_ID, test_cmd_adr, "0x000000"),
        c2a_enum.Tlm_CODE_HK,
    )

    tlm_CA = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_CA
    )
    assert tlm_CA["CA.CMD0.FUNC"] == test_cmd_adr
    assert tlm_CA["CA.CMD0.PARAM0_SIZE"] == "NONE"
    assert tlm_CA["CA.CMD0.PARAM1_SIZE"] == "NONE"
    assert tlm_CA["CA.CMD0.PARAM2_SIZE"] == "NONE"
    assert tlm_CA["CA.CMD0.PARAM3_SIZE"] == "NONE"
    assert tlm_CA["CA.CMD0.PARAM4_SIZE"] == "NONE"
    assert tlm_CA["CA.CMD0.PARAM5_SIZE"] == "NONE"

    assert "LEN" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_CA_REGISTER_CMD,
        (TEST_CMD_ID, test_cmd_adr, "0x0000"),
        c2a_enum.Tlm_CODE_HK,
    )
    assert "LEN" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_CA_REGISTER_CMD,
        (TEST_CMD_ID, test_cmd_adr, "0x00000000"),
        c2a_enum.Tlm_CODE_HK,
    )


@pytest.mark.real
@pytest.mark.sils
def test_command_analyze_final_check():
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_CA_SET_PAGE_FOR_TLM, (0,), c2a_enum.Tlm_CODE_HK
    )
    tlm_CA = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_CA
    )
    assert tlm_CA["CA.PAGE_NO"] == 0


if __name__ == "__main__":
    pass
