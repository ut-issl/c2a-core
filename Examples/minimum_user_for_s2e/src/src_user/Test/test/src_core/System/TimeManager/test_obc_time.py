#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import time

import isslwings as wings
import pytest

ROOT_PATH = "../../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings.Operation()


if __name__ == "__main__":
    pass
    # TODO: コマンドはないが内部のカウントアップ機能を試験してもよいかも
