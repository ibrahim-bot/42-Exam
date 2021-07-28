#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct		s_zone
{
	int			width;
	int			height;
	char		background;
	char		**array;
}					t_zone;

typedef struct		s_draw
{
	char		render;
	float		x;
	float		y;
	float		width;
	float		height;
	char		trace;
}					t_draw;


int		ft_error(const char *str)
{
	int		i;

	i = -1;
	while (str && str[++i])
		write(1, &str[i], 1);
	return (EXIT_FAILURE);
}

int		set_zone(t_zone *paper)
{
	int		x;
	int		y;

	if ((paper->array = malloc(sizeof(char *) * (paper->height + 1))) == NULL)
		return (EXIT_FAILURE);
	y = -1;
	while (++y < paper->height)
	{
		if ((paper->array[y] = malloc(sizeof(char) * (paper->width + 1))) == NULL)
			return (EXIT_FAILURE);
		x = -1;
		while (++x < paper->width)
			paper->array[y][x] = paper->background;
		paper->array[y][x] = '\0';
	}
	paper->array[y] = NULL;
	return (EXIT_SUCCESS);
}

void	print_paper(t_zone paper)
{
	int		x;
	int		y;

	y = -1;
	while (paper.array[++y])
	{
		x = -1;
		while (paper.array[y][++x])
			write(1, &paper.array[y][x], 1);
		write(1, "\n", 1);
	}
}

int		is_good(int x, int y, t_draw pencil)
{
	float	limit;

	if ((x < pencil.x) || (x > pencil.x + pencil.width)
	|| (y < pencil.y) || (y > pencil.y + pencil.height))
		return (0);
	limit = 1.00000000;
	if (((x - pencil.x < limit) || (pencil.x + pencil.width - x < limit))
	|| ((y - pencil.y < limit) || (pencil.y + pencil.height - y < limit)))
		return (2);
	return (1);
}

void	go_to_draw(t_zone paper, t_draw pencil)
{
	int		x;
	int		y;
	int		good;

	y = -1;
	while (++y < paper.height)
	{
		x = -1;
		while (++x < paper.width)
		{
			good = is_good(x, y, pencil);
			if ((pencil.render == 'r' && good == 2) || (pencil.render == 'R' && good > 0))
				paper.array[y][x] = pencil.trace;
		}
	}
}

int		main(int argc, char **argv)
{
	FILE	*fd;
	t_zone	paper;
	t_draw	pencil;
	int		i;
	int		ret;

	if (argc != 2)
		return (ft_error("Error: Argument\n"));
	if ((fd = fopen(argv[1], "r")) == NULL)
		return (ft_error("Error: Operation file corrupted\n"));
	ret = 0;
	if ((ret = fscanf(fd, "%d %d %c\n", &paper.width, &paper.height, &paper.background)) != 3)
		return (ft_error("Error: Operation file corrupted\n"));
	if ((paper.width < 1 || paper.width > 300) || (paper.height < 1 || paper.height > 300))
		return (ft_error("Error: Operation file corrupted\n"));
	if (set_zone(&paper) == EXIT_FAILURE)
		return (ft_error("Error: Malloc failed\n"));
	ret = 0;
	while ((ret = fscanf(fd, "%c %f %f %f %f %c\n", &pencil.render, &pencil.x, &pencil.y, &pencil.width, &pencil.height, &pencil.trace)) == 6)
	{
		if ((pencil.render != 'r' && pencil.render != 'R') || pencil.width <= 0.00000000 || pencil.height <= 0.00000000)
			break ;
		go_to_draw(paper, pencil);
	}
	if (ret == -1)
		print_paper(paper);
	if (paper.array != NULL)
	{
		i = -1;
		while (paper.array[++i])
			free(paper.array[i]);
		free(paper.array);
	}
	if (fd != NULL)
		fclose(fd);
	if (ret != -1)
		return (ft_error("Error: Operation file corrupted\n"));
	return (EXIT_SUCCESS);
}