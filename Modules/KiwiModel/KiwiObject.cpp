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

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        Object::Object(flip::Default&) : signalTrigger(Trigger, *this)
        {
            ;
        }
        
        Object::Object(std::string const& name, const uint32_t inlets, const uint32_t outlets) :
        signalTrigger(Trigger, *this),
        m_name(name),
        m_inlets(inlets),
        m_outlets(outlets),
        m_position_x(0.),
        m_position_y(0.)
        {
            ;
        }
        
        Object::Object(model::Object const& other) :
        signalTrigger(Trigger, *this),
        m_name(other.m_name),
        m_inlets(other.m_inlets),
        m_outlets(other.m_outlets),
        m_position_x(other.m_position_x),
        m_position_y(other.m_position_y)
        {
            
        }
        
        Object::~Object()
        {
            ;
        }
    }
}