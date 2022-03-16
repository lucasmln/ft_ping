#include "../inc/ft_ping.h"

void	create_socket()
{
	struct timeval		timeout;

	g_data.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_data.sockfd == -1)
	{
		printf("ft_ping: error socket : %d, errno : %s\n", g_data.sockfd, hstrerror(errno));
		exit(1);
	}

	timeout.tv_sec = (int)g_data.timeout;
	timeout.tv_usec = g_data.timeout - (int)g_data.timeout;
	if (setsockopt(g_data.sockfd, SOL_IP, IP_TTL, &g_data.ttl, sizeof(g_data.ttl)) != 0)
	{
		fprintf(stderr, "Setting socket options to TTL failed! %m\n");
		exit(1);
	}

	if (setsockopt(g_data.sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0)
	{
		fprintf(stderr, "Setting socket options to timeout failed! %m\n");
		exit(1);
	}
}

void	dns_lookup(const char *dest)
{
	struct addrinfo		*res = NULL;
	struct addrinfo		hint;
	char				tmp[INET_ADDRSTRLEN];

	ft_bzero(&hint, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(dest, NULL, &hint, &res) != 0)
	{
		printf("ft_ping: %s: Name or service not known\n", dest);
		exit(1);
	}
	ft_memcpy(&g_data.addr, res->ai_addr, sizeof(res->ai_addr));
	inet_ntop(AF_INET, &g_data.addr.sin_addr, tmp, sizeof(tmp));
	g_data.address_name = ft_strdup(tmp);
	create_socket(dest);
	if (res)
		freeaddrinfo(res);
}

void	reverse_dns_lookup()
{
	char	buf[65];

	if (getnameinfo((struct sockaddr *)&g_data.addr, sizeof(struct sockaddr_in),
		buf, sizeof(buf), NULL, 0, NI_NAMEREQD) == 0)
		g_data.reverse_hostname = ft_strdup(buf);
	else
		g_data.reverse_hostname = ft_strdup("");
}
