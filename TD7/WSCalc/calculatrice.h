// File: calculatrice.h 
//gsoap ns service name: calculatrice
//gsoap ns service namespace: urn:calculatrice 
//gsoap ns service location: http://localhost:18000 
int ns__add(int op1, int op2, double* response);
int ns__sub(int op1, int op2, double* response);
int ns__mul(int op1, int op2, double* response);
int ns__div(int op1, int op2, double* response);

