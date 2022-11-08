// Created on iPad-CJH.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _bit_100{
   uint64_t num1 : 36;
   uint64_t num2;
}int_100b;

int isOverflow(uint64_t a, uint64_t b){
   uint64_t sum = a+b;
   if(sum < a)
      return 1;
   if(sum < b)
      return 1;
   return 0;
}

void init100b(char* char_100b, int_100b* tmp){
   uint64_t* tmpNum = (uint64_t*)tmp;
   for(int i = 0; i < 25; i++){
      if(i==9)
         tmpNum = tmpNum+1;
      if( char_100b[i] >= 'A' && char_100b[i] <= 'F' )                
         *tmpNum = *tmpNum * 16 + char_100b[i] - 'A' + 10;
      else if ( char_100b[i] >= 'a' && char_100b[i] <= 'f' )              
         *tmpNum = *tmpNum * 16 + char_100b[i] - 'a' + 10;
      else if ( char_100b[i] >= '0' && char_100b[i] <= '9' )          
         *tmpNum = *tmpNum * 16 + char_100b[i] - '0';      
   }                  
}

int_100b* sum100b(int_100b* operand1, int_100b* operand2){
   int_100b* result = malloc(sizeof(int_100b));
   result->num2 = operand1->num2 + operand2->num2;
   result->num1 = operand1->num1 + operand2->num1 + isOverflow(operand1->num2, operand2->num2);
   result->num1 = result->num1 & 0x0000000111111111;
   return result;
}

int main() {
   setbuf(stdout, NULL); 
   int_100b a; 
   int_100b b;
   //init100b("AABBCCDDEFFFFFFFFFFFFFFF0",&a);
   //init100b("ABCDEFABCDEF0123456789000",&b);
   init100b("FFFFFFFFFFFFFFFFFFFFFFFFF",&a);
   init100b("0000000000000000000000001",&b);
   int_100b* c = sum100b(&a,&b);

   printf("hex : 0x%09llX%016llX",c->num1,c->num2);
   return 0;
}
