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


@pytest.mark.sils
@pytest.mark.real
def test_cdis_exec_err():
    # ===== 以下の 2 コマンドでチェックする =====
    # それぞれを RT/TL で実行する
    # 送信するコマンドと想定されるエラーは以下
    #
    # Cmd_TMGR_UPDATE_UNIXTIME
    #   - 無効なパラメタで送信
    #   - exec_sts: CCP_EXEC_ILLEGAL_PARAMETER = 2
    #   - err_code: 0
    # Cmd_TLCD_DEPLOY_BLOCK
    #   - 無効な block を展開
    #   - exec_sts: CCP_EXEC_ILLEGAL_CONTEXT = 3
    #   - err_code: PL_BC_INACTIVE_BLOCK = 8

    idx_gs_0, idx_tl_0 = check_cdis_exec_err(
        c2a_enum.Cmd_CODE_TMGR_UPDATE_UNIXTIME, (-10, 0, 0), c2a_enum.CCP_EXEC_ILLEGAL_PARAMETER, 0
    )
    idx_gs_1, idx_tl_1 = check_cdis_exec_err(
        c2a_enum.Cmd_CODE_TLCD_DEPLOY_BLOCK,
        (0, c2a_enum.BC_ID_MAX),  # BC_ID_MAX は inactive でしょう, という前提
        c2a_enum.CCP_EXEC_ILLEGAL_CONTEXT,
        c2a_enum.PL_BC_INACTIVE_BLOCK,
    )

    # 同じ CDIS の場合は同じ idx になることを確認
    assert idx_gs_0 == idx_gs_1
    assert idx_tl_0 == idx_tl_1

    # 最後に EL 初期化
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_INIT, (), c2a_enum.Tlm_CODE_HK
    )


def check_cdis_exec_err(cmd_id, params, exec_sts_expected, err_code_expected):
    # === ELの初期化 ===
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope, c2a_enum.Cmd_CODE_EL_INIT, (), c2a_enum.Tlm_CODE_HK
    )

    # === TLCD の EL を無効化 ===
    # EL 初期化で戻ってしまうのでこの位置におく
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_EL_DISABLE_LOGGING,
        (c2a_enum.EL_CORE_GROUP_TLCD_DEPLOY_BLOCK,),
        c2a_enum.Tlm_CODE_HK,
    )

    # === RT と TL でコマンド送信 ===
    tlm_HK = ope.get_latest_tlm(c2a_enum.Tlm_CODE_HK)[0]
    ti_now = tlm_HK["HK.SH.TI"]
    assert "SUC" != wings.util.send_rt_cmd_and_confirm(ope, cmd_id, params, c2a_enum.Tlm_CODE_HK)
    wings.util.send_tl_cmd(ope, ti_now + 50, cmd_id, params)
    time.sleep(5)

    # === ELのチェック ===
    tlm_EL = wings.util.generate_and_receive_tlm(
        ope, c2a_enum.Cmd_CODE_TG_GENERATE_RT_TLM, c2a_enum.Tlm_CODE_EL
    )

    # GS_cmd_dispatcher
    assert tlm_EL["EL.TLOGS.LOW.EVENTS3.GROUP"] == c2a_enum.EL_CORE_GROUP_CDIS_EXEC_ERR_CODE
    assert tlm_EL["EL.TLOGS.LOW.EVENTS3.LOCAL"] == cmd_id
    assert tlm_EL["EL.TLOGS.LOW.EVENTS3.NOTE"] == err_code_expected
    assert tlm_EL["EL.TLOGS.LOW.EVENTS2.GROUP"] == c2a_enum.EL_CORE_GROUP_CDIS_EXEC_ERR_STS
    assert tlm_EL["EL.TLOGS.LOW.EVENTS2.LOCAL"] == cmd_id
    idx_gs, r = divmod(tlm_EL["EL.TLOGS.LOW.EVENTS2.NOTE"], 2**24)  # 上位 8bit
    exec_sts, err_code = divmod(r, 2**16)  # 次の 8bit と下位 16bit
    assert exec_sts == exec_sts_expected
    assert err_code == err_code_expected

    # TL_cmd_dispatcher
    assert tlm_EL["EL.TLOGS.LOW.EVENTS1.GROUP"] == c2a_enum.EL_CORE_GROUP_CDIS_EXEC_ERR_CODE
    assert tlm_EL["EL.TLOGS.LOW.EVENTS1.LOCAL"] == cmd_id
    assert tlm_EL["EL.TLOGS.LOW.EVENTS1.NOTE"] == err_code_expected
    assert tlm_EL["EL.TLOGS.LOW.EVENTS0.GROUP"] == c2a_enum.EL_CORE_GROUP_CDIS_EXEC_ERR_STS
    assert tlm_EL["EL.TLOGS.LOW.EVENTS0.LOCAL"] == cmd_id
    idx_tl, r = divmod(tlm_EL["EL.TLOGS.LOW.EVENTS0.NOTE"], 2**24)  # 上位 8bit
    exec_sts, err_code = divmod(r, 2**16)  # 次の 8bit と下位 16bit
    assert exec_sts == exec_sts_expected
    assert err_code == err_code_expected

    # GS と TL0 を note で区別できているか
    assert idx_gs != idx_tl

    return [idx_gs, idx_tl]


if __name__ == "__main__":
    test_cdis_exec_err()
    pass
