#include "RegExpFun.h"

QValidator* RegExpFun::m_pNum_V = NULL; // 纯数字
QValidator* RegExpFun::m_pNumNegative_V = NULL; // 带负号的数字
QValidator* RegExpFun::m_pFloatOneDigit_v = NULL; // 一位小数
QValidator* RegExpFun::m_pFloatTwoDigit_v = NULL; // 两位小数
QValidator* RegExpFun::m_pFloatThreeDigit_v = NULL; // 三位小数
QValidator* RegExpFun::m_pIpAddress_v = NULL; // ip地址
QValidator* RegExpFun::m_pLocalPhone_v;//国内手机号
QValidator* RegExpFun::m_pForeignPhone_v = NULL;//国外
QValidator* RegExpFun::m_pVerifyCode_v;//验证码
QValidator* RegExpFun::m_pPassword_v;//密码

QValidator *RegExpFun::getNumValidator()
{
    if (m_pNum_V == NULL)
    {
        QRegExp regx("[0-9]+");
        m_pNum_V = new QRegExpValidator(regx);
    }
    return m_pNum_V;
}

QValidator *RegExpFun::getNumNegativeValidator()
{
    if (m_pNumNegative_V == NULL)
    {
        QRegExp regx("-?[0-9]+");
        m_pNumNegative_V = new QRegExpValidator(regx);
    }
    return m_pNumNegative_V;
}

QValidator *RegExpFun::getFloatOneDigitValidator()
{
    if (m_pFloatOneDigit_v == NULL)
    {
        QRegExp regx("^[0-9]+(\\.[0-9]{1})");
        m_pFloatOneDigit_v = new QRegExpValidator(regx);
    }
    return m_pFloatOneDigit_v;
}

QValidator *RegExpFun::getFloatTwoDigitValidator()
{
    if (m_pFloatTwoDigit_v == NULL)
    {
        QRegExp regx("^[0-9]+(\\.[0-9]{2})");
        m_pFloatTwoDigit_v = new QRegExpValidator(regx);
    }
    return m_pFloatTwoDigit_v;
}

QValidator *RegExpFun::getFloatThreeDigitValidator()
{
    if (m_pFloatThreeDigit_v == NULL)
    {
        QRegExp regx("^[0-9]+(\\.[0-9]{3})");
        m_pFloatThreeDigit_v = new QRegExpValidator(regx);
    }
    return m_pFloatThreeDigit_v;
}

QValidator *RegExpFun::getIpAddressValidator()
{
    if (m_pIpAddress_v == NULL)
    {
        QRegExp regx("^([1]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([1]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([1]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([1]?\\d\\d?|2[0-4]\\d|25[0-5])$");
        m_pIpAddress_v = new QRegExpValidator(regx);
    }
    return m_pIpAddress_v;
}

QValidator *RegExpFun::getLocalPhoneValidator()
{
    if (m_pLocalPhone_v == NULL)
    {
//        QRegExp regx("^((1[3,5,8][0-9])|(14[5,7])|(17[0,6,7,8])|(19[7]))\\d{8}$");
        QRegExp regx("^1[0-9]{10}");
        m_pLocalPhone_v = new QRegExpValidator(regx);
    }
    return m_pLocalPhone_v;
}

QValidator *RegExpFun::getForeignPhoneValidator()
{
    if (m_pForeignPhone_v == NULL)
    {
        QRegExp regx("[0-9,-]{20}");
        m_pForeignPhone_v = new QRegExpValidator(regx);
    }
    return m_pForeignPhone_v;
}

QValidator *RegExpFun::getVerifyCodeValidator()
{
    if (m_pVerifyCode_v == NULL)
    {
        QRegExp regx("[0-9]{6}");
        m_pVerifyCode_v = new QRegExpValidator(regx);
    }
    return m_pVerifyCode_v;
}

QValidator *RegExpFun::getPasswordValidator()
{
    if (m_pPassword_v == NULL)
    {
        QRegExp regx("^[^ ]{20}");
        m_pPassword_v = new QRegExpValidator(regx);
    }
    return m_pPassword_v;
}

