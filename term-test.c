#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termcap.h>

//gcc term-test.c -lncurses

# define BACKSPACE 127
# define LEFT_ARROW 4479771
# define RIGHT_ARROW 4414235
# define UP_ARROW 4283163
# define DOWN_ARROW 4348699

char *tab;

int ft_strlen(char *s)
{
	int i = 0;
	while(s[i])
		i++;
	return (i);
}

void	ft_putstr(char *s)
{
	int i;

	i = 0;
	if (s == NULL)
		return ;
	while (s[i] != '\0')
	{
		write(STDOUT_FILENO, &s[i], 1);
		i++;
	}
}

int	nbr_length(int n)
{
	int	i = 0;

	if (n <= 0)
		i++;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

void	get_cursor_position(int *col, int *rows)
{
	int		a = 0;
	int		i = 1;
	char	buf[255];
	int		ret;
	int		temp;

	write(0, "\033[6n", 4);  //report cursor location
	ret = read(0, buf, 254);
	buf[ret] = '\0';
	//ft_putstr(buf);
	while (buf[i])
	{
		if (buf[i] >= '0' && buf[i] <= '9')
		{
			if (a == 0)
				*rows = atoi(&buf[i]) - 1;
			else
			{
				temp = atoi(&buf[i]);
				*col = temp - 1;
			}
			a++;
			i += nbr_length(temp) - 1;
		}
		i++;
	}
}

int		putchar_tc(int tc)
{
	write(1, &tc, 1);
	return (0);
}

void	move_cursor_left(int *col, int *row, char *cm)
{
	if (*col == 0)
		return ;
	--(*col);
	tputs(tgoto(cm, *col, *row), 1, putchar_tc);

}

void	move_cursor_right(int *col, int *row, char *cm)
{
	++(*col);
	tputs(tgoto(cm, *col, *row), 1, putchar_tc);

}

void	delete_end(int *col, int *row, char *cm, char *ce)
{
	if (*col != 0)
		--(*col);
	tputs(tgoto(cm, *col, *row), 1, putchar_tc);
	tputs(ce, 1, putchar_tc);
	tab[ft_strlen(tab) - 1] = '\0';
}

char	*ft_strdup(const char *s1)
{
	int		i;
	int		j;
	char	*p;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
		i++;
	p = malloc(i * sizeof(char) + 1);
	if (p == 0)
		return (0);
	while (j < i)
	{
		p[j] = s1[j];
		j++;
	}
	p[j] = '\0';
	return (p);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*p;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	while (s1[i] != '\0')
		i++;
	while (s2[j] != '\0')
		j++;
	p = (char*)malloc(i + j + 1);
	if (p == 0)
		return (0);
	i = 0;
	while (s1[i] != '\0')
		p[k++] = s1[i++];
	j = 0;
	while (s2[j] != '\0')
		p[k++] = s2[j++];
	p[k] = '\0';
	return (p);
}




int		main(void)
{
	/* change term settings */
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	/* init termcap */
	tgetent(NULL, "xterm");
	char *cm = tgetstr("cm", NULL); //cursor motion
	char *ce = tgetstr("ce", NULL); //clear line from cursor
	
	int c = 0;
	int row;
	int col;
	tab = ft_strdup("");
	char *s;
	int i = 0;

	while (read(0, &c, sizeof(c)) > 0)
	{
		get_cursor_position(&col, &row);
		if (c == LEFT_ARROW)
			move_cursor_left(&col, &row, cm);
		else if (c == RIGHT_ARROW)
			move_cursor_right(&col, &row, cm);
		else if (c == BACKSPACE)
			delete_end(&col, &row, cm, ce);
		else if(c == 10)
		{
			ft_putstr("\r");
			ft_putstr(tab);
			ft_putstr("\n");
			//ft_putstr("\033[6n");
			break;
		}
		else
		{
			col++;
			write(0, &c, 1);
			if (c >= 32 && c <= 126)
			{
				s = (char*)malloc(sizeof(*s) * 2);
				s[0] = c;
				s[1] = '\0';
				tab = ft_strjoin(tab, s);
			}
		}
		c = 0; //flush buffer
	}
}