 /*
 *  Created on: 2024 Oct 10 14:52:20
 *  Author: frahmt
 */
#include "DAVE.h"


/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

int main(void) {

  DAVE_STATUS_t status = DAVE_Init();
  if(status != DAVE_STATUS_SUCCESS){
    while(1);
  }
  
  while(1U) {
	  int i = 0;
    for(int j = 0; j < 100; j++) {
      i++;
    }
  }
}