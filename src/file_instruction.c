/*
** EPITECH PROJECT, 2018
** myFTP
** File description:
** file instruction file
*/

#include "serveur.h"

void request_noop(char *cmd, mdata *data)
{
	if (cmd[4] != ' ')
		write(data->csfd, "500 Comand not found.\r\n", 23);
}

void request_retr_seq(char *cmd, mdata *data)
{
	pid_t pid;
	int st;

	cmd[strlen(cmd) - 2] = '\0';
	if (access(cmd, F_OK | R_OK) == -1)
		write(data->csfd,
			"550 Folder not avalible or not found.\r\n", 39);
	else {
		dprintf(data->csfd, "150 File status okay; "
			"about to open data connection.\r\n");
		while ((pid = fork()) == -1);
		if (pid == 0) {
			dup2(data->dfd, 1);
			execl("/bin/cat", "/bin/cat", tmp, NULL);
		}
		wait(&st);
		close_it(data);
		dprintf(data->csfd, "226 Closing data connection.\r\n");
	}
}

void request_retr(char *cmd, mdata *data)
{
	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[4] != ' ')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		if (data->dfd == -1)
			write(data->csfd,
				"503 Bad sequence of commands.\r\n", 31);
		else
			request_retr_seq(cmd, data);
	}
}

void request_stor_seq(char *cmd, mdata *data)
{
	int fd;
	int i;

	cmd[strlen(cmd) - 2] = '\0';
	dprintf(data->csfd, "150 File status okay; "
		"about to open data connection.\r\n");
	for (i = strlen(cmd); cmd[i] != '/' && i != 0; i--);
	fd = open(cmd + ((i == 0) ? i : i + 1), O_CREAT | O_RDWR,
		S_IRWXU | S_IRWXG | S_IRWXO);
	if (fork() == 0) {
		dup2(fd, 1);
		dup2(data->dfd, 0);
		execl("/bin/cat", "/bin/cat", NULL);
	}
	close_it(data);
	close(fd);
	dprintf(data->csfd, "226 Closing data connection.\r\n");
}

void request_stor(char *cmd, mdata *data)
{
	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[4] != ' ')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		if (data->dfd == -1)
			write(data->csfd,
				"503 Bad sequence of commands.\r\n", 31);
		else
			request_stor_seq(cmd, data);
	}
}
