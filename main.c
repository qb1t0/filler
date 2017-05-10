#include "filler.h"
#include <fcntl.h>


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

void		gameplay(t_info *g)
{
	int sum;

	//printf("%d\n", pos);
	sum = (pos == 1) ?\
		  ADD(g_i, g_j) : ADD(g->map->x - g_i, g->map->y - g_j);

	ft_putstr_fd("\nGAMPLAY START !!!\n",g_fd1);
	ft_putnbr_fd(g_i, g_fd1);
	ft_putchar_fd(' ', g_fd1);
	ft_putnbr_fd(g_j, g_fd1);
	ft_putchar_fd(' ', g_fd1);
	ft_putnbr_fd(sum, g_fd1);
	ft_putstr_fd("\nGAMPLAY END !!!\n",g_fd1);
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

	while(++g_i <= g->hight)
	{
		g_j = -1;
		while (++g_j < g->wid)
		{
			a = 1;
			g_n = -1;
			 while (++g_n < g->fig->x)
			 {
				 g_m = -1;
				 while (++g_m <= g->fig->y)
					 if (g->fig->field[g_n][g_m] == '*' && \
					 	g->map->field[g_i + g_n][g_j + g_m] != '.')
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

	i = -1;
	ft_putstr_fd("READFORM()",g_fd1);
	get_next_line(g_fd, &fig->trash);
	fig->x = ft_atoi(fig->trash + 6);
	fig->y = ft_atoi(fig->trash + ((fig->x < 10) ? 8 : 9));
	fig->field = (char **)malloc(sizeof(char *) * fig->x);
	while (++i < fig->x)
	{
		fig->field[i] = (char *) malloc(sizeof(char) * fig->y + 1);
		read(g_fd, fig->trash, (size_t) (fig->y + 1));
		fig->field[i] = ft_strdup(fig->trash);
		fig->field[i][fig->y] = '\0';
		ft_strclr(fig->trash);
	}
	i = -1;
//	while (++i < fig->x)
//		printf("%s\n", fig->field[i]);
	//free(fig->field);
	return 1;
}

int		readmap(t_cord *map, char *s)
{
	int			i;

	i = -1;
	ft_putstr_fd("READMAP()\n",g_fd1);
	map->x = ft_atoi(s + 8);
	map->y = ft_atoi(s + ((map->x < 100) ? 11 : 12));
	get_next_line(g_fd, &map->trash);
	map->field = (char **)malloc(sizeof(char *) * map->x);
	while (++i < map->x)
	{
		map->field[i] = (char *) malloc(sizeof(char) * map->y + 1);
		read(g_fd, map->trash, 4);
		read(g_fd, map->trash, (size_t) (map->y + 1));
		map->field[i] = ft_strdup(map->trash);
		map->field[i][map->y] = '\0';
	}
	i = -1;
//	while (++i < map->x)
//		printf("%s\n", map->field[i]);
	//free(map->field);
	return 1;
}

int main(void)
{
	char		*s;
	t_info		t_i;
int i = 0;
	//g_fd = open("../test", O_RDONLY);
	g_fd1 = open("buf", O_WRONLY);
	//g_fd = 0;
	get_next_line(g_fd, &s) ? t_i.p = (s[10] - '0') : 0;
	t_i.p = (t_i.p >> 1) ? 'X' : 'O';
	pos = 0;
	while (get_next_line(g_fd, &s))
	{
		ft_putstr_fd("STEP №",g_fd1);
		ft_putnbr_fd(i++, g_fd1);
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
		ft_putstr_fd("t_x to write:", g_fd1);
		ft_putnbr_fd(t_i.x, g_fd1);
		ft_putchar_fd('\n', g_fd1);
		ft_putstr_fd("t_y to write:", g_fd1);
		ft_putnbr_fd(t_i.y, g_fd1);
		ft_putchar_fd('\n', g_fd1);
		//free(t_i.map);
		//free(t_i.fig);
		if (t_i.len == 1000) {
			ft_putstr("0 0\n");
			return 0;
		}
	}
}