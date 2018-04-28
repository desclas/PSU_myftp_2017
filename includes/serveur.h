/*
** EPITECH PROJECT, 2018
** myFTP
** File description:
** serveur header file
*/

#ifndef SERVEUR_H_
# define SERVEUR_H_

# define _GNU_SOURCE
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <netdb.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <netdb.h>
# include <stdio.h>
# include <sys/wait.h>
# include <signal.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

typedef struct mdata {
	int csfd;
	int loged;
	char *dir;
	void (**func)(char *, struct mdata *);
	int dfd;
	char *str;
	char *buff;
	size_t quit;
	int port;
	int tfd;
	char **htab;
} mdata;

void close_it(mdata *data);
void clean_it(mdata *data);
void error_quit(int val, mdata *data);
void init_htab(char **res);
void append_msstr(mdata *data);
int get_instruction(mdata *data, FILE *fd);
void request_user(char *cmd, mdata *data);
void request_pass(char *cmd, mdata *data);
void request_cwd(char *cmd, mdata *data);
void request_coup(char *cmd, mdata *data);
void request_quit(char *cmd, mdata *data);
void request_dele(char *cmd, mdata *data);
void request_pwd(char *cmd, mdata *data);
void request_pasv_seq(mdata *data);
void request_pasv(char *cmd, mdata *data);
int check_addrport(char *cmd);
void request_port_seq(char *cmd, mdata *data);
void request_port(char *cmd, mdata *data);
void request_help_seq(char *cmd, mdata *data);
void request_help(char *cmd, mdata *data);
void request_noop(char *cmd, mdata *data);
void request_retr_seq(char *cmd, mdata *data);
void request_retr(char *cmd, mdata *data);
void request_stor_seq(char *cmd, mdata *data);
void request_stor(char *cmd, mdata *data);
void request_list_seq(char *cmd, mdata *data);
void request_list(char *cmd, mdata *data);
void get_exec(mdata *data);
void (**init_ptr_func(void))(char *, mdata *);
void init_mdata(mdata *data, int csfd, int port);
void request_loop(FILE *fd, int csfd, int port);
void go_child(int csfd, int port);
void print_conection(sockaddr_in csin);
int is_num(char *str);
size_t can_quit(size_t x);

#endif /* !SERVEUR_H_ */
