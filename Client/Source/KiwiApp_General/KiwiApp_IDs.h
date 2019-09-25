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

#include <juce_core/juce_core.h>

namespace kiwi
{
    // ================================================================================ //
    //                                        Ids                                       //
    // ================================================================================ //
    
    namespace Ids
    {
        #define DECLARE_ID(name)      const juce::Identifier name(#name)
        
        DECLARE_ID(name);
        DECLARE_ID(host);
        DECLARE_ID(api_port);
        DECLARE_ID(session_port);
        DECLARE_ID(remember_me);
        DECLARE_ID(server_address);
        DECLARE_ID(verify_certificate);
        
        DECLARE_ID(NETWORK_CONFIG);
        
        #undef DECLARE_ID
    };
}
