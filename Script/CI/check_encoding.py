# coding: UTF-8
"""
python 3.7以上を要求

必要ライブラリ
chardet
"""
import chardet
import pprint
import sys
import os.path
import json

# 環境変数
DEBUG = 0
# 0 : Release
# 1 : all
SETTING_FILE_PATH = "check_encoding.json"


def main():
    with open(SETTING_FILE_PATH, mode='r') as fh:
        settings = json.load(fh)
    if DEBUG:
        pprint.pprint(settings)

    target_dirs = []
    for target_dir in settings['target_dirs']:
        target_dirs.append(settings['root_dir'] + target_dir)

    flag = 0
    for target_dir in target_dirs:
        ret = check(target_dir)
        if ret != 0:
            flag = 1

    if flag:
        print("The above files are invalid encoding.")
        sys.exit(1)
    print("Completed!")
    sys.exit(0)


# 0: OK, 1: NG
def check(target_dir):
    flag = 0
    for root, dirs, files in os.walk(target_dir):
        for file in files:
            ext = (os.path.splitext(file))[1]
            # print(ext)
            # FIXME: md, txt, cmake, sh, bat もチェックするようにする
            if ext != ".h" and ext != ".c":
                continue
            path = root + r"/" + file
            ret = check_encoding(path)
            if ret != 0:
                print(path)
                flag = 1
    return flag


# 0: OK, 1: NG
def check_encoding(path):
    with open(path, 'rb') as f:
        # print(path)
        ret = chardet.detect(f.read())
        enc = ret['encoding']
    # print(enc)
    # FIXME: ユーザー定義にする
    # SJIS Ver
    # if enc == 'SHIFT_JIS' or enc == 'CP932' or enc == 'ascii':
    #     return 0
    # # なぜか以下のような誤認もあるので
    # if enc == 'Windows-1252' or enc == 'Windows-1254' or enc is None:
    #     return 0
    # UTF-8 Ver
    if enc == 'utf-8' or enc == 'ascii':
        return 0
    # なぜか以下のような誤認もあるので
    if enc == 'Windows-1252' or enc == 'ISO-8859-1' or enc is None:
        return 0
    print(ret)
    return 1


if __name__ == '__main__':
    main()

