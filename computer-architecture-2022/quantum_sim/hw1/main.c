//
//  main.c
//  Test program for quantum computer state vector simulation
//  Used as example program in computer architecture class, 2011
//

#include <stdio.h>
#include <math.h>
#include "sim.h"
#include <string.h>
#include <time.h>

#define LOOP 1000
#define REGISTER_SIZE 20

int main()
{
	
	Register reg; /* declare as user-defined type Register */
	int measarr[1],qubit=REGISTER_SIZE,time=0,type=0,loop;
	FILE *out=fopen("/dev/null","w+");

	initialize_simulator();		
	reg = create_register(REGISTER_SIZE, 0, 0, qubit, time, type, out);	
	set_state(reg, 0, 1, 0);	/* sets state of the system to 0000... */

	for ( loop = 0 ; loop < LOOP ; loop++ ) {
	  for(qubit = 0 ; qubit<REGISTER_SIZE ; qubit++ ) {
	    Hadamard(reg,qubit,0);
	    global_update(reg);
	  }
	  cnot(reg,1,3,0);
	}
	destroy_register(reg);
	return 0;
}
