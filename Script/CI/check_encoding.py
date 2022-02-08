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


def main():
    if len(sys.argv) != 2:
        print("Please give a setting file as an argumente.")
        sys.exit(1)

    setting_file_path = sys.argv[1]
    if not os.path.isfile(setting_file_path):
        print("Setting file not found.")
        sys.exit(1)

    with open(setting_file_path, mode="r") as fh:
        settings = json.load(fh)
    if DEBUG:
        pprint.pprint(settings)

    target_dirs = []
    for target_dir in settings["target_dirs"]:
        target_dirs.append(settings["root_dir"] + target_dir)

    flag = 0
    for target_dir in target_dirs:
        ret = check(target_dir, settings)
        if ret != 0:
            flag = 1

    if flag:
        print("The above files are invalid encoding.")
        sys.exit(1)
    print("Completed!")
    sys.exit(0)


# 0: OK, 1: NG
def check(target_dir, settings):
    flag = 0
    for root, dirs, files in os.walk(target_dir):
        for file in files:
            ext = (os.path.splitext(file))[1].replace(".", "")
            # print(ext)
            if ext in settings["text_file_config"]["extensions"]:
                encoding = settings["text_file_config"]["input_encoding"]
            elif ext in settings["code_file_config"]["extensions"]:
                encoding = settings["code_file_config"]["input_encoding"]
            elif ext in settings["script_file_config"]["extensions"]:
                encoding = settings["script_file_config"]["input_encoding"]
            elif ext in settings["exceptional_file_config"]["extensions"]:
                encoding = settings["exceptional_file_config"]["input_encoding"]
            else:
                continue

            path = root + r"/" + file
            ret = check_encoding(path, encoding)
            if ret != 0:
                print(path)
                flag = 1
    return flag


# 0: OK, 1: NG
def check_encoding(path, encoding):
    with open(path, "rb") as f:
        # print(path)
        ret = chardet.detect(f.read())
        enc = ret["encoding"]
    # print(enc)
    if encoding == "utf-8":
        if enc == "utf-8" or enc == "ascii":
            return 0
        # なぜか以下のような誤認もあるので
        if enc == "Windows-1252" or enc == "ISO-8859-1" or enc is None:
            return 0
    elif encoding == "shift_jis":
        if enc == "SHIFT_JIS" or enc == "CP932" or enc == "ascii":
            return 0
        # なぜか以下のような誤認もあるので
        if enc == "Windows-1252" or enc == "Windows-1254" or enc is None:
            return 0
    else:
        print("Invalid encoding in setting file!")
        return 1

    print(ret)
    return 1


if __name__ == "__main__":
    main()
