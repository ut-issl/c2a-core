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
def test_event_utility():
    tlm_EH = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EH
    )
    assert tlm_EH["EH.EVENT_UTIL.IS_ENABLED_EH_EXECUTION"] == "ENABLE"

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EVENT_UTIL_DISABLE_EH_EXEC, (), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EH
    )
    assert tlm_EH["EH.EVENT_UTIL.IS_ENABLED_EH_EXECUTION"] == "DISABLE"

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EVENT_UTIL_ENABLE_EH_EXEC, (), c2a_enum.Tlm_CODE_HK
    )
    tlm_EH = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_GENERATE_TLM, c2a_enum.Tlm_CODE_EH
    )
    assert tlm_EH["EH.EVENT_UTIL.IS_ENABLED_EH_EXECUTION"] == "ENABLE"

    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EVENT_UTIL_EXEC_EH, (), c2a_enum.Tlm_CODE_HK
    )


if __name__ == "__main__":
    pass
