#include "../inc/ft_ping.h"

void	wait_time(double time_sec)
{
	struct timeval		init;
	struct timeval		goal;
	int					sec = (int)time_sec;

	save_time(&init);
	goal = init;
	goal.tv_sec += sec;
	goal.tv_usec += (time_sec - sec) * USEC;
	if (goal.tv_usec >= USEC)
	{
		goal.tv_sec += 1;
		goal.tv_usec -= USEC;
	}
	while (1)
	{
		save_time(&init);
		if (goal.tv_sec - init.tv_sec <= 0)
			if (goal.tv_sec - init.tv_sec < 0)
				break;
			else if (goal.tv_usec - init.tv_usec <= 0)
				break;
	}
}

double		get_mdev()
{
	double		mdev;

	mdev = (g_data.stats.sum_square / ((double)g_data.received_msg + (double)g_data.lose_msg)) -
			g_data.stats.avg * g_data.stats.avg;
	return (sqrt(mdev));
}

double		get_ping_time(struct timeval start, struct timeval end)
{
	double	res;

	res = ((double)((double)end.tv_sec - (double)start.tv_sec) * 1000) + ((double)((double)end.tv_usec - (double)start.tv_usec) / 1000);
	return (res);
}

void		save_time(struct timeval *tz)
{
	if (gettimeofday(tz, NULL) != 0)
	{
		printf("Fatal error: gettimeofday failed\n");
		exit(1);
	}
}
