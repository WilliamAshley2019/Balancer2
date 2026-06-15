#include "PluginEditor.h"

GainMonoAudioProcessorEditor::GainMonoAudioProcessorEditor(GainMonoAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Gain Slider Setup
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(gainSlider);
    gainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, ParamIDs::gainDb, gainSlider);

    // Mono Toggle Setup
    monoToggle.setButtonText("Mono Sum");
    addAndMakeVisible(monoToggle);
    monoAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, ParamIDs::monoSum, monoToggle);

    // Bypass Toggle Setup
    bypassToggle.setButtonText("Bypass");
    bypassToggle.setColour(juce::ToggleButton::tickColourId, juce::Colours::red);
    addAndMakeVisible(bypassToggle);
    bypassAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, ParamIDs::bypass, bypassToggle);

    // Set a reasonable default size
    setSize(300, 200);

    // Start timer for any UI polling (optional, but good practice for bypass state visual feedback)
    startTimerHz(30);
}

GainMonoAudioProcessorEditor::~GainMonoAudioProcessorEditor()
{
}

void GainMonoAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Professional dark theme background
    g.fillAll(juce::Colour(0xff2b2b2b));

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(16.0f, juce::Font::bold));
    g.drawFittedText("Gain / Mono Utility", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
}

void GainMonoAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    area.removeFromTop(40); // Leave room for title

    // Layout: Bypass (top right), Mono (top left), Gain (center)
    auto topRow = area.removeFromTop(30);
    bypassToggle.setBounds(topRow.removeFromRight(80));
    monoToggle.setBounds(topRow.removeFromLeft(100));

    gainSlider.setBounds(area.removeFromTop(100).withWidth(80).withX(area.getCentreX() - 40));
}

void GainMonoAudioProcessorEditor::timerCallback()
{
    // Optional: Force repaint if you want the bypass button to visually react 
    // to host automation changing the bypass state.
    repaint();
}   