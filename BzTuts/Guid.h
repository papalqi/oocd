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

	friend void LexFromString(Guid& Result, const TCHAR* String)
	{
		Guid::Parse(String, Result);
	}

public:

	/**
	 * Exports the GUIDs value to a string.
	 *
	 * @param ValueStr Will hold the string value.
	 * @param DefaultValue The default value.
	 * @param Parent Not used.
	 * @param PortFlags Not used.
	 * @param ExportRootScope Not used.
	 * @return true on success, false otherwise.
	 * @see ImportTextItem
	 */
	 bool ExportTextItem(string& ValueStr, Guid const& DefaultValue, UObject* Parent, int32 PortFlags, class UObject* ExportRootScope) const;



	/**
	 * Invalidates the GUID.
	 *
	 * @see IsValid
	 */
	void Invalidate()
	{
		A = B = C = D = 0;
	}

	/**
	 * Checks whether this GUID is valid or not.
	 *
	 * A GUID that has all its components set to zero is considered invalid.
	 *
	 * @return true if valid, false otherwise.
	 * @see Invalidate
	 */
	bool IsValid() const
	{
		return ((A | B | C | D) != 0);
	}

	/**
	 * Converts this GUID to its string representation.
	 *
	 * @return The string representation.
	 */
	string ToString() const
	{
		return ToString(GuidFormats::Digits);
	}

	/**
	 * Converts this GUID to its string representation using the specified format.
	 *
	 * @param Format The string format to use.
	 * @return The string representation.
	 */
	 string ToString(GuidFormats Format) const;

public:

	/**
	 * Calculates the hash for a GUID.
	 *
	 * @param Guid The GUID to calculate the hash for.
	 * @return The hash.
	 */
	friend uint32 GetTypeHash(const Guid& Guid)
	{
		return FCrc::MemCrc_DEPRECATED(&Guid, sizeof(Guid));
	}

public:

	/**
	 * Returns a new GUID.
	 *
	 * @return A new GUID.
	 */
	static  Guid NewGuid();

	/**
	 * Converts a string to a GUID.
	 *
	 * @param GuidString The string to convert.
	 * @param OutGuid Will contain the parsed GUID.
	 * @return true if the string was converted successfully, false otherwise.
	 * @see ParseExact, ToString
	 */
	static  bool Parse(const string& GuidString, Guid& OutGuid);

	/**
	 * Converts a string with the specified format to a GUID.
	 *
	 * @param GuidString The string to convert.
	 * @param Format The string format to parse.
	 * @param OutGuid Will contain the parsed GUID.
	 * @return true if the string was converted successfully, false otherwise.
	 * @see Parse, ToString
	 */
	static  bool ParseExact(const string& GuidString, GuidFormats Format, Guid& OutGuid);

public:

	//��һ����
	uint32 A;

	uint32 B;

	uint32 C;

	uint32 D;
};