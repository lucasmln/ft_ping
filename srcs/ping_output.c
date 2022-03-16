#include "../inc/ft_ping.h"

void	print_usage()
{
	printf("usage: ./ft_ping [-Dqvh] [-c count] [-i interval] [-t ttl] [-w deadline] destination\n");
	exit(2);
}

void	print_start_ping()
{
	printf("PING %s (%s) 56(84) bytes of data.\n", g_data.user_request, g_data.address_name);
}

void	print_end_ping()
{
	struct timeval		end;
	double				total;
	double				mdev;
	int					lose;

	save_time(&end);
	total = get_ping_time(g_data.start_ping, end);
	lose = (g_data.lose_msg / g_data.count_msg) * 100;
	g_data.stats.min = g_data.stats.min < 0 ? 0 : g_data.stats.min;
	g_data.stats.max = g_data.stats.max < 0 ? 0 : g_data.stats.max;
	g_data.stats.avg = g_data.stats.avg < 0 ? 0 : g_data.stats.avg;
	mdev = get_mdev();
	printf("\n--- %s ft_ping statistics ---\n", g_data.user_request);
	printf("%d packets transmitted, %d received, %d%% packet loss, time %.0lfms\n",
		g_data.count_msg, g_data.received_msg, lose, total);
	if (g_data.received_msg > 0)
		printf("rtt min/avg/max/mdev = %.3lf/%.3lf/%.3lf/%.3lf ms\n", g_data.stats.min,
			g_data.stats.avg, g_data.stats.max, mdev);
}

void	print_packet(double time, int ttl)
{
	struct timeval		now;

	if (g_data.flags & FLAG_D)
	{
		save_time(&now);
		printf("[%.0lf.%.0lf] ", (double)now.tv_sec, (double)now.tv_usec);
	}
	if (ft_strncmp(g_data.user_request, g_data.address_name, ft_strlen(g_data.user_request)) != 0)
		printf("%d bytes from %s (%s)", PING_PKT, g_data.reverse_hostname, g_data.address_name);
	else
		printf("%d bytes from %s", PING_PKT, g_data.address_name);
	printf(": icmp_seq=%d ttl=%d ", g_data.seq, ttl);
	if (time >= 1)
		printf("time=%.2lf ms\n", time);
	else
		printf("time=%.3lf ms\n", time);
	update_ping_stat(time);
}
