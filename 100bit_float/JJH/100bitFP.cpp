#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "100bit.cpp"

using namespace std;

class _float_100b{
    private:
        bool signBit;
        unsigned int exp;
        _int_100b mantisa;
        
    public:
        _float_100b(bool sign, int exp, const char* man) :  signBit(sign),exp(exp),mantisa(_int_100b("400000000000000000")+_int_100b(man)){}
        _float_100b(bool sign, int exp, _int_100b man) : signBit(sign),exp(exp),mantisa(man){}
        _float_100b(char* str){

        }
        _float_100b(const char* str){
            _int_100b num(str);
            this->signBit = num[99];
            this->exp = ((num>>70).getNum2())&0x1FFFFFFF;
            this->mantisa = _int_100b((num.getNum1())&0x3F, num.getNum2()) + _int_100b("400000000000000000");
        }
        _float_100b(_int_100b num){
            this->signBit = num[99];
            this->exp = ((num>>70).getNum2())&0x1FFFFFFF;
            this->mantisa = _int_100b((num.getNum1())&0x3F, num.getNum2()) + _int_100b("400000000000000000");
        }
        
        _float_100b(void) : signBit(0), exp(0), mantisa(_int_100b("400000000000000000")){}
        bool isRound(_int_100b tmp, int shamt){
            if (shamt < 0)
                return 0;
            bool g,s,r;
            g = tmp[shamt]; //guard bit
            r = tmp[shamt-1]; // round bit
            s = shamt > 1 ? !( ((tmp<<(100-shamt-1)) >> (99-shamt))==0 ) : 0; //sticky bit
            return (r==1 && !(g==s && s==0));
        }
        bool isZero(){
            return (exp==0 && mantisa==0);
        }
        bool isInf(){
            return (exp==0x1FFFFFFF && mantisa==0);
        }
        bool isNaN(){
            return (exp==0x1FFFFFFF && !(mantisa==0) );
        }
        _float_100b operator+(_float_100b& ref){
            bool rsign = 0;
            int shamt = 0;
            unsigned int rexp = 0;
            _float_100b operand1, operand2;
            _int_100b rman;
                
            if(*this<ref){ // abs(operand1) >= abs(operand2)
                operand1 = ref;
                operand2 = *this;
            }
            else{
                operand1 = *this;
                operand2 = ref;
            }

            if(operand1.exp==0x1FFFFFFF){ //inf or NaN exception
                if(operand1.mantisa==0) //inf exception
                    return (operand1.signBit == operand2.signBit) ? operand1 : _float_100b(0,0x1FFFFFFF,_int_100b("000000003FFFFFFFFFFFFFFFF"));
                return _float_100b(0,0x1FFFFFFF,_int_100b("000000003FFFFFFFFFFFFFFFF")); //NaN exception
            }
            if((operand2.exp==0x00000000)&&(operand2.mantisa==0)) //zero exception
                return operand1;

            rsign = operand1.signBit; // return signBit
            rexp = operand1.exp; // tmp exponent
            shamt = operand1.exp - operand2.exp; //shift amount

            if(operand1.signBit == operand2.signBit) //add 
                rman = operand1.mantisa + (operand2.mantisa>>shamt)+(int)isRound(operand2.mantisa,shamt);
            else //sub // 2's complement
                rman = operand1.mantisa+((~((operand2.mantisa>>shamt)+(int)isRound(operand2.mantisa,shamt)))+1);

            int nomalization = 0; //re-align
            for(int i = 99; !((rman>>i)==1) ; i-- ){
                nomalization++;
                if (i == -1)
                    return _float_100b(rsign,0x00000000,_int_100b(0)); //same operand value exception
            }
            shamt = 29 - nomalization;
            rexp = rexp+shamt; //return exponent
            rman = (rman>>shamt)+(int)isRound(rman,shamt); //return mantisa //if shamt < 0 then will be left shift

            if(rexp>0x1FFFFFFF){ //over/underflow exception
                rexp = shamt > 0 ? 0x1FFFFFFF : 0x00000000;
                rman = _int_100b(0);
            }
            
            return _float_100b(rsign,rexp,rman);
        }
        _float_100b operator-(_float_100b& ref){
            _float_100b tmp(ref);
            tmp.signBit = !tmp.signBit;
            return *this + tmp;
        }

        bool operator<(_float_100b& ref){
            return this->exp < ref.exp ? 1 : (this->exp > ref.exp ? 0 : (this->mantisa < ref.mantisa ? 1 : 0));
        }
        bool operator>(_float_100b& ref){
            return this->exp > ref.exp ? 1 : (this->exp < ref.exp ? 0 : (this->mantisa > ref.mantisa ? 1 : 0));
        }  
        friend ostream& operator<<(ostream& out, _float_100b num){
            if (num.signBit)
                out << "-";
            if (num.isZero())
                out << "0.0";
            else if (num.isInf())
                out << "Inf";
            else if (num.isNaN())
                out << "NaN";
            else{
                uint64_t tmp = (((uint64_t)num.signBit)<<35) + (((uint64_t)num.exp)<<6) + (num.mantisa.getNum1()&0x000000000000003F);
                out.width(9);
                out.fill('0');
                out << hex <<tmp ;
                out.width(16);
                out.fill('0');
                out << hex <<num.mantisa.getNum2();
                out << dec;
            }
            
            return out;
        }
        

};


int main() {
    
    _float_100b tmp1(0,0x10000004,"3F0000000000000000"); //63.5
    cout << "tmp1 : "<<tmp1 << endl;
                    
    _float_100b tmp2("4000000AC2000000000000000"); //13.515625
    _float_100b tmp4("C000000AC2000000000000000"); //-13.515625
    cout << "tmp4 : " <<tmp4 << endl;
    
    _float_100b tmp3(tmp1 - tmp2); //49.984375
    cout << "tmp3 : " << tmp3 << endl;

    cout << "tmp4 - tmp4 : "<< (tmp4 - tmp4) << endl;
    return 0;
}