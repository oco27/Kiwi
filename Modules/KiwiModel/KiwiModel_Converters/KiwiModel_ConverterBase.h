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

#pragma once

#include <flip/BackEndIR.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                   CONVERTER                                      //
    // ================================================================================ //
    
    struct ConverterBase
    {
        ConverterBase(std::string const& from_version,
                      std::string const& to_version)
        : v_from(from_version)
        , v_to(to_version)
        {}
        
        virtual ~ConverterBase() = default;
        
        bool process(flip::BackEndIR& backend)
        {
            if(backend.version == v_from)
            {
                if(operator()(backend))
                {
                    backend.complete_conversion(v_to);
                    return true;
                }
            }
            
            return false;
        }
        
        const std::string v_from;
        const std::string v_to;
        
    protected:
        
        virtual bool operator () (flip::BackEndIR& backend) const = 0;
    };
    
}}
