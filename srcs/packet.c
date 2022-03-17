#include "../inc/ft_ping.h"

void	intialize_send()
{
	struct timeval	curr;

	gettimeofday(&curr, NULL);
	ft_bzero(&g_data.packet, sizeof(g_data.packet));
	g_data.packet.hdr.icmp_type = ICMP_ECHO;
	g_data.packet.hdr.icmp_code = 0;
	g_data.packet.hdr.icmp_id = BSWAP16(g_data.curr_pid);
	g_data.packet.hdr.icmp_seq = BSWAP16(g_data.seq);
	ft_memcpy(&g_data.packet.hdr.icmp_dun, &(curr.tv_sec), sizeof(curr.tv_sec));
	g_data.packet.hdr.icmp_cksum = checksum(&g_data.packet, sizeof(g_data.packet));
}

void	initialize_recv()
{
	ft_bzero(&g_data.reply, sizeof(g_data.reply));
	g_data.reply.iov.iov_base = g_data.reply.recv_buf;
	g_data.reply.iov.iov_len = sizeof(g_data.reply.recv_buf);
	g_data.reply.mhdr.msg_name = ft_strdup(g_data.address_name);
	g_data.reply.mhdr.msg_namelen = sizeof(g_data.address_name);
	g_data.reply.mhdr.msg_iov = &g_data.reply.iov;
	g_data.reply.mhdr.msg_iovlen = 1;
	g_data.reply.mhdr.msg_control = &g_data.reply.control;
	g_data.reply.mhdr.msg_controllen = sizeof(&g_data.reply.control);
}

int		send_packet(struct timeval *packet_ping_start)
{
	int		ret;

	intialize_send();
	gettimeofday(packet_ping_start, NULL);
	ret = sendto(g_data.sockfd, &(g_data.packet), sizeof(g_data.packet), 0,
		(struct sockaddr*)&g_data.addr, sizeof(g_data.addr));
	g_data.count_msg++;
	if (ret <= 0)
	{
		if (ret < 0)
			printf("Packet Sending Failed!\n");
		else if (!(g_data.flags & FLAG_Q))
			printf("Empty packet send\n");
		return (1);
	}
	return (0);
}

void	update_ping_stat(double time)
{
	g_data.stats.sum += time;
	g_data.stats.sum_square += (time * time);
	if (g_data.received_msg == 0)
	{
		g_data.stats.max = time;
		g_data.stats.avg = time;
		g_data.stats.min = time;
		g_data.received_msg++;
	}
	else
	{
		if (time > g_data.stats.max)
			g_data.stats.max = time;
		else if (time < g_data.stats.min)
			g_data.stats.min = time;
		g_data.stats.avg = (g_data.stats.avg * g_data.received_msg + time) / (g_data.received_msg + 1);
		g_data.received_msg++;
	}
}

int		get_packet(struct timeval *packet_ping_start)
{
	struct ip		*ip;
	struct timeval	end_ping;
	struct icmp		*ret_icmp;

	initialize_recv();
	g_data.reply.ret = recvmsg(g_data.sockfd, &(g_data.reply.mhdr), 0);
	gettimeofday(&end_ping, NULL);
	if (g_data.reply.ret > 0)
	{
		ip = (struct ip *)g_data.reply.iov.iov_base;
		if (ip->ip_p != IPPROTO_ICMP)
			return (1);
		ret_icmp = (struct icmp *)((ip->ip_hl << 2) + g_data.reply.recv_buf);
		if (ret_icmp->icmp_type == ICMP_TIMXCEED)
		{
			print_exceeded_packet(ip, ret_icmp);
			g_data.error_msg++;
			g_data.lose_msg++;
		}
		if (BSWAP16(ret_icmp->icmp_id) != g_data.curr_pid || ret_icmp->icmp_type != ICMP_ECHOREPLY)
		{
			free(g_data.reply.mhdr.msg_name);
			g_data.reply.mhdr.msg_name = NULL;
			get_packet(packet_ping_start);
			return (0);
		}
		if (!(g_data.flags & FLAG_Q))
			print_packet(get_ping_time(*packet_ping_start, end_ping), ip->ip_ttl);
	}
	else
	{
		if (errno == EAGAIN && g_data.flags & FLAG_V)
			printf("Request timeout for icmp_seq %d\n", g_data.seq);
		g_data.lose_msg++;
	}
	free(g_data.reply.mhdr.msg_name);
	return (0);
}

unsigned short	checksum(void *address, int len)
{
	unsigned short	*buff;
	unsigned long	sum;

	buff = (unsigned short *)address;
	sum = 0;
	while (len > 1)
	{
		sum += *buff;
		buff++;
		len -= sizeof(unsigned short);
	}
	if (len)
		sum += *(unsigned char *)buff;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ((unsigned short)~sum);
}
