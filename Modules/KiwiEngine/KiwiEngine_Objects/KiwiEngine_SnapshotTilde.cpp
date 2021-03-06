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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_SnapshotTilde.h>
#include <KiwiEngine/KiwiEngine_Factory.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                   SNAPSHOT~                                      //
    // ================================================================================ //
    
    void SnapshotTilde::declare()
    {
        Factory::add<SnapshotTilde>("snapshot~", &SnapshotTilde::create);
    }
    
    std::unique_ptr<Object> SnapshotTilde::create(model::Object const& model, Patcher & patcher)
    {
        return std::make_unique<SnapshotTilde>(model, patcher);
    }
    
    SnapshotTilde::SnapshotTilde(model::Object const& model, Patcher& patcher)
    : AudioObject(model, patcher)
    {
        ;
    }
    
    void SnapshotTilde::receive(size_t index, std::vector<tool::Atom> const& args)
    {
        if (index == 0 && !args.empty())
        {
            if(args[0].isBang())
            {
                send(0, {m_value.load()});
            }
            else
            {
                warning("snapshot~ inlet 1 doesn't understand args");
            }
        }
    }
    
    void SnapshotTilde::perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept
    {
        size_t sample_index = input[0].size();
        dsp::sample_t const* in = input[0].data();
        
        while(sample_index--)
        {
            m_value.store(*in++);
        }
    }
    
    void SnapshotTilde::prepare(dsp::Processor::PrepareInfo const& infos)
    {
        if(infos.inputs[0])
        {
            setPerformCallBack(this, &SnapshotTilde::perform);
        }
    }
    
}}
