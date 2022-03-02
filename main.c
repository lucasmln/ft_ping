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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "libft/libft.h"
#include <netinet/ip_icmp.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#ifdef __APPLE__
       // If the OS doesn't declare it, do it ourself (copy-pasted from GNU C Library, license: LGPL)
# include <stdint.h>
struct icmphdr
{
	uint8_t type;           /* message type */
	uint8_t code;           /* type sub-code */
	uint16_t checksum;
	union
	{
		struct
		{
			uint16_t	id;
			uint16_t	sequence;
		} echo;                 /* echo datagram */
		uint32_t	gateway;        /* gateway address */
		struct
		{
			uint16_t	__unused;
			uint16_t	mtu;
		} frag;                 /* path mtu discovery */
		/*uint8_t reserved[4];*/
	} un;
};

       // Fix slightly changed names
//# define SOL_IP IPPROTO_IP

#endif

struct ping_pkt
{
	struct icmphdr hdr;
	char msg[64-sizeof(struct icmphdr)];
};

void	create_socket(const char *dest, struct addrinfo *res);

int		sockfd;

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

void	dns_lookup(const char *dest, struct addrinfo *res)
{
	if (getaddrinfo(dest, "", NULL, &res) != 0)
	{
		printf("ft_ping: cannot resolve %s: Unknown host\n", dest);
		exit(1);
	}
	create_socket(dest, res);
}

void	create_socket(const char *dest, struct addrinfo *res)
{
	(void)dest;
	(void)res;
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	//socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		printf("ft_ping: error socket : %d, errno : %s\n", sockfd, hstrerror(errno));
		exit(1);
	}
	printf("socket success\n");
}

void	send_ping(struct addrinfo *addr, char *host)
{
	unsigned short		ttl = 64;
	struct timeval		timeout;
	struct icmphdr		hdr;
	//char				msg[4096];

	ttl = 64;
	if (setsockopt(sockfd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
	{
		fprintf(stderr, "Setting socket options to TTL failed! %m\n");
		exit(1);
	}
	else
	{
		printf("\nSocket set to TTL..\n");
	}

	while (1)
	{
		ft_bzero(&hdr, sizeof(hdr));
		hdr.type = ICMP_ECHO;
		hdr.un.echo.id = getpid();
	}
}

int		main(int ac, char **av)
{
	struct addrinfo		*res = NULL;

	if (ac <= 1 || ac > 2)
		print_usage();
	dns_lookup(av[1], res);
	send_ping(res, av[1]);
}
