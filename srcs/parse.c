#include "../inc/ft_ping.h"

int		parse(char **av, int ac)
{
	int		i;

	if (ac <= 1)
		print_usage();
	i = 1;
	while (i < ac)
	{
		if (av[i][0] == '-')
			parse_flag(av[i]);
		i++;
	}
}

int		parse_flag(char *flag)
{
	if (ft_strncmp(flag, "-h", 2) == 0)
		print_usage();
	else if (ft_strncmp(flag, "-v", 2) == 0)
		return (SUCCESS);
	else
	{
		printf("ft_ping: invalid option -- '%s'\n", flag);
		print_usage();
	}
}
