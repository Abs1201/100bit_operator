#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int isOverflow(uint64_t a, uint64_t b);

class _int_100b{
    private:
        uint64_t num1;
        uint64_t num2;
        void stringTo100b(string char_100b);
        string strSum(string a, string b);
    public:
        _int_100b(_int_100b& ref);
        _int_100b(const _int_100b& ref);
        _int_100b(const char* char_100b);
        _int_100b(char* char_100b);
        _int_100b(int num);
        _int_100b();
        _int_100b(long int num);
        _int_100b(short num);
        _int_100b(uint64_t num1 , uint64_t num2);

        
        void printHexNum(void);
        void printUDecimalNum(void);
        void printDecimalNum(void);
        void print(void);

        uint64_t getNum1();
        uint64_t getNum2();

        _int_100b operator+(_int_100b& ref);
        _int_100b operator+(const _int_100b& ref);
        _int_100b operator+(int& nref);
        _int_100b operator+(const int& nref);

        _int_100b operator-(_int_100b& ref);
        _int_100b operator-(const _int_100b& ref);
        _int_100b operator-(int& nref);
        _int_100b operator-(const int& nref);

        _int_100b operator~();
        _int_100b operator>>(const int& nref);
        _int_100b operator>>(int& ref);
        _int_100b operator<<(const int& nref);
        _int_100b operator<<(int& ref);
        bool operator==(const int& nref);
        bool operator==(int& ref);
        bool operator<(_int_100b& ref);
        bool operator<(const _int_100b& ref);
        bool operator>(_int_100b& ref);
        bool operator>(const _int_100b& ref);
        bool operator[](int idx);

        friend _int_100b operator+(int& nref, const _int_100b& origin);
        friend _int_100b operator+(const int& nref, const _int_100b& origin);
        friend _int_100b operator-(int& nref, const _int_100b& origin);
        friend _int_100b operator-(const int& nref, const _int_100b& origin);
};

_int_100b operator+(int& nref, const _int_100b& origin);
_int_100b operator+(const int& nref, const _int_100b& origin);
_int_100b operator-(int& nref, const _int_100b& origin);
_int_100b operator-(const int& nref, const _int_100b& origin);





void _int_100b::stringTo100b(string char_100b){
    int pivot = char_100b.length() > 15 ? char_100b.length()-16 : 0;
    uint64_t* tmpNum = &num1;
    for(int i = 0; i < 25; i++){
        if(i == pivot)
            tmpNum = &num2;
        if(char_100b[i]=='\0')
            break;
        if( char_100b[i] >= 'A' && char_100b[i] <= 'F' )                
            *tmpNum = *tmpNum * 16 + char_100b[i] - 'A' + 10;
        else if ( char_100b[i] >= 'a' && char_100b[i] <= 'f' )              
            *tmpNum = *tmpNum * 16 + char_100b[i] - 'a' + 10;
        else if ( char_100b[i] >= '0' && char_100b[i] <= '9' )          
            *tmpNum = *tmpNum * 16 + char_100b[i] - '0';      
    }
}

string _int_100b::strSum(string a, string b) {
    int diff = max(a.length(), b.length()) - min(a.length(), b.length());

    if(a.length() < b.length())
        for(int i=0; i<diff; i++) a = "0" + a;
    else if(a.length() > b.length())
        for(int i=0; i<diff; i++) b = "0" + b;

    vector<int> c;
    for(int i=0; i<a.length(); i++) c.push_back(a[i] - '0' + b[i] - '0');

    reverse(c.begin(), c.end());

    for(int i=0; i<c.size(); i++) {
        if(c[i] < 10) continue;

        if(i < c.size()-1) c[i+1] += c[i]/10;
        else c.push_back(c[i]/10);

        c[i] %= 10;
    }

    reverse(c.begin(), c.end());

    string ret;

    int i = 0; while(c[i] == 0) i++;
    if(i >= c.size()) ret.push_back('0');

    while(i < c.size()) {
        ret.push_back(char(c[i] + '0'));
        i++;
    }

    return ret;
}
int isOverflow(uint64_t a, uint64_t b){
    uint64_t sum = a+b;
    if(sum < a)
        return 1;
    if(sum < b)
        return 1;
    return 0;
}

_int_100b::_int_100b(_int_100b& ref) : num1(ref.num1), num2(ref.num2) {}
_int_100b::_int_100b(const _int_100b& ref) : num1(ref.num1), num2(ref.num2) {}
_int_100b::_int_100b(const char* char_100b) : num1(0), num2(0){ stringTo100b(char_100b); }
_int_100b::_int_100b(char* char_100b) : num1(0), num2(0){ stringTo100b(char_100b); }
_int_100b::_int_100b(int num) : num1(0), num2(num) {}
_int_100b::_int_100b() : num1(0), num2(0) {}
_int_100b::_int_100b(long int num) : num1(0), num2(num) {}
_int_100b::_int_100b(short num) : num1(0), num2(num) {}
_int_100b::_int_100b(uint64_t num1 , uint64_t num2) : num1(num1), num2(num2) {}


void _int_100b::printHexNum(void){
    cout.width(9);
    cout.fill('0');
    cout<< hex << num1;
    cout.width(16);
    cout.fill('0'); 
    cout<< num2 <<endl;
}
void _int_100b::printUDecimalNum(void){
    string tmp = to_string(0xFFFFFFFFFFFFFFFF);
    tmp = strSum(tmp,to_string(1));
    string arr[36];
    arr[0] = tmp;
    for(int i = 1; i<36; i++){
        arr[i] = strSum(arr[i-1],arr[i-1]);
    }
    string sum = to_string(this->num2);
    for(int i = 0; i < 36; i++){
        if(this->num1>>i & 1){
            sum = strSum(sum, arr[i]);
        }
    }
    cout << sum << endl;
}
void _int_100b::printDecimalNum(void){
    if(!((this->num1)>>35)){
        printUDecimalNum();
        return;
    }
    string tmp = to_string(0xFFFFFFFFFFFFFFFF);
    tmp = strSum(tmp,to_string(1));
    string arr[36];
    arr[0] = tmp;
    for(int i = 1; i<36; i++){
        arr[i] = strSum(arr[i-1],arr[i-1]);
    }
    _int_100b a(1);
    _int_100b b = *this-a;
    _int_100b c(~b.num1, ~b.num2);
    
    string sum = to_string(c.num2);
    for(int i = 0; i < 36; i++){
        if(c.num1>>i & 1){
            sum = strSum(sum, arr[i]);
        }
    }
    cout << dec << "-" << sum << endl;
}
uint64_t _int_100b::getNum1(){
    return num1;
}

uint64_t _int_100b::getNum2(){
    return num2;
}


void _int_100b::print(void){
    printHexNum();
    printDecimalNum();
}
_int_100b _int_100b::operator+(_int_100b& ref){
    return _int_100b(
        (this->num1 + ref.num1 + isOverflow(this->num2,ref.num2))&0x0000000FFFFFFFFF, 
        this->num2 + ref.num2);
}
_int_100b _int_100b::operator+(const _int_100b& ref){
    return _int_100b(
        (this->num1 + ref.num1 + isOverflow(this->num2,ref.num2))&0x0000000FFFFFFFFF, 
        this->num2 + ref.num2);
}
_int_100b _int_100b::operator+(int& nref){
    _int_100b ref(nref);
    return _int_100b(
        (this->num1 + ref.num1 + isOverflow(this->num2,ref.num2))&0x0000000FFFFFFFFF, 
        this->num2 + ref.num2);
}
_int_100b _int_100b::operator+(const int& nref){
    _int_100b ref(nref);
    return _int_100b(
        (this->num1 + ref.num1 + isOverflow(this->num2,ref.num2))&0x0000000FFFFFFFFF, 
        this->num2 + ref.num2);
}
_int_100b _int_100b::operator-(int& nref){
    _int_100b ref(nref);
    _int_100b tmp(
        (~ref.num1)+isOverflow((~ref.num2),1) , 
        (~ref.num2)+1);
    return *this+tmp;
}
_int_100b _int_100b::operator-(const int& nref){
    _int_100b ref(nref);
    _int_100b tmp(
        (~ref.num1)+isOverflow((~ref.num2),1) , 
        (~ref.num2)+1);
    return *this+tmp;
}
_int_100b _int_100b::operator-(_int_100b& ref){
    _int_100b tmp(
        (~ref.num1)+isOverflow((~ref.num2),1) , 
        (~ref.num2)+1);
    return *this+tmp;
}
_int_100b _int_100b::operator-(const _int_100b& ref){
    _int_100b tmp(
        (~ref.num1)+isOverflow((~ref.num2),1) , 
        (~ref.num2)+1);
    return *this+tmp;
}
 
_int_100b operator+(int& nref, const _int_100b& origin){
    _int_100b ref(nref);
    return _int_100b(
        (origin.num1 + ref.num1 + isOverflow(origin.num2,ref.num2))&0x0000000FFFFFFFFF, 
        origin.num2 + ref.num2);
}
_int_100b operator+(const int& nref, const _int_100b& origin){
    _int_100b ref(nref);
    return _int_100b(
        (origin.num1 + ref.num1 + isOverflow(origin.num2,ref.num2))&0x0000000FFFFFFFFF, 
        origin.num2 + ref.num2);
}
_int_100b operator-(int& nref, const _int_100b& origin){
    _int_100b ref(nref);
    _int_100b tmp(
        (~ref.num1)+isOverflow((~ref.num2),1) , 
        (~ref.num2)+1);
    return _int_100b(
        (origin.num1 + tmp.num1 + isOverflow(origin.num2,tmp.num2))&0x0000000FFFFFFFFF, 
        origin.num2 + tmp.num2);
}
_int_100b operator-(const int& nref, const _int_100b& origin){
    _int_100b ref(nref);
    _int_100b tmp(
        (~ref.num1)+isOverflow((~ref.num2),1) , 
        (~ref.num2)+1);
    return _int_100b(
        (origin.num1 + tmp.num1 + isOverflow(origin.num2,tmp.num2))&0x0000000FFFFFFFFF, 
        origin.num2 + tmp.num2);
}

_int_100b _int_100b::operator>>(const int& nref){
    if(nref < 0)
        return (*this)<<abs(nref);
    if(nref==0)
        return *this;
    if (nref<65){
        return _int_100b(this->num1>>nref, (this->num2>>nref)+ (this->num1<<(64-nref)));}
    else if (nref >= 100)
        return _int_100b(0);
    else
        return _int_100b(0, this->num1>>(nref-64));
}
_int_100b _int_100b::operator>>(int& nref){
    if(nref < 0)
        return (*this)<<abs(nref);
    if(nref==0)
        return *this;
    if (nref<65)
        return _int_100b(this->num1>>nref, (this->num2>>nref)+ (this->num1<<(64-nref)));
    else if (nref >= 100)
        return _int_100b(0);
    else
        return _int_100b(0, this->num1>>(nref-64));
}
_int_100b _int_100b::operator<<(const int& nref){
    if(nref < 0)
        return (*this)>>abs(nref);
    if(nref==0)
        return *this;
    if (nref<65)
        return _int_100b((this->num1<<nref)+(this->num2>>(64-nref)), (this->num2)<<nref );
    else if (nref >= 100)
        return _int_100b(0);
    else
        return _int_100b(this->num2<<(nref-64), 0);
}
_int_100b _int_100b::operator<<(int& nref){
    if(nref < 0)
        return (*this)>>abs(nref);
    if(nref==0)
        return *this;
    if (nref<65)
        return _int_100b((this->num1<<nref)+(this->num2>>(64-nref)), (this->num2)<<nref );
    else if (nref >= 100)
        return _int_100b(0);
    else
        return _int_100b(this->num2<<(nref-64), 0);
}
_int_100b _int_100b::operator~(){
    return _int_100b(~num1,~num2);
}

bool _int_100b::operator==(const int& nref){
    return (this->num1==0) && (this->num2 == nref);
}
bool _int_100b::operator==(int& nref){
    return (this->num1==0) && (this->num2 == nref);
}

bool _int_100b::operator>(_int_100b& ref){
    return this->num1 > ref.num1 ? 1 : (this->num1 < ref.num1 ? 0 : (this->num2 > ref.num2 ? 1 : 0));
}
bool _int_100b::operator>(const _int_100b& ref){
    return this->num1 > ref.num1 ? 1 : (this->num1 < ref.num1 ? 0 : (this->num2 > ref.num2 ? 1 : 0));
}

bool _int_100b::operator<(_int_100b& ref){
    return this->num1 < ref.num1 ? 1 : (this->num1 > ref.num1 ? 0 : (this->num2 < ref.num2 ? 1 : 0));
}
bool _int_100b::operator<(const _int_100b& ref){
    return this->num1 < ref.num1 ? 1 : (this->num1 > ref.num1 ? 0 : (this->num2 < ref.num2 ? 1 : 0));
}
bool _int_100b::operator[](int idx){
    return ((*this)>>idx).num2 & 1;
}