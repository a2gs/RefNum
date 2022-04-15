/* Andre Augusto Giannotti Scota (a2gs)                              
 * andre.scota@gmail.com
 *
 * RefNum - A shared reference number counter
 *
 * MIT License
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "refnum.h"

int rncmd_create(char *rn_name)
{
	
	return(0);
}

int rncmd_delete(char *rn_name)
{
	
	return(0);
}

int rncmd_getAndAdd(char *rn_name)
{
	
	return(0);
}

int rncmd_get(char *rn_name)
{
	
	return(0);
}

int rncmd_set(char *rn_name, RN_TYPE x)
{
	
	return(0);
}

int rncmd_lock(char *rn_name)
{
	
	return(0);
}

int rncmd_unlock(char *rn_name)
{
	
	return(0);
}


void printHelp(char *exec)
{
	printf("Usage:\n");
	printf("%s [-c][-d][-g][-G][-s X][-l][-u] \"REFNUM NAME\"\n", exec);
	printf("-c\t\tCreate\n");
	printf("-d\t\tDelete\n");
	printf("-g\t\tGet and Add\n");
	printf("-G\t\tGet\n");
	printf("-s\t\tSet\n");
	printf("-l\t\tLock\n");
	printf("-u\t\tUnlock\n");

	return;
}

int main(int argc, char *argv[])
{
	char opt = 0;

	opt = argv[1][1];

	if(argc != 3 && argc != 4){
		printHelp(argv[0]);
		return(1);
	}

	switch(opt){
		case 'c':
			rncmd_create(argv[2]);
			break;

		case 'd':
			rncmd_delete(argv[2]);
			break;

		case 'g':
			rncmd_getAndAdd(argv[2]);
			break;

		case 'G':
			rncmd_get(argv[2]);
			break;

		case 's':
			if(argc != 4){
				printHelp(argv[0]);
				return(1);
			}

			rncmd_set(argv[2], atoi(argv[3]));
			break;

		case 'l':
			rncmd_lock(argv[2]);
			break;

		case 'u':
			rncmd_unlock(argv[2]);
			break;

		default:
			printHelp(argv[0]);
			return(1);
			break;
	}

	return(0);
}