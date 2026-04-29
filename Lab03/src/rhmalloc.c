/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */

#include <errno.h>
#include <sys/mman.h>

#include "rhmalloc.h"

/**
 * For testing purposes, we need to record where our memory starts. Generally
 * this is hidden from the users of the library but we're just using it here to
 * make our tests more meaningful.
 */
static void *heap_mem_start = 0;

/**
 * Head of the free list. It is actually a pointer to the header of the first
 * free block.
 *
 * @warning
 *  In this assignment, "freelist" is somewhat of a misnomer, because
 *  this list contains both free and unfree nodes.
 */
static struct metadata *freelist = 0;

struct metadata *
freelist_head(void)
{
  return freelist;
}

void *
heap_start(void)
{
  return heap_mem_start;
}

int
rhmalloc_init(void)
{
  void *p = 0;

  p = mmap(NULL, MAX_HEAP_SIZE, PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if(p == MAP_FAILED) {
    errno = ENOMEM;
    return -1;
  }

  // TODO
  // =====
  //  Add code here to initialize heap_mem_start, freelist, and the content of
  //  freelist.

  struct metadata *first = (struct metadata *)p;
  first -> in_use = 0;
  first -> size = MAX_HEAP_SIZE - sizeof(struct metadata);
  first -> next = NULL;
  first -> prev = NULL;

  freelist = first;
  heap_mem_start = p;
  return 0;
}

int
rhfree_all(void)
{
  int rc         = munmap(heap_mem_start, MAX_HEAP_SIZE);
  heap_mem_start = 0;
  freelist       = 0;
  return rc;
}

void *
rhmalloc(size_t size)
{
  // check if we need to reset the space.
  if(!freelist && rhmalloc_init())
    return 0;

  // align the size
  size = ALIGN(size);

  // TODO:
  // =====
  //  Add code here to find a suitable block and return a pointer to the start
  //  of the usable memory region for it.

  struct metadata *curr = freelist;
  while(curr) {
    if(!curr->in_use && curr->size >= size) {
      size_t min_split = sizeof(struct metadata) + ALIGNMENT;

      if(curr->size >= size + min_split) {
        struct metadata *new_block = (struct metadata *)((char *)(curr + 1) + size);

        new_block->in_use = 0;
        new_block->size   = curr->size - size - sizeof(struct metadata);
        new_block->next   = curr->next;
        new_block->prev   = curr;
 
        if(curr->next){
          curr->next->prev = new_block;
        }
        curr->next  = new_block;
        curr->size  = size;

      }

      curr->in_use = 1;
      return (void *)(curr + 1);

    }
    curr = curr->next;
  }

  // return here when we can't find a block, so set errno to ENOMEM.
  errno = ENOMEM;
  return 0;
}

void
rhfree(void *p)
{
  // TODO:
  // =====
  //  Add code here to coalese the block to free with the next and previous
  //  blocks if applicable.

 
  struct metadata *block = (struct metadata *)p - 1;
 
  block->in_use = 0;
 
  if(block->next && !block->next->in_use) {
    struct metadata *next = block->next;
 
    block->size += sizeof(struct metadata) + next->size;
    block->next  = next->next;
 
    if(next->next){
      next->next->prev = block;
    }
  }
 
  if(block->prev && !block->prev->in_use) {
    struct metadata *prev = block->prev;
 
    prev->size += sizeof(struct metadata) + block->size;
    prev->next  = block->next;
 
    if(block->next){
      block->next->prev = prev;
    }
  }
}
