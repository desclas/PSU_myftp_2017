/*
** EPITECH PROJECT, 2018
** myFTP
** File description:
** user instruction file
*/

#include "serveur.h"

int get_instruction(mdata *data, FILE *fd)
{
	size_t size = 0;
	size_t brk = 0;

	while (brk == 0) {
		if (getline(&data->buff, &size, fd) == -1)
			return (-1);
		if (data->buff == NULL)
			return (-1);
		append_msstr(data);
		if (data->str[strlen(data->str) - 2] == '\r')
			brk = 1;
	}
	return (0);
}

void request_user(char *cmd, mdata *data)
{
	size_t res = 0;

	if (cmd[4] != ' ' && cmd[4] != '\r') {
		write(data->csfd, "500 Comand not found.\r\n", 23);
		res = 1;
	}
	if (res == 0 && data->loged == 1)
			write(data->csfd,
				"503 Bad sequence of commands.\r\n", 31);
	else if (res == 0 && data->loged == 2)
		write(data->csfd, "230 User logged in, proceed.\r\n", 30);
	else {
		if (res == 0 && strncmp(cmd + 5, "Anonymous\r\n", 11) == 0) {
			data->loged = 1;
			write(data->csfd,
				"331 User name okay, need password.\r\n", 36);
		} else if (res == 0)
			write(data->csfd, "501 Bad argument.\r\n", 19);
	}
}

void request_pass(char *cmd, mdata *data)
{
	size_t res = 0;

	if (cmd[4] != ' ' && cmd[4] != '\r') {
		write(data->csfd, "500 Comand not found.\r\n", 23);
		res = 1;
	}
	if (res == 0 && data->loged == 0)
		write(data->csfd, "332 Need account for login.\r\n", 29);
	else if (res == 0 && data->loged == 2)
		write(data->csfd, "230 User logged in, proceed.\r\n", 30);
	else {
		if (res == 0 && (cmd[4] == '\r' || cmd[5] == '\r')) {
			data->loged = 2;
			write(data->csfd,
				"230 User logged in, proceed.\r\n", 30);
		} else if (res == 0)
			write(data->csfd, "501 Bad argument.\r\n", 19);
	}
}

void request_cwd(char *cmd, mdata *data)
{
	char *tmp;

	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[3] != ' ')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		tmp = calloc(strlen(cmd), sizeof(char));
		strcpy(tmp, cmd + 4);
		tmp[strlen(tmp) - 2] = '\0';
		if (chdir(tmp) == -1)
			write(data->csfd, "550 Folder not found.\r\n", 23);
		else
			write(data->csfd,
			"250 Requested file action okay, completed.\r\n", 44);
		free(tmp);
	}
}

void request_coup(char *cmd, mdata *data)
{
	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[4] != '\r')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		if (chdir(data->dir) == -1)
			write(data->csfd, "550 Folder not found.\r\n", 23);
		else
			write(data->csfd, "200 Command okay.\r\n", 19);
	}
}
