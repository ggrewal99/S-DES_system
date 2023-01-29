#ifndef SDES_H
#define SDES_H

using namespace std;

class Sdes{
private:
	string text_in;
	string k;
	int opt;		// 1 = encryption, 2 = decryption
	string text_out;
	int S0[4][4] = {{1, 0, 3, 2},
					{3, 2, 1, 0},
					{0, 2, 1, 3},
					{3, 1, 3, 2}};
	int S1[4][4] = {{0, 1, 2, 3},
					{2, 0, 1, 3},
					{3, 0, 1, 0},
					{2, 1, 0, 3}};

	string permute(string, int[], int);
	string shift(string, int);
	string x_or(string, string);
	string sbox(string, int);
	string sw();
	string ip_inv();
	string fk(string, string);

	void getKeys(string&, string&);

	/* Encryption/Decryption */
	string process(string, string, int);

public:
	Sdes(string _text_in, string _k, int _opt){
		text_in = _text_in;
		k = _k;
		opt = _opt;
	}
	void run();
};

/* Takes string input, 'function', and string size as argument */
string Sdes::permute(string temp, int f[], int size){
	string out(size, ' ');
	for(int i = 0; i < size; i++){
		out[i] = temp[f[i] - 1];
	}
	return out;
}

/* Exclusive OR function */
string Sdes::x_or(string a, string b){
	string res(a.size(), ' ');
	for(int i = 0; i < a.size(); i++){
		if(a[i] == b[i])
			res[i] = '0';
		else
			res[i] = '1';
	}
	return res;
}

/* Shift left by amount specified in shamt */
string Sdes::shift(string s, int shamt){
	string out(5, ' ');
	for(int i = 0; i < 5; i++){
		int a = i - shamt;
		if(a < 0)
			a = a + 5;
		out[a] = s[i];
	}
	return out;
}

/* Returns sbox value */
string Sdes::sbox(string b, int box_num){
	string r = "00", c = "00";
	string res = "";
	int row = 0, col = 0;
	r[0] = b[0], r[1] = b[3];
	c[0] = b[1], c[1] = b[2];
	cout << "r: " << r << endl;
	cout << "c: " << c << endl;

	map<int, char> myMap;
	myMap[1] = 1, myMap[0] = 2;
	for(int i = 0; i < 2; i++){
		if(r[i] == '1')
			row = row + myMap[i];
		if(c[i] == '1')
			col = col + myMap[i];
	}
	cout << "row: " << row << endl;
	cout << "col: " << col << endl;
	int val;
	if(box_num == 0)
		val = S0[row][col];
	else
		val = S1[row][col];
	cout << "Val: " << val << endl;
	for(int i = 0; val > 0; i++){
		res = to_string(val % 2) + res;
		val = val / 2;
	}
	if(res.size() == 0)
		res = "00";
	else if(res.size() == 1)
		res = "0" + res;
	return res;
}

string Sdes::fk(string pt2, string k1){
	string pt, pt1, s0, s1, pt_4bit;
	int ep_f[] = {4, 1, 2, 3, 2, 3, 4, 1},
			p4_f[] = {2, 4, 3, 1};
	pt = permute(pt2, ep_f, 8);
	pt = x_or(pt, k1);
	cout << "xor: " << pt << endl;

	pt1 = pt.substr(0, 4);
	pt2 = pt.substr(4);

	s0 = sbox(pt1, 0);
	s1 = sbox(pt2, 1);

	cout << "sbox0: " << s0 << endl;
	cout << "sbox1: " << s1 << endl;
	pt_4bit = s0 + s1;
	cout <<"Before p4: " << pt_4bit << endl;
	pt_4bit = permute(pt_4bit, p4_f, 4);
	cout << "p4: " << pt_4bit << endl;

	return pt_4bit;
}

string Sdes::process(string k1, string k2, int opt){
	int ip_f[] = {2, 6, 3, 1, 4, 8, 5, 7},
		ip_inv_f[] = {4, 1, 3, 5, 7, 2, 8, 6};
	string in = text_in,	// temp variable to hold an 8 bit value
			in1, in2, // first 4 and last 4 bits of ip
			temp_in2, // temp variables to hold 2nd half
			in_4bit,			// holds 4 bit p4 value
			out;

	in = permute(in, ip_f, 8);
	in1 = in.substr(0, 4);	// first 4 bits of ip
	in2 = in.substr(4);	// last 4 bits of ip

	if(opt == 1)	// If encryption, use k1 else k2
		in_4bit = fk(in2, k1);
	else
		in_4bit = fk(in2, k2);
	in_4bit = x_or(in1, in_4bit);
	in = in2 + in_4bit;

	/* Same process again */

	in1 = in.substr(0, 4);
	in2 = in.substr(4);

	if(opt == 1)	// If encryption use k2 else k1
		in_4bit = fk(in2, k2);
	else
		in_4bit = fk(in2, k1);
	in_4bit = x_or(in1, in_4bit);
	in = in_4bit + in2 ;
	cout <<"pt: " << in << endl;
	out = permute(in, ip_inv_f, 8);

	return out;
}

void Sdes::getKeys(string& k1, string& k2){
	string temp_k = k, temp_k_left, temp_k_right, temp_k1, temp_k2;
	int p10_f[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6},
		p8_f[] = {6, 3, 7, 4, 8, 5, 10, 9};
	temp_k = permute(temp_k, p10_f, 10);
	cout << "p10: " << temp_k << endl;

	temp_k_left = temp_k.substr(0, 5); // LEFT 5
	temp_k_right = temp_k.substr(5); 	// RIGHT 5
	temp_k_left = shift(temp_k_left, 1);
	temp_k_right = shift(temp_k_right, 1);
	cout << "Shift left: " << temp_k_left << endl;
	cout << "Shift right: " << temp_k_right << endl;

	/* Key 1 */
	temp_k1 = temp_k_left + temp_k_right;
	k1 = permute(temp_k1, p8_f, 8);
	//cout << "k1: " << k1 << endl;
	/* end */

	temp_k_left = temp_k1.substr(0, 5);
	temp_k_right = temp_k1.substr(5);
	temp_k_left = shift(temp_k_left, 2);
	temp_k_right = shift(temp_k_right, 2);

	/* key 2 */
	temp_k2 = temp_k_left + temp_k_right;
	k2 = permute(temp_k2, p8_f, 8);
	//cout << "k2: " << k2 << endl;
		/* end */
}
void Sdes::run(){
	string k1, k2;
	getKeys(k1, k2);

	text_out = process(k1, k2, opt);
	cout << "K1: " << k1 << "\tK2: " << k2;
	if(opt == 1)
		cout << "\tCiphertext: " << text_out << endl;
	if(opt == 2)
		cout << "\tPlaintext: " << text_out << endl;

}

#endif
