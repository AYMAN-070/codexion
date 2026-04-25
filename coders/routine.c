/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 21:09:51 by ayhammou          #+#    #+#             */
/*   Updated: 2026/04/25 14:30:03 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	drop_dongle(t_sim *sim, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_available = 1;
	dongle->available_at = (get_current_time_ms() + sim->dongle_cooldown);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

static int	wait_for_dongle(t_coder *me, t_dongle *d)
{
	while (1)
	{
		if (!check_sim_status(me->sim))
		{
			pthread_mutex_unlock(&d->mutex);
			return (0);
		}
		if (!d->is_available || peek_queue(d->queue) != me->id)
			pthread_cond_wait(&d->cond, &d->mutex);
		else if (get_current_time_ms() < d->available_at)
		{
			pthread_mutex_unlock(&d->mutex);
			usleep(500);
			pthread_mutex_lock(&d->mutex);
		}
		else
			break ;
	}
	return (1);
}

static int	take_dongle(t_coder *me, t_dongle *d)
{
	long	p;

	p = get_current_time_ms();
	if (me->sim->scheduler != FIFO)
		p = me->last_compile_start + me->sim->time_to_burnout;
	pthread_mutex_lock(&d->mutex);
	push_queue(d->queue, me->id, p);
	if (!wait_for_dongle(me, d))
		return (0);
	d->is_available = 0;
	pop_queue(d->queue);
	print_status(me->sim, me->id, "has taken a dongle");
	pthread_mutex_unlock(&d->mutex);
	return (1);
}

static void	execute_work(t_coder *me)
{
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

void	*coder_routine(void *arg)
{
	t_coder		*me;
	t_dongle	*first;
	t_dongle	*second;

	me = (t_coder *)arg;
	first = me->left_dongle;
	second = me->right_dongle;
	if (me->id % 2 != 0)
	{
		first = me->right_dongle;
		second = me->left_dongle;
	}
	while (check_sim_status(me->sim))
	{
		if (has_finished(me) || !take_dongle(me, first))
			break ;
		if (!take_dongle(me, second))
		{
			drop_dongle(me->sim, first);
			break ;
		}
		execute_work(me);
	}
	return (NULL);
}
