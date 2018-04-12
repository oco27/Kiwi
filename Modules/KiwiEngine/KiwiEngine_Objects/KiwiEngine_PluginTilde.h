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

namespace juce
{
    class AudioPluginInstance;
}

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       PLUGIN~                                    //
    // ================================================================================ //
    
    class PluginTilde : public engine::AudioObject
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        PluginTilde(model::Object const& model, Patcher& patcher);
        ~PluginTilde();
        
        void receive(size_t index, std::vector<tool::Atom> const& args) final;
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        void prepare(dsp::Processor::PrepareInfo const& infos) final;
    private:
        
        class PluginWrapper;
        void load();
        bool isVST2();
        bool isVST3();
        
        static std::string parsePluginFile(std::vector<tool::Atom> const& args);
        
        std::unique_ptr<juce::AudioPluginInstance> m_plugin;
        std::string const                    m_plugin_file;
    };

}}
