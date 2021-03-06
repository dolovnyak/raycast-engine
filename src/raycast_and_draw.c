/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_and_draw.c                                   :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+     */
/*   By: rkeli <rkeli@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/07 00:57:48 by rkeli             #+#    #+#             */
/*   Updated: 2019/08/28 04:00:14 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycast.h"

float		fog_calculate_for_walls(t_rc_main *m)
{
	float distance;
	float intensity;

	distance = m->flr.wall_dist;
	intensity = clmp((distance / FOG_DIST), 0, 1);
	return (intensity);
}

float fog_calculate_for_floor(t_rc_main *m)
{
	float distance;
	float intensity;

	distance = m->player.current_dist;
	intensity = clmp((distance / FOG_DIST), 0, 1);
	return (intensity);
}

int		**choose_side(t_rc_main *m, float ray_dir_x, float ray_dir_y, int **tmp_arr)
{
	int		index;

	index = m->map[(int)m->wls.map_x][(int)m->wls.map_y];
	index = (index == 0 ? 1 : index);
	tmp_arr = (m->walls[1]).texture1;
	if (ray_dir_x == 0 && ray_dir_y == 0)
		exit(0);
	if (m->wls.side == 0 && m->flr.step_x < 0 )
		tmp_arr = (m->walls[index]).texture1;
	else if (m->wls.side == 0 && m->flr.step_x >= 0)
		tmp_arr = (m->walls[index]).texture3;
	else if (m->wls.side == 1 && m->flr.step_y < 0)
		tmp_arr = (m->walls[index]).texture2;
	else if (m->wls.side == 1 && m->flr.step_y >= 0)
		tmp_arr = (m->walls[index]).texture4;
	return (tmp_arr);
}

int draw_walls(t_rc_main *m, float ray_dir_x, float ray_dir_y, int i)
{
	int		j;
	int		draw_end;
	int		draw_start;
	int		line_h;
	int		wall_tex_x;
	int		wall_tex_y;
	int 	**tmp_arr;


	tmp_arr = NULL;
	line_h = (int)(m->sdl->win_h / m->flr.wall_dist);
	draw_start = -line_h / 2 + m->sdl->win_h / 2;
	draw_start = (draw_start < 0 ? 0 : draw_start);
	j = draw_start;
	draw_end = line_h / 2 + m->sdl->win_h / 2;
	draw_end = (draw_end >= m->sdl->win_h ? m->sdl->win_h - 1 : draw_end);
	wall_tex_x = (int)(m->flr.hit_x * (float)(COLUM));
	if (m->wls.side == 0 && ray_dir_x > 0)
		wall_tex_x = COLUM - wall_tex_x - 1;
	else if (m->wls.side == 1 && ray_dir_y < 0)
		wall_tex_x = COLUM - wall_tex_x - 1;
	tmp_arr = choose_side(m, ray_dir_x, ray_dir_y, tmp_arr);
	int color;
	m->player.intensity = fog_calculate_for_walls(m);
	while (j < draw_end)
	{
		int d = j * 256 - m->sdl->win_h * 128 + line_h * 128;
		wall_tex_y = ((d * COLUM) / line_h) / 256;
		color = rgb_mod(m->player.intensity, tmp_arr[wall_tex_y][wall_tex_x]);
		sdl_put_pixel(m->sdl, i, j, color);
		j++;
	}
	return (j);
}

void		draw_floor_or_celling(t_rc_main *m, int j, int i)
{
	float	weight;
	int		floor_tex_x;
	int		floor_tex_y;
	int 	**tmp_arr;
	int color;

	while (j < m->sdl->win_h)
	{
		m->player.current_dist = (float)m->sdl->win_h / (float)(2 * j - m->sdl->win_h);
		weight = (m->player.current_dist) / (m->flr.wall_dist);
		m->flr.floor_x = weight * m->flr.floor_wall_x + (1.0f - weight)
														* m->player.x;
		m->flr.floor_y = weight * m->flr.floor_wall_y + (1.0f - weight)
														* m->player.y;
		floor_tex_x = (int)(m->flr.floor_x * COLUM) % COLUM;
		floor_tex_y = (int)(m->flr.floor_y * COLUM) % COLUM;
		if (floor_tex_x < 0)
			floor_tex_x = 0;
		if (floor_tex_y < 0)
			floor_tex_y = 0;
		tmp_arr = (m->walls[2]).texture2;
		m->player.intensity = fog_calculate_for_floor(m);
		color = rgb_mod(m->player.intensity, tmp_arr[floor_tex_y][floor_tex_x]);
		sdl_put_pixel(m->sdl, i, j, color);
		tmp_arr = (m->walls[2]).texture4;
		color = rgb_mod(m->player.intensity, tmp_arr[floor_tex_y][floor_tex_x]);
		sdl_put_pixel(m->sdl, i, m->sdl->win_h - j, color);
		j++;
	}
}

void		raycast_and_draw(t_rc_main *m)
{
	int		i;
	int 	j;
	float	ray_dir_x;
	float	ray_dir_y;

	i = -1;
	while (++i < m->sdl->win_w)
	{
		m->wls.map_x = (int)m->player.x;
		m->wls.map_y = (int)m->player.y;
		m->player.camera_x = 2.f * i / (float)m->sdl->win_w - 1.f;
		ray_dir_x = m->player.fdir_x + m->player.plane_x * m->player.camera_x;
		ray_dir_y = m->player.fdir_y + m->player.plane_y * m->player.camera_x;
		m->player.delta_dist_x = ABS(1.f / ray_dir_x);
		m->player.delta_dist_y = ABS(1.f / ray_dir_y);
		find_dist_y(m, ray_dir_x, ray_dir_y);
		j = draw_walls(m, ray_dir_x, ray_dir_y, i);
		flr_or_clng_offset_calculate(m, ray_dir_x, ray_dir_y);
		draw_floor_or_celling(m, j, i);
		m->z_buffer[i] = m->flr.wall_dist;
	}
}
