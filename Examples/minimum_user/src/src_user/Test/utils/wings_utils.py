#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import json
import os
import isslwings as wings


def get_wings_operation():
    authorization = dict(
        client_id=os.environ.get("WINGS_CLIENT_ID"),
        client_secret=os.environ.get("WINGS_CLIENT_SECRET"),
        grant_type=os.environ.get("WINGS_GRANT_TYPE"),
        username=os.environ.get("WINGS_USERNAME"),
        password=os.environ.get("WINGS_PASSWORD"),
    )

    # 環境変数があればそちらを優先
    if None in authorization.values():
        with open(os.path.dirname(__file__) + "/../authorization.json") as f:
            authorization = json.load(f)
    return wings.Operation(authentication_info=authorization)
