#include "filler.h"
#include <fcntl.h>

#define SMALL(a) ((a) == 'X' ? 1 : 2)
#define BIG(b) ((b) == 'X' ? 2 : 1)
#define LEN(a, b, c, d) ((a - c)*(a - c) + (b - d)*(b - d))

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

void		gameplay(t_info *g, int x, int y)
{
	if (g->w < 2)
	g->answ = (pos == 1) ? g_i + g_j : g_i - x;
		//g->answ = (pos == 1) ? ADD(g_i, g_j) : ADD(x - g_i, y - g_j);
/*
	else if (x < 25 && pos != 1)
	{
		g->answ = (g->w == 3) ? LEN(g_i,g_j,0, y - 1) : x - g_i;
		if (g->w == 3)
			ft_strchr(g->map->f[0], 'O') ? g->w-- : 0;
	}
		*/
	else if (pos != 1)
	{
		g->answ = (g->w == 3) ? LEN(x/3, y- 1, g_i, g_j) : g_j - g_i;
		//g->answ = (g->w == 3) ? y - g_j : g_j - g_i;
		(g->w == 3 && g->map->f[x/3][y-1] != '.') ? g->w-- : 0;
	}
	else if (x < 50)
	{
		g->answ = (g->w == 3) ? LEN(x/2, 0, g_i, g_j) : y - g_j;
		g->w == 3 && ft_strchr(g->map->f[x/3], 'X') ? g->w-- : 0;
		g->w != 3 && ft_strchr(g->map->f[x - 1], 'X') ? g->w-- : 0;
	}
	else if (x > 50) //works!
	{
		g->answ = (g->w == 3) ? LEN(45, 0, g_i, g_j) : LEN(x/3, y- 1, g_i, g_j);
		//g->answ = (g->w == 3) ? LEN(36, 0, g_i, g_j) : x - g_i;
		if ((g->w == 3 && g_i < 35) || (g->w != 3 && ft_strchr(g->map->f[20], 'O')))
			g->w--;
	}
	ft_putstr_fd("g->w: ",g_fd1);
	ft_putnbr_fd(g->w, g_fd1);
	ft_putstr_fd("x: ",g_fd1);
	ft_putnbr_fd(g_i, g_fd1);
	ft_putstr_fd(" y: ",g_fd1);
	ft_putnbr_fd(g_j, g_fd1);
	ft_putstr_fd(" len: ",g_fd1);
	ft_putnbr_fd(g->answ, g_fd1);
	ft_putstr_fd("\n",g_fd1);
	if (g->len < g->answ)
		return ;
		g->len = g->answ;
		g->x = g_i;
		g->y = g_j;
		//REDEF(g->x, g_i, g->y, g_j);
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
					 if (g->fig->f[g_n][g_m] == '*' && \
					 	g->map->f[g_i + g_n][g_j + g_m] != '.')
						 a = (g->map->f[g_i + g_n][g_j + g_m] !=\
						 	g->p) ? -1 : a  - 1; //+32
			 }
			//if (a == 0 ? printf("%d %d\n", g_i, g_j) : 0)
			if (a == 0)
				gameplay(g, g->map->x, g->map->y);
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
	fig->f = (char **)malloc(sizeof(char *) * fig->x + 1);
	while (++i < fig->x)
	{
		fig->f[i] = (char *) malloc(sizeof(char) * fig->y + 1);
		read(g_fd, fig->f[i], (size_t)(fig->y + 1));
		//fig->f[i] = ft_strdup(fig->trash);
		fig->f[i][fig->y] = '\0';
		//ft_strclr(fig->trash);
	}
	i = -1;
//	while (++i < fig->x)
//		printf("%s\n", fig->f[i]);
//	free(fig->f);
	ft_putstr_fd("FIGURE FIELD: \n",g_fd1);
	while (++i < fig->x)
	{
//		printf("%s\n", map->f[i]);
		ft_putstr_fd(fig->f[i], g_fd1);
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
	map->f = (char **)malloc(sizeof(char *) * map->x + 1);
	ft_putstr_fd("MAP FIELD: \n",g_fd1);
	while (++i < map->x)
	{
		map->f[i] = (char *) malloc(sizeof(char) * map->y + 1);
		//read(g_fd, , 4);
		//read(g_fd, trash, (map->y + 5));
		get_next_line(g_fd, &trash);
		map->f[i] = ft_strdup(trash + 4);
		//read(g_fd, map->f[i], (size_t) (map->y + 1));
		map->f[i][map->y] = '\0';
		//ft_putstr_fd(map->f[i], g_fd1);
		//ft_putstr_fd("\n",g_fd1);
		ft_strclr(trash);
	}
	i = -1;
	/*
	while (++i < map->x)
	{
//		printf("%s\n", map->f[i]);
		ft_putstr_fd(map->f[i], g_fd1);
		ft_putstr_fd("\n",g_fd1);
	}
	 */
//	free(map->f);
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
	t_i.w = 3;
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
		t_i.len = 100000000;
		t_i.map = (t_cord *)malloc(sizeof(t_cord));
		t_i.fig = (t_cord *)malloc(sizeof(t_cord));
		readmap(t_i.map, s) ? readform(t_i.fig) : 0;
		t_i.hight = t_i.map->x - t_i.fig->x;
		t_i.wid = t_i.map->y - t_i.fig->y;
		if (!pos)
			pos = t_i.map->x < 25 ? SMALL(t_i.p) : BIG(t_i.p);
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

		free(t_i.map);
		free(t_i.fig);
		ft_strclr(s);
		if (t_i.len == 100000000)
			return 0;
	}
}