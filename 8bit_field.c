#include <stdio.h>
#define WORD unsigned int 
#define BYTE unsigned char

BYTE field_addition(BYTE A, BYTE B);        // return A + B
BYTE field_multiplication(BYTE A, BYTE B);  // return A * B mod f(x)
BYTE field_division(BYTE A, BYTE B);        // return A / B
BYTE field_inverse(BYTE A);                 // return A^{-1} in GF(2^8)

int main()
{
    /* 
    printf("Hello world!\n");
    return 0; 
    */

    BYTE A = 0x11;
    BYTE B = 0x01;
    BYTE C = 0xf9;

    BYTE D = field_multiplication(A,B);
    printf("%02x\n", D);
    D = field_multiplication(A,C);
    printf("%02x\n", D);


    return 0;
}

BYTE field_addition(BYTE A, BYTE B)
{
    return A^B;
}

BYTE field_multiplication(BYTE A, BYTE B)
{
    int i;
    BYTE temp = 0, mask = 0x01;

    for(int i = 0; i < 8; i++) {
        // B의 i번째 피트가 1이면 미리 계산한 A * x^i를 XOR
        if(B & mask)
            temp ^= A;

        // 매 루프마다 A에 x를 곱함(<<1). A의 최고차항이 7이면 carry가 발생하므로 0x1b(기약다항식)을 XOR 한다.
        // 즉, A * x^i를 미리 계산 
        if(A & 0x80)
            A = (A<<1)^0x1B;
        else
            A <<= 1;
        mask <<= 1;   
    }

    return temp;
}

// return (A // B)
BYTE field_division(BYTE A, BYTE B) 
{
    if (A < B)
        return 0x00;
    else if (A == B)
        return 0x01;

    // A 최고차항 차수 찾기
    BYTE mask = 0x08;
    int deg_A = 7;  // A 차수를 저장할 변수
    for(int i = 0; i < 8; i++) {
        if(A & mask)
            break;
        else {
            mask >> 01;
        }



    }


    while(A > B) {



    }

}

/*
BYTE field_inverse(BYTE A)  // A = g(x)
{

let g = 0x11b, h = A


}
*/