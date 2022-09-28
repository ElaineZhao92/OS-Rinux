#include "print.h"
#include "sbi.h"

void puts(char *s) {
    // unimplemented
    int i = 0;
    while(s[i++]!='\0')
        sbi_ecall(0x1, 0x0, s[i], 0,0,0,0,0);
    //sbi_ecall的第三个参数是输出字符的asc码，只要字符不是'\0'就可以输出
}

void puti(int x) {
    // unimplemented
    if(x < 0){
        char c = '-';
        sbi_ecall(0x1, 0x0, c, 0,0,0,0,0);
        x = x * (-1);
    }
    int a = x, cnt = 1, num = 1;
    while(x >= cnt*10) {
        cnt *= 10; //整数位数
        num ++;
        a /= 10;
    }
    for(int i = 0; i<num; i++){
        int t = x / cnt;
        sbi_ecall(0x1, 0x0, t+48, 0,0,0,0,0);
        x %= cnt;
        cnt /= 10;
    }


}
