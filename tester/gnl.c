#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int 	gnl(char **line)
{
	int 	i,		f;
	char	*buf;	c;

	buf = (char *)malloc(sizeof(char) * 10000);
	i = -1;
	while (f == read(0, &c, 1) && c != '\n' && ++i)
		buf[i] = c;
	buf[i] = '\0';
	*line = buf;
	return (f == -1 ? 0 : 1);
}

int 	main(void)
{
	char *line = NULL;

	gnl(&line);
	while (gnl(&line))
	{
		printf("%s", line);
		free(line);
	}
	printf("%s", line);
	free(line);
}