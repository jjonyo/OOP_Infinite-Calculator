#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"

inf_int::inf_int()
{
	this->digits = new char[2];
	this->digits[0] = '0';		// default : 0
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}


inf_int::inf_int(int n) {
	char buf[100];
	if (n < 0) {		// ���� ó��
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int idx = 0;
	while (n > 0) {			// ���ڸ� ���ڿ��� ��ȯ�ϴ� ����
		buf[idx] = n % 10 + '0';

		n /= 10;
		idx++;
	}

	if (idx == 0) {	// ������ ������ 0�� ���
		new (this) inf_int();	// ������ ��ȣ��...gcc���� �����Ͽ����� �ִٰ� ��. inf_int()�� ��� ������ �ν��Ͻ��� ������. 
	}
	else {
		buf[idx] = '\0';

		this->digits = new char[idx + 1];
		this->length = idx;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const char* str)
{
	int i, j;
	//sign set
	if (str[0] == '-') {
		this->thesign = false;
	}
	else {
		this->thesign = true;
	}
	//length set
	this->length = 0;
	i = 0;
	if (this->thesign == false) //str[0]='-'
		i++; 
	while (str[i] != '\0')
	{
		if (!CheckDigit(str[i]))
		{
			this->digits = new char[2];
			this->digits[0] = '#'; // error
			this->digits[1] = '\0';
			this->length = 1;
			return;
		}
		this->length++; //����			
		i++;
	}

	//digit set
	this->digits = new char[length + 1];

	i = 0; //i init
	if (this->thesign == false)
		i++; 

	j = length-1; //str index
	while (j >= 0) {
		digits[j] = str[i];
		i++;
		j--;
	}

	digits[length] = '\0'; //�� ���� ����
}

inf_int::inf_int(const inf_int& a) {
	this->digits = new char[a.length + 1];
	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}

inf_int::~inf_int() {
	delete digits;		// �޸� �Ҵ� ����
}

bool inf_int::CheckDigit(char ch)
{
	if (ch < '0' || ch>'9') return false;
	else return true;
}

std::string inf_int::toString()
{
	int i;
	std::string ret="";
	if (thesign == false) {
		ret+='-';
	}

	if (length == 1)
		ret+=digits;
	else
	{
		bool firstDigitzero = true; //��½� �� �տ� ������ 0 ���� ���� ����
		for (i = length - 1; i >= 0; i--) {
			if (digits[i] == '0' && firstDigitzero) continue;
			ret+=digits[i];
			firstDigitzero = false;
		}
	}

	return ret;
}

void inf_int::Append(char ch)
{
	if (CheckDigit(ch))
	{
		this->digits = (char*)realloc(this->digits, this->length + 2);
		this->length += 1;
		int len = this->length;
		char temp = this->digits[len - 2];
		for (int i = len - 1; i > 0; i--)
		{
			this->digits[i] = this->digits[i - 1];
		}
		this->digits[len] = temp;
		this->digits[0] = ch;
	}
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// �̹� ���ڿ��� ���� ��� ����.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	// we assume 0 is always positive.
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// ��ȣ�� ����, ������ ��ġ�ؾ���.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	bool result; //a>b ? true:false
	int i = 0;
	if (a.thesign == b.thesign) {
		if (a.length > b.length) { 
			result = true; //a>b
		}
		else if (a.length < b.length) { 
			result = false; //a<b
		}
		else { // a.length==b.length
			if (a == b)
				return false;

			//absolute number compare
			i = a.length-1;

			while (i >= 0) {
				if (a.digits[i] > b.digits[i]) {
					result = true;
					break;
				}
				else if (a.digits[i] < b.digits[i]) {
					result = false;
					break;
				}
				else //a.digits[i] == b.digits[i]
					i--;
			}
		}
		// �� �� ����� ��� ���� ���� ���� �״�� return
		if (a.thesign == true)
			return result;
		// �� �� ������ ��� ���� ���� ���� �����Ͽ� return
		else
			return !result;
	}
	//��ȣ�� �ٸ� ���, a�� ����� ��� b�� ����, a�� ������ ��� b�� ����̱⿡ a�� ��ȣ�������� ��ȯ
	else
	{
		return a.thesign;
	}
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

const inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {	// ������ ��ȣ�� ���� ��� + �����ڷ� ����
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// ������ ��ȣ�� �ٸ� ��� - �����ڷ� ����
		if (a.thesign == true) // a : positive, b : negative
		{
			c = b;
			c.thesign = a.thesign;
			return a - c;
		}
		else // a : negaive, b : positive
		{
			c = a;
			c.thesign = b.thesign;
			return b - c;
		}
		
	}
}


const inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;
	inf_int tempA, tempB;

	if (a.thesign == b.thesign) {
		tempA = a; tempA.thesign = true; 
		tempB = b; tempB.thesign = true; 
		//|a|<|b|
		if (tempA < tempB) {
			//c.digits=b.digits
			for (i = 0; i < b.length; i++) {
				c.Add(b.digits[i], i + 1);
			}
			//c.digits[i]- a.digits[i]
			for (i = 0; i < a.length; i++) {
				c.Sub(a.digits[i], i); 
			}
			c.thesign = false;
		}
		//|a|>=|b|
		else {
			//c.digits=a.digits
			for (i = 0; i < a.length; i++) {
				c.Add(a.digits[i], i + 1);
			}
			//c.digits[i]- b.digits[i]
			for (i = 0; i < b.length; i++) {
				c.Sub(b.digits[i], i); 
			}
			c.thesign = true;
		}

		return c;
	}
	else {
		c = b;
		c.thesign = a.thesign; //|b|

		return a + c;
	}
}

const inf_int operator*(const inf_int& a, const inf_int& b)
{
	inf_int c;
	inf_int temp;

	int pow = 0;
	for (unsigned int i = 0; i < b.length; i++)
	{
		temp = a;
		temp.Mult(b.digits[i]);
		for (int i = 0; i < pow; i++)
		{
			temp.Append('0');
		}
		pow++;
		c = c + temp;
	}

	if (a.thesign == b.thesign)
		c.thesign = true;
	else
		c.thesign = false;

	return c;
}

std::ostream& operator<<(std::ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {
		out << '-';
	}

	if (a.length == 1)
		out << a.digits;
	else
	{
		bool firstDigitzero = true; //��½� �� �տ� ������ 0 ���� ���� ����
		for (i = a.length - 1; i >= 0; i--) {
			if (a.digits[i] == '0' && firstDigitzero) continue;
			out << a.digits[i];
			firstDigitzero = false;
		}
	}

	return out;
}

void inf_int::Add(const char num, const unsigned int index)	// a�� index �ڸ����� n�� ���Ѵ�. 0<=n<=9, ex) a�� 391�϶�, Add(a, 2, 2)�� ����� 411
{
	/*�޸� �Ҵ�*/
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1); //�� ���� �ڸ��� ����

		if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
			std::cout << "Memory reallocation failed, the program will terminate.\n";

			exit(0);
		}

		this->length = index;					// ���� ���� , a�� ����
		this->digits[this->length] = '\0';	// �������� �ι��� ����
	}

	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] = '0';
	}

	/*�� ����*/
	this->digits[index - 1] += num - '0';
	/*�ø�*/
	if (this->digits[index - 1] > '9') {	// �ڸ��ø��� �߻��� ���
		this->digits[index - 1] -= 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
		Add('1', index + 1);			// ���ڸ��� 1�� ���Ѵ�
	}
}

void inf_int::Sub(const char num, const unsigned int index) {
	if (this->digits[index] < num) {
		Sub('1', index + 1);
		this->digits[index] += 10;
	}
	this->digits[index] -= num - '0';
}

void inf_int::Mult(const char num) 
{
	int n = num - '0';
	if (n == 0)
	{
		*this = "0";
		return;
	}
	int carry = 0;
	char temp[1000];
	int idx = 0;
	for (unsigned int i = 0; i < this->length; i++)
	{
		int res = (this->digits[i] - '0') * n;

		res = res + carry;
		if (res > 10) //carry �߻�
		{
			carry = res / 10;
			res %= 10;
			temp[idx++] = res +'0';
		}
		else
		{
			carry = 0;
			temp[idx++] = res+'0';
		}
	}

	if (carry != 0)
		temp[idx++] = carry + '0';

	if ((unsigned)idx > this->length) //�޸� ���Ҵ�
	{
		this->digits = (char*)realloc(this->digits, this->length + 2);
		this->length = this->length + 1;
		this->digits[this->length] = '\0';
		for (unsigned int i = 0; i < this->length; i++)
		{
			this->digits[i] = temp[i];
		}
	}
	else
	{
		for (unsigned int i = 0; i < this->length; i++)
		{
			this->digits[i] = temp[i];
		}
	}

	return;
}