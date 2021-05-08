t_sdlist *prev_node;
	t_sdlist *new_node;
	prev_node = ft_lstlast(hist)->prev;
    /*1. check if the given prev_node is NULL */
    // if (prev_node == NULL) {
    //     printf("the given previous node cannot be NULL");
    //     return;
    // }
 
    /* 2. allocate new node */
    //t_sdlist *new_node = (t_sdlist*)malloc(sizeof(t_sdlist));
 
    /* 3. put in the data  */
    new_node = ft_lstnew(ft_strdup(new_data));
 
    /* 4. Make next of new node as next of prev_node */
	printf("%s\n", prev_node->line);
    new_node->next = prev_node->next;
 
    /* 5. Make the next of prev_node as new_node */
    prev_node->next = new_node;
 
    /* 6. Make prev_node as previous of new_node */
    new_node->prev = prev_node;
 
    /* 7. Change previous of new_node's next node */
    if (new_node->next != NULL)
        new_node->next->prev = new_node;