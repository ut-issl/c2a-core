import sys

file = sys.argv[1]
compiler = sys.argv[2]

with open(file) as f:
    s = f.read()
    log = s.split(compiler)
    if "" in log:
        log.remove("")
    # print(log)

    errors = []
    for l in log:  # noqa: E741
        (cmd, err) = l.split("\n", 1)
        if err in errors:
            print("duplicate: " + cmd, file=sys.stderr)
        else:
            errors.append(err)
            print(compiler + l, end="")
