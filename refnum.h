/* Andre Augusto Giannotti Scota (https://sites.google.com/view/a2gs/) */


/*

LIB:
rn_create()
rn_destroy()
rn_bind()

long long rn_addAndGet()
long long rn_get()
rn_set(long long x)
rn_lock()
rn_unlock()




Sample:

sample [-c][-d][-ga][-g][-s][-l][-u] "REFNUM NAME"

*/


#define RN_OK		(0)
#define RN_ERRO		(1)

#define RN_TPYE unsigned long long

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
	unsigned long long *mem;
}rn_ctx_t;

int rn_setup(char *rn_name, rn_ctx_t *rn_ctx, rn_erro_t *err);
int rn_start(rn_ctx_t *rn_ctx, rn_erro_t *err); /* open or create */
int rn_destroy();