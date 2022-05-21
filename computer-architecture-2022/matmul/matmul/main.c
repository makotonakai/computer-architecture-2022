//
//  main.c
//  matmul
//
//  Created by Nico Roble on 2019/06/24.
//  Copyright © 2019 Nico Roble. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    
    int DIM = atoi(argv[1]);
    
    double A[DIM][DIM];
    double B[DIM][DIM];
    double C[DIM][DIM];
    
    int i, j, k;
    
    for ( i = 0 ; i < DIM ; i++ ) {
        for ( j = 0 ; j < DIM ; j++ ) {
//            A[i][j] = (double) (rand() % 10);
//            B[i][j] = (double) (rand() % 10);
//            C[i][j] = (double) (rand() % 10);
            A[i][j] = 1.0;
            B[i][j] = 1.0;
            C[i][j] = 1.0;
        }
    }
    
    for ( i = 0 ; i < DIM ; i++ ) {
        for ( j = 0 ; j < DIM ; j++ ) {
            for ( k = 0 ; k < DIM ; k++ ) {
                C[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    
    for ( i = 0 ; i < DIM ; i++ ) {
        for ( j = 0 ; j < DIM ; j++ ) {
            printf("C[%d][%d] = %f\n",i, j, C[i][j]);
        }
    }
    
    return 0;
}

