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

#include <KiwiDsp/KiwiDsp_Signal.h>

#include <KiwiEngine/KiwiEngine_Object.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                    GATE~                                         //
    // ================================================================================ //
    
    class GateTilde : public AudioObject
    {
    public:
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        GateTilde(model::Object const& model, Patcher& patcher);
        
        void receive(size_t index, std::vector<tool::Atom> const& args) override final;
        
        void prepare(PrepareInfo const& infos) override final;
        
        void performValue(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        void performSig(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
    private:
        
        void openOutput(int output);
        
    private:
        
        size_t m_opened_output;
        size_t m_num_outputs;
    };
    
}}
