/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalata <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 14:29:09 by ysalata           #+#    #+#             */
/*   Updated: 2017/05/23 17:53:14 by ysalata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include "libft/libft.h"
#include "libft/ft_printf/ft_printf.h"
#include <wchar.h>
int 	g_c;
int		g_i;
int 	g_y;
int     g_retx;
int     g_rety;

void	message(int i)
{
    int  nfd;
    char *s;

    if (i == 1)
        s = "attachments/start";
    else if (i == 2)
        s = "attachments/end";
    else if (i == 3)
        s = "attachments/win";
    else
        s = "attachments/lose";
    nfd = open(s, O_RDONLY);
    while (get_next_line(nfd, &s))
        ft_printf("%s\n", s);
    close(nfd);
}

int			valid(char *s)
{
	if (ft_strcmp(s, "WW2") == 0)
		g_c = 1;
	else if (ft_strcmp(s, "1969") == 0)
		g_c = 2;
	else if (ft_strcmp(s, "UAH") == 0)
		g_c = 3;
	else if (ft_strcmp(s, "alpha") == 0)
		g_c = 4;
	else
		return (0);
	return (1);
}

int boarddrow(char *s, int y)
{
	int x;
	int n;
	int flag;

	flag = 1;
	if (!y)
	{
		if (s[1] == 'l' ? x = ft_atoi(s + 8) : 0)
			y = ft_atoi(s + ((x != 100) ? 11 : 12));
		else if (s[1] == 'i' ? x = ft_atoi(s + 6) : 0)
			y = ft_atoi(s + ((x < 10) ? 8 : 9));
		else
			x = 0;
		y *= 2;
		s[1] == 'l' ? ft_printf("\nMAP [%d x %d]", x, y) : 0;
		s[1] == 'i' ? ft_printf("\nFIELD [%d x %d]", x, y) : 0;
	}
	n = y;
	write(1, "\n", 1);
	while (y-- > 0)
		if (flag ? write(1, "+", 1) : 0)
			flag--;
		else if (++flag)
			ft_putstr(" - ");
	ft_putstr("+\n");
	return (n);
}

int draw(char *s)
{
	//wchar_t symb[5] = {☭☺₴A࿘☻$Z}
	wint_t *symb[9] = {L"☭", L"☭", L"☺", L"₴", L"A", L"࿘", \
		L"☻", L"$", L"Z"};
	s[0] == '<' ? ft_printf("FOR PLAYER: |") : 0;
	while (s[++g_i])
		if (s[g_i] == 'x' || s[g_i] == 'X')
			ft_printf(COL_RED" %S |"COL_EOC, symb[g_c]);
		else if ((s[g_i] == 'o' && s[g_i - 1] != 'g' && \
					s[g_i - 1] != 'i') || s[g_i] == 'O')
			ft_printf(COL_MAGENTA" %S |"COL_EOC, \
			symb[g_c + 4]);
		else if (s[g_i] == '.')
			s[g_i] == '.' ? ft_printf("   |") : 0;
		else if (s[g_i] == '*' ? \
			ft_printf(COL_WHITE" # "COL_EOC) : 0)
			write(1, "|", 1);
		s[0] == '<' ? write(1, "\n", 1) : 0;
		//else
		//	write(1, &s[g_i], 1);
		return (1);
}

int main (int ac, char **av)
{
    int     a;
	char	*s;
	int		type;

    a = 2;
	if (((ac != 2) || !valid(av[1]))? \
		ft_printf("usage: ./bonus [ww2 || 1969 || gender]") : 0)
		return (0);
    message(1);
	while (get_next_line(0, &s))
	{
		g_i = -1;
		if (s[0] == ' ')
			continue ;
		s[0] == 'P' ? g_y = boarddrow(s, 0) : 0;
		if (s[0] == '=' ? a--: 0)
		{
			while (!ft_isdigit(*s))
				s++;
			a == 1? g_retx = ft_atoi(s) : 0;
			a == 0 ? g_rety = ft_atoi(s) : 0;
			continue ;
		}
		if ((ft_isdigit(*s) ? s += 4 : 0) || *s == '.' || *s == '*')
			write(1, "|", 1);
		draw(s);
		s[0] == 'P' || s[0] == '<' ? 0 : boarddrow(s, g_y);
      //  if (s[0] == '=' ? a--: 0)
        //    if (a == 0 ? :  g_p1)
        //s[0] == '=' ?
	}
    message(2);
    message(3);
	//output()
	//ft_printf("|S|C|O|R|E|:| ", g_rp1, g_rp2);
	//ft_printf("");
	return (0);
}
