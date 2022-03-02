#include "ft_ping.h"

void	sighandler(int code)
{
	if (code == SIGINT)
	{
		g_data.end = 1;
	}
	else if (code == SIGALRM)
	{
		g_data.alarm = !g_data.alarm;
	}
}
