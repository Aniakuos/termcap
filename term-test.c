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
// void append(t_sdlist **head_ref, char *new_data)
// {
//     /* 1. allocate node */
//     t_sdlist *new_node = (t_sdlist*)malloc(sizeof(t_sdlist));
 
//     t_sdlist *last = *head_ref; /* used in step 5*/
 
//     /* 2. put in the data  */
//     new_node->line = ft_strdup(new_data);
 
//     /* 3. This new node is going to be the last node, so
//           make next of it as NULL*/
//     new_node->next = NULL;
 
//     /* 4. If the Linked List is empty, then make the new
//           node as head */
//     if (*head_ref == NULL) {
//         new_node->prev = NULL;
//         *head_ref = new_node;
//         return;
//     }
 
//     /* 5. Else traverse till the last node */
//     while (last->next != NULL)
//         last = last->next;
 
//     /* 6. Change the next of last node */
//     last->next = new_node;
 
//     /* 7. Make last node as previous of new node */
//     new_node->prev = last;
 
//     return;
// }

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
	//write(1, tgetstr("ce", NULL), ft_strlen(tgetstr("ce", NULL)));
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
void insertAfter(t_sdlist *prev_node, char *new_data)
{
	
    /*1. check if the given prev_node is NULL */
    // if (prev_node == NULL) {
    //     printf("the given previous node cannot be NULL");
    //     return;
    // }
 
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

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int nb;

	if (n < 0)
	{
		write(fd, "-", 1);
		nb = n * (-1);
	}
	else
		nb = n;
	if (nb < 10)
		ft_putchar_fd(nb + 48, fd);
	else
	{
		ft_putnbr_fd(nb / 10, fd);
		ft_putchar_fd(nb % 10 + 48, fd);
	}
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
	char *tmp;
	int c = 0;
	int row;
	int col;
	tab = ft_strdup("");
	char *s;
	int i = 0;
	t_sdlist *last;
	if(ft_lstsize(hist) == 0)
	{
		hist = ft_lstnew(ft_strdup(""));
		hist->prev = NULL;
	}
	last = ft_lstlast(hist);
	while (read(0, &c, sizeof(c)) > 0)
	{
		get_cursor_position(&col, &row);
		if (c == LEFT_ARROW)
			move_cursor_left(&col, &row, cm);
		else if (c == RIGHT_ARROW)
			move_cursor_right(&col, &row, cm);
		else if (c == BACKSPACE)
			delete_end(&col, &row, cm, ce);
		else if(c == ENTER_ARROW)
		{
			//ft_putnbr_fd(ft_lstsize(hist), 1);
			if(ft_lstsize(hist) == 1) {
				ft_lstadd_front(&hist, ft_lstnew(ft_strdup(tab)));
				hist->prev = NULL;
				ft_lstlast(hist)->prev = hist;
			}
			else if (ft_lstsize(hist) > 1){
				insertAfter(ft_lstlast(hist)->prev, tab);
			}
			//ft_putstr("\r");
			free(tab);
			printList(hist);
			tab = ft_strdup("");
			last = ft_lstlast(hist);
			//break;
		}
		else if (c == UP_ARROW)
		{

			if(last && last->prev) {
				ft_putstr("\r");
				write(1, tgetstr("ce", NULL), ft_strlen(tgetstr("ce", NULL)));
				last = last->prev;
				ft_putstr(last->line);
			}
		}
		else if (c == DOWN_ARROW)
		{	
			if (last && last->next) {
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