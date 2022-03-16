#include "../inc/ft_ping.h"

int		parse(char **av, int ac)
{
	int		i;
	int		flags;

	if (ac <= 1)
		print_usage();
	i = 1;
	flags = 0;
	g_data.count = -1;
	g_data.interval = -1;
	g_data.deadline = -1;
	g_data.timeout = -1;
	g_data.ttl = -1;
	while (i < ac)
	{
		if (flags & FLAG_C && g_data.count == -1)
			get_count(av, ac, i);
		if (flags & FLAG_I && g_data.interval == -1)
			get_interval(av, ac, i);
		if (flags & FLAG_T && g_data.ttl == -1)
			get_ttl(av, ac, i);
		if (flags & FLAG_w && g_data.deadline == -1)
			get_deadline(av, ac, i);
		if (flags & FLAG_W && g_data.timeout == -1)
			get_timeout(av, ac, i);
		if (av[i][0] == '-')
			parse_flag(&flags, av[i]);
		i++;
	}
	return (flags);
}

void	get_deadline(char **av, int ac, const int i)
{
	g_data.deadline = ft_atoi(av[i]);
	if (g_data.deadline < 0 || !ft_isdigit(av[i][0]))
	{
		printf("ft_ping: bad wait time\n");
		exit(2);
	}
	if (i == ac - 1)
		print_usage();
}

void	get_timeout(char **av, int ac, const int i)
{
	g_data.timeout = atof(av[i]);
	if (g_data.timeout < 0 || !ft_isdigit(av[i][0]))
	{
		printf("ft_ping: bad wait time\n");
		exit(2);
	}
	if (i == ac - 1)
	{
		printf("HERER %s, %d\n", av[i], ac);
		print_usage();
	}
}

void	get_ttl(char **av, int ac, const int i)
{
	g_data.ttl = (unsigned int)ft_atoi(av[i]);
	if (g_data.ttl <= 0 || g_data.ttl > 255)
	{
		printf("ft_ping: ttl %d out of range\n", g_data.ttl);
		exit(2);
	}
	if (i == ac - 1)
		print_usage();
}

void	get_interval(char **av, int ac, const int i)
{
	g_data.interval = atof(av[i]);
	if (g_data.interval < 0 || g_data.interval > 2147482 || !ft_isdigit(av[i][0]))
	{
		printf("ft_ping: bad timing interval\n");
		exit(2);
	}
	if (i == ac - 1)
		print_usage();
}

void	get_count(char **av, int ac, const int i)
{
	g_data.count = ft_atoi(av[i]);
	if (g_data.count <= 0)
	{
		printf("ft_ping: bad number of packets to transmit.\n");
		exit(2);
	}
	if (i == ac - 1)
		print_usage();
}

int		parse_flag(int *flags, char *str)
{
	if (ft_strncmp(str, "-h", 2) == 0)
		print_usage();
	else if (ft_strncmp(str, "--", 2) == 0)
		print_usage();
	else if (ft_strncmp(str, "-v", 2) == 0)
		*flags = *flags | FLAG_V;
	else if (ft_strncmp(str, "-c", 2) == 0)
		*flags = *flags | FLAG_C;
	else if (ft_strncmp(str, "-D", 2) == 0)
		*flags = *flags | FLAG_D;
	else if (ft_strncmp(str, "-i", 2) == 0)
		*flags = *flags | FLAG_I;
	else if (ft_strncmp(str, "-t", 2) == 0)
		*flags = *flags | FLAG_T;
	else if (ft_strncmp(str, "-w", 2) == 0)
		*flags = *flags | FLAG_w;
	else if (ft_strncmp(str, "-W", 2) == 0)
		*flags = *flags | FLAG_W;
	else if (ft_strncmp(str, "-q", 2) == 0)
		*flags = *flags | FLAG_Q;
	else
	{
		printf("ft_ping: invalid option -- '%s'\n", &str[1]);
		print_usage();
	}
}
