#include "ft_ping.h"

void	send_packet()
{
	ft_bzero(&g_data.packet, sizeof(g_data.packet));
	g_data.packet.hdr.type = ICMP_ECHO;
	g_data.packet.hdr.un.echo.id = getpid();
	g_data.packet.hdr.un.echo.sequence += 1;
	g_data.packet.hdr.checksum = checksum(&g_data.packet, sizeof(g_data.packet));
	for (int i = 0; i < sizeof(g_data.packet.msg) - 1; i++)
		g_data.packet.msg[i] = i + '0';
	g_data.packet.msg[sizeof(g_data.packet.msg) - 1] = 0;
	if (sendto(g_data.sockfd, &g_data.packet.msg, sizeof(g_data.packet.msg), 0,
		(struct sockaddr*)g_data.addr, sizeof(*g_data.addr)) <= 0)
	{
		fprintf(stderr, "Packet Sending Failed! %m\n");
		exit(1);
	}
	else
		printf("Packet send with success\n");
}

void	get_packet()
{
	bool			alarm_state = g_data.alarm;
	struct iovec	iov[1];
	char			control[1000];

	g_data.mhdr.msg_name = &g_data.addr;
	g_data.mhdr.msg_namelen = sizeof(g_data.addr);
	g_data.mhdr.msg_iov = iov;
	g_data.mhdr.msg_iovlen = 1;
	g_data.mhdr.msg_control = &control;
	g_data.mhdr.msg_controllen = sizeof(control);

	while (alarm_state == g_data.alarm)
	{
		if (recvmsg(g_data.sockfd, &g_data.mhdr, 0) > 0)
		{
			printf("Receive packet\n");
			alarm_state = !g_data.alarm;
		}
	}
}

unsigned short	checksum(void *p, int size)
{
	char			*buf = p;
	int				sum = 0;
	unsigned short	result;

	for (sum = 0; size > 1; size -= 2)
		sum += *buf++;
	if (size == 1)
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}


