/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:29:03 by ayhammou          #+#    #+#             */
/*   Updated: 2026/04/28 18:39:49 by ayhammou         ###   ########.fr       */
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
	if (q->size >= q->capacity)
		return ;
	q->nodes[q->size].coder_id = coder_id;
	q->nodes[q->size].priority_value = priority;
	q->size++;
	if (q->size == 2)
	{
		if (q->nodes[1].priority_value < q->nodes[0].priority_value)
		{
			swap_nodes(&q->nodes[0], &q->nodes[1]);
		}
	}
}

void	pop_queue(t_queue *q)
{
	if (q->size <= 0)
		return ;
	q->size--;
	if (q->size == 1)
		q->nodes[0] = q->nodes[1];
}

int	peek_queue(t_queue *q)
{
	if (q->size == 0)
		return (-1);
	return (q->nodes[0].coder_id);
}
