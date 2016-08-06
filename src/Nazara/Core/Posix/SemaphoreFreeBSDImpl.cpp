// Copyright (C) 2015 Alexandre Janniaux
// This file is part of the "Nazara Engine - Core module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Core/Posix/SemaphoreFreeBSDImpl.hpp>
#include <Nazara/Core/Config.hpp>
#include <Nazara/Core/Error.hpp>
#include <limits>
#include <sys/time.h>
#include <Nazara/Core/Debug.hpp>

namespace Nz
{
    SemaphoreImpl::SemaphoreImpl(unsigned int count)
    {
        const char* name = "NazaraSemaphore";
        m_semaphore = sem_open(name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, count);
        if (m_semaphore == SEM_FAILED)
            NazaraError("Failed to create semaphore: " + Error::GetLastSystemError());
        else
            sem_unlink(name);
        
        m_count = count;
    }
    
    SemaphoreImpl::~SemaphoreImpl()
    {
        sem_close(m_semaphore);
    }
    
    unsigned int SemaphoreImpl::GetCount() const
    {
        return m_count;
    }
    
    void SemaphoreImpl::Post()
    {
        #if NAZARA_CORE_SAFE
        if (sem_post(m_semaphore) == -1)
            NazaraError("Failed to release semaphore: " + Error::GetLastSystemError());
        #else
        sem_post(m_semaphore);
        #endif
        
        ++m_count;
    }
    
    void SemaphoreImpl::Wait()
    {
        #if NAZARA_CORE_SAFE
        if (sem_wait(m_semaphore) == -1 )
            NazaraError("Failed to wait for semaphore: " + Error::GetLastSystemError());
        #else
        sem_wait(m_semaphore);
        #endif
        
        --m_count;
    }
    
    bool SemaphoreImpl::Wait(UInt32 timeout)
    {
        timeval tv;
        gettimeofday(&tv, nullptr);
        
        timespec ti;
        ti.tv_nsec = (tv.tv_usec + (timeout % 1000)) * 1000000;
        ti.tv_sec = tv.tv_sec + (timeout / 1000) + (ti.tv_nsec / 1000000000);
        ti.tv_nsec %= 1000000000;
        
        auto isBefore = [](const timeval& lhs, const timespec& rhs) {
            if (lhs.tv_sec == rhs.tv_sec)
                return lhs.tv_usec < rhs.tv_nsec * 1000;
            else
                return lhs.tv_sec < rhs.tv_sec;
        };
        
        bool hasLock = false;
        while (!hasLock && isBefore(tv, ti))
        {
            hasLock = sem_trywait(m_semaphore) == 0;
            gettimeofday(&tv, nullptr);
        }
        
        #if NAZARA_CORE_SAFE
        if (!isBefore(tv, ti))
        {
            NazaraError("Failed to wait for semaphore: " + Error::GetLastSystemError());
            return false;
        }
        #endif
            
        
        if (hasLock)
            --m_count;

        return hasLock;
    }
}
