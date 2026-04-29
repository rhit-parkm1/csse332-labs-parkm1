
/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#include "hello.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
/**
 * Implementation of print_hello_world
 */	
int
print_hello_world()
{
  void *p   = malloc(sizeof(int));
  *(int *)p = 3;

  //sleep(20);
  printf("Hello world!\n");

  free(p);
  return 3;
}