#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/**
 * @struct st_linked_list_node
 * @brief Linked list node for Entry structures.
 */
typedef struct st_linked_list_node {
  void* vp_data;        /**< Entry vp_data. */
	struct st_linked_list_node *stp_ll_next; /**< Pointer to the next node. */
} t_linked_list_node;

/**
 * @brief Initializes an list.
 * @param vp_data - pointer to vp_data
 * @return - Pointer to the tpp_ll_head of the list if successful - NULL otherwise
 */
t_linked_list_node *linked_list_init(void *vp_data);

/**
 * @brief Prepends an item to the list.
 * @param tpp_ll_head - Pointer to the tpp_ll_head of the list.
 * @param vp_data - pointer to vp_data
 * @return - 0 if successful - otherwise 1
 */
int linked_list_prepend_node(t_linked_list_node **tpp_ll_head, void* vp_data);

/**
 * @brief append an item to the list.
 * @param tpp_ll_head - Pointer to the tpp_ll_head of the list.
 * @param vp_data - pointer to vp_data
 * @return - 0 if successful - otherwise 1
 */
int linked_list_append_node(t_linked_list_node **tpp_ll_head, void *vp_data);

/**
 * @brief Gets the length of the list.
 * @param tp_ll_head - Pointer to the tp_ll_head of the list.
 * @return - Length of the list.
 */
unsigned int linked_list_get_length(t_linked_list_node *tp_ll_head);

/**
 * @brief remove a the i-th element in the list while preserving linked list conectivity, counting starts at 0.
 * this doesnt free the vp_data.
 * @param tpp_ll_head - Pointer to the tpp_ll_head of the list.
 * @param i_index - the element at this location is deleted
 * @return - 0 if successful - 1 if index is out of range- otherwise 2
 */
int linked_list_remove_node(t_linked_list_node **tpp_ll_head, int i_index);

/**
 * @brief Iterates over the list and applies a callback function to each entry.
 * @param tp_ll_head - Pointer to the tp_ll_head of the list.
 * @param fnp_callback - Callback function to apply.
 * @param vp_parameter_passed_to_callback - Additional parameter for the callback.
 */
void linked_list_itterate_function(t_linked_list_node *tp_ll_head,
											 void *fnp_callback(void *vp_parameter, void *vp_data),
											 void *vp_parameter_passed_to_callback);

/**
 * @brief Get node at position index (starting at 0)
 * @param tpp_ll_head - Pointer to the head of the list.
 * @param i_index - Index of the node to retrieve.
 * @return - Pointer to the node at the specified index, NULL if index is out of range.
 */
t_linked_list_node *linked_list_get(t_linked_list_node **tpp_ll_head, int i_index);

/**
 * @brief Get first node in the list
 * @param tpp_ll_head - Pointer to the head of the list.
 * @return - Pointer to the first node, NULL if list is empty.
 */
t_linked_list_node *linked_list_get_first(t_linked_list_node **tpp_ll_head);

/**
 * @brief Get last node in the list
 * @param tpp_ll_head - Pointer to the head of the list.
 * @return - Pointer to the last node, NULL if list is empty.
 */
t_linked_list_node *linked_list_get_last(t_linked_list_node **tpp_ll_head);

/**
 * @brief Remove and return the node at the specified index from the list (starting at 0)
 * @param tpp_ll_head - Pointer to the head of the list.
 * @param i_index - Index of the node to pop.
 * @return - Pointer to the removed node, NULL if index is out of range.
 */
t_linked_list_node *linked_list_pop(t_linked_list_node **tpp_ll_head, int i_index);

/**
 * @brief Remove and return the first node from the list
 * @param tpp_ll_head - Pointer to the head of the list.
 * @return - Pointer to the removed first node, NULL if list is empty.
 */
t_linked_list_node *linked_list_pop_first(t_linked_list_node **tpp_ll_head);

/**
 * @brief Remove and return the last node from the list
 * @param tpp_ll_head - Pointer to the head of the list.
 * @return - Pointer to the removed last node, NULL if list is empty.
 */
t_linked_list_node *linked_list_pop_last(t_linked_list_node **tpp_ll_head);

#endif

