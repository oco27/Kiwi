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

#pragma once

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi
{
    namespace engine
    {
        
        // ================================================================================ //
        //                                       PLUS~                                      //
        // ================================================================================ //
        
        class PlusTilde : public AudioObject
        {
        public: // methods
            
            PlusTilde(model::Object const& model, Patcher& patcher, std::vector<Atom> const& args);
            
            void receive(size_t index, std::vector<Atom> const& args) override;
            
            void performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void performVec(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
            
            void prepare(dsp::Processor::PrepareInfo const& infos) override final;
            
        private: // members
            
            std::atomic<dsp::sample_t>   m_value{0.f};
        };
    }
}
