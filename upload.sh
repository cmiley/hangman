#!/bin/bash
if [ "$1" == "" ]; then
echo "Please add a commit message" 
else
git pull
git add .
git commit -m "$1"
git push
fi

