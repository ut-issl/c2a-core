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


# 現在 AL は deprecated (後継は EL) なので，一旦コメントアウト


# 他をコメントアウトしてると， pytest がコケるので
@pytest.mark.sils
@pytest.mark.real
def test_al_nop():
    pass


# @pytest.mark.sils
# @pytest.mark.real
# def test_al_add_anomaly():
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     al_counter = tlm_HK["HK.ANOMALY_MOBC_AL_COUNTER"]

#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_ADD_ANOMALY, (100, 1), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.ANOMALY_MOBC_AL_COUNTER"] == al_counter + 1
#     assert tlm_HK["HK.ANOMALY_MOBC_AL_LATEST_CODE_GROUP"] == 100
#     assert tlm_HK["HK.ANOMALY_MOBC_AL_LATEST_CODE_LOCAL"] == 1


# @pytest.mark.sils
# @pytest.mark.real
# def test_al_clear_list():
#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_ADD_ANOMALY, (100, 1), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.ANOMALY_MOBC_AL_COUNTER"] != 0
#     assert tlm_HK["HK.ANOMALY_MOBC_AL_LATEST_CODE_GROUP"] == 100
#     assert tlm_HK["HK.ANOMALY_MOBC_AL_LATEST_CODE_LOCAL"] == 1

#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_CLEAR_LIST, (), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_HK = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_HK
#     )
#     assert tlm_HK["HK.ANOMALY_MOBC_AL_COUNTER"] == 0
#     assert tlm_HK["HK.ANOMALY_MOBC_AL_LATEST_CODE_GROUP"] == 0
#     assert tlm_HK["HK.ANOMALY_MOBC_AL_LATEST_CODE_LOCAL"] == 0


# @pytest.mark.sils
# @pytest.mark.real
# def test_al_set_page_for_tlm():
#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_CLEAR_LIST, (), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_AL = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_AL
#     )
#     assert tlm_AL["AL.COUNTER_AL"] == 0
#     assert tlm_AL["AL.PAGE_NO"] == 0
#     assert tlm_AL["AL.RECORD0_CODE_GROUP"] == 0
#     assert tlm_AL["AL.RECORD0_CODE_LOCAL"] == 0

#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_SET_PAGE_FOR_TLM, (1,), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_AL = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_AL
#     )
#     assert tlm_AL["AL.COUNTER_AL"] == 0
#     assert tlm_AL["AL.PAGE_NO"] == 1
#     assert tlm_AL["AL.RECORD0_CODE_GROUP"] == 0
#     assert tlm_AL["AL.RECORD0_CODE_LOCAL"] == 0

#     for i in range(33):
#         ope.send_rt_cmd(c2a_enum.Cmd_CODE_AL_ADD_ANOMALY, (100 + i, 0))
#         time.sleep(0.5)

#     tlm_AL = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_AL
#     )
#     assert tlm_AL["AL.COUNTER_AL"] == 33
#     assert tlm_AL["AL.PAGE_NO"] == 1
#     assert tlm_AL["AL.RECORD0_CODE_GROUP"] == 132
#     assert tlm_AL["AL.RECORD0_CODE_LOCAL"] == 0


# @pytest.mark.sils
# @pytest.mark.real
# def test_al_enable_disable_logging():
#     tlm_AL = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_AL
#     )
#     assert tlm_AL["AL.IS_LOGGING_ENABLE0"] == 255

#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_DISABLE_LOGGING, (0,), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_AL = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_AL
#     )
#     assert tlm_AL["AL.IS_LOGGING_ENABLE0"] == 127  # 0b0111111

#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_ENABLE_LOGGING, (0,), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_AL = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_AL
#     )
#     assert tlm_AL["AL.IS_LOGGING_ENABLE0"] == 255  # 0b11111111


# @pytest.mark.sils
# @pytest.mark.real
# def test_al_init_logging_ena_flag():
#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_DISABLE_LOGGING, (0,), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_AL = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_AL
#     )
#     assert tlm_AL["AL.IS_LOGGING_ENABLE0"] == 127  # 0b0111111


# @pytest.mark.sils
# @pytest.mark.real
# def test_al_set_thres_of_nearly_full():
#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_SET_THRES_OF_NEARLY_FULL, (100,), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_AL = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_AL
#     )
#     assert tlm_AL["AL.THRES_OF_NEARLY_FULL"] == 100

#     wings.util.send_cmd_and_confirm(
#         ope, c2a_enum.Cmd_CODE_AL_INIT_LOGGING_ENA_FLAG, (), c2a_enum.Tlm_CODE_HK
#     )
#     tlm_AL = wings.util.generate_and_receive_tlm(
#         ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_AL
#     )
#     assert tlm_AL["AL.IS_LOGGING_ENABLE0"] == 255  # 0b11111111


if __name__ == "__main__":
    pass
