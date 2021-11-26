#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import os

import c2aenum as c2a


def get_c2a_enum():
    c2a_abs_path = os.environ.get("C2A_ABS_PATH")

    # このifは環境変数が設定されているならsetting.jsonを無視したいという意図
    if c2a_abs_path == None:
        with open(
            os.path.dirname(__file__).replace("\\", "/") + "/../settings.json"
        ) as f:
            json_dict = json.load(f)
        c2a_abs_path = (
            os.path.dirname(__file__).replace("\\", "/")
            + "/../"
            + json_dict["c2a_rel_path"]
        )

    return c2a.load_enum(c2a_abs_path)
