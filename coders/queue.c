/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:29:03 by ayhammou          #+#    #+#             */
/*   Updated: 2026/04/24 15:09:13 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap_nodes(t_qnode *a, t_qnode *b)
{
	t_qnode	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	push_queue(t_queue *q, int coder_id, long priority)
{
	int	i;
	int	parent;

	if (q->size >= q->capacity)
		return ;
	i = q->size;
	q->nodes[i].coder_id = coder_id;
	q->nodes[i].priority_value = priority;
	q->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (q->nodes[i].priority_value < q->nodes[parent].priority_value)
		{
			swap_nodes(&q->nodes[i], &q->nodes[parent]);
			i = parent;
		}
		else
			break ;
	}
}

void	pop_queue(t_queue *q)
{
	int	i;
	int	child;

	if (q->size <= 0)
		return ;
	q->size--;
	q->nodes[0] = q->nodes[q->size];
	i = 0;
	while (1)
	{
		child = (2 * i) + 1;
		if (child >= q->size)
			break ;
		if (child + 1 < q->size
			&& q->nodes[child + 1].priority_value
			< q->nodes[child].priority_value)
			child++;
		if (q->nodes[i].priority_value > q->nodes[child].priority_value)
		{
			swap_nodes(&q->nodes[i], &q->nodes[child]);
			i = child;
		}
		else
			break ;
	}
}

int	peek_queue(t_queue *q)
{
	if (q->size == 0)
		return (-1);
	return (q->nodes[0].coder_id);
}
