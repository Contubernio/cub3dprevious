/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:47:19 by albealva          #+#    #+#             */
/*   Updated: 2025/05/15 09:22:17 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

t_texture	*get_texture_for_wall_segfault(t_game *game, t_wall_info info)
{
	int	tex_id;

	tex_id = 0;
	if (info.wall_type == 'D' && BONUS)
		return (&game->door_texture[0]);
	else if (info.wall_type == 'O' && BONUS)
		return (&game->door_texture[1]);
	else if (info.side == 0)
	{
		if (info.raydir_x > 0)
			tex_id = 0;
		else
			tex_id = 1;
	}
	else
	{
		if (info.raydir_y > 0)
			tex_id = 2;
		else
			tex_id = 3;
	}
	return (&game->textures[tex_id]);
}

t_texture	*get_texture_for_wall(t_game *game, t_wall_info info)
{
	int	tex_id;

	if (info.wall_type == 'D' && BONUS)
		return (&game->door_texture[0]);
	else if (info.wall_type == 'O' && BONUS)
		return (&game->door_texture[1]);
	else if (info.wall_type == '1' || info.wall_type == 'D' || info.wall_type == 'O')  // ✅ Solo aplicar esto si es un muro real
	{
		if (info.side == 0)
		{
			if (info.raydir_x > 0)
				tex_id = 0;
			else
				tex_id = 1;
		}
		else
		{
			if (info.raydir_y > 0)
				tex_id = 2;
			else
				tex_id = 3;
		}
		return (&game->textures[tex_id]);
	}
	return (&game->textures[4]);  // ✅ Textura por defecto para celdas desconocidas
}


void	init_render_vars(t_game *game, t_render *render)
{
	render->total_pixels = WIN_WIDTH * WIN_HEIGHT;
	render->fov_length = tan(game->william.fov / 2.0);
	memset(game->img_data, 0, render->total_pixels * sizeof(int));
	draw_background(game);
	game->william.plane_x = -game->william.dir_y * render->fov_length;
	game->william.plane_y = game->william.dir_x * render->fov_length;
	render->x = 0;
}

static void	calculate_delta_distances(t_render *render)
{
	if (render->raydir_x == 0)
		render->deltadist_x = 1e30;
	else
		render->deltadist_x = fabs(1.0 / render->raydir_x);
	if (render->raydir_y == 0)
		render->deltadist_y = 1e30;
	else
		render->deltadist_y = fabs(1.0 / render->raydir_y);
}

static void	calculate_step_and_side_distances(t_game *game, t_render *render)
{
	if (render->raydir_x < 0)
		render->step_x = -1;
	else
		render->step_x = 1;
	if (render->raydir_x < 0)
		render->sidedist_x = (game->william.x - render->map_x)
			* render->deltadist_x;
	else
		render->sidedist_x = (render->map_x + 1.0 - game->william.x)
			* render->deltadist_x;
	if (render->raydir_y < 0)
		render->step_y = -1;
	else
		render->step_y = 1;
	if (render->raydir_y < 0)
		render->sidedist_y = (game->william.y - render->map_y)
			* render->deltadist_y;
	else
		render->sidedist_y = (render->map_y + 1.0 - game->william.y)
			* render->deltadist_y;
	render->hit = 0;
}

void	calculate_ray_vars(t_game *game, t_render *render)
{
	init_ray_direction(game, render);
	calculate_delta_distances(render);
	calculate_step_and_side_distances(game, render);
}
