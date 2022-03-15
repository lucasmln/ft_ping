#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include "../libft/libft.h"
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <time.h>
# include <signal.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/time.h>
# include <endian.h>
# include <math.h>

# define MAX_ADDR_LEN 46
# define PING_PKT 64
# define BSWAP16(x)			((__uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
# define BSWAP32(x)			((__uint32_t) ((((x) >> 16) & 0xffff) | (((x) & 0xffff) << 16)))

typedef struct		ping_pkt_s
{
	struct icmp		hdr;
	char			msg[PING_PKT - sizeof(struct icmp)];
}					ping_pkt_t;

typedef struct		ping_reply_s
{
	int				ret;
	struct msghdr	mhdr;
	struct iovec	iov;
	char			control[CMSG_SPACE(sizeof(int))];
	char			recv_buf[84];

}					ping_reply_t;

typedef struct		ping_stats_s
{
	double		min;
	double		max;
	double		avg;
	double		sum;
	double		sum_square;
}					ping_stats_t;

typedef struct		ping_s
{
	ping_pkt_t			packet;
	ping_reply_t		reply;
	struct sockaddr_in	addr;
	ping_stats_t		stats;
	char				*address_name;
	char				*reverse_hostname;
	char				*user_request;
	bool				alarm;
	int					end;
	int					sockfd;
	int					count_msg;
	int					lose_msg;
	int					received_msg;
	struct timeval		start_ping;
	pid_t				curr_pid;
	int					seq;
	int					ttl;
}						ping_t;

ping_t		g_data;

/*
 ** main.c
*/
void	print_usage();
void	send_ping();
void	init(char **av);
void	free_ping();

/*
 ** packet.c
*/
void	intialize_send();
void	initialize_recv();
int		send_packet(struct timeval *packet_ping_start);
void	update_ping_stat(double time);
int		get_packet(struct timeval *packet_ping_start);
unsigned short	checksum(void *address, int len);

/*
 ** get_signal.c
*/
void	sighandler(int code);

/*
 ** ping_output.c
*/
void	print_end_ping();
void	print_packet(double time, int ttl);
void	print_start_ping();

/*
 ** time.c
*/
void		wait_time();
double		get_mdev();
double		get_ping_time(struct timeval start, struct timeval end);
void		save_time(struct timeval *tz);

/*
 ** socket.c
*/
void	dns_lookup(const char *dest);
void	reverse_dns_lookup();
void	create_socket(const char *dest);

#endif
