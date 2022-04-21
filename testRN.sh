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

RNNAME='RNSAMPLETEST'

function rncmd_run()
{
# $1 - name
# $2 - times
# $3 - sleep

	for (( i=1; i<$2; i++ ))
	do
		echo "[$1] get add: "
		$RNCMD_CMD -g "$RNNAME"
		sleep $3
	done
}

RFSRVPORT=46123
RFSRVADDRESS='localhost'

function rmcmd_runcli()
{
# $1 - name
# $2 - times
# $3 - sleep

	for (( i=1; i<$2; i++ ))
	do
		echo "[$1] get add: "
		printf "GETADD" | $NETCAT_CMD "$RFSRVADDRESS" "$RFSRVPORT"
		sleep $3
	done
}

function rmcmd_runsrv()
{
	rnsrv "$RNNAME" "$RFSRVPORT" 1 0
}

# ----------------------------------------------

RNCMD_CMD='rncmd'

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

NETCAT_CMD='nc'

command -v "$NETCAT_CMD" >/dev/null 2>&1
NC_EXIST=$?

if [ "$NC_EXIST" != 0 ];
then
	echo "There is no netcat. Unable to test RefNum TCP server."
fi

# ----------------------------------------------

rncmd_run "A" 1 20 &
PROC_A_PID=$!
echo "PROC A PID: $PROC_A_PID"

rncmd_run "B" 5 4 &
PROC_B_PID=$!
echo "PROC B PID: $PROC_B_PID"

if [ "$NC_EXIST" == 0 ];
then
	echo "Running server."
	rmcmd_runsrv &

	echo "Running client."
	rmcmd_runcli "CLI A" 4 5 &
	PROC_CLI_A_PID=$!
	echo "PROC CLI A PID: $PROC_CLI_A_PID"
fi 

wait $PROC_A_PID
wait $PROC_B_PID
wait $PROC_CLI_A_PID

echo "ALL DONE!"
