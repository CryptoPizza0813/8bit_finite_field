#include <stdio.h>
#define WORD unsigned int 
#define BYTE unsigned char

BYTE field_addition(BYTE A, BYTE B);        // return A + B
BYTE field_multiplication(BYTE A, BYTE B);  // return A * B mod f(x)
BYTE field_division(BYTE A, BYTE B);        // return A / B
BYTE field_inverse(BYTE h);                 // return A^{-1} in GF(2^8)
int find_deg(BYTE A);

BYTE polynomial_multiplication(BYTE A, BYTE B);

int main()
{
    /* 
    printf("Hello world!\n");
    return 0; 
    */

    BYTE A = 0x53;

    printf("%02x\n", field_inverse(A));


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
            A = (A<<1)^0x1b;
        else
            A <<= 1;
        mask <<= 1;   
    }

    return temp;
}

BYTE polynomial_multiplication(BYTE A, BYTE B)
{
    int i;
    BYTE temp = 0, mask = 0x01;

    for(int i = 0; i < 8; i++) {
        if(B & mask)
            temp ^= A;
 
        A <<= 1;
        mask <<= 1;   
    }

    return temp;
}

// return (A // B)
BYTE field_division(BYTE A, BYTE B) 
{
    int deg_A, deg_B;
    BYTE quotient = 0x00, temp = 0x00;

    while(1) {
        BYTE mask = 0x01;
        deg_A = find_deg(A);
        deg_B = find_deg(B);

        if(deg_A < deg_B)
            break;
        
        A ^= (B << (deg_A - deg_B));
        quotient ^= (mask << (deg_A - deg_B));
    }

    return quotient;
}



BYTE field_inverse(BYTE h)  // A = g(x)
{
    if(h == 0)
        return 0;

    BYTE g = 0x1b;  // 기약다항식

    int deg_h = find_deg(h);
    g ^= (h << (8-deg_h));
    BYTE init_quotient = (0x01 << (8-deg_h)), quotient;

    BYTE t2 = 0, t1 = 1, r, t, temp;
    int flag = 0;

    int round = 0;
    while(h != 0) {
        printf("round = %d\n", round);
        round += 1;

        // q = g div h
        quotient = field_division(g, h);
        if(flag == 0) {
            quotient ^= init_quotient;
            g = 0x1b;
            flag += 1;
        }


        // r = g - h * q
        temp = polynomial_multiplication(h, quotient);
        r = field_addition(g, temp);


        // t <- t2 - q * t1
        temp = polynomial_multiplication(quotient, t1);
        t = field_addition(t2, temp);

        // g <- h, h <- r
        g = h;
        h = r;


        // t2 <- t1, t1 <- t
        t2 = t1;
        t1 = t;
    }

    // t <- t2
    t = t2;
    printf("answer = t = %02x\n", t);

    return t;
}


int find_deg(BYTE A)
{
    BYTE mask = 0x80;
    int deg = 7;

    for(int i = 0; i < 8; i++) {
        if(A & mask)
            break;
        else {
            deg -= 1;
            mask >>= 1;
        }

    }

    return deg;
}