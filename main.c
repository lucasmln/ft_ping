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

#include "ft_ping.h"

void	print_usage()
{
	printf("usage: ./ft_ping [-vh] destination\n");
	exit(64);
}

void	fatal()
{
	printf("Error: fatal\n");
	exit(1);
}

void	error(char *error)
{
	printf("%s\n", error);
	exit(1);
}

void	create_socket(const char *dest)
{
	unsigned short		ttl = 64;
	struct timeval		timeout;

	g_data.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_data.sockfd == -1)
	{
		printf("ft_ping: error socket : %d, errno : %s\n", g_data.sockfd, hstrerror(errno));
		exit(1);
	}
	printf("socket success\n");

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	if (setsockopt(g_data.sockfd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
	{
		fprintf(stderr, "Setting socket options to TTL failed! %m\n");
		exit(1);
	}
	printf("Socket set to TTL..\n");

	if (setsockopt(g_data.sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0)
	{
		fprintf(stderr, "Setting socket options to timeout failed! %m\n");
		exit(1);
	}
	printf("Socket set to timeout..\n");
}

void	dns_lookup(const char *dest)
{
	struct addrinfo		*res;
	struct addrinfo		hint;

	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_RAW;
	hint.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(dest, NULL, NULL, &res) != 0)
	{
		printf("ft_ping: cannot resolve %s: Unknown host\n", dest);
		exit(1);
	}
	g_data.addr = (struct sockaddr_in *)res->ai_addr;
	create_socket(dest);
}

void	send_ping(char *host)
{

	while (!g_data.end)
	{
		send_packet();
		alarm(1);
		get_packet();
	}
	printf("\nend of ping\n");
}

int		main(int ac, char **av)
{
	if (ac <= 1 || ac > 2)
		print_usage();
	dns_lookup(av[1]);
	g_data.alarm = 0;
	g_data.end = 0;
	signal(SIGINT, sighandler);
	signal(SIGALRM, sighandler);
	send_ping(av[1]);
}
