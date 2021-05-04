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

typedef struct		s_dlist
{
	char			*line;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}					t_sdlist;

char		*tab;
t_sdlist	*hist;

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
/* Given a reference (pointer to pointer) to the head
   of a DLL and an int, appends a new node at the end  */
void append(t_sdlist **head_ref, char *new_data)
{
    /* 1. allocate node */
    t_sdlist *new_node = (t_sdlist*)malloc(sizeof(t_sdlist));
 
    t_sdlist *last = *head_ref; /* used in step 5*/
 
    /* 2. put in the data  */
    new_node->line = ft_strdup(new_data);
 
    /* 3. This new node is going to be the last node, so
          make next of it as NULL*/
    new_node->next = NULL;
 
    /* 4. If the Linked List is empty, then make the new
          node as head */
    if (*head_ref == NULL) {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }
 
    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;
 
    /* 6. Change the next of last node */
    last->next = new_node;
 
    /* 7. Make last node as previous of new node */
    new_node->prev = last;
 
    return;
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

void printList(t_sdlist *node)
{
    //t_sdlist *last;
    //printf("\nTraversal in forward direction \n");
    while (node != NULL) {
		ft_putstr("------\n");
		ft_putstr("Prev : ");
		if(node->prev)
		{
			ft_putstr(node->prev->line);
		}
		ft_putstr(" | ");
		ft_putstr("line : ");
		ft_putstr(node->line);
		//ft_putstr("\n");
		ft_putstr(" | Next : ");
		if(node->next)
		{
			ft_putstr(node->next->line);	
		}
			
		//ft_putstr("\n");
		
		ft_putstr("\n");
        node = node->next;
    }
 
    // printf("\n--- \n");
    // while (last != NULL) {
	// 	//ft_putstr(last->line);
    //     //printf("|%s|", last->line);
    //     last = last->prev;
    // }
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
	lst = hist;
	if (!lst)
		lst = (void *)0;
	else
	{
		while (lst->next)
			lst = lst->next;
	}
	return (lst);
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
	char *ce = tgetstr("ce", NULL);
	//write(1,tgetstr("ce", NULL), ft_strlen(tgetstr("ce", NULL))); //clear line from cursor
	
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
			//ft_putstr(tab);
			ft_putstr("\n");
			append(&hist, tab);
			ft_putstr("\r");
			printList(hist);
			free(tab);
			tab = ft_strdup("");

			//break;
		}
		else if (c == 4283163)
		{
			t_sdlist *last;
			t_sdlist *last1;

			last = ft_lstlast(last1);
			//if(last != NULL) {
				ft_putstr("\r");
				ft_putstr(last->line);
				last = last->prev;
				
			//}
		}
		// else if (c == 4348699)
		// {
		// 	t_sdlist *new;

		// 	new = hist;
		// 	if (new != NULL) {
		// 		ft_putstr(new->line);
		// 		//new = new->next;
		// 	}
		// }
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