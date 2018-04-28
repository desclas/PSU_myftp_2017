/*
** EPITECH PROJECT, 2018
** myFTP
** File description:
** request loop file
*/

#include "serveur.h"

void request_loop(FILE *fd, int csfd, int port)
{
	mdata data;

	init_mdata(&data, csfd, port);
	while (data.quit != 1) {
		if (get_instruction(&data, fd) == -1)
			break;
		get_exec(&data);
	}
	clean_it(&data);
}

void go_child(int csfd, int port)
{
	FILE *fd;

	fd = fdopen(csfd, "r+");
	write(csfd, "220 Service ready for new user\r\n", 32);
	request_loop(fd, csfd, port);
	write(csfd, "221 Service closing control connection.\r\n", 41);
	fclose(fd);
	close(csfd);
	exit(0);
}

void print_conection(sockaddr_in csin)
{
	printf("%s:%d\n", inet_ntoa(csin.sin_addr), csin.sin_port);
}

int is_num(char *str)
{
	for (size_t i = 0; str[i] != '\0'; i++)
		if (str[i] < 48 || str[i] > 57)
			return (-1);
	return (0);
}

size_t can_quit(size_t x)
{
	static size_t quit = 0;

	if (x != 0)
		quit = x;
	return (quit);
}
