//#include "GetSysInfo.h"
//
//#include <QDebug>
//#include <Windows.h>
//#include <stdio.h>
//using namespace std;
//#define MB (1024 * 1024)
//
//DWORD deax;
//DWORD debx;
//DWORD decx;
//DWORD dedx;
//
//// init cpu in assembly language
//void initCpu(DWORD veax)
//{
//    __asm
//    {
//        mov eax, veax
//                cpuid
//                mov deax, eax
//                mov debx, ebx
//                mov decx, ecx
//                mov dedx, edx
//    }
//}
//
//long getCpuFreq()
//{
//    int start, over;
//    _asm
//    {
//        RDTSC
//                mov start, eax
//    }
//    Sleep(50);
//    _asm
//    {
//        RDTSC
//                mov over, eax
//    }
//    return (over - start) / 50000;
//}
//
//std::string getManufactureID()
//{
//    char manuID[25];
//    memset(manuID, 0, sizeof(manuID));
//
//    initCpu(0);
//    memcpy(manuID + 0, &debx, 4); // copy to array
//    memcpy(manuID + 4, &dedx, 4);
//    memcpy(manuID + 8, &decx, 4);
//
//    return manuID;
//}
//
//QString getCpuType()
//{
//    const DWORD id = 0x80000002; // start 0x80000002 end to 0x80000004
//    char cpuType[49];
//    memset(cpuType, 0, sizeof(cpuType));
//
//    for (DWORD t = 0; t < 3; t++)
//    {
//        initCpu(id + t);
//
//        memcpy(cpuType + 16 * t + 0, &deax, 4);
//        memcpy(cpuType + 16 * t + 4, &debx, 4);
//        memcpy(cpuType + 16 * t + 8, &decx, 4);
//        memcpy(cpuType + 16 * t + 12, &dedx, 4);
//    }
//
//    return QString(cpuType);
//}
//
//
//QString GetSysInfo::getOsInfo()
//{
//    // get os name according to version number
//    OSVERSIONINFO osver = {sizeof(OSVERSIONINFO)};
//    GetVersionEx(&osver);
//    QString osName;
//    QString version = QString::number((int)(osver.dwMajorVersion)) + "." + QString::number((int)(osver.dwMinorVersion));
//    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
//    {
//        osName = "Windows 2000";
//    }
//    else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
//    {
//        osName = "Windows XP";
//    }
//    else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
//    {
//        osName = "Windows 2003";
//    }
//    else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
//    {
//        osName = "windows vista";
//    }
//    else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
//    {
//        osName = "windows 7";
//    }
//    else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
//    {
//
//        osName = "windows 8 or windows 10";
//    }
//
//    return QString(QStringLiteral("系统名称:")) + osName + QString(QStringLiteral(" 系统版本号:")) + version + ", " ;
//
//}
//
//const QString &GetSysInfo::getCpuInfo()
//{
//    static QString cpuIno;
//    //qDebug() << "CPU main frequency: " << getCpuFreq() << "MHz";
//    //qDebug() << "CPU manufacture: " << QString::fromStdString(getManufactureID());
//    //qDebug() << "CPU type: " << getCpuType();
//    if(cpuIno.isEmpty())
//    {
//        cpuIno = QStringLiteral("cpu型号:") + getCpuType();
//    }
//    return cpuIno;
//
//}
//
//const QString GetSysInfo::getMemoryInfo()
//{
//    MEMORYSTATUSEX statex;
//    statex.dwLength = sizeof (statex);
//    GlobalMemoryStatusEx (&statex);
//    QString memoryInfo;
//    memoryInfo += QStringLiteral("物理内存使用率:") + QString::number(statex.dwMemoryLoad) + ", ";
//    memoryInfo += QStringLiteral("物理内存总量:") + QString::number(statex.ullTotalPhys/MB) + "MB, ";
//    memoryInfo += QStringLiteral("可用的物理内存:") + QString::number(statex.ullAvailPhys/MB)+ "MB, " ;
//    memoryInfo += QStringLiteral("虚拟内存总量:") + QString::number(statex.ullTotalVirtual/MB)+ "MB, " ;
//    memoryInfo += QStringLiteral("可用的虚拟内存:") + QString::number(statex.ullAvailVirtual/MB)+ "MB, ";
//    return memoryInfo;
//}
//
//
//
//int GetSysInfo::getDiskFreeSpace(QString driver)
//{
//    LPCWSTR lpcwstrDriver=(LPCWSTR)driver.utf16();
//    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
//    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
//    {
//        qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
//        return 0;
//    }
//    return (quint64) liTotalFreeBytes.QuadPart/1024/1024/1024;
//}
//
//__int64 CompareFileTime(FILETIME time1, FILETIME time2)
//{
//    __int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
//    __int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;
//
//    return (b - a);
//}
//
//int GetSysInfo::getCpuUsage()
//{
//    HANDLE hEvent;
//    BOOL res;
//    FILETIME preidleTime;
//    FILETIME prekernelTime;
//    FILETIME preuserTime;
//    FILETIME idleTime;
//    FILETIME kernelTime;
//    FILETIME userTime;
//
//    res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
//    preidleTime = idleTime;
//    prekernelTime = kernelTime;
//    preuserTime = userTime;
//
//    hEvent = CreateEventA(NULL, FALSE, FALSE, NULL); // 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
//
////    while (true){
//        WaitForSingleObject(hEvent, 1000);
//        res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
//
//        __int64 idle = CompareFileTime(preidleTime, idleTime);
//        __int64 kernel = CompareFileTime(prekernelTime, kernelTime);
//        __int64 user = CompareFileTime(preuserTime, userTime);
//
//        __int64 cpu = (kernel + user - idle) * 100 / (kernel + user);
//        __int64 cpuidle = (idle)* 100 / (kernel + user);
//        if (cpu>100||cpu<0)
//        {
//            getCpuUsage();
//            return 0;
//        }
////        qDebug() <<QStringLiteral("CPU利用率:")<< cpu << "%" << QStringLiteral(" CPU空闲率:") << cpuidle << "%" << endl;
////        preidleTime = idleTime;
////        prekernelTime = kernelTime;
////        preuserTime = userTime;
////    }
//        return cpu;
//}
//
//
//
//
//
