#include "filler.h"
#include <fcntl.h>

#define BUFF_SIZE 1
#define SMALLF(a) ((a) == 'X' ? 1 : 2)
#define SBIGF(b) ((b) == 'X' ? 2 : 1)
#define ADD(a, b) a + b

int g_fd;
int g_fd1;
int g_i;
int g_j;
int g_n;
int g_m;
int pos;

/*
** ./resources/filler_vm -p1 ./resources/players/carli.filler -p2 ./filler -f ./maps/map00
*/







/* GOVNO STARTED
 */
int		tobigarr(char **arr)
{
	char	*temp;

	temp = NULL;
	if (!(temp = ft_strnew(ft_strlen(*arr))))
		return (0);
	ft_strcpy(temp, *arr);
	ft_strdel(arr);
	if (!(*arr = ft_strnew(ft_strlen(temp) + BUFF_SIZE)))
		return (0);
	ft_strcpy(*arr, temp);
	ft_strdel(&temp);
	return (1);
}

int		copyinline(char **arr, char **line)
{
	int x;
	int i;

	x = 0;
	i = 0;
	while ((*arr)[x] != '\n')
	{
		(*line)[x] = (*arr)[x];
		x++;
	}
	(*line)[x] = '\0';
	x++;
	while ((*arr)[x])
	{
		(*arr)[i] = (*arr)[x];
		(*arr)[x] = '\0';
		x++;
		i++;
	}
	(*arr)[i] = '\0';
	while ((*arr)[i])
		(*arr)[i++] = '\0';
	return (1);
}

int		read_alg(int fd, char **arr)
{
	char	*mass;
	int		i;

	if (fd < 0 || BUFF_SIZE <= 0 || read(fd, *arr, 0) == -1
		|| BUFF_SIZE > 3000000)
		return (-1);
	if (!*arr)
		*arr = ft_strnew(BUFF_SIZE);
	mass = NULL;
	if (!(mass = ft_strnew(BUFF_SIZE)))
		return (-1);
	while ((i = read(fd, mass, BUFF_SIZE)) > 0)
	{
		if (!(tobigarr(arr)))
			return (-1);
		ft_strncat(*arr, mass, BUFF_SIZE);
		if (ft_memchr(mass, '\n', BUFF_SIZE))
		{
			ft_strdel(&mass);
			break ;
		}
		ft_bzero(mass, BUFF_SIZE);
	}
	ft_strdel(&mass);
	return (i);
}

void	without_lf(char *arr1)
{
	int x;

	x = 0;
	while (arr1[x])
		x++;
	if (arr1[x - 1] != '\n')
	{
		arr1[x] = '\n';
		arr1[x + 1] = '\0';
	}
}

int		get_next_pidor(const int fd, char **line)
{
	static char	*arr;
	int			x;
	int			i;

	x = 0;
	i = read_alg(fd, &arr);
	if (i == -1)
		return (-1);
	if ((ft_strcmp(arr, "\0") == 0))
	{
		ft_strdel(&arr);
		return (i);
	}
	if (i == 0)
		without_lf(arr);
	if (ft_memchr(arr, '\n', ft_strlen(arr)) && (ft_strcmp(arr, "\0") == 0))
	{
		ft_strdel(&arr);
		return (0);
	}
	if (!(*line = ft_strnew(ft_strlen(arr))) || !line)
		return (-1);
	copyinline(&arr, line);
	return (1);
}

/* END GOVNO
 */
void		gameplay(t_info *g)
{
	int sum;

	//printf("%d\n", pos);
	sum = (pos == 1) ?\
		  ADD(g_i, g_j) : ADD(g->map->x - g_i, g->map->y - g_j);

	ft_putstr_fd("x: ",g_fd1);
	ft_putnbr_fd(g_i, g_fd1);
	ft_putstr_fd(" y: ",g_fd1);
	ft_putnbr_fd(g_j, g_fd1);
	ft_putstr_fd(" len: ",g_fd1);
	ft_putnbr_fd(sum, g_fd1);
	ft_putstr_fd("\n",g_fd1);
	if (g->len < sum)
		return ;
	g->x = g_i;
	g->y = g_j;
	g->len = sum;
}

int		analyze(t_info *g)
{
	int a;

	g_i = -1;
	ft_putstr_fd("\nStart analyzing ... \n",g_fd1);
	while(++g_i <= g->hight)
	{
		g_j = -1;
		while (++g_j <= g->wid)
		{
			a = 1;
			g_n = -1;
			 while (++g_n < g->fig->x)
			 {
				 g_m = -1;
				 while (++g_m < g->fig->y)
					 if (g->fig->field[g_n][g_m] == '*' && g->map->field[g_i + g_n][g_j + g_m] != '.')
						 a = (g->map->field[g_i + g_n][g_j + g_m] !=\
						 	g->p) ? -1 : a  - 1; //+32
			 }
			//if (a == 0 ? printf("%d %d\n", g_i, g_j) : 0)
			if (a == 0)
				gameplay(g);
		}
	}
	return (1);
}

int 	readform(t_cord *fig)
{
	int		i;
	char *buf;

	i = -1;
	ft_putstr_fd("readform()\n ",g_fd1);
	//fig->trash = (char *)malloc(sizeof(char) * 100);
	get_next_line(g_fd, &buf);
	ft_putstr_fd("FIRST READ :\n",g_fd1);
	ft_putstr_fd(buf, g_fd1);
	fig->x = ft_atoi(buf + 6);
	fig->y = ft_atoi(buf + ((fig->x < 10) ? 8 : 9));
	ft_putstr_fd("fig x: ",g_fd1);
	ft_putnbr_fd(fig->x, g_fd1);
	ft_putstr_fd("fig y: ",g_fd1);
	ft_putnbr_fd(fig->y, g_fd1);
	ft_putstr_fd("\n",g_fd1);
	ft_strclr(buf);
	fig->field = (char **)malloc(sizeof(char *) * fig->x + 1);
	while (++i < fig->x)
	{
		fig->field[i] = (char *) malloc(sizeof(char) * fig->y + 1);
		read(g_fd, fig->field[i], (size_t)(fig->y + 1));
		//fig->field[i] = ft_strdup(fig->trash);
		fig->field[i][fig->y] = '\0';
		//ft_strclr(fig->trash);
	}
	i = -1;
//	while (++i < fig->x)
//		printf("%s\n", fig->field[i]);
//	free(fig->field);
	ft_putstr_fd("FIGURE FIELD: \n",g_fd1);
	while (++i < fig->x)
	{
//		printf("%s\n", map->field[i]);
		ft_putstr_fd(fig->field[i], g_fd1);
		ft_putstr_fd("\n",g_fd1);
	}
	return 1;
}

int		readmap(t_cord *map, char *s)
{
	int			i;
	char		*trash;

	i = -1;
	ft_putstr_fd("readmap()\n",g_fd1);
	map->x = ft_atoi(s + 8);
	map->y = ft_atoi(s + ((map->x !=  100) ? 11 : 12));
	//map->trash = (char *)malloc(sizeof(char) * 100);
	ft_putstr_fd("fig x: ",g_fd1);
	ft_putnbr_fd(map->x, g_fd1);
	ft_putstr_fd("fig y: ",g_fd1);
	ft_putnbr_fd(map->y, g_fd1);
	ft_putstr_fd("\n",g_fd1);
	get_next_line(g_fd, &trash);
	//ft_strclr(trash);
	map->field = (char **)malloc(sizeof(char *) * map->x + 1);
	ft_putstr_fd("MAP FIELD: \n",g_fd1);
	while (++i < map->x)
	{
		map->field[i] = (char *) malloc(sizeof(char) * map->y + 1);
		//read(g_fd, , 4);
		//read(g_fd, trash, (map->y + 5));
		get_next_line(g_fd, &trash);
		map->field[i] = ft_strdup(trash + 4);
		//read(g_fd, map->field[i], (size_t) (map->y + 1));
		map->field[i][map->y] = '\0';
		//ft_putstr_fd(map->field[i], g_fd1);
		//ft_putstr_fd("\n",g_fd1);
		ft_strclr(trash);
	}
	i = -1;
	/*
	while (++i < map->x)
	{
//		printf("%s\n", map->field[i]);
		ft_putstr_fd(map->field[i], g_fd1);
		ft_putstr_fd("\n",g_fd1);
	}
	 */
//	free(map->field);
	return 1;
}


int main(void)
{
	char		*s;
	t_info		t_i;

	int i = 0;
	//g_fd = open("../test", O_RDONLY);
	g_fd1 = open("buf", O_WRONLY);
	g_fd = 0;
	get_next_line(g_fd, &s) ? t_i.p = (s[10] - '0') : 0;
	t_i.p = (t_i.p >> 1) ? 'X' : 'O';
	pos = 0;
	ft_strclr(s);
	while (get_next_line(g_fd, &s))
	{

		ft_putstr_fd("\n STEP №",g_fd1);
		ft_putnbr_fd(i++, g_fd1);
		ft_putchar_fd('\n', g_fd1);
		ft_putstr_fd("STRING S :\n",g_fd1);
		ft_putstr_fd(s, g_fd1);
		ft_putchar_fd('\n', g_fd1);
		t_i.len = 1000;
		t_i.map = (t_cord *)malloc(sizeof(t_cord));
		t_i.fig = (t_cord *)malloc(sizeof(t_cord));
		readmap(t_i.map, s) ? readform(t_i.fig) : 0;
		t_i.hight = t_i.map->x - t_i.fig->x;
		t_i.wid = t_i.map->y - t_i.fig->y;
		if (!pos)
			pos = t_i.map->x < 25 ? SMALLF(t_i.p) : SBIGF(t_i.p);

		analyze(&t_i);
		ft_putnbr(t_i.x);
		write(1, " ", 1);
		ft_putnbr(t_i.y);
		write(1, "\n", 1);
		ft_putstr_fd("t_x to write: ", g_fd1);
		ft_putnbr_fd(t_i.x, g_fd1);
		ft_putchar_fd('\n', g_fd1);
		ft_putstr_fd("t_y to write: ", g_fd1);
		ft_putnbr_fd(t_i.y, g_fd1);
		ft_putchar_fd('\n', g_fd1);

		//free(t_i.map);
		//free(t_i.fig);
		// ft_strclr(s);
		if (t_i.len == 1000) {
			ft_putstr("0 0\n");
			return 0;
		}
	}
}