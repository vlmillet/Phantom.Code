// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Thread.h"

#include <phantom/utils/SmallMap.h>
/* *********************************************** */
namespace phantom
{
namespace detail
{
static SmallMap<std::thread::id, Thread*> g_Threads;
static std::mutex                         g_ThreadsMutex;
static std::thread::id                    g_MainThreadId = std::this_thread::get_id();

Thread::Thread()
    : m_pStdThread(nullptr),
      m_pMutex(new_<std::mutex>()),
      m_pDeferredCallsMutex(new_<std::mutex>()),
      m_pCallbackMutex(new_<std::mutex>()),
      m_MutexCounter(0),
      m_bInterrupted(false)
{
}

Thread::Thread(StringView a_strName)
    : m_strName(a_strName),
      m_pStdThread(nullptr),
      m_pMutex(new_<std::mutex>()),
      m_pCallbackMutex(new_<std::mutex>()),
      m_pDeferredCallsMutex(new_<std::mutex>()),
      m_MutexCounter(0),
      m_bInterrupted(false),
      m_bTerminated(false)
{
}

Thread::Thread(StringView a_strName, std::thread::id id)
    : m_strName(a_strName),
      m_pStdThread(nullptr),
      m_pMutex(new_<std::mutex>()),
      m_pCallbackMutex(new_<std::mutex>()),
      m_pDeferredCallsMutex(new_<std::mutex>()),
      m_MutexCounter(0),
      m_bInterrupted(false),
      m_bTerminated(false),
      m_Id(id)
{
    threadStarted();
}

Thread::Thread(const std::function<bool()>& a_Callback)
{
    m_Callback = a_Callback;
}

Thread::~Thread(void)
{
    if (m_pStdThread)
    {
        interrupt();
    }
    delete_<std::mutex>(m_pDeferredCallsMutex);
    delete_<std::mutex>(m_pMutex);
    delete_<std::mutex>(m_pCallbackMutex);
}

void Thread::start()
{
    PHANTOM_ASSERT(m_pStdThread == nullptr);
    m_pStdThread = new_<std::thread>(execution(this, &m_bInterrupted, &m_bTerminated));
}

void Thread::interrupt()
{
    PHANTOM_ASSERT(m_pStdThread);
    m_bInterrupted = true;
    if (m_MutexCounter)
    {
        unlock();
    }
    if (m_pStdThread->joinable())
        m_pStdThread->join();
    while (!(m_bTerminated))
        std::this_thread::yield();
    if (m_MutexCounter)
    {
        lock();
    }
    delete_<std::thread>(m_pStdThread);
    m_pStdThread = nullptr;
    m_bInterrupted = false;
}

void Thread::threadStarted()
{
    PHANTOM_ASSERT(m_Id == std::thread::id());
    m_Id = std::this_thread::get_id();
    g_ThreadsMutex.lock();
    g_Threads[m_Id] = this;
    g_ThreadsMutex.unlock();
}

void Thread::threadEnded()
{
    PHANTOM_ASSERT(std::this_thread::get_id() == m_Id);
    g_ThreadsMutex.lock();
    g_Threads.erase(m_Id);
    g_ThreadsMutex.unlock();
    m_Id = std::thread::id();
}

void Thread::setTickCallback(const std::function<bool()>& a_TickCallback)
{
    m_Callback = a_TickCallback;
}

void Thread::execute()
{
    PHANTOM_ASSERT(std::this_thread::get_id() == m_Id);
    if (m_pGroup)
        m_pGroup->lock();
    m_pDeferredCallsMutex->lock();
    for (auto& call : m_DeferredCalls)
        call();
    m_DeferredCalls.clear();
    m_pDeferredCallsMutex->unlock();
    m_pDeferredCallsMutex->lock();
    for (auto& call : m_Futures)
        call();
    m_Futures.clear();
    m_pDeferredCallsMutex->unlock();
    m_pCallbackMutex->lock();
    if (m_Callback)
    {
        auto f = m_Callback;
        m_Callback = std::function<bool()>();
        m_pCallbackMutex->unlock();
        if (f())
        {
            m_pCallbackMutex->lock();
            if (!m_Callback)
            {
                m_Callback = f;
            }
            m_pCallbackMutex->unlock();
        }
    }
    else
    {
        m_pCallbackMutex->unlock();
    }
    if (m_pGroup)
        m_pGroup->unlock();
    std::this_thread::yield();
}

Thread* Thread::GetCurrent()
{
    return Get(GetCurrentId());
}

Thread* Thread::GetMain()
{
    return Get(GetMainId());
}

Thread* Thread::Get(std::thread::id id)
{
    g_ThreadsMutex.lock();
    auto    found = g_Threads.find(id);
    Thread* pThread = found == g_Threads.end() ? nullptr : found->second;
    g_ThreadsMutex.unlock();
    return pThread;
}

std::thread::id Thread::GetMainId()
{
    return g_MainThreadId;
}

std::thread::id Thread::GetCurrentId()
{
    return std::this_thread::get_id();
}

void Thread::lock()
{
    m_pMutex->lock();
}

void Thread::unlock()
{
    m_pMutex->unlock();
}

void Thread::setName(StringView a_Name)
{
    m_strName = a_Name;
}

void Thread::pushCall(const std::function<void()>& a_Call)
{
    if (std::this_thread::get_id() == m_Id)
        a_Call();
    else
    {
        m_pDeferredCallsMutex->lock();
        m_DeferredCalls.push_back(a_Call);
        m_pDeferredCallsMutex->unlock();
    }
}

void Thread::pushCall(const std::future<void>&)
{
    PHANTOM_UNREACHABLE();
}

void Thread::pushCallAndWait(const std::function<void()>& a_Call)
{
    if (std::this_thread::get_id() == m_Id)
        a_Call();
    else
    {
        m_pDeferredCallsMutex->lock();
        m_DeferredCalls.push_back(a_Call);
        m_pDeferredCallsMutex->unlock();

        bool called;
        do
        {
            if (m_pGroup &&
                Thread::GetCurrent()->getGroup() ==
                m_pGroup) // thread waiting and this one are from the same group => we unlock the
                          // thread we are waiting for so that it can perform our call
                m_pGroup->unlock();
            std::this_thread::yield();
            if (m_pGroup && Thread::GetCurrent()->getGroup() == m_pGroup)
                m_pGroup->lock();
            m_pDeferredCallsMutex->lock();
            called = m_DeferredCalls.empty();
            m_pDeferredCallsMutex->unlock();
        } while (!called);
    }
}

void Thread::setGroup(ThreadGroup* a_pGroup)
{
    if (m_pGroup == a_pGroup)
        return;
    if (m_pGroup)
    {
        auto found = std::find(m_pGroup->m_Threads.begin(), m_pGroup->m_Threads.end(), this);
        PHANTOM_ASSERT(found != m_pGroup->m_Threads.end());
        m_pGroup->m_Threads.erase(found);
    }
    m_pGroup = a_pGroup;
    if (m_pGroup)
    {
        PHANTOM_ASSERT(std::find(m_pGroup->m_Threads.begin(), m_pGroup->m_Threads.end(), this) ==
                       m_pGroup->m_Threads.end());
        m_pGroup->m_Threads.push_back(this);
    }
}

void Thread::sleepMs(int ms)
{
    PHANTOM_ASSERT(GetCurrent() == this);
    if (m_pGroup)
        m_pGroup->unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    if (m_pGroup)
        m_pGroup->lock();
}

int Thread::incrementLock()
{
    if (m_MutexCounter == 0)
    {
        lock();
    }
    m_MutexCounter++;
    return m_MutexCounter;
}

int Thread::decrementLock()
{
    m_MutexCounter--;
    if (m_MutexCounter == 0)
    {
        unlock();
    }
    return m_MutexCounter;
}
} // namespace detail
} // namespace phantom
