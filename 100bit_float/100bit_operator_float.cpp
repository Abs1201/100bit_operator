#include <iostream>
#include <bitset>
#include <string>

#define BIAS_29 268435455

using namespace std;

int power(int n, int expo){
	int result =1;
	for(int i = 0; i < expo; i++){
		result *= n;
	}
	return result;
}


struct mantissa_70 {
	uint64_t mantissa0 : 64;
	char mantissa1 : 6;
};

string itos_hex(int n) {
	char hex[16] = { '0', '1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
	string result = "";
	int rest = 0;
	while (n >= 16) {
		rest = n % 16;
		n /= 16;
		result = hex[rest] + result;
	}
	result = hex[n] + result;

	return result;
}

class my_float_100 { // 1, 29, 70
private:
	uint64_t m_f0;
	uint64_t m_f1 : 36;

public:
	my_float_100(string f_in) {
		m_f0 = 0;
		m_f1 = 0;
		int i = 1;
		uint64_t int_part = 0;
		string tmp = "";
		int mantissa_len_d = 0, mantissa_len_b = 0;
		int mantissa = 0;
		if (f_in.length() != 0) {
			if (f_in[0] == '-') {
				m_f0 = 1 << 63;
				for (i = 1; f_in[i] != '.'; i++) {
					tmp += f_in[i];
				}
				i += 1;
				mantissa_len_d = f_in.length() - i;
			}
			else {
				for (i = 0; f_in[i] != '.'; i++) {
					tmp += f_in[i];
				}
				i += 1;
				mantissa_len_d = f_in.length() - i;
			}
			int_part = stoi(tmp);

			tmp = "";
			for (int j = i; j < f_in.length(); j++) {
				tmp += f_in[j];
			}
			mantissa = stoi(tmp);

			int int_part_len_b_m1=0, int_part_len_b = 0;

			int temp = int_part;
			if (int_part > 0) {
				while (temp != 0) {	 // 0.XXX impossible
					temp >>= 1;
					int_part_len_b++;
				}
				int_part_len_b_m1 = int_part_len_b-1;
			}
			int standard = power(10, (int)mantissa_len_d);
			int mantissa_idx = 64 - int_part_len_b; //todo) +6 mantissa part
			while (mantissa_idx > 0 && mantissa != 0) {
				mantissa *= 2;
				if (mantissa >= standard) {
					int_part <<= 1;
					int_part += 1;
					mantissa -= standard;
				}
				else int_part <<= 1;
				mantissa_idx--;
				mantissa_len_b++; //todo) plif from while
			}

			if (int_part_len_b == 0 && int_part > 0) { // if int starts with 0.xx(x>0)
				temp = int_part;
				while (temp != 1) {
					temp >>= 1;
					int_part_len_b--;
				}
				int_part_len_b_m1 = int_part_len_b +1;
			}
			cout << bitset<64>(m_f0) << endl;
			cout << bitset<64>((m_f0 + BIAS_29) << 34) << endl;
			uint64_t expo = int_part_len_b_m1 +BIAS_29;
			m_f0 += expo << 34;
			cout << "int_part: " << bitset<64>(int_part) << endl;
			if(int_part_len_b + mantissa_len_b < 34){
				m_f0 += int_part << 34-(int_part_len_b + mantissa_len_b);
			}
			else{
				m_f1 += int_part;
				m_f1 <<= 64-(int_part_len_b + mantissa_len_b)+30;
				m_f0 += int_part >> (int_part_len_b + mantissa_len_b) - 34;
			}
			m_f0 += int_part << 34-(int_part_len_b + mantissa_len_b);
			cout << bitset<64>(m_f0) << endl;
			// add mantissa to m_f0, m_f1


		}
	}

	uint64_t getSignificant() {// 1
		return m_f0 & (1 << 63);
	}
	uint64_t getExpo() {// 29 
		int expo = 0;
		for (int i = 34; i < 63; i++) {
			expo += m_f0 & (1 << i);
		}
		return expo;
	}
	mantissa_70 getMantissa() { // 70
		mantissa_70 m;
		for (int i = 0; i < 34; i++) {
			m.mantissa0 += m_f0 & (1 << i);
		}
		m.mantissa0 <<= 30;
		uint64_t tmp = 0;
		for (int i = 6; i < 36; i++) {
			tmp += m_f1 & (1 << i);
		}
		tmp >>= 6;
		m.mantissa0 += tmp;
		for (int i = 0; i < 6; i++) {
			m.mantissa1 += m_f1 & (1 << i);
		}
		m.mantissa1 <<= 2;

		return m;
	}
	void setSignificant(int newSignificant) {
		int tmp = 0;
		for (int i = 0; i < 63; i++) {
			tmp += (1 << i);
		}
		m_f0 &= tmp;
		m_f0 += newSignificant;
	}
	void setExpo(int newExpo) { // 29
		uint64_t tmp = 0;
		for (int i = 0; i < 34; i++) {
			tmp += (1 << i);
		}
		tmp += (1 << 63);
		m_f0 &= tmp;
		m_f0 += newExpo;
	}
	void setMantissa(mantissa_70 newMantissa) {
		m_f1 = 0;
		uint64_t tmp = 0;
		m_f0 >>= 34;
		m_f0 <<= 34;
		tmp = newMantissa.mantissa0 >> 30;
		m_f0 += tmp;

		m_f1 = newMantissa.mantissa0 << 34;
		tmp = 0;
		tmp = (uint64_t)newMantissa.mantissa1;
		tmp <<= 28;
		m_f1 += tmp;
	}
};

int main(void) {
	my_float_100 f("13.75");


	return 0;
}
