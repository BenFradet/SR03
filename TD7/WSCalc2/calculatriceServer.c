#include "soapH.h"
#include "calc.nsmap"

int main(int argc, char **argv) { 
	int m, s; /* master and slave sockets */
    struct soap soap;
    soap_init(&soap);
    if (argc < 2) {
        fprintf(stderr, "ERROR: usage ctserver port", m);
    } else { 
        m = soap_bind(&soap, NULL, atoi(argv[1]), 100);
        if (m < 0) { 
            soap_print_fault(&soap, stderr);
            exit(-1);
        }
        fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
        for ( ; ; ) { 
            s = soap_accept(&soap);
            fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);
            if (s < 0) { 
                soap_print_fault(&soap, stderr);
                exit(-1);
            } 
            soap_serve(&soap);
            soap_end(&soap);
        }
    }
    return 0;
} 

int ns2__add(struct soap *soap, double op1, double op2, double* response) {
	*response = op1 + op2;
	return SOAP_OK;
}

int ns2__sub(struct soap *soap, double op1, double op2, double* response) {
	*response = op1 - op2;
	return SOAP_OK;
}

int ns2__mul(struct soap *soap, double op1, double op2, double* response) {
	*response = op1 * op2;
	return SOAP_OK;
}

int ns2__div(struct soap *soap, double op1, double op2, double* response) {
	if(op2 == 0) {
		*response = -1;
	} else {
		*response = op1 / (double)op2;
	}
	return SOAP_OK;
}

int ns2__pow(struct soap *soap, double op1, double op2, double* response) {
	if(op2 == 0) {
		*response = -1;
	} else {
		*response = op1 / (double)op2;
	}
	return SOAP_OK;
}
