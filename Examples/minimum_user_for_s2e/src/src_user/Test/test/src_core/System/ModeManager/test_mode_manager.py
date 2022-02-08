#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys

# import time

# import isslwings as wings
import pytest

ROOT_PATH = "../../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()


# 他をコメントアウトしてると， pytest がコケるので
@pytest.mark.sils
@pytest.mark.real
def test_mm_nop():
    pass


# @pytest.mark.sils
# # @pytest.mark.real
# def test_mm_set_mode_list():
#     invalid_mode = 16
#     invalid_bc = 0x180
#     valid_mode = 15
#     valid_bc = 0x17E

#     # ===== 存在しないモード =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_BCE_ACTIVATE_BLOCK_BY_ID,
#         (valid_bc,),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_MODE_LIST,
#         (invalid_mode, valid_bc),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK, _ = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == ""

#     # ===== 範囲外のブロックコマンド =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_MODE_LIST,
#         (valid_mode, invalid_bc),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK, _ = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "ERR"

#     # ===== 無効化されているブロックコマンド =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_BCE_INACTIVATE_BLOCK_BY_ID,
#         (valid_bc,),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_MODE_LIST,
#         (valid_mode, valid_bc),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK, _ = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "ERR"

#     # ===== 成功 =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_BCE_ACTIVATE_BLOCK_BY_ID,
#         (valid_bc,),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_MODE_LIST,
#         (valid_mode, valid_bc),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK, _ = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "SUC"

#     tlm_MM = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_MM
#     )
#     assert tlm_MM["MM.MODE_LIST_15"] == valid_bc - 0x100  # FIXME: 0x17Eで動くようにする


# @pytest.mark.sils
# # @pytest.mark.real
# def test_mm_set_transition_table():

#     invalid_mode = 16
#     invalid_bc = 0x180
#     valid_mode = 15
#     valid_bc = 0x17E

#     # ===== 存在しないモード =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_TRANSITION_TABLE,
#         (invalid_mode, valid_mode, valid_bc),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK, _ = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "ERR"

#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_TRANSITION_TABLE,
#         (valid_mode, invalid_mode, valid_bc),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK, _ = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "ERR"

#     # ===== 範囲外のブロックコマンド =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_TRANSITION_TABLE,
#         (valid_mode, valid_mode, invalid_bc),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK, _ = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "ERR"

#     # ===== 無効化されているブロックコマンド =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_BCE_INACTIVATE_BLOCK_BY_ID,
#         (valid_bc,),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_TRANSITION_TABLE,
#         (valid_mode, valid_mode, valid_bc),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK, _ = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "ERR"

#     # ===== 成功 =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_BCE_ACTIVATE_BLOCK_BY_ID,
#         (valid_bc,),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_TRANSITION_TABLE,
#         (valid_mode, valid_mode, valid_bc),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK, _ = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "SUC"

#     tlm_MM = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_MM
#     )
#     # assert tlm_MM["MM.MODE_LIST_15"] == 0x17E
#     assert (
#         tlm_MM["MM.MODE_TRANSITION_TABLE_15_15"] == valid_bc - 0x100
#     )  # FIXME: 0x17Eで動くようにする


# @pytest.mark.sils
# # @pytest.mark.real
# def test_mm_set_start_transition():

#     invalid_mode1 = 16
#     valid_mode_not_registered = 15
#     BC_SL_NOP = 17
#     BC_TL_INITIAL = 20
#     MM_NOT_DEFINED = 383

#     # ===== 存在しないモード =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_START_TRANSITION,
#         (invalid_mode1,),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "ERR"

#     # ===== 存在しないモード遷移 =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_START_TRANSITION,
#         (valid_mode_not_registered,),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "ERR"

#     # ===== 成功 =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_MODE_LIST,
#         (valid_mode_not_registered, BC_TL_INITIAL),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_TRANSITION_TABLE,
#         (0, valid_mode_not_registered, BC_SL_NOP),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_START_TRANSITION,
#         (valid_mode_not_registered,),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "SUC"
#     assert tlm_HK["HK.OBC_MM_OPSMODE"] == "RESERVE_3"
#     assert tlm_HK["HK.OBC_MM_STS"] == "PROGRESS:G"

#     time.sleep(5)
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.OBC_MM_STS"] == "FINISHED"

#     # ===== 後処理 =====
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_TRANSITION_TABLE,
#         (valid_mode_not_registered, 0, BC_SL_NOP),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_START_TRANSITION,
#         (0,),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "SUC"
#     assert tlm_HK["HK.OBC_MM_OPSMODE"] == "INITIAL"
#     assert tlm_HK["HK.OBC_MM_STS"] == "PROGRESS:G"

#     time.sleep(5)
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.OBC_MM_STS"] == "FINISHED"

#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_TRANSITION_TABLE,
#         (valid_mode_not_registered, 0, MM_NOT_DEFINED),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "SUC"
#     wings.util.send_cmd_and_confirm(
#         ope,
#         c2a_enum.Cmd_CODE_MM_SET_TRANSITION_TABLE,
#         (0, valid_mode_not_registered, MM_NOT_DEFINED),
#         c2a_enum.Tlm_CODE_HK,
#     )
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.OBC_GS_CMD_LAST_EXEC_STS"] == "SUC"


# @pytest.mark.sils
# @pytest.mark.real
# def test_mm_set_finish_transition():
#     # NOPのBCでSTART_TRANSITIONして、このコマンドを打つとSTATが0になることを見る
#     pass


# if __name__ == "__main__":
#     test_mm_set_mode_list()
#     test_mm_set_transition_table()
#     test_mm_set_start_transition()
#     test_mm_set_finish_transition()
#     pass
