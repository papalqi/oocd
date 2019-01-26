#pragma once

#include <windows.h>
#include <stdexcept>
//将HRESULT转换为string
inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}
//是否错误的辅助类
class ErrorException : public std::runtime_error
{
public:
	ErrorException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {};
	~ErrorException() {};
	HRESULT Error() const { return m_hr; }
private:
	const HRESULT m_hr;
};
//辅助函数帮助判断错误
inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw ErrorException(hr);
	}
	
}



#define IF_FALSE_RETURN_FALSE(input) if(!input)return false;


