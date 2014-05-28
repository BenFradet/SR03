#include "soapH.h"
#include "calculatrice.nsmap"
#include <stdio.h>

int main(int argc, char **argv) { 
	struct soap soap;
    double response;	
    if (argc < 2) {
        fprintf(stderr, "Usage: ip:port\n");
        exit(0);
    }
    soap_init(&soap);

    int op1, op2;
    char op;
    printf("Entrez l'operande 1:\n");
    scanf("%d", &op1);
    fflush(stdin);
    printf("Entrez l'operande 2:\n");
    scanf("%d", &op2);
    fflush(stdin);
    printf("Entrez l'operateur:");
    scanf(" %c", &op);
    fflush(stdin);
    printf("%c", op);

    switch(op) {
        case '+':
            soap_call_ns__add(&soap, argv[1], "", op1, op2, &response);
            break;
        case '-':
            soap_call_ns__sub(&soap, argv[1], "", op1, op2, &response);
            break;
        case '*':
            soap_call_ns__mul(&soap, argv[1], "", op1, op2, &response);
            break;
        case '/':
            soap_call_ns__div(&soap, argv[1], "", op1, op2, &response);
            break;
        default:
            printf("invalid operator\n");
            break;
    }	

    if (soap.error){
        soap_print_fault(&soap, stderr);
        exit(1);
    } else {
        printf("resultat = %f\n", response);
    }
    soap_destroy(&soap);
    soap_end(&soap);
    soap_done(&soap);
    return 0;
}
