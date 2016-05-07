/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#include "KiwiObject.hpp"
#include "KiwiPatcher.hpp"
#include "KiwiInstance.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        Object::Object() noexcept : m_stack_count(0)
        {
            
        }
        
        Object::~Object() noexcept
        {
            m_outlets.clear();
        }
        
        void Object::addOutputLink(Link* link)
        {
            const size_t idx = link->getSenderIndex();
            m_outlets[idx].insert(link);
        }
        
        void Object::removeOutputLink(Link* link)
        {
            const size_t idx = link->getSenderIndex();
            m_outlets[idx].erase(link);
        }
        
        void Object::send(const uint32_t index, std::vector<Atom> args)
        {
            const auto idx = static_cast<std::vector<Outlet>::size_type>(index);
            
            if(idx < m_outlets.size())
            {
                for(auto* link : m_outlets[idx])
                {
                    Object& receiver = link->getReceiverObject();
                    
                    if(++receiver.m_stack_count < 256)
                    {
                        receiver.receive(link->getReceiverIndex(), args);
                    }
                    else
                    {
                        // commented because of an xcode f*c*i*g indentation bug
                        std::cout << "object " << getName() << " => Stack overflow !" << '\n';
                    }
                    
                    receiver.m_stack_count--;
                }
            
            }
        }
        
        void Object::modelChanged(model::Object& object_m)
        {
            if(object_m.added())
            {
                m_model = &object_m;
                
                m_outlets.resize(m_model->getNumberOfOutlets());
                
                // connect signals
                m_signal_cnx = m_model->signalTrigger.connect(*this, &Object::internal_signalTriggerCalled);
            }
            
            if(object_m.removed())
            {
                m_model = nullptr;
            }
        }
        
        void Object::internal_signalTriggerCalled()
        {
            signalTriggerCalled();
        }
    }
}
