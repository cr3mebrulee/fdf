/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handling2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 17:04:16 by frafal            #+#    #+#             */
/*   Updated: 2022/11/25 17:04:21 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define CHUNK_SIZE 1024 // Define the chunk size for memory allocation

char	*get_file(int fd, t_data *data)
{
	char *line;
	char *file = NULL;
    	int total_size = 0;
    	int current_size = 0;
	int cols;

	data->map->y_dim = 0;

	// Allocate memory for initial chunk
	file = malloc(CHUNK_SIZE);
	if (file == NULL) 
		return NULL; // Allocation failed
	total_size = CHUNK_SIZE;

	while ((line = get_next_line(fd)) != NULL) 
	{
		cols = count_cols(line);
		if (!lines_consistent(cols, data)) 
		{
			free(file);
			return NULL;
		}
		data->map->x_dim = cols;
		data->map->y_dim += 1;

		// Calculate the length of the line
		int line_length = ft_strlen(line);

		// Check if the line can fit into the remaining space in the buffer
		if (current_size + line_length >= total_size) 
		{
			// Increase total_size to accommodate the new line
			total_size += (line_length > CHUNK_SIZE) ? line_length : CHUNK_SIZE;
			char *temp = malloc(total_size);
			if (temp == NULL) 
			{
				free(file);
				return NULL;
			}
		// Copy existing data to the new buffer
		ft_memcpy(temp, file, current_size);
		free(file); // Free the old buffer
		file = temp;
		}

		// Copy the line to the buffer
		ft_strlcpy(file + current_size, line, total_size - current_size);
		current_size += line_length;

		free(line); // Free memory allocated by get_next_line
	}

	return file;
}

int	lines_consistent(int cols, t_data *data)
{
	if (data->map->y_dim != 0 && data->map->x_dim != cols)
	{
		ft_putstr_fd("Found wrong line length. Exiting.\n", 2);
		return (0);
	}
	return (1);
}

void	append_line_to_file(char **line, char **file)
{
	char	*tmp;

	tmp = *file;
	*file = ft_strjoin(tmp, *line);
	free(tmp);
	free(*line);
	*line = "";
}
