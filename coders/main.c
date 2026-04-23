/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 21:33:19 by ayhammou          #+#    #+#             */
/*   Updated: 2026/04/23 21:51:47 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	fill_args(char **argv, t_sim *sim)
{
	sim->nb_coders = atoi(argv[1]);
	sim->time_to_burnout = atoi(argv[2]);
	sim->time_to_compile = atoi(argv[3]);
	sim->time_to_debug = atoi(argv[4]);
	sim->time_to_refactor = atoi(argv[5]);
	sim->nb_compiles_req = atoi(argv[6]);
	sim->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		sim->scheduler = FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		sim->scheduler = EDF;
	else
		return (0);
	return (1);
}

int	parse_args(int argc, char **argv, t_sim *sim)
{
	int	i;

	if (argc != 9)
		return (printf("Error: Invalid number of arguments.\n"), 0);
	i = 1;
	while (i < 8)
	{
		if (!is_valid_number(argv[i]))
			return (printf("Error: Arguments must be positive integers.\n"), 0);
		i++;
	}
	if (!fill_args(argv, sim))
		return (printf("Error: Scheduler must be 'fifo' or 'edf'.\n"), 0);
	if (sim->nb_coders <= 0)
		return (0);
	return (1);
}

int	start_simulation(t_sim *sim)
{
	int			i;
	pthread_t	monitor_thread;

	sim->start_time = get_current_time_ms();
	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].last_compile_start = sim->start_time;
		pthread_create(&sim->coders[i].thread, NULL,
			coder_routine, &sim->coders[i]);
		i++;
	}
	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	return (1);
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	memset(&sim, 0, sizeof(t_sim));
	if (!parse_args(argc, argv, &sim))
		return (1);
	if (!init_simulation(&sim))
		return (1);
	printf("Initialisation reussie ! %d codeurs prets.\n", sim.nb_coders);
	start_simulation(&sim);
	free_simulation(&sim);
	return (0);
}
