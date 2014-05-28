#include "soapH.h"
#include "currentTime.nsmap"

int main(int argc, char **argv) {
    struct soap soap;
    time_t response;
    if (argc < 2) {
        fprintf(stderr, "Usage: ip:port \n");
        exit(0);
    }
    soap_init(&soap);
    soap_call_ns__currentTime(&soap, argv[1],"",&response);
    if (soap.error) {
        soap_print_fault(&soap, stderr);
        exit(1);
    } else {
        printf("TIME = %d\n", response);
    }
    soap_destroy(&soap);
    soap_end(&soap);
    soap_done(&soap);
    return 0;
}
