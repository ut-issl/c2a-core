#!/bin/bash
# print C2A core Git revision as C header

cd $(dirname $0)    # src_core/Script/Git
#cd ../../           # src_core

git_rev_core=$(git log --pretty=format:%H -1)
git_rev_core_short=$(git log --pretty=format:%h -1)

cat << EOS
#define GIT_REVISION_C2A_CORE "${git_rev_core}"
#define GIT_REVISION_C2A_CORE_SHORT 0x${git_rev_core_short}
EOS
