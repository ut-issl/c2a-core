#! /bin/sh

# @brief git revision 埋め込み

# current directoryに移動
cd `dirname $0`

git_rev_user=$(git log --pretty=format:%H -1)
git_rev_user_short=$(git log --pretty=format:%h -1)

cd ../../src_core

git_rev_core=$(git log --pretty=format:%H -1)
git_rev_core_short=$(git log --pretty=format:%h -1)

cd ../src_user/Script

sed -e s/git_rev_core_hash/${git_rev_core}/ -e s/git_rev_core_short_hash/${git_rev_core_short}/ -e s/git_rev_core_short_hash/${git_rev_core_short}/ -e s/git_rev_user_hash/${git_rev_user}/ -e s/git_rev_user_short_hash/${git_rev_user_short}/ git_revision.tmp > ../Library/git_revision.c
