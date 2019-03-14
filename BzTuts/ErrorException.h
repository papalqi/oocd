#pragma once

#include <windows.h>
#include <stdexcept>
#include <winstring.h>
#include <string>
#include <comdef.h>
//将HRESULT转换为string
inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}




class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

	std::wstring ToString()const;

	HRESULT ErrorCode = S_OK;
	std::wstring FunctionName;
	std::wstring Filename;
	int LineNumber = -1;
};
#define IF_FALSE_RETURN_FALSE(input) if(!input)return false;
#define CHECK_HR_RETURN(HR) if(FAILED(HR)){return false;}
#define CHECK_HR_RUN(HR) if(FAILED(HR)){Running=false;}
#define CHECK_AND_OUT(input,ErrorString)  if(input ==false) {MessageBox(0, ErrorString,L"Error", MB_OK);return 1;}
#define CHECK_NULL_RETURN(OTHER) if(OTHER==nullptr){return false;}

//辅助函数帮助判断错误
inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		//throw DxException(hr);
	}
}
