#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class GainMonoAudioProcessorEditor : public juce::AudioProcessorEditor,
    private juce::Timer
{
public:
    GainMonoAudioProcessorEditor(GainMonoAudioProcessor&);
    ~GainMonoAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override; // For smooth UI updates if needed

private:
    GainMonoAudioProcessor& audioProcessor;

    // UI Components
    juce::Slider gainSlider;
    juce::ToggleButton monoToggle;
    juce::ToggleButton bypassToggle;

    // Attachments link the UI to the APVTS
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> gainAttachment;
    std::unique_ptr<ButtonAttachment> monoAttachment;
    std::unique_ptr<ButtonAttachment> bypassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainMonoAudioProcessorEditor)
};