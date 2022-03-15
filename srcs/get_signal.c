#include "../inc/ft_ping.h"

void	sighandler(int code)
{
	if (code == SIGINT)
	{
		print_end_ping();
		free_ping();
		exit(0);
	}
	else if (code == SIGALRM)
	{
		printf("ALARM\n");
	}
}
