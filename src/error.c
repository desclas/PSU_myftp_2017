/*
** EPITECH PROJECT, 2018
** myFTP
** File description:
** error handling
*/

#include "serveur.h"

void close_it(mdata *data)
{
	if (data->dfd != -1)
		close(data->dfd);
	data->dfd = -1;
	if (data->tfd != -1)
		close(data->tfd);
	data->tfd = -1;
}

void clean_it(mdata *data)
{
	if (data->str != NULL)
		free(data->str);
	if (data->csfd != -1)
		close(data->csfd);
	free(data->func);
	free(data->dir);
	free(data->htab);
	close_it(data);
}

void error_quit(int val, mdata *data)
{
	if (val == -1) {
		if (data != NULL)
			clean_it(data);
		exit(84);
	}
}

void init_htab(char **res)
{
	res[0] = "USER<SP> <username> <CRLF> "
		": Specify user for authentication";
	res[1] = "PASS "
		"<SP> <password> <CRLF> : Specify password for authentication";
	res[2] = "CWD <SP> <pathname> <CRLF> : Change working directory";
	res[3] = "CDUP <CRLF> : Change working directory to parent directory";
	res[4] = "QUIT <CRLF> : Disconnection";
	res[5] = "DELE <SP> <pathname> <CRLF> : Delete file on the server";
	res[6] = "PWD <CRLF> : Print working directory";
	res[7] = "PASV <CRLF> : Enable \"passive\" mode for data transfer";
	res[8] = "PORT <SP> <host-port> <CRLF> "
		": Enable \"active\" mode for data transfer";
	res[9] = "HELP [<SP> <string>] <CRLF> : List available commands";
	res[10] = "NOOP <CRLF> : Do nothing";
	res[11] = "RETR <SP> <pathname> <CRLF> "
		": Download file from server to client";
	res[12] = "STOR <SP> <pathname> <CRLF> "
		": Upload file from client to server";
	res[13] = "LIST [<SP> <pathname>] <CRLF> "
		": List files in the current working directory";
}

void append_msstr(mdata *data)
{
	char *tmp;

	if (data->str == NULL)
		data->str = calloc(1, sizeof(char));
	tmp = calloc(strlen(data->str) + strlen(data->buff) + 2, sizeof(char));
	strcat(tmp, data->str);
	strcat(tmp, data->buff);
	free(data->str);
	free(data->buff);
	data->buff = NULL;
	data->str = tmp;
}
