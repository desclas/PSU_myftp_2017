/*
** EPITECH PROJECT, 2018
** myFTP
** File description:
** passive instructions file
*/

#include "serveur.h"

void request_quit(char *cmd, mdata *data)
{
	if (cmd[4] != '\r')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else
		data->quit = 1;
}

void request_dele(char *cmd, mdata *data)
{
	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[4] != ' ')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		if (remove(cmd + 5) == -1)
			write(data->csfd, "550 Folder not found.\r\n", 23);
		else
			write(data->csfd,
			"250 Requested file action okay, completed.\r\n", 44);
	}
}

void request_pwd(char *cmd, mdata *data)
{
	char *tmp, *str;

	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[3] != '\r')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		if (data->dfd == -1)
			write(data->csfd,
				"503 Bad sequence of commands.\r\n", 31);
		else if ((tmp = getcwd(NULL, 0)) == NULL)
			write(data->csfd, "550 Folder not found.\r\t", 23);
		else {
			asprintf(&str, "257 \"%s\" created\r\n", tmp);
			write(data->csfd, str, strlen(str));
			free(str);
			free(tmp);
		}
	}
}

void request_pasv_seq(mdata *data)
{
	sockaddr_in sin = { 0 };
	sockaddr_in csin = { 0 };
	unsigned int sinsize = sizeof(csin);
	char *tmp;

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(data->port + 1);
	if (bind(data->tfd, (sockaddr *) &sin, sizeof(sin)) == -1 ||
		listen(data->tfd, 5) == -1)
		write(data->csfd, "550 Enable passive or active mod.\r\n", 35);
	else {
		asprintf(&tmp,
			"227 Entering Passive Mode (127,0,0,1,%d,%d).\r\n",
			(data->port + 1) / 256, (data->port + 1) % 256);
		write(data->csfd, tmp, strlen(tmp));
		free(tmp);
		data->dfd = accept(data->tfd, (sockaddr *) &csin, &sinsize);
	}
}

void request_pasv(char *cmd, mdata *data)
{
	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[4] != '\r')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		close_it(data);
		if ((data->tfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			write(data->csfd, "550 Enable passive "
				"or active mod.\r\n", 35);
		else
			request_pasv_seq(data);
	}
}
