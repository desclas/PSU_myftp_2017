/*
** EPITECH PROJECT, 2018
** myFTP
** File description:
** server
*/

#include "serveur.h"

void sig_handler(int sig)
{
	if (sig == SIGINT)
		can_quit(1);
}

void client_loop(int port, int sfd)
{
	sockaddr_in csin = { 0 };
	unsigned int sinsize = sizeof(csin);
	pid_t pid;
	int csfd;

	while (can_quit(0) == 0) {
		pid = -1;
		csfd = accept(sfd, (sockaddr *) &csin, &sinsize);
		error_quit(csfd, NULL);
		while ((pid = fork()) == -1);
		if (pid == 0)
			go_child( csfd, port);
		print_conection(csin);
	}
}

int main(int av, char **ac)
{
	int sfd, port;
	sockaddr_in sin = { 0 };

	if (av != 3)
		return (84);
	error_quit(is_num(ac[1]), NULL);
	port = atoi(ac[1]);
	error_quit(chdir(ac[2]), NULL);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	error_quit(sfd, NULL);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	error_quit(bind(sfd, (sockaddr *) &sin, sizeof(sin)), NULL);
	error_quit(listen(sfd, 5), NULL);
	signal(SIGINT, sig_handler);
	client_loop(port, sfd);
	close(sfd);
}