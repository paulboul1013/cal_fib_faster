#if __STDC_VERSION__ < 199901L
  #error "C99 Minimum Required"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <gmp.h> //for big integer arithmetic
#include <time.h>
#include <pthread.h>



int main(int argc,char *argv[]){

    //user input number
    if (argc!=2){
        printf("Improper input. Exiting.\n");
		return EXIT_FAILURE; 
    }


    long count = strtol(argv[1], NULL, 10);
    // convert string to long
   



    //setup fib init with gmp function
    mpz_t a,b,p,q;
    mpz_init_set_ui(a,1);
    mpz_init_set_ui(b,0);
    mpz_init_set_ui(p,0);
    mpz_init_set_ui(q,1);

    mpz_t tmp;
    mpz_init(tmp);
    // mpz_init(c);

    //cal start time
    clock_t start_time=clock();
    if (start_time==(clock_t){-1}){
        fprintf(stderr,"Error: start_time clock() failed\n");
        return EXIT_FAILURE;
    }

    //cal fibonacci
    // long i=0;
    while (count > 0) {
		if (count % 2 == 0) {
			mpz_mul(tmp, q, q);
			mpz_mul(q, q, p);
			mpz_mul_2exp(q, q, 1);
			mpz_add(q, q, tmp);

			mpz_mul(p, p, p);
			mpz_add(p, p, tmp);

			count /= 2;
		} else {
			mpz_mul(tmp, a, q);

			mpz_mul(a, a, p);
			mpz_addmul(a, b, q);
			mpz_add(a, a, tmp);

			mpz_mul(b, b, p);
			mpz_add(b, b, tmp);

			count -= 1;
		}
   	}


    clock_t end_time=clock();
    if (end_time==(clock_t){-1}){
        fprintf(stderr,"Error: end_time clock() failed\n");
        return EXIT_FAILURE;
    }

    // Print the results
    // printf("Fibonacci Number %ld: ", limit);
    mpz_out_str(stdout, 10, b);
    printf("\n");

    //clean up
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(tmp);
    // mpz_clear(c);

    //print time taken
    double time_taken=((double)(end_time-start_time))/CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n",time_taken);

    

    return 0;
}