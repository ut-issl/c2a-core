#!/bin/bash

function make_link () {
  if [ -e $1 ]; then
    rm -rf $1
  fi
  ln -s $2 $1
}


echo setup.sh
echo setting for local environment...
echo

echo [SET USER HOOKS]
git config --local core.hooksPath Script/GitHooks
echo done.
echo

echo [SET UP DEVELOP ENVIRONMENT FOR C2A MINIMUM USER SAMPLE]
make_link './Examples/minimum_user/src/src_core' '../../../'
echo done.
echo

echo [SET UP DEVELOP ENVIRONMENT FOR 2ND OBC USER SAMPLE]
make_link './Examples/minimum_user/src/src_core' '../../../'
echo done.
echo

echo The process has been completed.
