#pragma once
#include"Type.h"
#include<string>
using namespace std;
#define checkSlow(pre) if(pre);


enum class GuidFormats
{
	//32����
	Digits,

	/**
	 * 32 digits separated by hyphens.
	 *
	 * For example: 00000000-0000-0000-0000-000000000000
	 */
	DigitsWithHyphens,

	/**
	 * *�����ַ��ָ����ô�������������32λ���֡�
	 *
	 * For example: {00000000-0000-0000-0000-000000000000}
	 */
	DigitsWithHyphensInBraces,

	/**
	 * �����ַ��ָ�����������������32λ���֡�
	 *
	 * For example: (00000000-0000-0000-0000-000000000000)
	 */
	DigitsWithHyphensInParentheses,

	/**
	 * �ö��ŷָ���ʮ������ֵ���ô�������������
	 *
	 * For example: {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
	 */
	HexValuesInBraces,

};
struct Guid
{
public:

	Guid()
		: A(0)
		, B(0)
		, C(0)
		, D(0)
	{ }

	Guid(uint32 InA, uint32 InB, uint32 InC, uint32 InD)
		: A(InA), B(InB), C(InC), D(InD)
	{ }

public:


	friend bool operator==(const Guid& X, const Guid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	}


	friend bool operator!=(const Guid& X, const Guid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	}


	friend bool operator<(const Guid& X, const Guid& Y)
	{
		return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
			((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
			((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
				((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false)))))))); //-V583
	}


	uint32& operator[](int32 Index)
	{
		checkSlow(Index >= 0);
		checkSlow(Index < 4);

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}


	const uint32& operator[](int32 Index) const
	{
		checkSlow(Index >= 0);
		checkSlow(Index < 4);

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}



	/**
	* Guid default string conversion.
	*/
	friend string LexToString(const Guid& Value)
	{
		return Value.ToString();
	}


public:


	



	void Invalidate()
	{
		A = B = C = D = 0;
	}


	bool IsValid() const
	{
		return ((A | B | C | D) != 0);
	}


	string ToString() const
	{
	}



public:



public:


	static  Guid NewGuid();

	
	static  bool Parse(const string& GuidString, Guid& OutGuid);

	
	static  bool ParseExact(const string& GuidString, GuidFormats Format, Guid& OutGuid);

public:

	//��һ����
	uint32 A;

	uint32 B;

	uint32 C;

	uint32 D;
};