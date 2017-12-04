#!/bin/bash
if [ "$1" == "" ]; then
git pull
echo "Please add a commit message"
else
git pull
git add .
git commit -m "$1"
git push
fi

