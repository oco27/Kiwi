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

#include "KiwiTypedObjects.hpp"

#include "KiwiPatcherModel.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        void NewBox::declare()
        {
            if(PatcherModel::has<NewBox>()) return;
            
            PatcherModel::declare<NewBox>()
            .name("cicm.kiwi.NewBox")
            .inherit<model::Object>();
            
            Factory::add<NewBox>("newbox");
        }
        
        NewBox::NewBox(std::string const& name, std::vector<Atom> const& args)
        {
            setNumberOfInlets(1);
            setNumberOfOutlets(0);
        }
        
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        void ErrorBox::declare()
        {
            if(PatcherModel::has<ErrorBox>()) return;
            
            PatcherModel::declare<ErrorBox>()
            .name("cicm.kiwi.ErrorBox")
            .inherit<model::Object>();
            
            Factory::add<ErrorBox>("errorbox");
        }
        
        ErrorBox::ErrorBox(std::string const& name, std::vector<Atom> const& args)
        {
            setNumberOfInlets(0);
            setNumberOfOutlets(0);
        }
        
        void ErrorBox::setNumberOfInlets(size_t inlets)
        {
            model::Object::setNumberOfInlets(inlets);
        }
        
        void ErrorBox::setNumberOfOutlets(size_t outlets)
        {
            model::Object::setNumberOfOutlets(outlets);
        }
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
 
        void ObjectPlus::declare()
        {
            if(PatcherModel::has<ObjectPlus>()) return;
            
            PatcherModel::declare<ObjectPlus>()
            .name("cicm.kiwi.ObjectPlus")
            .inherit<model::Object>();
            
            Factory::add<ObjectPlus>("plus");
        }
        
        ObjectPlus::ObjectPlus(std::string const& name, std::vector<Atom> const& args)
        {
            if(!args.empty() && args[0].isNumber())
            {
                setNumberOfInlets(1);
            }
            else
            {
                setNumberOfInlets(2);
            }
            
            setNumberOfOutlets(1);
        }
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        ObjectPrint::ObjectPrint(std::string const& name, std::vector<Atom> const& args)
        {
            ;
        }
        
        //! @internal flip static declare method
        void ObjectPrint::declare()
        {
            if(PatcherModel::has<ObjectPrint>()) return;
            
            PatcherModel::declare<ObjectPrint>()
            .name("cicm.kiwi.ObjectPrint")
            .inherit<model::Object>();
            
            Factory::add<ObjectPrint>("print");
        }
    }
}
