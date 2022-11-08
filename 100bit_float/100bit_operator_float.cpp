#include <iostream>
#include <bitset>

using namespace std;

class my_float_100 { // 1, 29, 70
private:
	uint64_t m_f0;
	uint64_t m_f1 : 36;

public:
	my_float_100(string& f_in) {
		
	}

	int getSignificant() {
		return m_f0 & (1 << 63);
	}
	int getExpo() {
		int expo = 0;
		for (int i = 33; i < 62; i++) {
			expo += m_f0 & (1 << i);
		}
		return expo;
	}
	int getMantissa() {
		int mantissa = 0;
		for(int i = )
	}
};

int main(void) {



	return 0;
}