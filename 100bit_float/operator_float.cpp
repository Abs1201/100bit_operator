#include <iostream>
#include <bitset>
#include <math.h>
#include <string>
#include <sstream>

#define BIAS_8 127



using namespace std;


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

class my_float {
private:
	int m_f;

public:
	my_float(string f_in = "") {
		m_f = 0;
		int i = 1;
		int int_part = 0;
		string tmp = "";
		int mantissa_len_d = 0, mantissa_len_b = 0;
		int mantissa = 0;
		if (f_in.length() != 0) {
			if (f_in[0] == '-') {
				m_f = 1 << 31;
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

			int int_part_len_b = 0;
			int temp = int_part;
			while (temp != 1) {
				temp >>= 1;
				int_part_len_b++;
			}
			int standard = pow(10, mantissa_len_d);
			int mantissa_idx = 23 - int_part_len_b;
			while (mantissa_idx > 0 && mantissa != 0) {
				mantissa *= 2;
				if (mantissa >= standard) {
					int_part <<= 1;
					int_part += 1;

					mantissa -= standard;
				}
				else {
					int_part <<= 1;
				}
				mantissa_idx--;
				mantissa_len_b++;
			}

			int expo = -mantissa_len_b;
			temp = int_part;
			while (temp != 1) {
				temp >>= 1;
				expo++;
			}
			temp = 0;
			temp = expo + mantissa_len_b; //5 = 3+2
			int mask = 0;
			for (i = 0; i < temp; i++) {
				mask <<= 1;
				mask++;
			}
			mantissa = mask & int_part;
			m_f += mantissa << (23 - temp);
			expo += BIAS_8;
			expo <<= 23;
			m_f += expo;
		}
	}

	int getSignificant() {
		return m_f & (1 << 31);
	}
	int getMantissa() {
		int mantissa = 0;
		for (int i = 0; i < 23; i++) {
			mantissa += m_f & (1 << i);
		}
		return mantissa;
	}
	int getExpo() {
		int expo = 0;
		for (int i = 23; i < 31; i++) {
			expo += m_f & (1 << i);
		}

		return expo;
	}
	void setSignificant(int newSignificant) {
		int tmp = 0;
		for (int i = 0; i < 31; i++) {
			tmp += (1 << i);
		}
		m_f &= tmp;
		m_f += newSignificant;
	}
	void setMantissa(int newMantissa) {
		int tmp = 0;
		for (int i = 23; i < 32; i++) {
			tmp += (1 << i);
		}
		m_f &= tmp;
		m_f += newMantissa;
	}
	void setExpo(int newExpo) {
		int tmp = 0;
		for (int i = 0; i < 23; i++) {
			tmp += (1 << i);
		}
		tmp += (1 << 31);
		m_f &= tmp;
		m_f += newExpo;
	}

	my_float& operator+ (my_float& f) {
		int expo1 = this->getExpo();
		int expo2 = f.getExpo();
		expo1 >>= 23;
		expo2 >>= 23;
		int result = 0;
		int expoGap = abs(expo1 - expo2);
		int smaller_mantissa = 0;
		if (expo1 > expo2) {
			smaller_mantissa = f.getMantissa();
			smaller_mantissa += (1 << 23);
			for (int i = 0; i < expoGap; i++) {
				smaller_mantissa >>= 1;
			}
			int bigger_mantissa = this->getMantissa();
			bigger_mantissa += (1 << 23);
			bigger_mantissa += smaller_mantissa;
			if (bigger_mantissa & (1 << 24)) {
				expo1++;
				bigger_mantissa >>= 1;
			}
			this->setExpo(expo1);
			this->setMantissa(bigger_mantissa);
		}
		else {
			smaller_mantissa = this->getMantissa();
			smaller_mantissa += (1 << 23);
			for (int i = 0; i < expoGap; i++) {
				smaller_mantissa >>= 1;
			}
			int bigger_mantissa = f.getMantissa();
			bigger_mantissa += (1 << 23);
			bigger_mantissa += smaller_mantissa;
			if (bigger_mantissa & (1 << 24)) {
				expo2++;
				bigger_mantissa >>= 1;
			}
			this->setExpo(expo2);
			this->setMantissa(bigger_mantissa);
		}
		cout << bitset<32>(m_f) << endl;

		return *this;
	}

	friend ostream& operator << (ostream& out, my_float f) { // Todo) express hex.
		string result = "";
		if (f.getSignificant()) {
			result += '-';
		}
		int expo = f.getExpo();
		expo >>= 23;
		expo -= BIAS_8;
		int mantissa = f.getMantissa();
		mantissa += (1 << 23);
		int int_part = (mantissa >> (23 - expo));
		//std::ostringstream ss;
		//ss << "0x" << std::hex << int_part;
		//result += ss.str();
		cout << "int_par: " << int_part << endl;
		result += itos_hex(int_part);
		result += '.';
		/*for(int i = 22-expo; i)*/
		int tmp = 0;
		for (int i = 0; i < 23 - expo; i++) {
			tmp += mantissa & (1 << i);
		}
		cout << "tmp: " << bitset<20>(tmp) << endl;
		result += itos_hex(tmp);

		out << result;

		return out;
	}
};



int main(void) {

	float* ff = new float(13.75);
	my_float f("13.75");
	my_float f2("1.5");
	//f = f + f2;
	cout << f << endl;

	int i = 4; // 0b100

	cout << (i >> 0) << endl;
	cout << (i << 0) << endl;
	cout << (1 << 0) << endl;

	return 0;
}
