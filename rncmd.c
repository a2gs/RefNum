/* Andre Augusto Giannotti Scota (https://sites.google.com/view/a2gs/  andre.scota@gmail.com) */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "refnum.h"

int rn_create(char *rn_name)
{
	
	return(0);
}

int rn_delete(char *rn_name)
{
	
	return(0);
}

int rn_getAndAdd(char *rn_name)
{
	
	return(0);
}

int rn_get(char *rn_name)
{
	
	return(0);
}

int rn_set(char *rn_name, RN_TYPE x)
{
	
	return(0);
}

int rn_lock(char *rn_name)
{
	
	return(0);
}

int rn_unlock(char *rn_name)
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
	if(!(argc == 3 || argc == 4)){
		printHelp(argv[0]);
		return(1);
	}

	switch(argv[1][1]){
		case 'c':
			rn_create(argv[2]);
			break;

		case 'd':
			rn_delete(argv[2]);
			break;

		case 'g':
			rn_getAndAdd(argv[2]);
			break;

		case 'G':
			rn_get(argv[2]);
			break;

		case 's':
			if(argc != 4){
				printHelp(argv[0]);
				return(1);
			}

			rn_set(argv[2], atoi(argv[3]));
			break;

		case 'l':
			rn_lock(argv[2]);
			break;

		case 'u':
			rn_unlock(argv[2]);
			break;

		default:
			printHelp(argv[0]);
			return(1);
			break;
	}

	return(0);
}
