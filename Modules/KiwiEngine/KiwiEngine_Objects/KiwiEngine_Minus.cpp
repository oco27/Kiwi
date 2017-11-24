/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Minus.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    OBJECT MINUS                                  //
    // ================================================================================ //
    
    void Minus::declare()
    {
        Factory::add<Minus>("-", &Minus::create);
    }
    
    std::unique_ptr<Object> Minus::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<Minus>(model, patcher);
    }
    
    Minus::Minus(model::Object const& model, Patcher& patcher):
    Operator(model, patcher)
    {
    }
    
    double Minus::compute(double lhs, double rhs) const
    {
        return lhs - rhs;
    }
    
}}
