#include <omp.h>
#include <iostream>
using namespace std;


int main() {
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        cout << ID << '\n';
    }
}
