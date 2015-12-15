#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "util.h"
#include "dispatch.h"
#include "fox.h"

int test_1(){

  return EXIT_SUCCESS;
}

int test_2(){
  
  return EXIT_SUCCESS;
}

int test_3(){

  return EXIT_SUCCESS;
}

int main(void){
  assert(test_1() == EXIT_SUCCESS);
  assert(test_2() == EXIT_SUCCESS);
  assert(test_3() == EXIT_SUCCESS);
  return EXIT_SUCCESS;
}
