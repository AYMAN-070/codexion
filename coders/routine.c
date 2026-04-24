/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 21:09:51 by ayhammou          #+#    #+#             */
/*   Updated: 2026/04/23 22:46:30 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_sim_status(t_sim *sim)
{
	int	status;

	pthread_mutex_lock(&sim->state_mutex);
	status = sim->is_running;
	pthread_mutex_unlock(&sim->state_mutex);
	return (status);
}

static void	take_dongle(t_coder *me, t_dongle *dongle)
{
	long	priority;

	if (me->sim->scheduler == FIFO)
		priority = get_current_time_ms();
	else
		priority = me->last_compile_start + me->sim->time_to_burnout;
	pthread_mutex_lock(&dongle->mutex);
	push_queue(dongle->queue, me->id, priority);
	while (!dongle->is_available || peek_queue(dongle->queue) != me->id
		|| get_current_time_ms() < dongle->available_at)
	{
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	dongle->is_available = 0;
	pop_queue(dongle->queue);
	print_status(me->sim, me->id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->mutex);
}

static void	drop_dongle(t_sim *sim, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_available = 1;
	dongle->available_at = (get_current_time_ms() + sim->dongle_cooldown);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

static int	has_finished(t_coder *me)
{
	int	is_done;

	is_done = 0;
	pthread_mutex_lock(&me->sim->state_mutex);
	if (me->sim->nb_compiles_req > 0
		&& me->compiles_done >= me->sim->nb_compiles_req)
	{
		is_done = 1;
	}
	pthread_mutex_unlock(&me->sim->state_mutex);
	return (is_done);
}

void	*coder_routine(void *arg)
{
	t_coder	*me;

	me = (t_coder *)arg;
	while (check_sim_status(me->sim))
	{
		if (has_finished(me))
			break ;
		if (me->id % 2 == 0)
		{
			take_dongle(me, me->left_dongle);
			take_dongle(me, me->right_dongle);
		}
		else
		{
			take_dongle(me, me->right_dongle);
			take_dongle(me, me->left_dongle);
		}
		pthread_mutex_lock(&me->sim->state_mutex);
		me->last_compile_start = get_current_time_ms();
		me->compiles_done++;
		pthread_mutex_unlock(&me->sim->state_mutex);
		print_status(me->sim, me->id, "is compiling");
		ms_sleep(me->sim->time_to_compile);
		drop_dongle(me->sim, me->left_dongle);
		drop_dongle(me->sim, me->right_dongle);
		print_status(me->sim, me->id, "is debugging");
		ms_sleep(me->sim->time_to_debug);
		print_status(me->sim, me->id, "is refactoring");
		ms_sleep(me->sim->time_to_refactor);
	}
	return (NULL);
}
