#ifndef SIM_H
#define SIM_H

#include <stdio.h>

typedef struct {
   double x,y ;
} Complex;

typedef struct {
   Complex *state;
   int *steps;
   int nq, nc, qub,time,type;
   double err, sigma;
} Register;

extern double Pi, Pio2, Pio4, Pio8, root2_2;
extern Register reg;

Register create_register(int nq_L, double err_L, double sigma_L,int qubits,int time, int type, FILE *out);
void destroy_register(Register reg);
void equate_registers(Register reg1, Register reg2);
void initialize_simulator();
void clear(Register reg);
void set_state(Register reg, int n, double x, double y);
int query_state(Register reg, int n, double tol);
void print(Register reg, FILE *out);
void change_err(Register *regPtr, double err_L);
void change_sigma(Register *regPtr, double sigma_L);
void update(Register reg, int q1, int q2);
void global_update(Register reg);
void error2(Register reg, int q, int time);

Complex eitheta(double theta);
Complex add(Complex z1, Complex z2);
Complex multiply(Complex z1, Complex z2);
Complex zstarz(Complex z1, Complex z2);
inline double modsq(Complex z);
double inner_product(Register reg1, Register reg2);

void esigi(Complex *zPtr1, Complex *zPtr2, double theta);
void esigx(Complex *zPtr1, Complex *zPtr2, double theta);
void esigy(Complex *zPtr1, Complex *zPtr2, double theta);
void esigz(Complex *zPtr1, Complex *zPtr2, double theta);
void unitary(Complex *zPtr1, Complex *zPtr2,
             double alpha, double beta, double theta);

inline void one_qubit_indices(int n, int q, int *iPtr, int *jPtr);
inline void controlled_indices(int n, int q1, int q2, int *iPtr, int *jPtr);
void swap_indices(int n, int q1, int q2, int *iPtr, int *jPtr);
void two_qubit_indices(int n, int q1, int q2, int *i1Ptr, int *i2Ptr, int *i3Ptr, int*i4Ptr);

void irot(Register reg, int q, double theta, int time);
void xrot(Register reg, int q, double theta, int time);
void yrot(Register reg, int q, double theta, int time);
void zrot(Register reg, int q, double theta, int time);

void not(Register reg, int q, int time);
void Z(Register reg, int q, int time);
void XZ(Register reg, int q, int time);
void cnot(Register reg, int qcont, int qtarg, int time);
void swap(Register reg, int q1, int q2, int time);
void cnots(Register reg, int qcont, int qtarg, int time);
void phase(Register reg, int q, double theta, int time);
void error(Register reg, int q, int time);
void global_error(Register reg,int qubit, int time);
void cphase(Register reg, int qcont, int qtarg, double theta, int time);
void cphases(Register reg, int qcont, int qtarg, double theta, int time);
void Hadamard(Register reg, int q, int time);
/* compound operators act left to right : Hcnot == H then cnot */
void Hcnot(Register reg, int qcont, int qtarg, int time);
void cnotH(Register reg, int qcont, int qtarg, int time);
void Hcnots(Register reg, int qcont, int qtarg, int time);
void scnotH(Register reg, int qcont, int qtarg, int time);


/*-------General Shor's Algorithm---*/

void generalqft(Register reg,int N,int tq,int bq,int maxangle);
void generalqftinv(Register reg,int N, int tq, int bq,int maxangle);
void addergen(Register reg,int N,int initial, int inverse,int tq,int bq);
void contaddergen(Register reg,int N, int initial,int inverse,int tq,int bq,int control);
void addmodgen(Register reg, int N, int a, int mod, int inverse,int tq,int bq,int ancilla,int control,int maxangle);
void toffoli(Register reg,int qcont1,int qcont2,int qtarg);
void contmultgen(Register reg,int N, int a, int mod, int inverse, int tq, int bq,int control,int maxangle);
void contswap(Register reg, int tq,int bq,int control);
void cswap(Register reg,int control,int qtarg1,int qtarg2);
void ugategen(Register reg, int N, int a, int mod, int ainv, int tq,int bq,int control,int maxangle);
void shorgen(Register reg,int N, int a, int mod,int tq,int bq,int maxangle);
void shortrickgen(Register reg,int N,int a,int mod,int tq,int bq,int maxangle,int j,FILE *out);


/*-------Linear Shor's Algorithm-----------------*/

void scnot(Register reg, int qcont, int qtarg, int time);
void sqrtnot(Register reg, int qcont, int qtarg, int dagger, int time);
void phases(Register reg,int qcont, int q, double theta, int time);
void linearqft(Register reg,int N,int inverse,int tq,int bq,int maxangle);
void adder(Register reg, int initial, int N, int inverse,int ci,int cf,int carry, int time);
void contadder(Register reg, int initial, int N, int inverse, int ci, int cf,int up);
void addmod1(Register reg, int N,int a,int mod, int tq, int bq, int maxangle);
void conmodinv(Register reg,int N,int a,int mod,int tq,int bq,int maxangle);
void conmod1(Register reg, int N,int a, int mod,int tq,int bq,int maxangle);
void conswaptoffoli(Register reg, int qcont, int qtarg1, int qtarg2);
void linearmesh(Register reg, int tqmesh, int bqmesh,int inverse);
void swapupdown(Register reg, int qstart, int number,int up);
void linearconswap(Register reg, int N, int tq, int bq);
void addmod1inv(Register reg, int N,int a,int mod, int tq, int bq, int maxangle);
void ugate1(Register reg,int N,int a,int mod,int ainv,int tq,int bq,int maxangle);
void shortrick(Register reg,int N,int a,int mod,int tq,int bq,int j, FILE *out,int maxangle);

/*--------------------------------------------------------------------*/

void countstate(Register reg);
void printmod(Register reg, FILE *out);
int main2(int num, int a, char run[5], int linear,int maxangle,int j,double p,double sigma);

void qec5en(Register reg, int q4, int q3, int q2, int q1, int q0);
void qec5dec(Register reg, int q4, int q3, int q2, int q1, int q0);
int qec5cor(Register reg, int q4, int q3, int q2, int q1, int q0);

void qec7en0(int q1, int q2, int q3, int q4, int q5, int q6, int q7);
void qec7dec(int q1, int q2, int q3, int q4, int q5, int q6, int q7);
int qec7cor(int q1, int q2, int q3, int q4, int q5, int q6, int q7, int q8);

void mesh(Register reg, int nq_L);
void swapreg(Register reg, int nq_L);

int measure(Register reg, int nq2m, int *lq2m);

double set_measure(Register reg, int nq2m, int *lq2m, int val);


/////////C-added

void flagprint(Register reg);

#endif
