/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 15:25:11 by lmoulin           #+#    #+#             */
/*   Updated: 2022/03/02 13:46:50 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

void	send_ping()
{
	struct timeval		packet_time_start;
	struct timeval		tmp;
	int					ret;
	double				time_to_wait;

	if (g_data.flags & FLAG_w)
		alarm(g_data.deadline);
	print_start_ping();
	save_time(&g_data.start_ping);
	while (1)
	{
		ret = send_packet(&packet_time_start);
		if (ret == 0)
			get_packet(&packet_time_start);
		g_data.seq++;
		if (g_data.flags & FLAG_C && g_data.count == g_data.seq - 1)
			break;
		save_time(&tmp);
		time_to_wait = g_data.interval - ((double)tmp.tv_sec - (double)packet_time_start.tv_sec +
				(((double)tmp.tv_usec - (double)packet_time_start.tv_usec) / USEC));
		if (time_to_wait > 0)
			wait_time(time_to_wait);
	}
	print_end_ping();
}

void	init(char **av, int ac)
{
	if (g_data.ttl == -1 && !(g_data.flags & FLAG_T))
		g_data.ttl = 128;
	if (!(g_data.flags & FLAG_I))
		g_data.interval = 1;
	if (!(g_data.flags & FLAG_W))
		g_data.timeout = 1;
	g_data.seq = 1;
	g_data.user_request = av[ac -1];
	g_data.curr_pid = getpid();
	g_data.stats.min = -1;
	g_data.stats.max = -1;
	g_data.stats.avg = -1;
	g_data.stats.sum = 0;
	g_data.received_msg = 0;
	g_data.error_msg = 0;
	g_data.lose_msg = 0;
	g_data.count_msg = 0;
	g_data.stats.sum_square = 0;
	signal(SIGINT, sighandler);
	signal(SIGALRM, sighandler);
}

void	free_ping()
{
	close(g_data.sockfd);
	free(g_data.address_name);
	free(g_data.reverse_hostname);
}

void	check_flags()
{
	if (g_data.flags & FLAG_C && g_data.count == -1)
	{
		printf("ft_ping: option requires an argument -- 'c'\n");
		print_usage();
	}
}

int		main(int ac, char **av)
{
	g_data.flags = parse(av, ac);
	check_flags();
	init(av, ac);
	dns_lookup(av[ac - 1]);
	reverse_dns_lookup();
	send_ping();
	free_ping();
	return (0);
}
