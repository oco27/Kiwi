/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2019, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Switch.h>

#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    SWITCH                                        //
    // ================================================================================ //
    
    void Switch::declare()
    {
        Factory::add<Switch>("switch", &Switch::create);
    }
    
    std::unique_ptr<Object> Switch::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Switch>(model, patcher);
    }
    
    Switch::Switch(model::Object const& model, Patcher& patcher) :
    Object(model, patcher),
    m_opened_input(),
    m_num_inputs(model.getArguments()[0].getInt())
    {
        std::vector<tool::Atom> const& args = model.getArguments();
        
        if (args.size() > 1)
        {
            openInput(args[1].getInt());
        }
    }
    
    void Switch::openInput(int input)
    {
        m_opened_input = std::max(0, std::min(input, static_cast<int>(m_num_inputs)));
    }
    
    void Switch::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (args.size() > 0)
        {
            if (index == 0)
            {
                if (args[0].isBang())
                {
                    send(0, {static_cast<int>(m_opened_input)});
                }
                else if(args[0].isNumber())
                {
                    openInput(args[0].getInt());
                }
                else
                {
                    warning("switch inlet 1 receives only numbers");
                }
            }
            else if(index == m_opened_input)
            {
                send(0, args);
            }
        }
    }
    
}}
