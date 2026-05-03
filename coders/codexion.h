/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 15:59:02 by ayhammou          #+#    #+#             */
/*   Updated: 2026/05/03 14:29:02 by ayhammou         ###   ########.fr       */
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

typedef enum s_scheduler
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

struct	s_sim;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	long			last_compile_start;
	int				compiles_done;
	long			wait_start_time;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
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

int		parse_args(int argc, char **argv, t_sim *sim);

int		init_simulation(t_sim *sim);

void	free_simulation(t_sim *sim);

long	get_current_time_ms(void);
void	*monitor_routine(void *arg);
void	print_status(t_sim *sim, int coder_id, char *status);
void	push_queue(t_queue *q, int coder_id, long priority);
void	pop_queue(t_queue *q);
int		peek_queue(t_queue *q);

int		init_simulation(t_sim *sim);
void	*coder_routine(void *arg);
int		check_sim_status(t_sim *sim);
int		has_finished(t_coder *me);
long	get_current_time_ms(void);
void	print_status(t_sim *sim, int coder_id, char *status);
int		check_sim_status(t_sim *sim);
void	ms_sleep(long time_in_ms);
#endif
