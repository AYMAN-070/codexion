/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 16:12:27 by ayhammou          #+#    #+#             */
/*   Updated: 2026/04/23 22:24:10 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].is_available = 1;
		sim->dongles[i].available_at = 0;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		pthread_cond_init(&sim->dongles[i].cond, NULL);
		sim->dongles[i].queue = malloc(sizeof(t_queue));
		if (!sim->dongles[i].queue)
			return (0);
		sim->dongles[i].queue->nodes = malloc(sizeof(t_qnode) * sim->nb_coders);
		sim->dongles[i].queue->capacity = sim->nb_coders;
		sim->dongles[i].queue->size = 0;
		i++;
	}
	return (1);
}

void	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].id = i;
		sim->coders[i].compiles_done = 0;
		sim->coders[i].sim = sim;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % sim->nb_coders];
		i++;
	}
}

int	init_simulation(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->nb_coders);
	sim->dongles = malloc(sizeof(t_dongle) * sim->nb_coders);
	if (!sim->coders || !sim->dongles)
		return (0);
	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->state_mutex, NULL);
	sim->is_running = 1;
	if (!init_dongles(sim))
		return (0);
	init_coders(sim);
	return (1);
}

void	free_simulation(t_sim *sim)
{
	int	i;

	if (sim->dongles)
	{
		i = 0;
		while (i < sim->nb_coders)
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			pthread_cond_destroy(&sim->dongles[i].cond);
			if (sim->dongles[i].queue)
			{
				if (sim->dongles[i].queue->nodes)
					free(sim->dongles[i].queue->nodes);
				free(sim->dongles[i].queue);
			}
			i++;
		}
		free(sim->dongles);
	}
	if (sim->coders)
		free(sim->coders);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->state_mutex);
}
