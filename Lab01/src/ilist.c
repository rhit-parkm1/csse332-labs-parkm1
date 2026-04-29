/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#include <stdlib.h>

#include "ilist.h"

/**
 * Implementation of insert_at_end
 */
void
insert_at_end(struct ilist_node *head, struct ilist_node *n)
{
  struct ilist_node *tail = head;
  // avoid self loops
  if(head == n)
    return;
  while(tail->next != 0)
    tail = tail->next;

  tail->next = n;
}

/**
 * Implementation of find_node
 */
struct ilist_node *
find_node(struct ilist_node *head, int val)
{
  struct ilist_node *p = head;

  while(p) {
    if(p->value == val)
      return p;

    p = p->next;
  }

  return 0;
}

/**
 * Implementation of get_node_count
 */
int
get_node_count(struct ilist_node *head)
{
  int count = 0;
  while(head) {
    head = head->next;
    count++;
  }
  return count;
}

/**
 * Implementation of delete_vals
 */
void
delete_vals(struct ilist_node *head, int val, int dofree)
{
  struct ilist_node *prev = 0, *n = 0;

  // Be careful here as we want to keep moving through the list and not just
  // delete the first match.
  while(head) {
    if(head->value == val) {
      // adjust previous if needed, if at head do nothing to prev.
      if(prev)
        prev->next = head->next;
      // delete the node itself and move forward
      n       = head;
      head    = n->next;
      n->next = 0;
      if(dofree)
        free(n);
    } else {
      prev = head;
      head = head->next;
    }
  }
}

/**
 * Implementation of ilist_to_array
 */
int *
ilist_to_array(struct ilist_node *head)
{
  int count = get_node_count(head);
  int *p    = malloc(count * sizeof(int));
  int i     = 0;

  while(head) {
    p[i++] = head->value;
    head   = head->next;
  }

  return p;
}