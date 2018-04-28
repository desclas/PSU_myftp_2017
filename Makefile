##
## EPITECH PROJECT, 2018
## myFTP
## File description:
## makefile
##

SSRC	=	src/serveur.c			\
		src/active_instruction.c	\
		src/error.c			\
		src/file_instruction.c		\
		src/mdata.c			\
		src/passive_instruction.c	\
		src/request_loop.c		\
		src/user_instruction.c

CSRC	=

SOBJ	=	$(SSRC:.c=.o)

COBJ	=	$(CSRC:.c=.o)

SNAME	=	serveur

CNAME	=	myftp

CFLAGS	=	-W -Wall -Werror -Wextra -g3 -Iincludes

all: $(SNAME) $(CNAME)

$(SNAME): $(SOBJ)
	gcc -o $@ $^ $(CFLAGS) -g3

$(CNAME): $(COBJ)
#	gcc -o $@ $^ $(CFLAGS) -g3

clean:
	rm -f $(SOBJ) $(COBJ)

fclean: clean
	rm -f $(SNAME) $(CNAME)

re: fclean all