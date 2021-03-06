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

#include <KiwiTool/KiwiTool_Listeners.h>

#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_ObjectView.h>

namespace kiwi
{
    // ================================================================================ //
    //                               EDITABLE OBJECT VIEW                               //
    // ================================================================================ //
    
    //! @brief Abstract class for object's views that can be edited in mode unlock.
    class EditableObjectView
    : public ObjectView
    , public juce::Label::Listener
    {
    public: // classes
        struct Listener;
        
    public: // methods
        
        //! @brief Constructor.
        //! @details The label is not a child component by default, initialised with not text.
        EditableObjectView(model::Object & object_model);
        
        //! @brief Destructor.
        ~EditableObjectView();
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief Remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Edits the label.
        void edit();
        
        //! @brief Set a new text.
        void setText(juce::String const& new_text);
        
        //! @brief Get the text.
        juce::String getText() const;
        
        //! @brief Set a new font.
        void setFont(juce::Font font);
        
        //! @brief Get the current font used by the box.
        juce::Font getFont() const;
        
        //! @brief Set a padding value.
        void setPadding(int padding);
        
        //! @brief Get the current padding value.
        int getPadding() const;
        
        //! @brief Check Component bounds.
        void checkBounds (juce::Rectangle<int>& bounds,
                          juce::Rectangle<int> const& previousBounds,
                          juce::Rectangle<int> const& limits,
                          bool isStretchingTop,
                          bool isStretchingLeft,
                          bool isStretchingBottom,
                          bool isStretchingRight) override;
        
        //! @brief Get the minimum height.
        //! @details The minimum height is (font_height + 2 * padding)
        int getMinHeight() const;
        
        //! @brief Get the minimum width.
        //! @details The minimum width is (font_height + 2 * padding)
        int getMinWidth() const;
        
    private: // classes
        
        class Label : public juce::Label
        {
        public: // methods
            
            //! @brief Constructor.
            Label(EditableObjectView & object_view);
            
            //! @brief Destructor.
            ~Label();
            
            //! @brief Called to create the text editor once edit is called.
            juce::TextEditor* createEditorComponent() override final;
            
            void paint(juce::Graphics& g) override;
            
        private: // members
            
            EditableObjectView & m_object_view;
        };
        
    protected: // methods
        
        //! @brief Called when the object is resized.
        //! @details Default implementation apply local bounds to the label.
        virtual void resized() override;
        
        //! @brief Returns the label created by the editable object.
        juce::Label & getLabel();
        
        //! @brief Sets the editable object view as editable or not.
        //! @details Editable object is editable by default.
        void setEditable(bool editable);
        
        //! @brief Try to find the bounding bow of a text
        //! @details This is a default implementation,
        //! subclasses can override this method if they need.
        virtual juce::Rectangle<float> getTextBoundingBox(juce::String const& text,
                                                          float max_width) const;
        
    private: // methods
        
        void paint(juce::Graphics& g) override;
        
        void drawLabel(juce::Graphics& g);
        
        //! @brief Creates the text editor used by label when it's edited.
        virtual juce::TextEditor* createdTextEditor() = 0;
        
        //! @brief Called when the label text has changed before calling observers.
        virtual void textChanged() = 0;
        
        //! @brief Called when the label text has changed.
        //! @details Overrides juce::Label::labelTextChanged.
        void labelTextChanged (juce::Label* label) override final;
        
        //! @brief Called when edition ends.
        //! @details Overrides juce::Label::editorHidden.
        void editorHidden (juce::Label* label, juce::TextEditor& text_editor) override final;
        
        //! @brief Called when edition begins.
        //! @details Overrides juce::Label::editorShown.
        void editorShown(juce::Label* label, juce::TextEditor& text_editor) override final;
        
    private: // members
        
        Label                       m_label;
        bool                        m_editable {true};
        tool::Listeners<Listener>   m_listeners;
        int                         m_padding {3};
        
    private: // deleted methods
        
        EditableObjectView() = delete;
        EditableObjectView(EditableObjectView const& other) = delete;
        EditableObjectView(EditableObjectView && other) = delete;
        EditableObjectView& operator=(EditableObjectView const& other) = delete;
        EditableObjectView& operator=(EditableObjectView && other) = delete;
    };
    
    struct EditableObjectView::Listener
    {
        //! @brief Destructor.
        virtual ~Listener() = default;
        
        //! @brief Called when the text has been edited and return key was pressed.
        virtual void textChanged(std::string const& new_text) =  0;
        
        //! @brief Called when the classic view ends its edition.
        virtual void editorHidden() =  0;
        
        //! @brief Called when the classic view enters its edition mode.
        virtual void editorShown() =  0;
    };
}
