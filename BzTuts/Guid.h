#pragma once
#include"Type.h"
#include<string>
using namespace std;
#define checkSlow(pre) if(pre);


enum class GuidFormats
{
	//32个数
	Digits,

	/**
	 * 32 digits separated by hyphens.
	 *
	 * For example: 00000000-0000-0000-0000-000000000000
	 */
	DigitsWithHyphens,

	/**
	 * *由连字符分隔并用大括号括起来的32位数字。
	 *
	 * For example: {00000000-0000-0000-0000-000000000000}
	 */
	DigitsWithHyphensInBraces,

	/**
	 * 用连字符分隔并用括号括起来的32位数字。
	 *
	 * For example: (00000000-0000-0000-0000-000000000000)
	 */
	DigitsWithHyphensInParentheses,

	/**
	 * 用逗号分隔的十六进制值，用大括号括起来。
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

	//第一部分
	uint32 A;

	uint32 B;

	uint32 C;

	uint32 D;
};