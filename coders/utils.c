/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 21:33:32 by ayhammou          #+#    #+#             */
/*   Updated: 2026/04/25 14:29:18 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ms_sleep(long time_in_ms)
{
	long	start_time;

	start_time = get_current_time_ms();
	while ((get_current_time_ms() - start_time) < time_in_ms)
	{
		usleep(500);
	}
}

void	print_status(t_sim *sim, int coder_id, char *status)
{
	long	current_time;

	pthread_mutex_lock(&sim->print_mutex);
	if (check_sim_status(sim))
	{
		current_time = get_current_time_ms() - sim->start_time;
		printf("%ld %d %s\n", current_time, coder_id, status);
	}
	pthread_mutex_unlock(&sim->print_mutex);
}

int	check_sim_status(t_sim *sim)
{
	int	status;

	pthread_mutex_lock(&sim->state_mutex);
	status = sim->is_running;
	pthread_mutex_unlock(&sim->state_mutex);
	return (status);
}

int	has_finished(t_coder *me)
{
	int	is_done;

	is_done = 0;
	pthread_mutex_lock(&me->sim->state_mutex);
	if (me->sim->nb_compiles_req > 0
		&& me->compiles_done >= me->sim->nb_compiles_req)
		is_done = 1;
	pthread_mutex_unlock(&me->sim->state_mutex);
	return (is_done);
}
