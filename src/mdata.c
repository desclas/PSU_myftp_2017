/*
** EPITECH PROJECT, 2018
** myFTP
** File description:
** mdata file
*/

#include "serveur.h"

void request_list_seq(char *cmd, mdata *data)
{
	char *tmp = NULL;

	write(data->csfd,
		"150 File status okay; about to open data connection.\r\n", 54);
	if (cmd[4] == ' ') {
		tmp = calloc(strlen(cmd), sizeof(char));
		strcpy(tmp, cmd + 5);
		tmp[strlen(tmp) - 2] = '\0';
	}
	if (fork() == 0) {
		dup2(data->dfd, 1);
		execl("/bin/ls", "/bin/ls", tmp, NULL);
	}
	write(data->csfd, "226 Closing data connection.\r\n", 30);
	close_it(data);
}

void request_list(char *cmd, mdata *data)
{
	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[4] != ' ' && cmd[4] != '\r')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		if (data->dfd == -1)
			write(data->csfd,
				"503 Bad sequence of commands.\r\n", 31);
		else
			request_list_seq(cmd, data);
	}
}

void get_exec(mdata *data)
{
	char tab[14][6] = {"USER", "PASS", "CWD", "COUP", "QUIT", "DELE", "PWD",
			"PASV", "PORT", "HELP", "NOOP", "RETR", "STOR", "LIST"};
	size_t i;

	for (i = 0; i != 14; i++)
		if (strncmp(data->str, tab[i], strlen(tab[i])) == 0)
			break;
	if (i != 14)
		data->func[i](data->str, data);
	else
		write(data->csfd, "500 Comand not found.\r\n", 23);
	for (i = 0; data->str[i] != '\r' && data->str[i + 1] != '\n'; i++);
	if (strlen(data->str + i + 2) != 0)
		memmove(data->str, data->str + i + 2, strlen(data->str + i+ 2));
	else {
		free(data->str);
		data->str = NULL;
	}
}

void (**init_ptr_func(void))(char *, mdata *)
{
	void (**ptr)(char *, mdata *);

	ptr = malloc(sizeof(*ptr) * 14);
	ptr[0] = request_user;
	ptr[1] = request_pass;
	ptr[2] = request_cwd;
	ptr[3] = request_coup;
	ptr[4] = request_quit;
	ptr[5] = request_dele;
	ptr[6] = request_pwd;
	ptr[7] = request_pasv;
	ptr[8] = request_port;
	ptr[9] = request_help;
	ptr[10] = request_noop;
	ptr[11] = request_retr;
	ptr[12] = request_stor;
	ptr[13] = request_list;
	return (ptr);
}

void init_mdata(mdata *data, int csfd, int port)
{
	data->loged = 0;
	data->dir = getcwd(NULL, 0);
	data->func = init_ptr_func();
	data->dfd = -1;
	data->csfd = csfd;
	data->str = NULL;
	data->buff = NULL;
	data->quit = 0;
	data->port = port;
	data->tfd = -1;
	data->htab = calloc(14, sizeof(char *));
	init_htab(data->htab);
}
