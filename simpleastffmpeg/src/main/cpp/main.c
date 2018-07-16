/*
Administrator="ADSF"
*.fileName: main.c
*..project:DemoLearnFFmpeg01
*.....user:dongxiang 
*.....date:2018/7/13
*..descrip:
//
*/


#include <stdio.h>

int countPPP;
extern void write_extern();
int main(){
    countPPP=5;
    write_extern();


}