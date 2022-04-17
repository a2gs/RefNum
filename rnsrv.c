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

void printHelp(char *exec)
{
	printf("Usage:\n");
	printf("%s \n", exec);

	return;
}

#define RUNNING_DIR  "./"
#define LOCK_FILE    "servlock"

static int lockFd;
/*static FILE *log;*/

int unlockAndDeleteLockFile(void)
{
	if(lockf(lockFd, F_ULOCK, 0) < 0){
		fprintf(stderr, "Cannt unlock 'only one instance' file.\n");
		return(-1);
	}

	close(lockFd);
	if(unlink(LOCK_FILE) != 0){
		fprintf(stderr, "Erro deleting lock file [%s].\n", LOCK_FILE);
		return(-1);
	}

	return(0);
}

void signal_handler(int sig)
{
	/* Qualquer sinal recebido ira terminar o server */

	fprintf(stderr, "Got signal [%d]!\n", sig);

	switch(sig){
		case SIGHUP:
			break;
		case SIGTERM:
			break;
	}

	unlockAndDeleteLockFile();

	/* Termina servidor */
	exit(0);

	return;
}

int daemonize(void)
{
	pid_t i = 0, father = 0;
	char str[10];

	father = getppid();
	if(father == 1)
		return(-1);

	i = fork();

	if(i == -1)
		return(-1);

	if(i > 0)
		exit(1); /* Pai termina */

	/* Continuando filho... */
	setsid(); /* novo grupo */

	umask(027);

	chdir(RUNNING_DIR);

	/* Criando e travando arquivo de execucao unica */
	lockFd = open(LOCK_FILE, O_RDWR|O_CREAT|O_EXCL, 0640);
	if(lockFd == -1){
		fprintf(stderr, "There is another instance already running.\n");
		exit(-1);
	}

	if(lockf(lockFd, F_TLOCK, 0) < 0){
		fprintf(stderr, "Cannt test 'only one instance' file.\n");
		return(-1);
	}

	if(lockf(lockFd, F_LOCK, 0) < 0){
		fprintf(stderr, "Cannt lock 'only one instance' file.\n");
		return(-1);
	}

	/* Primeira instancia */
	sprintf(str, "%d\n", getpid());
	write(lockFd, str, strlen(str));

	/* Configurando sinais */
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGHUP, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGPIPE, signal_handler);

	return(1);
}

#define MAXLINE (100)
int main(int argc, char *argv[])
{
	pid_t p;
	int listenfd = 0, connfd = 0, readRet = 0;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;
	char addStr[255 + 1] = {0};
	char msg[MAXLINE + 1] = {0}, *endLine = NULL;
	RN_TYPE rn = 0;
	rn_ctx_t  rn_ctx = {0};
	rn_erro_t err = {0};
	char mngAllowed = 0;
	char itMode = 0;
	char *rnName = NULL;
	int rnErro = 0;

	if(argc != 5){
		fprintf(stderr, "%s <RNREF> <PORT> <MNG> <IT>\n", argv[0]);
		fprintf(stderr, "\t<RNREF> RefNum name.\n");
		fprintf(stderr, "\t<PORT> Port to listening.\n");
		fprintf(stderr, "\t<MNG> 0 or 1. Server is able to execute or not management commands (delete, set, lock and unlock).\n");
		fprintf(stderr, "\t<IT> 0 or 1. Iterative mode. Server doesn't disconect until exit cmd.\n");
		return(1);
	}

	rnName = argv[2];
	mngAllowed = (argv[3][0] == '0' ? '0' : '1');
	itMode = (argv[4][0] == '0' ? '0' : '1');

	/*
	if((log = fopen("./log.text", "wr")) == NULL){
		fprintf(stderr, "Unable to open/create log.text! [%s].", strerror(errno));
		return(1);
	}
	setbuf(log, NULL);
	*/

	fprintf(stderr, "StartUp!\n");

	if(daemonize() == -1){
		fprintf(stderr, "Cannt daemonize server.\n");
		return(1);
	}

	listenfd = socket(AF_INET, SOCK_STREAM, 0); /* For IPv6: AF_INET6 (but listen() must receives a sockaddr_in6 struct) */
	if(listenfd == -1){
		fprintf(stderr, "ERRO socket(): [%s].\n", strerror(errno));
		return(1);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET; /* The best thing to do is specify: AF_UNSPEC (IPv4 and IPv6 clients support) */
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(atoi(argv[1]));

	if(bind(listenfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) != 0){
		fprintf(stderr, "ERRO bind(): [%s].\n", strerror(errno));
		unlockAndDeleteLockFile();
		return(1);
	}

	if(listen(listenfd, 1024) != 0){
		fprintf(stderr, "ERRO listen(): [%s].\n", strerror(errno));
		unlockAndDeleteLockFile();
		return(1);
	}

	for(;;){
		len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len);
		if(connfd == -1){
			fprintf(stderr, "ERRO accept(): [%s].\n", strerror(errno));
			return(1);
		}

		fprintf(stderr, "Connection from %s, port %d\n", inet_ntop(cliaddr.sin_family, &cliaddr.sin_addr, addStr, sizeof(addStr)), ntohs(cliaddr.sin_port));
		p = fork();

		if(p == 0){ /* child */

			if(rn_setup(rnName, &rn_ctx, &err) == RN_ERRO){
				printf("ERRO rn_setup: [%d]:[%s]", err.err, err.msg);
				return(1);
			}

			if(rn_start(&rn_ctx, &err) == RN_ERRO){
				printf("ERRO rn_start: [%d]:[%s]", err.err, err.msg);
				return(1);
			}

			do{
				memset(msg, 0, MAXLINE);

				readRet = recv(connfd, msg, MAXLINE, 0);
				if(readRet == 0){
					fprintf(stderr, "End of data\n");
					break;
				}

				if(readRet < 0){
					fprintf(stderr, "ERRO recv(): [%s].\n", strerror(errno));
					break;
				}

				endLine = strrchr(msg, '\r');
				if(endLine != NULL) (*endLine) = '\0';

				fprintf(stderr, "CMD: [%s]\n", msg);

				if     ((strncmp((char *)msg, "exit"  , 4) == 0) && (mngAllowed == '1')) break;
				else if((strncmp((char *)msg, "DELETE", 6) == 0) && (mngAllowed == '1')) rnErro = rn_delete(&rn_ctx, &err);
				else if( strncmp((char *)msg, "GET"   , 3) == 0)                         rnErro = rn_get(&rn_ctx, &err, &rn);
				else if( strncmp((char *)msg, "GETADD", 6) == 0)                         rnErro = rn_addAndGet(&rn_ctx, &err, &rn);
				else if((strncmp((char *)msg, "SET"   , 3) == 0) && (mngAllowed == '1')) rnErro = rn_set(&rn_ctx, &err, &rn);
				else if((strncmp((char *)msg, "LOCK"  , 4) == 0) && (mngAllowed == '1')) rnErro = rn_lock(&rn_ctx, &err);
				else if((strncmp((char *)msg, "UNLOCK", 6) == 0) && (mngAllowed == '1')) rnErro = rn_unlock(&rn_ctx, &err);
				else                                                                     {err.err = 1; strncpy(err.msg, "BAD CMD", RN_ERRO_MSG_SZ);}

				if(rnErro == RN_ERRO) snprintf((char *)msg, MAXLINE, "ERRO: [%d]:[%s]", err.err, err.msg);
				else                  snprintf((char *)msg, MAXLINE, "%lld", rn);

				fprintf(stderr, "%s\n", msg);

				if(send(connfd, msg, strlen(msg), 0) == -1){
					printf("ERRO: send() [%s].\n", strerror(errno));
					break;
				}

			}while(itMode == '1');

			shutdown(connfd, SHUT_RDWR);

			if(rn_end(&rn_ctx, &err)  == RN_ERRO){
				printf("ERRO rn_end: [%d]:[%s]", err.err, err.msg);
				return(1);
			}

			break; /* for() */

		}else if(p == -1)
			fprintf(stderr, "ERRO fork(): [%s].\n", strerror(errno));

		close(connfd);
	}

	return(0);
}