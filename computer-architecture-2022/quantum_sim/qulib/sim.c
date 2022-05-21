/* $Header: $ */
/*
 * This file contains generic functions for the quantum simulations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sim.h"
#include "norm.h"

#define TRUE 1
#define FALSE 0

double Pi, Pio2, Pio4, Pio8, root2_2;
Register create_register(int nq_L, double err_L, double sigma_L, int qub, int time, int type, FILE *out)
{
	Register reg;
	
	reg.nq = nq_L;	/* number of logical qubits */
	reg.nc = pow(2, nq_L);		/* number of coefficients */
	reg.err = err_L;		/* logical error rate?*/
	reg.sigma = sigma_L;	
	reg.qub = qub;
	reg.time = time;
	reg.type = type;
	
	reg.state = (Complex *)calloc(reg.nc, sizeof(Complex));
	if (reg.state == NULL) {
		fprintf(out, "Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	reg.steps = (int *)calloc(reg.nq, sizeof(int));
	if (reg.steps == NULL) {
		fprintf(out, "Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	
	return reg;
}



///////////////////////////////


// reg.state[n] is the complex coefficient (x+iy) of the binary-represented state n. So if n=0, this is the state |00...0>

void set_state(Register reg, int n, double x, double y)
{
	reg.state[n].x = x;
	reg.state[n].y = y;
}




//////////////////////////////

/* erroryn - error yes/no (0 for no 1 for yes)*/

void cnot(Register reg, int qcont, int qtarg, int erroryn)
{
	int i, j, k;
	Complex z;
	
	update(reg, qcont, qtarg);
	
	/* XXX parallelizing this loop is tricky, but it's a "big" loop, so worth doing... */
#pragma omp parallel for schedule(static,8192) private(j,k,z)
	for (i=0; i<reg.nc/4; i++) {
		controlled_indices(i, qcont, qtarg, &j, &k);
		/* Note that the toffoli gate does not work with pseudo cnot gates
		   ie : you cant use esigy(reg.state+j, reg.state+k, Pi) */
		z = reg.state[j];
		reg.state[j] = reg.state[k];
		reg.state[k] = z;
	}
	
	error(reg, qcont, erroryn);
	reg.steps[qcont]+=erroryn;
	error(reg, qtarg, erroryn);
	reg.steps[qtarg]+=erroryn;
}



//////////////////////////////



void Hadamard(Register reg, int q, int erroryn)
{
	int i, j, k;		
	Complex z1, z2;
	
	/* XXX parallelizing this loop is tricky, but it's a "big" loop, so worth doing... */
#pragma omp parallel for schedule(static,8192) private(j,k,z1,z2)
	for (i=0; i<reg.nc/2; i++) {
		one_qubit_indices(i, q, &j, &k);	/* ln 255 function defines values */  
		z1 = reg.state[j];				/* for j and k, changing every */
		z2 = reg.state[k];				/* time it is called */
		reg.state[j].x = root2_2*(z1.x+z2.x);  
		reg.state[j].y = root2_2*(z1.y+z2.y);
		reg.state[k].x = root2_2*(z1.x-z2.x);
		reg.state[k].y = root2_2*(z1.y-z2.y);
	}
	error(reg, q, erroryn);
	reg.steps[q]+=erroryn;
}



/////////////////////////////




void error(Register reg, int q, int erroryn)
{
	int i, j, k;
	double p;
	double delta, alpha, beta, theta;
	Complex z;
	
	if (erroryn == 0) return;
	//	printf("%d, %d, %d\n", reg.qub, reg.time, 1);
	if(reg.qub == q && reg.time == reg.steps[q]){
		
		if (reg.type==0)  {
			not(reg, q, 0);
			reg.steps[q]++;
			//printf("%g, %d, %d, %d\n",p, reg.qub, reg.time, 1);
		}
		if (reg.type==1)  {
			Z(reg, q, 0);
			reg.steps[q]++;
			//printf("%g, %d, %d, %d\n",p, reg.qub, reg.time, 2);
		}
		if (reg.type==2)  {
			XZ(reg, q, 0);
			reg.steps[q]++;
			//printf("%g, %d, %d, %d\n",p, reg.qub, reg.time, 3);
		}
	}
	
	if (reg.sigma > 0) {
		alpha = reg.sigma*norm();
		beta = reg.sigma*norm();
		theta = reg.sigma*norm();
#pragma omp parallel for schedule(static,8192) private(j,k)
		for (i=0; i<reg.nc/2; i++) {
			one_qubit_indices(i, q, &j, &k);
			unitary(reg.state+j, reg.state+k, alpha, beta, theta);
		}
	}
}



//////////////////////////////




void global_update(Register reg)
{
	int q, max_steps = 0, i;
	
	#pragma omp parallel for schedule(static,8192)
	for (q=0; q<reg.nq; q++)
		if (reg.steps[q] > max_steps)
			max_steps = reg.steps[q];
	
#pragma omp parallel for schedule(static,8192) private(i)
	for (q=0; q<reg.nq; q++)
		for (i=reg.steps[q]; i<max_steps; i++)
			error(reg, q, 1);
	
	#pragma omp parallel for schedule(static,8192)
	for (q=0; q<reg.nq; q++)
		reg.steps[q] = max_steps;
}



//////////////////////////



/* Function used when performing a single qubit operation. n runs from 0
 to nc/2 - 1 and simply keeps track of how far through the array the
 calculation is. q runs from 0 to nq - 1 and denotes which qubit the
 operation is being performed on. The function outputs the next pair of
 indices to apply the desired transformation to. */
inline void one_qubit_indices(int n, int q, int *jPtr, int *kPtr)
{
	/* l is used as a mask to calculate nl --- the low part of n */
	int l, nl;
	
	/* calculate mask of 1s for section of n less significant than bit q */
	l = 1;
	l <<= q;
	l--;
	
	/* calculate section of n less significant than bit q */
	nl = n & l;
	
	/* remove section of n less significant than bit q and shift n higher for
	 later insertion of bit q */
	n -= nl;
	n <<= 1;
	
	/* calculate value to be used to insert value of bit q */
	l++;
	
	*jPtr = n + 0 + nl;
	*kPtr = n + l + nl;
}





void initialize_simulator()
{
	srand32(time(NULL));
	
	Pi = acos(-1);
	Pio2 = asin(1);
	Pio4 = atan(1);
	Pio8 = Pio4 / 2;
	root2_2 = 1 / sqrt(2);
}


/*------------set/measure----------------------------*/

/* nq2m : number of qubits to measure
 lq2m : list of qubits to measure (sorted from largest to smallest)
 val  : value you wish to observe
 return value is the probability of observing the single state we are interested in.

 n.b.: As seems to be the practice in this code, Register is a typedef struct that is an argument on the stack,
 and contains double *state; which is a pointer to malloced space.  So, changes to reg itself won't be
 visible outside this routine, but changes to the state vector will.
 */

//CH: Added renorm flag to cope with the state vector being zero'd if a single qubit is measured and the probability of it reaching the projected state is zero. Specifically for the surface code, the new action is to flip the measured qubit and then not renormalise (as this is not required).

/*
 * rdv comment, 2011/9/7:
 * To measure a single qubit, call w/ nq2m = 1 and lq2m[0] = the index of the single qubit you want to measure,
 * and val set to either 0 or 1, depending on which probability you want.
 * To measure e.g. finding qubit 2 = 0, this must total up the modsq of state[i] for i = 0,1,2,3, 8,9,10,11, 16,17,18,19,...
 * To measure e.g. finding qubit 2 = 1, this must total up the modsq of state[i] for i = 4,5,6,7, 12,13,14,15, 20,21,22,23,...
 *
 * probabilities[0] holds the sum of probs for finding the qubit = 0,
 * probabilities[1] holds the sum of probs for finding the qubit = 1.
 * For measuring more than one qubit (nq2m > 1), probabilities is a bigger array and the probs of finding multi-bit combinations
 * are tracked accordingly.
 * 
 * This works as follows:
 * index i loops through all of the entries in the whole state vector
 * for each i, probabilities[j] is updated for some j = f(i)
 * j is set by picking the set of qubits to be measured out of the index i
 * k is used to help parse i, setting j
 *
 * After calculating this probability, we scan through the state vector again, and renormalize.
 * 
 * n.b.: If the state you are asking about has probability zero, then the entire state vector
 * is zeroed out!!!  Thus, you have to be careful what you wish for -- you might get it!
 *			-- fixed (CH)
 *
 * Finally, only the probability of the particular state we are interested in is returned.
 */

double set_measure(Register reg, int nq2m, int *lq2m, int val)
{
	int i, j, k, imeas, spread, mask, *masks,renorm;
	double p, *probabilities, norm, ptmp;
	
	/* probably not necessary but simplifies things */
	global_update(reg);
	
	/* create array of single bit masks for reducing array index */
	masks = (int *)calloc(nq2m, sizeof(int));
	for (i=0; i<nq2m; i++) {
		masks[i] = 1;
		masks[i] <<= lq2m[i];
	}
	
	/* create array of possible measured values */
	probabilities = (double *)calloc(1 << nq2m, sizeof(double));
	
	/* XXX parallelizing this loop is tricky, but it's a "big" loop, so worth doing...
	 * need a reduction on probabilities[] */
#pragma omp parallel for schedule(static,8192) private(j,k,ptmp)
	for (i=0; i<reg.nc; i++) {
		/* reduce array index i */
		j = 0;
		for (k=0; k<nq2m; k++) {
			j <<= 1;
			if (i & masks[k]) {
				j++;
			}
		}

		/* This code updates all probabilities.  However, at the moment,
		 * only the one probability of interest actually gets used in the code below.
		 * Calculating only one of them would be better, if less general.
		 * Also, efficiency varies pretty dramatically depending on what fraction
		 * of the entries are non-zero, because the atomic operation has a high cost. */
		ptmp = modsq(reg.state[i]);
		if (ptmp) {
#pragma omp atomic
			probabilities[j] += ptmp;
		}
	}

	/* pick the probability of the state the caller requested */
	p = probabilities[val];
	imeas = val;

	renorm=TRUE;		/* Flags whether renormalisation should be run or not */
	if (probabilities[imeas] == 0) {			/* How to deal with a probability of zero for the projected state */
		if (nq2m>1) {		/* If measuring many qubits, renorm as the routine below */
			norm = 0;
		} else {		/* If measuring one qubit, renorming as below will zero out the state vector if p=0 */
			not(reg,lq2m[0],0);		/* Instead, rotate to the orthogonal state (that has p=1), but still return 0 */
			renorm=FALSE;   /* Do not use the subsequent routine to renormalise (as is still normalised). */
		}
	} else {
		norm = 1/sqrt(probabilities[imeas]);
	}

	if (renorm) {				/* only do this renormalisation if it won't zero the state vector. */
		mask = 0;
		for (i=0; i<nq2m; i++) mask += masks[i];
	
		spread = 0;
		i = 1;
		for (j =0; j<nq2m; j++) {
			if (imeas & i) spread += masks[nq2m-1-j];
			i <<= 1;
		}
	
		#pragma omp parallel for schedule(static,8192)
		for (i=0; i<reg.nc; i++) {
			if ((i & mask) == spread) {
				reg.state[i].x *= norm;
				reg.state[i].y *= norm;
			} else {
				reg.state[i].x = 0;
				reg.state[i].y = 0;
			}
		}
	}
	
	free(masks);
	free(probabilities);
	
	return p;
}



void destroy_register(Register reg)
{
	free(reg.steps);
	free(reg.state);
}


void update(Register reg, int q1, int q2)
{
	while (reg.steps[q1] < reg.steps[q2]) {
		error(reg, q1, 1);
		reg.steps[q1]++;
	}
	while (reg.steps[q2] < reg.steps[q1]) {
		error(reg, q2, 1);
		reg.steps[q2]++;
	}
}

/* See comments for one_qubit_indices(). */
inline void controlled_indices(int n, int qcont, int qtarg, int *iPtr, int *jPtr)
{
	int m, l, nm, nl, qm, ql;
	
	if (qcont > qtarg) {
		qm = qcont;
		ql = qtarg;
	}
	else {
		qm = qtarg;
		ql = qcont;
	}
	
	/* calculate mask of 1s for section of n less significant than bit ql */
	l = 1;
	l <<= ql;
	l--;
	
	/* calculate section of n less significant than bit ql */
	nl = n & l;
	
	/* remove section of n less significant than bit ql and shift n higher for
	 later insertion of bit q2 */
	n -= nl;
	n <<= 1;
	
	/* calculate mask of 1s for section of n less significant than bit qm but
	 more significant than bit ql */
	m = 1;
	m <<= qm;
	m--;
	
	/* calculate section of n less significant than bit qm but more significant
	 than bit ql */
	nm = n & m;
	
	/* remove section of n less significant than bit qm and shift n higher for
	 later insertion of bit qm. Add nm and nl to complete preparation of n */
	n -= nm;
	n <<= 1;
	n += nm + nl;
	
	/* calculate values to be used to insert values of bits q1 and q2 */
	m++;
	l++;
	
	if (qcont > qtarg) {
		n += m;
		*iPtr = n;
		*jPtr = n + l;
	}
	else {
		n += l;
		*iPtr = n;
		*jPtr = n + m;
	}
}


void unitary(Complex *zPtr1, Complex *zPtr2,
             double alpha, double beta, double theta)
{
	double c = cos(theta/2), s = sin(theta/2);
	double c11 = cos(+alpha/2+beta/2);
	double s11 = sin(+alpha/2+beta/2);
	double c12 = cos(+alpha/2-beta/2);
	double s12 = sin(+alpha/2-beta/2);
	Complex z1 = *zPtr1, z2 = *zPtr2;
	
	zPtr1->x =  c*(c11*z1.x-s11*z1.y) + s*(c12*z2.x-s12*z2.y);
	zPtr1->y =  c*(c11*z1.y+s11*z1.x) + s*(c12*z2.y+s12*z2.x);
	zPtr2->x = -s*(c12*z1.x+s12*z1.y) + c*(c11*z2.x+s11*z2.y);
	zPtr2->y = -s*(c12*z1.y-s12*z1.x) + c*(c11*z2.y-s11*z2.x);
}

void not(Register reg, int q, int time)
{
	int i, j, k;
	Complex z;
	
#pragma omp parallel for schedule(static,8192) private(j,k,z)
	for (i=0; i<reg.nc/2; i++) {
		one_qubit_indices(i, q, &j, &k);
		z = reg.state[j];
		reg.state[j] = reg.state[k];
		reg.state[k] = z;
	}
	
	error(reg, q, time);
	reg.steps[q]+=time;
}

void Z(Register reg, int q, int time)
{
	int i, j, k;
	Complex z;
	
#pragma omp parallel for schedule(static,8192) private(j,k,z)
	for (i=0; i<reg.nc/2; i++) {
		one_qubit_indices(i, q, &j, &k);
		reg.state[k].x = -reg.state[k].x;
		reg.state[k].y = -reg.state[k].y;
	}
	
	error(reg, q, time);
	reg.steps[q]+=time;
}

void XZ(Register reg, int q, int time)
{
	int i, j, k;
	Complex z;
	
#pragma omp parallel for schedule(static,8192) private(j,k,z)
	for (i=0; i<reg.nc/2; i++) {
		one_qubit_indices(i, q, &j, &k);
		z = reg.state[j];
		reg.state[j].x = -reg.state[k].x;
		reg.state[j].y = -reg.state[k].y;
		reg.state[k] = z;
	}
	
	error(reg, q, time);
	reg.steps[q]+=time;
}


inline double modsq(Complex z)
{
	return (z.x)*(z.x)+(z.y)*(z.y);
}


// Flagprint prints which elements of the state vector have a non-zero coefficient. Return value is the decimal representation of the binary state vector. The co-efficients themselves are not returned.

void flagprint(Register reg)		
{
	int flag,n;
	flag=0;
	/* XXX parallelizing this loop is tricky because of the printf, but it's a "big" loop, so worth doing... */
	for (n=0; n<=reg.nc/2 ; n++){
		if (reg.state[n].x != 0 || reg.state[n].y != 0){
			printf("%d ",n);
			flag++;}
	}
	if (flag==0){
		printf("NA ");
	}
}
