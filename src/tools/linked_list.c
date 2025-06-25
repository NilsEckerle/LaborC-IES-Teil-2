#include "tools/linked_list.h"
#include <stdlib.h>

t_linked_list_node *linked_list_init(void *vp_data) {
  t_linked_list_node *t_ll_node = malloc(sizeof(t_linked_list_node));
  if (t_ll_node == NULL) {
    return NULL;
  }

  t_ll_node->vp_data = vp_data;
  t_ll_node->stp_ll_next = NULL;

  return t_ll_node;
}

int linked_list_prepend_node(t_linked_list_node **tpp_ll_head, void *vp_data) {
  if (tpp_ll_head == NULL) { return 1; }
  if (*tpp_ll_head == NULL) { return 1; }
  if (vp_data == NULL) { return 1; }

  t_linked_list_node *tp_ll_new_node = linked_list_init(vp_data);
  if (tp_ll_new_node == NULL) { return 1; }

  // prepend node
  tp_ll_new_node->stp_ll_next = *tpp_ll_head;
  *tpp_ll_head = tp_ll_new_node;

  return 0;
}

int linked_list_append_node(t_linked_list_node **tpp_ll_head, void *vp_data) {
  if (tpp_ll_head == NULL) { return 1; }
  if (*tpp_ll_head == NULL) { return 1; }
  if (vp_data == NULL) { return 1; }

  t_linked_list_node *tp_ll_new_node = linked_list_init(vp_data);
  if (tp_ll_new_node == NULL) { return 1; }

  // traverse til end of list
  t_linked_list_node *pst_current = *tpp_ll_head;
  while (pst_current->stp_ll_next != NULL) {
    pst_current = pst_current->stp_ll_next;
  }

  // append node
  pst_current->stp_ll_next = tp_ll_new_node;

  return 0;
}

unsigned int linked_list_get_length(t_linked_list_node *tpp_ll_head) {
  if (tpp_ll_head == NULL) { return 0; }

  unsigned int i_length = 1;
  t_linked_list_node *tp_ll_current = tpp_ll_head;

  // traverse list til end
  while (tp_ll_current->stp_ll_next != NULL) {
    i_length++;
    tp_ll_current = tp_ll_current->stp_ll_next;
  }

  return i_length;
}

int linked_list_remove_node(t_linked_list_node **tpp_ll_head, int i_index) {
  if (*tpp_ll_head == NULL) { return 1; }

  if (i_index == 0) {
    t_linked_list_node *tp_ll_temp = *tpp_ll_head;
    *tpp_ll_head = (*tpp_ll_head)->stp_ll_next;

    free(tp_ll_temp);

    return 0;
  }

  t_linked_list_node *tp_ll_current = *tpp_ll_head;

  // Traverse to find the node just before the one to remove
  for (int i = 0; i < i_index - 1; i++) {
    if (tp_ll_current->stp_ll_next == NULL) { return 1; } // Index out of bounds
    tp_ll_current = tp_ll_current->stp_ll_next;
  }
  
  // Check if we're trying to remove a node past the end
  if (tp_ll_current->stp_ll_next == NULL) return 1;
  
  // Remove the stp_ll_next node (which is at position 'index')
  t_linked_list_node *tp_ll_temp = tp_ll_current->stp_ll_next;
  tp_ll_current->stp_ll_next = tp_ll_temp->stp_ll_next;

  free(tp_ll_temp);

  return 0;
}

void linked_list_itterate_function(t_linked_list_node *tp_ll_head,
											 void *fnp_callback(void *vp_parameter, void *vp_data),
											 void *vp_parameter_passed_to_callback) {
  t_linked_list_node *pst_current = tp_ll_head;
  while (pst_current != NULL) {
    fnp_callback(vp_parameter_passed_to_callback, pst_current->vp_data);
    pst_current = pst_current->stp_ll_next;
  }
}

t_linked_list_node *linked_list_get(t_linked_list_node **tpp_ll_head, int i_index) {
	t_linked_list_node *tp_ll_current = *tpp_ll_head;

	// traverse list til index-th element
	for (int i = i_index; i > 0; i--) {
		if (tp_ll_current == NULL) { return NULL; }
		tp_ll_current = tp_ll_current->stp_ll_next;
	}

	return tp_ll_current;
}

t_linked_list_node *linked_list_get_first(t_linked_list_node **tpp_ll_head) {
	if (tpp_ll_head == NULL) { return NULL; }
	return *tpp_ll_head;
}

t_linked_list_node *linked_list_get_last(t_linked_list_node **tpp_ll_head) {
	if (tpp_ll_head == NULL) { return NULL; }
	if (*tpp_ll_head == NULL) { return NULL; }

	t_linked_list_node *tp_ll_current = *tpp_ll_head;
	// traverse til end of list
	while (tp_ll_current->stp_ll_next != NULL) {
		tp_ll_current = tp_ll_current->stp_ll_next;
	}

	return tp_ll_current;
}

t_linked_list_node *linked_list_pop(t_linked_list_node **tpp_ll_head, int i_index) {
	// guards
	if (tpp_ll_head == NULL) { return NULL; }
	if (*tpp_ll_head == NULL) { return NULL; }
	if (i_index == 0) { 
		t_linked_list_node *tp_ll_target = *tpp_ll_head;
		*tpp_ll_head = tp_ll_target->stp_ll_next;
		tp_ll_target->stp_ll_next = NULL;
		return tp_ll_target;
	}

	// travwerse til (i_index-1)-th position
	t_linked_list_node *tp_ll_current = *tpp_ll_head;
	for (int i = i_index - 1; i > 0; i--) {
		if (tp_ll_current == NULL) { return NULL; }
		tp_ll_current = tp_ll_current->stp_ll_next;
	}

	// next element is the target
	t_linked_list_node *tp_ll_target = tp_ll_current->stp_ll_next;
	if (tp_ll_target == NULL) { return NULL; }

	// cut target node from list
	tp_ll_current->stp_ll_next = tp_ll_target->stp_ll_next;
	tp_ll_target->stp_ll_next = NULL;

	return tp_ll_target;
}

t_linked_list_node *linked_list_pop_first(t_linked_list_node **tpp_ll_head) {
	if (tpp_ll_head == NULL) { return NULL; }
	if (*tpp_ll_head == NULL) { return NULL; }

	t_linked_list_node *tp_ll_target = *tpp_ll_head;
	*tpp_ll_head = tp_ll_target->stp_ll_next;
	tp_ll_target->stp_ll_next = NULL;

	return tp_ll_target;
}

t_linked_list_node *linked_list_pop_last(t_linked_list_node **tpp_ll_head) {
	// guards
	if (tpp_ll_head == NULL) { return NULL; }
	if (*tpp_ll_head == NULL) { return NULL; }
	if ((*tpp_ll_head)->stp_ll_next == NULL) { 
		t_linked_list_node *tp_ll_target = *tpp_ll_head;
		tpp_ll_head = NULL;
		return tp_ll_target; 
	}

	// travwerse til end
	t_linked_list_node *tp_ll_current = *tpp_ll_head;
	while (tp_ll_current->stp_ll_next->stp_ll_next != NULL) {
		tp_ll_current = tp_ll_current->stp_ll_next;
	}

	// cut node
	t_linked_list_node *tp_ll_target = tp_ll_current->stp_ll_next;
	tp_ll_current->stp_ll_next = NULL;

	return tp_ll_target;
}
