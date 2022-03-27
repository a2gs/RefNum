#!/usr/bin/env bash

# Andre Augusto Giannotti Scota (https://sites.google.com/view/a2gs/  andre.scota@gmail.com)

# Script exit if a command fails:
#set -e
#OR set -o errexit

# Script exit if a referenced variable is not declared:
#set -u
#OR set -o errexit

# If one command in a pipeline fails, its exit code will be returned as the result of the whole pipeline:
#set -o pipefail

# Activate tracing:
#set -x


function test1()
{
	echo "$1 sleep $2"
	sleep $2
	echo "$1 done!"
}


echo "GETPID: $$"
test1 "A" 5 &
PROC_A_PID=$!

echo "PROC A PID: $PROC_A_PID"






wait $PROC_A_PID

echo "ALL DONE!"