// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <atomic>
#include <functional>
#include <future>
#include <phantom/property>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace std
{
class mutex;
class thread;
} // namespace std
namespace phantom
{
namespace detail
{
class Thread;

class ThreadGroup
{
    friend class Thread;

public:
    virtual void lock() = 0;
    virtual void unlock() = 0;

private:
    SmallVector<Thread*> m_Threads;
};

template<class t_Mutex>
class ThreadGroupT : public ThreadGroup
{
public:
    virtual void lock() { m_Mutex.lock(); }
    virtual void unlock() { m_Mutex.unlock(); }

private:
    t_Mutex m_Mutex;
};

class Thread
{
    friend class Phantom;

protected:
    Thread();
    /// Main Thread Constructor
    Thread(StringView a_strName, std::thread::id id);

public:
    Thread(const char* a_strName) : Thread(String(a_strName)) {}
    Thread(StringView a_strName);
    Thread(const std::function<bool()>& a_TickCallback);
    ~Thread(void);

    static Thread* GetCurrent();
    static Thread* GetMain();
    static Thread* Get(std::thread::id id);

    static std::thread::id GetMainId();
    static std::thread::id GetCurrentId();

    std::thread::id getId() const { return m_Id; }

    void setTickCallback(const std::function<bool()>& a_TickCallback);

    void execute();

    void start();

    void interrupt();

    inline bool isLocked() const { return m_MutexCounter != 0; }

    int incrementLock();
    int decrementLock();

    void lock();
    void unlock();

    StringView getName() const { return m_strName; }
    void       setName(StringView a_Name);

    void pushCall(const std::future<void>& a_Future);
    void pushCall(const std::function<void()>& a_Call);
    void pushCallAndWait(const std::function<void()>& a_Call);

    void         setGroup(ThreadGroup* a_pGroup);
    ThreadGroup* getGroup() const { return m_pGroup; }

    void sleepMs(int ms);

protected:
    HAUNT_PROPERTY(StringView, Name);

private:
    void threadStarted();

    void threadEnded();

    struct execution
    {
        execution(Thread* a_pThread, bool* a_pInterruptCheck, bool* a_pTerminated)
            : m_pThread(a_pThread), m_pInterruptCheck(a_pInterruptCheck), m_pTerminated(a_pTerminated)
        {
        }

        void operator()()
        {
            m_pThread->threadStarted();

            while (!(*m_pInterruptCheck))
            {
                m_pThread->execute();
            }

            m_pThread->threadEnded();

            *m_pTerminated = true;
        }

    protected:
        Thread* m_pThread;
        bool*   m_pInterruptCheck;
        bool*   m_pTerminated;
    };

    friend struct execution;

protected:
    String m_strName;

private:
    ThreadGroup*                       m_pGroup = nullptr;
    std::thread*                       m_pStdThread;
    std::function<bool()>              m_Callback;
    SmallVector<std::function<void()>> m_DeferredCalls;
    SmallVector<std::function<void()>> m_Futures;
    std::mutex*                        m_pDeferredCallsMutex;
    std::mutex*                        m_pMutex;
    std::mutex*                        m_pCallbackMutex;
    std::thread::id                    m_Id;
    std::atomic<int>                   m_MutexCounter;
    bool                               m_bInterrupted;
    bool                               m_bTerminated;
};
} // namespace detail

} // namespace phantom
