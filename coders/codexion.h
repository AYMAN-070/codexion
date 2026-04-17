/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 15:59:02 by ayhammou          #+#    #+#             */
/*   Updated: 2026/04/17 16:04:29 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>

typedef enum scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_qnode
{
	int		coder_id;
	long	priority_value;
}	t_qnode;

typedef struct s_queue
{
	t_qnode	*nodes;
	int		size;
	int		capacity;
}	t_queue;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				is_available;
	long			available_at;
	t_queue			*queue;
}	t_dongle;

typedef struct sim
{
	int				nb_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				nb_compiles_req;
	int				dongle_cooldown;
	t_scheduler		scheduler;
	long			start_time;
	int				is_running;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_sim;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	long			last_compile_start;
	int				compiles_done;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	struct s_sim	*sim;
}	t_coder;

int	parse_args(int argc, char **argv, t_sim *sim);

#endif
