#include <JuceHeader.h>
#include "CustomTextEditor.h"

CustomTextEditor::CustomTextEditor(const juce::String& placeholderText)
    : placeholderText(placeholderText)
{
    setText(""); 
    setTextToShowWhenEmpty(placeholderText, juce::Colours::grey); 
}

CustomTextEditor::~CustomTextEditor() {}

void CustomTextEditor::paint(juce::Graphics& g)
{
    juce::TextEditor::paint(g);
}

void CustomTextEditor::focusGained(FocusChangeType cause)
{
    if (getText() == placeholderText)
    {
        clear();
    }
    juce::TextEditor::focusGained(cause);
}

void CustomTextEditor::focusLost(FocusChangeType cause)
{
    if (getText().isEmpty())
    {
        setText(placeholderText);
    }
    juce::TextEditor::focusLost(cause);
}

void CustomTextEditor::setPlaceholder(const juce::String& placeholder)
{
    placeholderText = placeholder;
    setTextToShowWhenEmpty(placeholderText, juce::Colours::grey);
    repaint();
}
