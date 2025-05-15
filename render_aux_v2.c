/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_aux_v2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albealva <albealva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:05:37 by albealva          #+#    #+#             */
/*   Updated: 2025/05/15 09:15:58 by albealva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void	draw_texture_on_wall_pre(t_game *game, t_render *render)
{
	render->tex_x = (int)(render->wall_x * (double)TEX_WIDTH);
	if ((render->side == 0 && render->raydir_x > 0) || (render->side == 1
			&& render->raydir_y < 0))
		render->tex_x = TEX_WIDTH - render->tex_x - 1;
	render->wall_info.wall_type = game->map[render->map_y][render->map_x];
	render->wall_info.side = render->side;
	render->wall_info.raydir_x = render->raydir_x;
	render->wall_info.raydir_y = render->raydir_y;
	render->tex = get_texture_for_wall(game, render->wall_info);
	render->y = render->drawstart;
	while (render->y < render->drawend)
	{
		render->d = render->y * 256 - WIN_HEIGHT * 128 + render->lineheight
			* 128;
		render->tex_y = ((render->d * TEX_HEIGHT) / render->lineheight) / 256;
		if (render->tex_y >= TEX_HEIGHT)
			render->tex_y = TEX_HEIGHT - 1;
		render->color = render->tex->data[render->tex_y
			* (render->tex->line_length / 4) + render->tex_x];
		game->img_data[render->y * WIN_WIDTH + render->x] = render->color;
		render->y++;
	}
}

void draw_texture_on_wall(t_game *game, t_render *render)
{
    render->tex_x = (int)(render->wall_x * (double)TEX_WIDTH);
    if ((render->side == 0 && render->raydir_x > 0) || (render->side == 1
            && render->raydir_y < 0))
        render->tex_x = TEX_WIDTH - render->tex_x - 1;

    render->wall_info.wall_type = game->map[render->map_y][render->map_x];
    render->wall_info.side = render->side;
    render->wall_info.raydir_x = render->raydir_x;
    render->wall_info.raydir_y = render->raydir_y;
    render->tex = get_texture_for_wall(game, render->wall_info);

    if (!render->tex || !render->tex->data)
        return;
    if (render->tex_x < 0 || render->tex_x >= render->tex->width)
        return;

    render->y = render->drawstart;
    while (render->y < render->drawend)
    {
        render->d = render->y * 256 - WIN_HEIGHT * 128 + render->lineheight * 128;
        render->tex_y = ((render->d * TEX_HEIGHT) / render->lineheight) / 256;

        if (render->tex_y < 0 || render->tex_y >= render->tex->height)
            break;

        render->color = render->tex->data[render->tex_y * (render->tex->line_length / 4) + render->tex_x];
        game->img_data[render->y * WIN_WIDTH + render->x] = render->color;
        render->y++;
    }
}


void	draw_texture_on_wall_borrar(t_game *game, t_render *render)
{
	if (!render || !render->tex || !render->tex->data)
		clean_exit(game, "Error: Invalid texture data.");
	render->tex_x = (int)(render->wall_x * (double)TEX_WIDTH);
	if ((render->side == 0 && render->raydir_x > 0) || (render->side == 1
			&& render->raydir_y < 0))
		render->tex_x = TEX_WIDTH - render->tex_x - 1;
	render->wall_info.wall_type = game->map[render->map_y][render->map_x];
	render->wall_info.side = render->side;
	render->wall_info.raydir_x = render->raydir_x;
	render->wall_info.raydir_y = render->raydir_y;
	render->tex = get_texture_for_wall(game, render->wall_info);
	render->y = render->drawstart;
	while (render->y < render->drawend)
	{
		render->d = render->y * 256 - WIN_HEIGHT * 128 + render->lineheight
			* 128;
		render->tex_y = ((render->d * TEX_HEIGHT) / render->lineheight) / 256;
		if (render->tex_y >= TEX_HEIGHT)
			render->tex_y = TEX_HEIGHT - 1;
		render->color = render->tex->data[render->tex_y
			* (render->tex->line_length / 4) + render->tex_x];
		game->img_data[render->y * WIN_WIDTH + render->x] = render->color;
		render->y++;
	}
}

void	init_minimap_vars(t_render *render)
{
	render->minimap_area_width = MAP_WIDTH * 10;
	render->minimap_area_height = MAP_HEIGHT * 10;
	render->minimap_offset_x = 10;
	render->minimap_offset_y = WIN_HEIGHT - render->minimap_area_height - 10;
}

void	render_walls(t_game *game, t_render *render)
{
	while (render->x < WIN_WIDTH)
	{
		calculate_ray_vars(game, render);
		perform_ray_casting(game, render);
		calculate_wall_properties(game, render);
		draw_texture_on_wall(game, render);
		render->x++;
	}
}

void	clear_minimap_area(t_game *game, t_render *render)
{
	render->y = render->minimap_offset_y;
	while (render->y < render->minimap_offset_y + render->minimap_area_height)
	{
		render->x = render->minimap_offset_x;
		while (render->x < render->minimap_offset_x
			+ render->minimap_area_width)
		{
			game->img_data[render->y * WIN_WIDTH + render->x] = BLACK;
			render->x++;
		}
		render->y++;
	}
}
