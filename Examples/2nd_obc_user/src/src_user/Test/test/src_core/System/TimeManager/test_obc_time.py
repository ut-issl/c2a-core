#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys

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
def test_obct_nop():
    pass


if __name__ == "__main__":
    pass
    # TODO: コマンドはないが内部のカウントアップ機能を試験してもよいかも
