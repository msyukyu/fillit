/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhunders <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/28 17:57:12 by rhunders          #+#    #+#             */
/*   Updated: 2018/12/10 16:52:27 by rhunders         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

static COORD	get_start_coord(MAP *map, TETRO *piece)
{
	if (piece->previous)
	{
		if (map->start.y < piece->previous->origin.y ||
			(map->start.y == piece->previous->origin.y &&
			map->start.x < piece->previous->origin.x))
	 		return (piece->previous->origin);
		return (map->start);
	}
	else
	{
		if (map->start.y < piece->footprint.y ||
			(map->start.y == piece->footprint.y &&
			map->start.x < piece->footprint.x))
			return (piece->footprint);
		return (map->start);
	}
}

static int		init_check_gaps(MAP *map)
{
	map->dead_size = 0;
	init_coord(&(map->start));
	check_gaps(map);
	return (map->dead_size <= map->max_dead_size);
}

static int		fill_pattern(MAP *map, TETRO *pc, int index, COORD p)
{
	int		i;

	i = -1;
	pc->origin = p;
	p.x -= pc->footprint.x;
	p.y -= pc->footprint.y;
	while (++i < TETRO_SIZE)
		if (map->board[pc->pattern[i].y + p.y][pc->pattern[i].x + p.x] != '.')
			return (0);
	i = -1;
	while (++i < TETRO_SIZE)
		map->board[pc->pattern[i].y + p.y]
			[pc->pattern[i].x + p.x] = 'A' + index;
	return (1);
}

static int		erase_pattern(char **board, TETRO *piece, COORD point)
{
	int i;

	i = -1;
	point.x -= piece->footprint.x;
	point.y -= piece->footprint.y;
	while (++i < TETRO_SIZE)
		board[piece->pattern[i].y + point.y]
			[piece->pattern[i].x + point.x] = '.';
	return (1);
}

int				fillit(BOX *box, MAP *map, int index, int try)
{
	COORD			point;

	if (index == box->nb_tetro)
		return (1);
	if (try && init_map(map, &try, box) == -1)
		return (0);
	point = get_start_coord(map, box->tetro_box[index]);
	while (point.y < map->l_map)
	{
		while (point.x < map->l_map)
		{
			if (fill_pattern(map, box->tetro_box[index], index, point))
			{
				if (init_check_gaps(map))
					if (fillit(box, map, index + 1, 0))
						return (1);
				erase_pattern(map->board, box->tetro_box[index], point);
			}
			point.x++;
		}
		point.x = box->tetro_box[index]->footprint.x;
		point.y++;
	}
	return ((!index) ? fillit(box, map, 0, 1) : 0);
}
