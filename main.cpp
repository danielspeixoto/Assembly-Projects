#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#define PI 3.14159265

double PCFreq = 0.0;
__int64 CounterStart = 0;



//Timer_______________________________________________________________________

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
        printf("QueryPerformanceFrequency Failed.\n");
    PCFreq = (double)(li.QuadPart) / 1000.0;
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return (double)(li.QuadPart - CounterStart) / PCFreq;
}



//Operacoes em ASM_______________________________________________________________________

//Função que calcula o valor absoluto de um numero real
float absoluto(float n){
    float resultado;
    asm("fld %1;"
            "fabs;"
            "fstp %0;"
    : "=m" (resultado)
    : "m"  (n)
    );
    return resultado;
}

float loge(float n){
    float resultado;
    asm(
    		"fld %1;"
            "fld1;"
            "fxch;"
            "fyl2x;"
            "fldl2e;"
            "fxch;"
            "fdivp;"
            "fstp %0;"
    : "=m" (resultado)
    : "m" (n)
    );
    return resultado;
}


//função que calula a raiz quadrada de um numero real
float raiz( float n) {
    float resultado;
    asm( "fld %1;"
            "fsqrt;"
            "fstp %0;"
    :"=m" (resultado)
    : "m" (n)
    );
    return resultado ;
}

//função que calcula o seno de um numero real
float seno(float n) {
    float resultado, aux = 180.0f, radianos ;
    asm("fld %1;"
            "fld %2;"
            "fldpi;"
            "fmulp;"
            "fdivp;"
            "fstp %0;"
    :"=m" (radianos)
    :"m"(aux), "m" (n)
    );
    asm("fld %1;"
            "fsin;"
            "fstp %0;"
    : "=m" (resultado)
    : "m" (radianos)
    ) ;
    return resultado ;
}

//função que calcula o cosseno de um numero real
float cosseno(float n) {
    float resultado, aux = 180.0f, radianos ;
    asm("fld %1;"
            "fld %2;"
            "fldpi;"
            "fmulp;"
            "fdivp;"
            "fstp %0;"
    :"=m" (radianos)
    :"m"(aux), "m" (n)
    );
    asm("fld %1;"
            "fcos;"
            "fstp %0;"
    : "=m" (resultado)
    : "m" (radianos)
    ) ;
    return resultado;
}

//função que calcula a tangente de um numero real
float tangente(float n) {
    float resultado, aux = 180.0f, radianos ;
    asm("fld %1;"
            "fld %2;"
            "fldpi;"
            "fmulp;"
            "fdivp;"
            "fstp %0;"
    :"=m" (radianos)
    :"m"(aux), "m" (n)
    );
    asm("fld %1;"
            "fptan;"
            "fstp %0;"
    : "=m" (resultado)
    : "m" (radianos)
    ) ;
    return resultado;
}

float arccos(float n){
    float resultado;
    asm(
    		"fld %1;"
    		"fld %1;"
            "fmulp;"
            "fld %1;"
            "fld1;"
            "fsubp;"
            "fdivp;"
            "fsqrt;"
            "fld1;"
            "fpatan;"
            "fstp %0;"
    : "=m" (resultado)
    : "m" (n)
    );
    return resultado * ((180.0) / PI);
}

float arcsen(float n){
    float resultado;
    asm(
    		"fld %1;"
    		"fld %1;"
            "fmulp;"
            "fld %1;"
            "fld1;"
            "fsubp;"
            "fdivrp;"
            "fsqrt;"
            "fld1;"
            "fpatan;"
            "fstp %0;"
    : "=m" (resultado)
    : "m" (n)
    );
    return resultado* ((180.0) / PI);
}

float arctg(float n){
    float resultado, val = 180.0;
    asm(
    "fld %1;"
            "fld1;"
            "fpatan;"
            "fstp %0;"
    : "=m" (resultado)
    : "m" (n)
    );

    return resultado *(val / PI);
}




//Operacoes em C_______________________________________________________________________





float calcabs(float a){
    return abs(a);
}
float calcexp(float a){
    return exp(a);
}
float calclog(float a){
    return log(a);
}
float calcsqrt(float a){
    return sqrt(a);
}
float calccos(float a){
    return cos(a);
}
float calcsin(float a){
    return sin(a);
}
float calctan(float a){
    return tan(a);
}
float calcacos(float a){
    return acos(a);
}
float calcasin(float a){
    return asin(a);
}
float calcatan(float a){
    return atan(a);
}




//MAIN____________________________________________________________





int main() {
    float a = 1;
    double timeC, timeAsm;
    // --------------------------------------------------------------------------------
    // ABS
    // C
    timeC = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        absoluto(a);
        timeC += GetCounter();
    }
    // ASM
    timeAsm = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        calcabs(a);
        timeAsm += GetCounter();
    }
    printf("Cálculo de valor absoluto\n");
    printf("Tempo em C = %lf, Tempo em Assembly = %lf \n", timeC, timeAsm);
    printf("Ganho de perfomance de %lf vezes\n\n", timeC/timeAsm);
    // --------------------------------------------------------------------------------
    // SQRT
    // C
    timeC = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        calcsqrt(a);
        timeC += GetCounter();
    }
    // ASM
    timeAsm = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        raiz(a);
    	timeAsm += GetCounter();
    }
    printf("Cálculo de raiz quadrada\n");
    printf("Tempo em C = %lf, Tempo em Assembly = %lf \n", timeC, timeAsm);
    printf("Ganho de perfomance de %lf vezes\n\n", timeC/timeAsm);
    // --------------------------------------------------------------------------------
    // LOG
    // C
    timeC = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        calclog(a);
        timeC += GetCounter();
    }
    // ASM
    timeAsm = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        loge(a);
        timeAsm += GetCounter();
    }
    
    printf("Cálculo de logaritmo\n");
    printf("Tempo em C = %lf, Tempo em Assembly = %lf \n", timeC, timeAsm);
    printf("Ganho de perfomance de %lf vezes\n\n", timeC/timeAsm);
    // --------------------------------------------------------------------------------
    // SENO
    // C
    timeC = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        calcsin(a);
        timeC += GetCounter();
    }
    // ASM
    timeAsm = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        seno(a);
    	timeAsm += GetCounter();
    }
    printf("Cálculo de seno\n");
    printf("Tempo em C = %lf, Tempo em Assembly = %lf \n", timeC, timeAsm);
    printf("Ganho de perfomance de %lf vezes\n\n", timeC/timeAsm);
    // --------------------------------------------------------------------------------
    // COS
    // C
    timeC = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        calccos(a);
        timeC += GetCounter();
    }
    // ASM
    timeAsm = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        cosseno(a);
    	timeAsm += GetCounter();
    }
    printf("Cálculo de cosseno\n");
    printf("Tempo em C = %lf, Tempo em Assembly = %lf \n", timeC, timeAsm);
    printf("Ganho de perfomance de %lf vezes\n\n", timeC/timeAsm);
    // --------------------------------------------------------------------------------
    // Tangente
    // C
    timeC = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        calctan(a);
        timeC += GetCounter();
    }
    // ASM
    timeAsm = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        tangente(a);
    	timeAsm += GetCounter();
    }
    printf("Cálculo de tangente\n");
    printf("Tempo em C = %lf, Tempo em Assembly = %lf \n", timeC, timeAsm);
    printf("Ganho de perfomance de %lf vezes\n\n", timeC/timeAsm);
    // --------------------------------------------------------------------------------
    // arcsen
    // C
    timeC = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        calcasin(a);
        timeC += GetCounter();
    }
    // ASM
    timeAsm = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        arcsen(a);
    	timeAsm += GetCounter();
    }
    printf("Cálculo de arco seno\n");
    printf("Tempo em C = %lf, Tempo em Assembly = %lf \n", timeC, timeAsm);
    printf("Ganho de perfomance de %lf vezes\n\n", timeC/timeAsm);
    // --------------------------------------------------------------------------------
    // arccos
    // C
    timeC = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        calcacos(a);
        timeC += GetCounter();
    }
    // ASM
    timeAsm = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        arccos(a);
    	timeAsm += GetCounter();
    }
    printf("Cálculo de arco cosseno\n");
    printf("Tempo em C = %lf, Tempo em Assembly = %lf \n", timeC, timeAsm);
    printf("Ganho de perfomance de %lf vezes\n\n", timeC/timeAsm);
    // --------------------------------------------------------------------------------
    // arctg
    // C
    timeC = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        calcatan(a);
        timeC += GetCounter();
    }
    // ASM
    timeAsm = 0;
    for(int i = 0; i < 1000000; i++) {
    	StartCounter();
        arctg(a);
    	timeAsm += GetCounter();
    }
    printf("Cálculo de arco tangente\n");
    printf("Tempo em C = %lf, Tempo em Assembly = %lf \n", timeC, timeAsm);
    printf("Ganho de perfomance de %lf vezes\n\n", timeC/timeAsm);

    return 0;
}
