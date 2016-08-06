// Copyright (C) 2015 Alexandre Janniaux
// This file is part of the "Nazara Engine - Core module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_SEMAPHOREFREEBSDIMPL_HPP
#define NAZARA_SEMAPHOREFREEBSDIMPL_HPP

#include <Nazara/Prerequesites.hpp>
#include <semaphore.h>
#include <atomic>

namespace Nz
{
    class SemaphoreImpl
    {
        public:
            SemaphoreImpl(unsigned int count);
            ~SemaphoreImpl();
        
            unsigned int GetCount() const;
            void Post();
            void Wait();
            bool Wait(UInt32 timeout);
        
        private:
            sem_t* m_semaphore;
            std::atomic<unsigned int> m_count;
    };
}

#endif // NAZARA_SEMAPHOREFREEBSDIMPL_HPP
