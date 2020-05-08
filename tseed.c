#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

char	*recurs(int depth, int *ret, int fd)
{
	char	buff[1];
	char	*out;
	int		test;

	test = read(fd, buff, 1);
	if (test == 0)
		buff[0] = 0;
	if (buff[0] == '\n' || buff[0] == 0)
	{
		if (!(out = malloc(sizeof(char) * depth + 1)))
			return (0);
		out[depth] = 0;
		*ret = 1;
		if (buff[0] == 0)
			*ret = 0;
		return (out);
	}
	else
	{
		out = recurs(depth + 1, ret, fd);
		out[depth] = buff[0];
	}
	return (out);
}

int		get_next_line(int fd, char **out)
{
	int ret;

	ret = 1;
	*out = recurs(0, &ret, fd);
	return (ret);
}

int	main(int ac, char **av)
{
	int		fd;
	char	*line;
	char	*arr[200];
	int		size = 0;
	int		u;
	int		x;

	if (ac < 2)
		printf("No file selected.\n");
	else
	{
		srand((unsigned int)time(NULL));
		if ((fd = open(av[1], O_RDONLY)) == -1)
		{
			printf("Couldn't open file.\n");
			exit(0);
		}
		while (get_next_line(fd, &line) > 0)
		{
			if (isalnum(*line))
				arr[size++] = line;
			else
				break ;
		}
		if (isalnum(*line))
			arr[size++] = line;
		else
			free(line);
		u = size;
		while (--u > 0)
		{
			x = rand() % size;
			line = arr[u];
			arr[u] = arr[x];
			arr[x] = line;
		}
		u = -1;
		while (++u < size)
		{
			if (u % 4 == 0)
				printf("Table %d\n", (u + 1) / 4);
			printf("%s\n", arr[u]);
			if ((u + 1) % 4 == 0 && u + 1 < size)
				write(1, "\n", 1);
			free(arr[u]);
		}
		close(fd);
	}
	return (0);
}
