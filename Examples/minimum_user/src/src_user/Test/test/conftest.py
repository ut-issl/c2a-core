#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import time
import pytest

ROOT_PATH = "../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()


@pytest.fixture(scope="session", autouse=True)
def increase_hk_frequency():
    _increase_hk_frequency()
    yield


def _increase_hk_frequency():

    ope.send_rt_cmd(
        c2a_enum.Cmd_CODE_TLCD_CLEAR_ALL_TIMELINE,
        (2,),
    )
    time.sleep(0.1)

    ope.send_rt_cmd(
        c2a_enum.Cmd_CODE_BCT_CLEAR_BLOCK,
        (c2a_enum.BC_HK_CYCLIC_TLM,),
    )
    time.sleep(0.1)

    for ti in range(1, 10, 2):
        ope.send_bl_cmd(
            ti,
            c2a_enum.Cmd_CODE_GENERATE_TLM,
            (0x40, c2a_enum.Tlm_CODE_HK, 1),
        )
        time.sleep(0.1)

    ope.send_bl_cmd(
        10,
        c2a_enum.Cmd_CODE_TLCD_DEPLOY_BLOCK,
        (2, c2a_enum.BC_HK_CYCLIC_TLM),
    )
    time.sleep(0.1)

    ope.send_rt_cmd(c2a_enum.Cmd_CODE_BCE_ACTIVATE_BLOCK, ())
    time.sleep(0.1)

    ope.send_rt_cmd(
        c2a_enum.Cmd_CODE_TLCD_DEPLOY_BLOCK,
        (2, c2a_enum.BC_HK_CYCLIC_TLM),
    )


if __name__ == "__main__":
    _increase_hk_frequency()
