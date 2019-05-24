#include "SingletonManager.h"
#include "FilePathManage.h"
#include "ELog.h"
#include "EMainWindow.h"
#include "ThreadManager.h"



SingletonManager::SingletonManager()
{
    FilePathManage::instance();

    ThreadManager::getDownloadThread()->start();
}

SingletonManager::~SingletonManager()
{
    FilePathManage::instance()->release();

    ThreadManager::Release();
    EMainWindow::Release();

}
