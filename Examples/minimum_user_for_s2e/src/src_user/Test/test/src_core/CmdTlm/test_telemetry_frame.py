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
TF_TLM_PAGE_SIZE = 64
TF_TLM_PAGE_MAX = 4

# 使われてない tlm id
NO_USED_TLM_ID = 1


@pytest.mark.real
@pytest.mark.sils
def test_telemetry_frame_set_page():
    init_tf()

    tlm_TF = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TF
    )
    assert tlm_TF["TF.PAGE_NO"] == 0

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TF_SET_PAGE_FOR_TLM, (1,), c2a_enum.Tlm_CODE_HK
    )
    tlm_TF = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TF
    )
    assert tlm_TF["TF.PAGE_NO"] == 1

    assert "PRM" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TF_SET_PAGE_FOR_TLM, (TF_TLM_PAGE_MAX,), c2a_enum.Tlm_CODE_HK
    )
    tlm_TF = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TF
    )
    assert tlm_TF["TF.PAGE_NO"] == 1


@pytest.mark.real
@pytest.mark.sils
def test_telemetry_frame_tlm_func():
    init_tf()

    # 登録されている tlm func の確認
    tlm_TF = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TF
    )
    assert int(tlm_TF["TF.TLM0"], base=16) != 0  # tlm id = 0 は MOBC tlm が普通はある

    # 登録されていない tlm func の確認
    page = NO_USED_TLM_ID // TF_TLM_PAGE_SIZE
    offset = NO_USED_TLM_ID % TF_TLM_PAGE_SIZE

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TF_SET_PAGE_FOR_TLM, (page,), c2a_enum.Tlm_CODE_HK
    )
    tlm_TF = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TF
    )

    assert tlm_TF["TF.TLM" + str(offset)] == "0x00000000"

    # tlm func の登録
    func_adr = "0x12345678"
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_TF_REGISTER_TLM,
        (NO_USED_TLM_ID, int(func_adr, base=16)),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_TF = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_TF
    )
    assert tlm_TF["TF.TLM" + str(offset)] == func_adr

    # 不正な登録
    # → 第一引数が uint8_t なので，そもそも TF_TLM_PAGE_SIZE * TF_TLM_PAGE_MAX 以上の値を打てない
    # assert "PRM" == wings.util.send_rt_cmd_and_confirm(
    #     ope, c2a_enum.Cmd_CODE_TF_REGISTER_TLM, (TF_TLM_PAGE_SIZE * TF_TLM_PAGE_MAX, int(func_adr, base=16)), c2a_enum.Tlm_CODE_HK
    # )


@pytest.mark.real
@pytest.mark.sils
def test_telemetry_frame_final_check():
    init_tf()


def init_tf():
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_TF_INIT, (), c2a_enum.Tlm_CODE_HK
    )


if __name__ == "__main__":
    pass
