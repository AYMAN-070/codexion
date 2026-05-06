/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:19:40 by ayhammou          #+#    #+#             */
/*   Updated: 2026/05/06 10:43:13 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_death(t_sim *sim, t_coder *coder)
{
	int	j;

	pthread_mutex_lock(&sim->state_mutex);
	if ((sim->nb_compiles_req > 0 && coder->compiles_done
			>= sim->nb_compiles_req)
		|| get_current_time_ms() - coder->last_compile_start
		< sim->time_to_burnout)
	{
		pthread_mutex_unlock(&sim->state_mutex);
		return (0);
	}
	sim->is_running = 0;
	pthread_mutex_unlock(&sim->state_mutex);
	j = -1;
	while (++j < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->dongles[j].mutex);
		pthread_cond_broadcast(&sim->dongles[j].cond);
		pthread_mutex_unlock(&sim->dongles[j].mutex);
	}
	pthread_mutex_lock(&sim->print_mutex);
	printf("%ld %d burned out\n",
		get_current_time_ms() - sim->start_time, coder->id);
	pthread_mutex_unlock(&sim->print_mutex);
	return (1);
}

static int	check_all_compiles(t_sim *sim)
{
	int	i;
	int	finished_count;

	if (sim->nb_compiles_req <= 0)
		return (0);
	finished_count = 0;
	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->state_mutex);
		if (sim->coders[i].compiles_done >= sim->nb_compiles_req)
			finished_count++;
		pthread_mutex_unlock(&sim->state_mutex);
		i++;
	}
	if (finished_count == sim->nb_coders)
	{
		pthread_mutex_lock(&sim->state_mutex);
		sim->is_running = 0;
		pthread_mutex_unlock(&sim->state_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	usleep(1000);
	while (1)
	{
		i = 0;
		while (i < sim->nb_coders)
		{
			if (check_death(sim, &sim->coders[i]))
				return (NULL);
			i++;
		}
		if (check_all_compiles(sim))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
