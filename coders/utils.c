/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 21:33:32 by ayhammou          #+#    #+#             */
/*   Updated: 2026/04/21 18:19:20 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_sim *sim, int coder_id, char *status)
{
	long	current_time;

	pthread_mutex_lock(&sim->print_mutex);
	pthread_mutex_lock(&sim->state_mutex);
	if (sim->is_running)
	{
		current_time = get_current_time_ms() - sim->start_time;
		printf("%ld %d %s\n", current_time, coder_id, status);
	}
	pthread_mutex_unlock(&sim->state_mutex);
	pthread_mutex_unlock(&sim->print_mutex);
}
