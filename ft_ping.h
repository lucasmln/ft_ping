#ifndef FT_PING_H
# define FT_PING_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include "libft/libft.h"
# include <netinet/ip_icmp.h>
# include <time.h>
# include <signal.h>
# include <errno.h>
#include <stdbool.h>

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

typedef struct		ping_pkt_s
{
	struct icmphdr		hdr;
	char				msg[64-sizeof(struct icmphdr)];
}					ping_pkt_t;

typedef struct		ping_s
{
	ping_pkt_t			packet;
	struct sockaddr_in	*addr;
	struct msghdr		mhdr;
	bool				alarm;
	int					end;
	int					sockfd;
}						ping_t;

ping_t		g_data;

/*
 ** packet.c
*/
void			send_packet();
void			get_packet();
unsigned short	checksum(void *p, int size);

/*
 ** get_signal.c
*/
void	sighandler(int code);

#endif
