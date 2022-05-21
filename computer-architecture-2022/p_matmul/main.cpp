#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...

    int DIM = stoi(argv[1]);

    cout << DIM << '\n';

    auto **A = new double*[DIM];
    auto **B = new double*[DIM];
    auto **C = new double*[DIM];


    int i, j, k;
    int x, y;

  for(x = 0; x < DIM; ++x) {
    A[x] = new double[DIM];
    B[x] = new double[DIM];
    C[x] = new double[DIM];
  }

  for ( x = 0 ; x < DIM ; x++ ) {
        for ( y = 0 ; y < DIM ; y++ ) {
//            A[i][j] = (double) (rand() % 10);
//            B[i][j] = (double) (rand() % 10);
//            C[i][j] = (double) (rand() % 10);
            A[x][y] = 1.0;
            B[x][y] = 1.0;
            C[x][y] = 1.0;
        }
    }

  chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

#pragma omp parallel shared(A, B, C) private(i,j,k)
#pragma omp for schedule(static)
      for ( i = 0 ; i < DIM ; i++ ) {
        for ( k = 0 ; k < DIM ; k++ ) {
          for ( j = 0 ; j < DIM ; j++ ) {
            C[i][j] += A[i][k]*B[k][j];
          }
        }
      }

  chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
  double time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
  cout << "Finished after " << time << " nanoseconds \n";

//    for ( x = 0 ; x < DIM ; x++ ) {
//        for ( y = 0 ; y < DIM ; y++ ) {
//            printf("C[%d][%d] = %f\n", x, y, C[x][y]);
//        }
//    }

  for(x = 0; x < DIM; ++x) {
    delete A[x];
    delete B[x];
    delete C[x];
  }

  delete[] A;
  delete[] B;
  delete[] C;

  return 0;
}
