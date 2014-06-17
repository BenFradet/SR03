#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define MAX 1000000

float Random (float coef) {
    return (float)rand() * coef / RAND_MAX;
}

// pour la droite y=ax+b
// si l'argument 'dessus_ou_dessous' = 0 
// alors on teste si le point (x,y) est au dessus de la droite y=ax+b
// si l'argument 'dessus_ou_dessous' = 1 
// alors on teste si le point (x,y) est en dessous de la droite y=ax+b
int eval(int dessus_ou_dessous, float a, float b, float x, float y) {
    int resultat = 0; // on suppose que la reponse est negative 
    int inter = 1; // on suppose que le point est au dessus

    if (a * x + b > y) {
        // finalement il est en dessous
        inter = 0;
    }

    if (inter == 1 && dessus_ou_dessous == 0) {
        // le point est au dessus ? oui il est au dessus
        // donc reponse positive
        resultat = 1;
    }
    if (inter == 0 && dessus_ou_dessous == 1) {
        // le point est en dessous ? oui il est en  dessous
        // donc reponse positive
        resultat = 1;
    }

    return resultat;
}

int dedans(float x, float y) {
    int i;
    int dessus_ou_dessous;
    float a, b;

    // D1 -> y = 2 + (1/2)x
    a = 0.5;
    b = 2.0;
    dessus_ou_dessous = 1; // point (x,y) en dessous de D1?
    i = eval(dessus_ou_dessous, a, b, x, y);
    // D2 -> y = 11 - x
    a = -1.0;
    b = 11.0;
    dessus_ou_dessous = 1; // point (x,y) en dessous D2 ?
    i = i * eval(dessus_ou_dessous, a, b, x, y);
    // D3 -> y = 32 - 4x
    a = -4.0;
    b = 32.0;
    dessus_ou_dessous = 1; // point (x,y) en dessous D3 ?
    i = i * eval(dessus_ou_dessous, a, b, x, y);
    // D4 -> y = 2 - (1/4)x
    a = -0.25;
    b = 2.0;
    dessus_ou_dessous = 0; // point (x,y) au dessus D4 ?
    i = i * eval(dessus_ou_dessous, a, b, x, y);

    /* a = -1; b= 1.0; */
    /* dessus_ou_dessous = 1; // au dessus ? */
    /* i = eval(dessus_ou_dessous, a, b, x, y); */

    return i;
}


int main (int argc,char *argv[]) {
    int rank, size, int_error_handling;
    float a, b, x, y, elapsed, max_x = 8.0, max_y = 5.0, surf = 0.0, surface;
    long i, count = 0;
    int dessus_ou_dessous = 0;
    long t1, t2, r;

    MPI_Init(&argc,&argv);

    int_error_handling = MPI_Comm_size(MPI_COMM_WORLD, &size); 
    int_error_handling = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    t1 = clock();
    srand(rank * time (NULL));

    // exemple 1
    a = 0.5;
    b = 2.0;
    x = 8.0;
    y = 0.0;
    dessus_ou_dessous = 0; // point (8.0, y = 0.0) au dessus de D1 ?
    i = eval(dessus_ou_dessous, a, b, x, y);
    dessus_ou_dessous = 1; //  point (8.0, y = 0.0) en dessous de D1 ?
    i = eval(dessus_ou_dessous, a, b, x, y);

    a = -0.25;
    b = 2.0;
    x = 7.0;
    y = 1.0;
    dessus_ou_dessous = 0; // point (7.0, 1.0) au dessus de D3 ?
    i = eval(dessus_ou_dessous, a, b, x, y);
    dessus_ou_dessous = 1; // point (7.0, 1.0) en dessous de D3 ?
    i = eval(dessus_ou_dessous, a, b, x, y);

    a = -0.25;
    b = 2.0;
    x = 1.0;
    y = 1.0;
    dessus_ou_dessous = 0; // point (1.0, 1.0) au dessus de D3 ?
    i = eval(dessus_ou_dessous, a, b, x, y);
    dessus_ou_dessous = 1; //  point (1.0, 1.0) en dessous de D3 ?
    i = eval(dessus_ou_dessous, a, b, x, y);

    x = 4.0;
    y = 2.0;
    i = dedans(x, y); // point (4.0, 2.0) dans le polygone ?
    x = 1.0;
    y = 1.0;
    i = dedans(x, y); // point (1.0, 1.0) dans le polygone ?
    x = 1.0;
    y = 3.0;
    i = dedans(x, y); // point (1.0, 3.0) dans le polygone ?
    x = 7.0;
    y = 5.0;
    i = dedans(x, y); // point (7.0, 5.0) dans le polygone ?
    x = 8.0;
    y = 2.0;
    i = dedans(x, y); // point (8.0, 2.0) dans le polygone ?

    // Tirage aleatoire de MAX point et somme des points a l'interieur du polygone.
    for (i = 0; i < MAX; i++)
    {
        x = Random(Max_x);
        y = Random(Max_y);
        r = dedans(x, y);
        count += r;
    }
    t2 = clock();
    elapsed = (float)(t2 - t1) / CLOCKS_PER_SEC;
    surf = ((float)count / MAX);
    printf("\nsurf:%f\n", surf);
    MPI_Reduce(&surf, &surface, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    surface /= 3;//probabilite
    surface *= 40;
    if(rank == 0) {
        printf ("\n\nS : %f\n", surface);
    }

    MPI_Finalize();
    return 0;
} 
