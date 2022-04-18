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

#include "refnum.h"

int rncmd_create(char *rn_name)
{
	rn_ctx_t  rn_ctx = {0};
	rn_erro_t err    = {0};
	
	if(rn_setup(rn_name, &rn_ctx, &err) == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_create;
	if(rn_start(&rn_ctx, &err)          == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_create;
	if(rn_end(&rn_ctx, &err)            == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_create;

	return(0);

RN_NICE_ERROR_RETURN_rncmd_create:
	printf("ERRO: [%d]:[%s]", err.err, err.msg);
	return(-1);
}

int rncmd_delete(char *rn_name)
{
	rn_ctx_t  rn_ctx = {0};
	rn_erro_t err    = {0};
	
	if(rn_setup(rn_name, &rn_ctx, &err) == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_delete;
	if(rn_delete(&rn_ctx, &err)         == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_delete;

	return(0);

RN_NICE_ERROR_RETURN_rncmd_delete:
	printf("ERRO: [%d]:[%s]", err.err, err.msg);
	return(-1);
}

int rncmd_getAndAdd(char *rn_name, RN_TYPE *rn)
{
	rn_ctx_t  rn_ctx = {0};
	rn_erro_t err    = {0};
	
	if(rn_setup(rn_name, &rn_ctx, &err) == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_getAndAdd;
	if(rn_start(&rn_ctx, &err)          == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_getAndAdd;
	if(rn_addAndGet(&rn_ctx, &err, rn)  == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_getAndAdd;
	if(rn_end(&rn_ctx, &err)            == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_getAndAdd;

	return(0);

RN_NICE_ERROR_RETURN_rncmd_getAndAdd:
	printf("ERRO: [%d]:[%s]", err.err, err.msg);
	return(-1);
}

int rncmd_get(char *rn_name, RN_TYPE *rn)
{
	rn_ctx_t  rn_ctx = {0};
	rn_erro_t err    = {0};
	
	if(rn_setup(rn_name, &rn_ctx, &err) == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_get;
	if(rn_start(&rn_ctx, &err)          == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_get;
	if(rn_get(&rn_ctx, &err, rn)        == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_get;
	if(rn_end(&rn_ctx, &err)            == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_get;

	return(0);

RN_NICE_ERROR_RETURN_rncmd_get:
	printf("ERRO: [%d]:[%s]", err.err, err.msg);
	return(-1);
}

int rncmd_set(char *rn_name, RN_TYPE *rn)
{
	rn_ctx_t  rn_ctx = {0};
	rn_erro_t err    = {0};
	
	if(rn_setup(rn_name, &rn_ctx, &err) == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_set;
	if(rn_start(&rn_ctx, &err)          == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_set;
	if(rn_set(&rn_ctx, &err, rn)        == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_set;
	if(rn_end(&rn_ctx, &err)            == RN_ERRO) goto RN_NICE_ERROR_RETURN_rncmd_set;

	return(0);

RN_NICE_ERROR_RETURN_rncmd_set:
	printf("ERRO: [%d]:[%s]", err.err, err.msg);
	return(-1);
}

int rncmd_lock(char *rn_name)
{
	rn_ctx_t  rn_ctx = {0};
	rn_erro_t err    = {0};

	if(rn_lock(&rn_ctx, &err) == RN_ERRO){
		printf("ERRO: [%d]:[%s]", err.err, err.msg);
		return(-1);
	}

	return(0);
}

int rncmd_unlock(char *rn_name)
{
	rn_ctx_t  rn_ctx = {0};
	rn_erro_t err    = {0};

	if(rn_unlock(&rn_ctx, &err) == RN_ERRO){
		printf("ERRO: [%d]:[%s]", err.err, err.msg);
		return(-1);
	}

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
	RN_TYPE x = 0;

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
			if(rncmd_getAndAdd(argv[2], &x) == -1)
				return(1);

			printf("Get and Add: [%lld]\n", x);
			break;

		case 'G':
			if(rncmd_get(argv[2], &x) == -1)
				return(1);

			printf("Get: [%lld]\n", x);
			break;

		case 's':
			if(argc != 4){
				printHelp(argv[0]);
				return(1);
			}

			x = (RN_TYPE) atoi(argv[3]);
			if(rncmd_set(argv[2], &x) == -1)
				return(1);

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