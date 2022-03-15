#include "../inc/ft_ping.h"

void	sighandler(int code)
{
	if (code == SIGINT)
	{
		g_data.end = 1;
		print_end_ping();
		free_ping();
		exit(0);
	}
	else if (code == SIGALRM)
	{
		printf("ALARM\n");
	}
}
