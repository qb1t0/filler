#ifndef FILLER_H
# define FILLER_H

# include "libft/libft.h"
# include <unistd.h>

typedef	struct s_cord
{
	int 			x;
	int 			y;
	char 			*trash;
	char 			**field;
}					t_cord;

typedef struct	s_info
{
	int				p;
	int				x;
	int 			y;
	int				len;
	int 			wid;
	int 			hight;
	t_cord		*fig;
	t_cord		*map;
}					t_info;

# endif