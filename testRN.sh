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

# ----------------------------------------------

RNCMD_CMD="rncmd"

if [ ! -e "$RNCMD_CMD" ];
then
	command -v "$RNCMD_CMD" >/dev/null 2>&1
	RNCMD_CMD=$?

	if [ "$RNCMD_CMD" != 0 ];
	then
		echo "There is no RefeNum command line. Aborting."
		exit 1
	fi
fi

# ----------------------------------------------

NETCAT_CMD="nc"

command -v "$NETCAT_CMD" >/dev/null 2>&1
NC_EXIST=$?

if [ "$NC_EXIST" != 0 ];
then
	echo "There is no netcat. Unable to test RefNum TCP server."
fi

# ----------------------------------------------

echo "GETPID: $$"
test1 "A" 7 &
PROC_A_PID=$!
echo "PROC A PID: $PROC_A_PID"


echo "GETPID: $$"
test1 "B" 3 &
PROC_B_PID=$!
echo "PROC B PID: $PROC_B_PID"

echo "GETPID: $$"
test1 "C" 5 &
PROC_C_PID=$!
echo "PROC C PID: $PROC_C_PID"

echo "GETPID: $$"
test1 "D" 1 &
PROC_D_PID=$!
echo "PROC D PID: $PROC_D_PID"


wait $PROC_A_PID
wait $PROC_B_PID
wait $PROC_C_PID
wait $PROC_D_PID


echo "ALL DONE!"
