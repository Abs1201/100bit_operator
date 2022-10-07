#include <iostream>
#include <bitset>
using namespace std;

class Big {
public:
	char m_c[13] = { 0 }; //104 bits

public:
	Big() {}
	Big(int i_in) {
		for (int i = 0; i < 4; i++) { // little endian 
			m_c[i] = i_in >> 8 * i;
		}
	}
	//Big(int** []args, int* ar) {} // Todo) get char array then, put in into m_c[13];

	Big operator+(Big& b) {
		
		bool a_bit = m_c[0] & (1 << 0);
		bool b_bit = b.m_c[0] & (1 << 0);
		bool s = 0; // xor									// Half Adder
		//bool c = a_bit & b_bit; // and
		bool c = 0;

		Big tmp_B;
		tmp_B[0] |= s; //tmp_B.m_c[0]
		bool tmp1 = 0, tmp2 = 0, tmp3 = 0;
		for (int i = 0; i < 4; i++) {
			
			for (int j = 0; j < 8; j++) {								// Full Adder
				tmp1 = ((*this)[i] & (1 << j)) ^ (b[i] & (1 << j));
				tmp2 = tmp1 & c;
				tmp3 = ((*this)[i] & (1 << j)) & (b[i] & (1 << j));
				s = tmp1 ^ c;
				c = tmp2 | tmp3;

				cout << "s: " << s << endl;
				cout << "c: " << c << endl;
				
				tmp_B[i] |= (s << j);
				cout << "tmp_B[i]: " << bitset<8>(tmp_B[i]) << endl;
			}
		}
		if (c) cout << "wow" << endl;

		return tmp_B;
	}

	char& operator[](int index) {
		return this->m_c[index];
	}

	friend ostream& operator <<(ostream& out, Big& b) {
		out << endl;
		for (int i = 0; i < 13; i++) {
			out << bitset<8>(b[i]) << " ";
		}
		out << endl;

		return out;
	}
};

int main(void) {

	Big test1(7),test2(3); // 0111 , 0011 -> 1010
	Big sum;
	cout << "sum: " <<  sum << endl;
	cout << "test1: " << test1 << endl;
	cout << "test2: " << test2 << endl;
	sum = test1 + test2;
	cout << sum << endl;



	return 0;
}