#include "Frame.h"

string tool::dec2hex(int i) //��intת��16�����ַ���
{
	stringstream ioss; //�����ַ�����
	string s_temp; //���ת�����ַ�
	ioss << setiosflags(ios::uppercase) << hex << i; //��ʮ����(��д)��ʽ���
	//ioss << resetiosflags(ios::uppercase) << hex << i; //��ʮ����(Сд)��ʽ���//ȡ����д������
	ioss >> s_temp;
	return s_temp;
}


//�޷�������16λ  
u_short tool::bswap_16(const u_short &x)
{
	return (((u_short)(x) & 0x00ff) << 8) | \
		(((u_short)(x) & 0xff00) >> 8);
}

//�޷�������32λ
u_int tool::bswap_32(const u_int &x)
{
	return (((u_int)(x) & 0xff000000) >> 24) | \
		(((u_int)(x) & 0x00ff0000) >> 8) | \
		(((u_int)(x) & 0x0000ff00) << 8) | \
		(((u_int)(x) & 0x000000ff) << 24);
}

u_char tool::hex2dec(char a, char b)
{
	u_char x, y;

	if (a >= '0' && a <= '9')
	{
		x = a - '0';
	}
	else
	{
		x = a - 'A' +10;
	}

	if (b >= '0' && b <= '9')
	{
		y = b - '0';
	}
	else
	{
		y = b - 'A' +10;
	}
	u_char res = (x << 4) + y;
	return res;
}

vector<string> tool::split(const string& str, const string& pattern)
{
	vector<string> res;
	if (str == "")
		return res;
	//���ַ���ĩβҲ����ָ����������ȡ���һ��
	string strs = str + pattern;
	size_t pos = strs.find(pattern);

	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
		strs = strs.substr(pos + 2, strs.size());
		pos = strs.find(pattern);
		
		if (temp == "")
		{
			res.push_back(strs);
			return res;
		}
	}

	return res;
}