#include "GlobalSignal.h"

GlobalSignal* GlobalSignal::m_pInstance = NULL;

GlobalSignal *GlobalSignal::instance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new GlobalSignal;
    }
    return m_pInstance;
}

GlobalSignal::GlobalSignal()
{

}
