#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import struct

import isslwings as wings
import pytest

ROOT_PATH = "../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()


# nbc_test_bcl.c で用いている引数の値
PARAMS_LIST = [
    # Cmd_MM_SET_TRANSITION_TABLE
    [("uint8", 1), ("uint8", 22), ("uint16", 333)],
    # Cmd_TLCD_CLEAR_TIMELINE_AT
    [("uint8", 44), ("uint32", 555555)],
    # Cmd_TMGR_SET_UTL_UNIXTIME_EPOCH
    [("double", 12345.6789)],
    [("double", -12345.6789)],
    # AOBC_Cmd_CODE_MM_START_TRANSITION
    [("uint8", 3)],
]


@pytest.mark.sils
@pytest.mark.real
def test_bcl_prepare_param():

    # BC_TEST_BCL を BLテレメに降ろしてくる
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_BCT_SET_BLOCK_POSITION,
        (c2a_enum.BC_TEST_BCL, 0),
        c2a_enum.Tlm_CODE_HK,
    )
    tlm_BL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_BL
    )
    assert tlm_BL["BL.BLOCK_NO"] == c2a_enum.BC_TEST_BCL

    # i: BCの何番目のコマンドか
    # j: そのコマンドの引数の何byte目か
    for i, params in enumerate(PARAMS_LIST):
        hex_str = params2hex(params)
        param_len = min(len(hex_str) // 2, 6)  # BLテレメでは最大 6byte までしか引数が見えないので
        for j in range(param_len):
            tlm_name = "BL.CMD" + str(i) + "_PARAM" + str(j)
            assert tlm_BL[tlm_name] == "0x" + hex_str[j * 2 : j * 2 + 2]


# コマンドのパラメタを16進数byte列に変換する関数
def params2hex(params):
    # params = [("uint8", 12), ("uint16", 345)] のようなものを想定
    hex_str = ""
    for type, num in params:
        if type == "uint8" or type == "int8":
            temp = hex(num & 0xFF)
            hex_str += format_hex_str(temp, 2)
        elif type == "uint16" or type == "int16":
            temp = hex(num & 0xFFFF)
            hex_str += format_hex_str(temp, 4)
        elif type == "uint32" or type == "int32":
            temp = hex(num & 0xFFFFFFFF)
            hex_str += format_hex_str(temp, 8)
        elif type == "float":
            temp = float_to_hex(num)
            hex_str += format_hex_str(temp, 8)
        elif type == "uint64" or type == "int64":
            temp = hex(num & 0xFFFFFFFFFFFFFFFF)
            hex_str += format_hex_str(temp, 16)
        elif type == "double":
            temp = double_to_hex(num)
            hex_str += format_hex_str(temp, 16)
        else:
            raise ValueError("未実装の引数タイプです")
    return hex_str


def format_hex_str(hex_str, size):
    hex_str = hex_str[2:]  # '0x'を除去
    while len(hex_str) < size:
        hex_str = "0" + hex_str  # size分のbyte数になるまで先頭を0で埋める
    return hex_str


def float_to_hex(param):
    # ビッグエンディアンの場合は引数を ">f" とする
    return hex(struct.unpack(">I", struct.pack(">f", param))[0])


def double_to_hex(param):
    # ビッグエンディアンの場合は引数を ">d" とする
    return hex(struct.unpack(">Q", struct.pack(">d", param))[0])


if __name__ == "__main__":
    test_bcl_prepare_param()
    pass
