#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import json
import os
import isslwings as wings

def get_wings_operation():
    with open(os.path.dirname(__file__) + "/../authorization.json") as f:
        authorization = json.load(f)
    return wings.Operation(authentication_info=authorization)
