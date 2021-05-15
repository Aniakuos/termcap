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
# define ENTER_ARROW 10

typedef struct		s_dlist
{
	char			*line;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}					t_sdlist;

char		*tab;
t_sdlist	*hist;
t_sdlist	*last;

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

void ft_printlist_hist(t_sdlist *node)
{
    while (node != NULL) {
		ft_putstr("------\n");
		ft_putstr("Prev : ");
		if(node->prev)
			ft_putstr(node->prev->line);
		ft_putstr(" | ");
		ft_putstr("line : ");
		ft_putstr(node->line);
		ft_putstr(" | Next : ");
		if(node->next)
			ft_putstr(node->next->line);
		ft_putstr("\n");
        node = node->next;
    }
}

int ft_strlen(char *s)
{
	int i = 0;
	while(s[i])
		i++;
	return (i);
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

void	get_cursor_position(int *col, int *rows, int temp)
{
	int		a = 0;
	int		i = 1;
	char	buf[255];
	int		ret;

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

void	move_cursor_left(int *col)
{
	if (*col == 0)
		return ;
	--(*col);
	write(1, tgetstr("le", NULL), ft_strlen(tgetstr("le", NULL)));
}

void	move_cursor_right(int *col)
{
	// while (*col < tgetnum("co"))
	// {
		write(1, tgetstr("nd", NULL), ft_strlen(tgetstr("nd", NULL)));	
		(*col)++;
	//}
}

void	delete_end(int *col)
{
	if (*col != 0)
		--(*col);
	write(1, tgetstr("le", NULL), ft_strlen(tgetstr("le", NULL)));
	write(1, tgetstr("ce", NULL), ft_strlen(tgetstr("ce", NULL)));
	tab[ft_strlen(tab) - 1] = '\0';
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

t_sdlist	*ft_lstlast(t_sdlist *lst)
{
	if (!lst)
		lst = (void *)0;
	else
	{
		while (lst->next)
			lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_front(t_sdlist **alst, t_sdlist *new)
{
	if (!new)
		new->next = NULL;
	new->next = *alst;
	*alst = new;
}
int		ft_lstsize(t_sdlist *lst)
{
	int		i;

	i = 0;
	while (lst != NULL)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}
t_sdlist	*ft_lstnew(void *line)
{
	t_sdlist *lst;

	lst = malloc(sizeof(t_sdlist));
	if (!lst)
		return (NULL);
	lst->line = line;
	lst->next = NULL;
	return (lst);
}
void ft_insertafter(t_sdlist *prev_node, char *new_data)
{
    /* 2. allocate new node */
    t_sdlist *new_node = (t_sdlist*)malloc(sizeof(t_sdlist));
    /* 3. put in the data  */
    new_node->line = ft_strdup(new_data) ;
    /* 4. Make next of new node as next of prev_node */
    new_node->next = prev_node->next;
    /* 5. Make the next of prev_node as new_node */
    prev_node->next = new_node;
    /* 6. Make prev_node as previous of new_node */
    new_node->prev = prev_node;
    /* 7. Change previous of new_node's next node */
    //if (new_node->next != NULL)
        new_node->next->prev = new_node;
}
                                                                                                                                        
void	ft_ini_term()
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
	
}
void	ft_enter_arrow()
{
	if(ft_lstsize(hist) == 1) 
	{
		if(tab[0] != '\0')
		{
			ft_lstadd_front(&hist, ft_lstnew(ft_strdup(tab)));
			hist->prev = NULL;
			ft_lstlast(hist)->prev = hist;
		}	
	}
	else if (ft_lstsize(hist) > 1)
	{
		if(tab[0] != '\0')
			ft_insertafter(ft_lstlast(hist)->prev, tab);	
	}
	//ft_putstr("\r");
	free(tab);
	//ft_printlist_hist(hist);
	tab = ft_strdup("");
	last = ft_lstlast(hist);
	ft_putstr("\n");
	//break;
}

void	ft_declaration()
{
	char *cm;
	char *ce;

	cm = tgetstr("cm", NULL); //cursor motion
	ce = tgetstr("ce", NULL);//clear line from cursor
	tab = ft_strdup("");
}

void	ft_up_arrow()
{
	if(last && last->prev) 
	{
		ft_putstr("\r");
		write(1, tgetstr("ce", NULL), ft_strlen(tgetstr("ce", NULL)));
		last = last->prev;
		ft_putstr(last->line);
	}
}

void	ft_down_arrow()
{
	if (last && last->next) 
	{
	//	ft_putstr("\nok\n");
		ft_putstr("\r");
		write(1, tgetstr("ce", NULL), ft_strlen(tgetstr("ce", NULL)));
		last = last->next;
		ft_putstr(last->line);
	}
	if(!last ->next)
	{
		ft_putstr("\r");
		ft_putstr(tab);
	}
}

void	ft_init_hist()
{
	ft_ini_term();
	ft_declaration();
	if(ft_lstsize(hist) == 0)
	{
		hist = ft_lstnew(ft_strdup(""));
		hist->prev = NULL;
	}
	last = ft_lstlast(hist);
}

void	ft_remplir_tab(char *s, int col, int c)
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

int		main(void)
{
	int c;
	int row;
	int col;
	char *s;
	
	c = 0;
	ft_init_hist();
	while (read(0, &c, sizeof(c)) > 0)
	{
		get_cursor_position(&col, &row, 0);
		if (c == LEFT_ARROW)
			move_cursor_left(&col);
		else if (c == RIGHT_ARROW)
			move_cursor_right(&col);
		else if (c == BACKSPACE)
			delete_end(&col);
		else if(c == ENTER_ARROW)
			ft_enter_arrow();
		else if (c == UP_ARROW)
			ft_up_arrow();
		else if (c == DOWN_ARROW)
			ft_down_arrow();
		else
			ft_remplir_tab(s, col, c);
		c = 0; //flush buffer
	}
}