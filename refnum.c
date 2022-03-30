/* Andre Augusto Giannotti Scota (a2gs)
 * andre.scota@gmail.com
 * https://sites.google.com/view/a2gs
 *
 * SCB - Shared Circular Buffer
 *
 * MIT License
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.H>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#include "refnum.h"

int rn_setup(char *rn_name, rn_ctx_t *rn_ctx, rn_erro_t *err)
{
	memset(rn_ctx, 0, sizeof(rn_ctx_t));
	memset(err, 0, sizeof(rn_erro_t));

	strncpy(rn_ctx->name, rn_name, RN_NAME_SZ);

	return(RN_OK);
}

#define __RN_SET_ERROR(__rn_error_message__) {                                                                         \
	char msgErr[RN_ERRO_MSG_SZ + 1] = {0};                                                                             \
                                                                                                                       \
	err->err = errno;                                                                                                  \
	strerror_r(err->err, msgErr, RN_ERRO_MSG_SZ);                                                                      \
	snprintf(err->msg, RN_ERRO_MSG_SZ, "RefNum#%d [%d] [%s]: [%s]", __LINE__, err->err, __rn_error_message__, msgErr); \
}

int rn_start(rn_ctx_t *rn_ctx, rn_erro_t *err)
{
	/* creating semaphore */
	rn_ctx->sem = sem_open(rn_ctx->name, O_CREAT|O_EXCL, O_RDWR, 0);
	if(rn_ctx->sem == SEM_FAILED){
		if(err != NULL)
			__RN_SET_ERROR("sem_open");

		rn_destroy(rn_ctx, NULL);
		return(RN_ERRO);
	}

	/* creating long long shared memory */
	rn_ctx->fdmem = shm_open(rn_ctx->name, O_CREAT|O_EXCL, O_RDWR);
	if(rn_ctx->fdmem == -1){
		if(err != NULL)
			__RN_SET_ERROR("shm_open");

		rn_destroy(rn_ctx, NULL);
		return(RN_ERRO);
	}

	if(ftruncate(rn_ctx->fdmem, sizeof(RN_TYPE)) == -1){
		if(err != NULL)
			__RN_SET_ERROR("ftruncate");

		rn_destroy(rn_ctx, NULL);
		return(RN_ERRO);
	}

	rn_ctx->mem = (RN_TYPE *)mmap(NULL, sizeof(unsigned long long), PROT_READ | PROT_WRITE, MAP_SHARED, rn_ctx->mem, 0);
	if(rn_ctx->mem == MAP_FAILED){
		if(err != NULL)
			__RN_SET_ERROR("mmap");

		rn_destroy(rn_ctx, NULL);

		return(RN_ERRO);
	}

	close(rn_ctx->fdmem);

	return(RN_OK);
}

int rn_end(rn_ctx_t *rn_ctx, rn_erro_t *err)
{
	sem_close(rn_ctx->sem);
	close(rn_ctx->fdmem);

	return(RN_OK);
}

int rn_delete(rn_ctx_t *rn_ctx, rn_erro_t *err)
{
	rn_end(rn_ctx, NULL);

	sem_unlink(rn_ctx->sem);
	shm_unlink(rn_ctx->name);

	return(RN_OK);
}

int rn_addAndGet(rn_ctx_t *rn_ctx, rn_erro_t *err, RN_TYPE *rn)
{
	if(rn_lock(rn_ctx, err) == RN_ERRO)
		return(RN_ERRO);

	*rn = rn_ctx->mem++;

	rn_lock(rn_ctx, err) == RN_ERRO)
		return(RN_ERRO);

	return(RN_OK);
}

int rn_get(rn_ctx_t *rn_ctx, rn_erro_t *err, RN_TYPE *rn)
{
	*rn = *rn_ctx->mem;

	return(RN_OK);
}

int rn_set(rn_ctx_t *rn_ctx, rn_erro_t *err, RN_TYPE *rn)
{
	if(rn_lock(rn_ctx, err) == RN_ERRO)
		return(RN_ERRO);

	*rn_ctx->mem = *rn;

	if(rn_lock(rn_ctx, err) == RN_ERRO)
		return(RN_ERRO);

	return(RN_OK);
}

int rn_lock(rn_ctx_t *rn_ctx, rn_erro_t *err)
{
	if(sem_wait(rn_ctx->sem)) == -1){

		if(err != NULL)
			__RN_SET_ERROR("sem_wait");

		return(RN_ERRO);
	}

	return(RN_OK);
}

int rn_unlock(rn_ctx_t *rn_ctx, rn_erro_t *err)
{
	if(sem_post(rn_ctx->sem)) == -1){

		if(err != NULL)
			__RN_SET_ERROR("sem_post");

		return(RN_ERRO);
	}

	return(RN_OK);
}