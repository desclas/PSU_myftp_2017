/*
** EPITECH PROJECT, 2018
** myFTP
** File description:
** active instruction file
*/

#include "serveur.h"

int check_addrport(char *cmd)
{
	size_t coma, num;

	cmd[strlen(cmd) - 2] = '\0';
	coma = 0;
	num = 0;
	for (size_t i = 0; cmd[i] != '\0'; i++) {
		if (cmd[i] == ',') {
			coma++;
			num = 0;
		} else if (cmd[i] >= 48 && cmd[i] <= 57)
			num++;
		else
			return (0);
		if (num > 3 || coma > 5)
			return (1);
	}
	return (0);
}

void request_port_seq(char *cmd, mdata *data)
{
	sockaddr_in sin = { 0 };
	int size = sizeof(sin);
	char *adr;
	size_t ip[4], port[2];

	if (check_addrport(cmd + 5) == 0) {
		sscanf(cmd, "PASV %lu,%lu,%lu,%lu,%lu,%lu", &ip[0], &ip[1],
			&ip[2], &ip[3], &port[0], &port[1]);
		asprintf(&adr, "%lu.%lu.%lu.%lu", ip[0], ip[1], ip[2], ip[3]);
		sin.sin_addr.s_addr = inet_addr(adr);
		sin.sin_family = AF_INET;
		sin.sin_port = htons((port[0] * 256) + port[1]);
		if (connect(data->dfd, (sockaddr *) &sin, size) != -1)
			write(data->csfd,
				"425 Error opening data socket.\r\n", 32);
		else
			write(data->csfd, "200 Command okay.\r\n", 19);
		free(adr);
	} else
		write(data->csfd, "501 Bad argument.\r\n", 19);
}

void request_port(char *cmd, mdata *data)
{
	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[4] != ' ')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		close_it(data);
		if ((data->tfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			write(data->csfd, "550 Enable passive "
				"or active mod.\r\n", 35);
		else
			request_port_seq(cmd, data);
	}
}

void request_help_seq(char *cmd, mdata *data)
{
	size_t i;

	if (cmd[4] == '\r') {
		for (i = 0; i != 14; i++)
			dprintf(data->csfd, "%s\n", data->htab[i]);
		close_it(data);
	} else {
		for (i = 0; strncmp(data->htab[i], cmd + 5, 4) != 0 && i != 14;
		i++);
		if (i == 14)
			write(data->csfd, "501 Bad argument.\r\n", 19);
		else {
			dprintf(data->csfd, "%s\n", data->htab[i]);
			close_it(data);
		}
	}
}

void request_help(char *cmd, mdata *data)
{
	if (data->loged != 2)
		write(data->csfd, "530 Need to log in.\r\n", 21);
	else if (cmd[4] != ' ' && cmd[4] != '\r')
		write(data->csfd, "500 Comand not found.\r\n", 23);
	else {
		if (data->dfd == -1)
			write(data->csfd, "550 Enable passive "
				"or active mod.\r\n", 35);
		else
			request_help_seq(cmd, data);
	}
}
