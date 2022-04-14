/* Andre Augusto Giannotti Scota (https://sites.google.com/view/a2gs/) */

#include <semaphore.h>

#define RN_OK		(0)
#define RN_ERRO		(1)

#define RN_TYPE unsigned long long

typedef struct{
	#define RN_ERRO_MSG_SZ		(300)
	char msg[RN_ERRO_MSG_SZ + 1];
	int err;
}rn_erro_t;

typedef struct{
	#define RN_NAME_SZ		(80)
	char name[RN_NAME_SZ + 1];
	sem_t *sem;
	int fdmem;
	RN_TYPE *mem;
}rn_ctx_t;

int rn_setup(char *rn_name, rn_ctx_t *rn_ctx, rn_erro_t *err);
int rn_start(rn_ctx_t *rn_ctx, rn_erro_t *err); /* open or create */
int rn_end(rn_ctx_t *rn_ctx, rn_erro_t *err);
int rn_delete(rn_ctx_t *rn_ctx, rn_erro_t *err);
int rn_addAndGet(rn_ctx_t *rn_ctx, rn_erro_t *err, RN_TYPE *rn);
int rn_get(rn_ctx_t *rn_ctx, rn_erro_t *err, RN_TYPE *rn);
int rn_set(rn_ctx_t *rn_ctx, rn_erro_t *err, RN_TYPE *rn);
int rn_lock(rn_ctx_t *rn_ctx, rn_erro_t *err);
int rn_unlock(rn_ctx_t *rn_ctx, rn_erro_t *err);