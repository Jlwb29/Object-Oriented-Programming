#pragma once
#include <JuceHeader.h>

class CustomTextEditor : public juce::TextEditor {
public:
    CustomTextEditor() : CustomTextEditor("") {} 

    CustomTextEditor(const juce::String& placeholderText);
    ~CustomTextEditor() override;

    void setPlaceholder(const juce::String& placeholder);

private:
    void paint(juce::Graphics& g) override;
    void focusGained(FocusChangeType cause) override;
    void focusLost(FocusChangeType cause) override;

    juce::String placeholderText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomTextEditor)
};

