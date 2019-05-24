#ifndef REGEXP_H
#define REGEXP_H

#include <QValidator>

class RegExpFun
{
public:
    explicit RegExpFun(QObject *parent = 0);
    static QValidator* getNumValidator(); // 纯数字
    static QValidator* getNumNegativeValidator(); // 带负号的数字
    static QValidator* getFloatOneDigitValidator(); // 一位小数
    static QValidator* getFloatTwoDigitValidator(); // 两位小数
    static QValidator* getFloatThreeDigitValidator(); // 三位小数
    static QValidator* getIpAddressValidator(); // IP地址
    static QValidator* getLocalPhoneValidator();//手机号
    static QValidator* getForeignPhoneValidator();//手机号
    static QValidator* getVerifyCodeValidator();//验证码
    static QValidator* getPasswordValidator();//密码

private:
    static QValidator* m_pNum_V; // 纯数字
    static QValidator* m_pNumNegative_V; // 带负号的数字
    static QValidator* m_pFloatOneDigit_v; // 一位小数
    static QValidator* m_pFloatTwoDigit_v; // 两位小数
    static QValidator* m_pFloatThreeDigit_v; // 三位小数
    static QValidator* m_pIpAddress_v; // ip地址
    static QValidator* m_pLocalPhone_v;//国内手机号
    static QValidator* m_pForeignPhone_v;//国外手机号
    static QValidator* m_pVerifyCode_v;//验证码
    static QValidator* m_pPassword_v;//密码
};

#endif // REGEXP_H
