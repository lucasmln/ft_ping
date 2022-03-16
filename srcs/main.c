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
	struct timeval		ping_time;
	struct timeval		packet_time_start;
	int					ret;
	bool				state;

	if (g_data.flags & FLAG_W)
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
		wait_time(g_data.interval);
	}
	print_end_ping();
}

void	init(char **av, int ac)
{
	if (!(g_data.flags & FLAG_T))
		g_data.ttl = 128;
	if (!(g_data.flags & FLAG_I))
		g_data.interval = 1;
	g_data.count_msg = 0;
	g_data.seq = 1;
	g_data.user_request = av[ac -1];
	g_data.curr_pid = getpid();
	g_data.stats.min = -1;
	g_data.stats.max = -1;
	g_data.stats.avg = -1;
	g_data.stats.sum = 0;
	g_data.received_msg = 0;
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
