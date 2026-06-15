#include "PluginProcessor.h"
#include "PluginEditor.h"

GainMonoAudioProcessor::GainMonoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
#endif
    apvts(*this, nullptr, "PARAMS", createLayout())
{
    smoothedGain.reset(44100.0, 0.02); // 20ms smoothing, will be updated in prepareToPlay
}

juce::AudioProcessorValueTreeState::ParameterLayout GainMonoAudioProcessor::createLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::gainDb, "Gain (dB)",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.01f, 1.0f),
        0.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        ParamIDs::monoSum, "Mono Sum", false));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        ParamIDs::bypass, "Bypass", false));

    return { params.begin(), params.end() };
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainMonoAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& mainIn = layouts.getMainInputChannelSet();
    const auto& mainOut = layouts.getMainOutputChannelSet();

    if (mainIn.isDisabled() || mainOut.isDisabled())
        return false;

    // Support mono and stereo I/O
    if (!(mainIn == juce::AudioChannelSet::mono() || mainIn == juce::AudioChannelSet::stereo()))
        return false;
    if (!(mainOut == juce::AudioChannelSet::mono() || mainOut == juce::AudioChannelSet::stereo()))
        return false;

    return true;
}
#endif

void GainMonoAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    smoothedGain.reset(sampleRate, 0.02f);
    const float dB = apvts.getRawParameterValue(ParamIDs::gainDb)->load();
    smoothedGain.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(dB));
}

void GainMonoAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ignoreUnused(midi);

    const int numCh = buffer.getNumChannels();
    const int numSamp = buffer.getNumSamples();

    const bool isBypassed = apvts.getRawParameterValue(ParamIDs::bypass)->load() > 0.5f;
    const bool doMono = apvts.getRawParameterValue(ParamIDs::monoSum)->load() > 0.5f;
    const float targetLin = juce::Decibels::decibelsToGain(apvts.getRawParameterValue(ParamIDs::gainDb)->load());

    // PROFESSIONAL OPTIMIZATION: Early exit if no processing is needed
    if (isBypassed || (!doMono && targetLin == 1.0f))
    {
        smoothedGain.setCurrentAndTargetValue(1.0f); // Keep smoothed value in sync
        return;
    }

    smoothedGain.setTargetValue(targetLin);

    if (doMono && numCh > 1)
    {
        // 1. Sum all channels into channel 0 safely using JUCE's optimized methods
        buffer.clear(0, 0, numSamp);
        for (int c = 0; c < numCh; ++c)
        {
            buffer.addFrom(0, 0, buffer, c, 0, numSamp);
        }

        // 2. Apply mono scaling (0.5f for stereo to prevent clipping, like a passive summer) 
        //    and smoothed gain per-sample to avoid zipper noise.
        const float monoScale = (numCh == 2) ? 0.5f : (1.0f / (float)numCh);
        auto* ch0 = buffer.getWritePointer(0);

        for (int n = 0; n < numSamp; ++n)
        {
            ch0[n] = ch0[n] * monoScale * smoothedGain.getNextValue();
        }

        // 3. Duplicate the processed mono signal to all other output channels
        for (int c = 1; c < numCh; ++c)
        {
            buffer.copyFrom(c, 0, buffer, 0, 0, numSamp);
        }
    }
    else
    {
        // Standard stereo/mono passthrough with smoothed gain
        for (int c = 0; c < numCh; ++c)
        {
            auto* data = buffer.getWritePointer(c);
            for (int n = 0; n < numSamp; ++n)
            {
                data[n] *= smoothedGain.getNextValue();
            }
        }
    }
}

void GainMonoAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void GainMonoAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, (size_t)sizeInBytes);
    if (tree.isValid())
        apvts.replaceState(tree);
}

juce::AudioProcessorEditor* GainMonoAudioProcessor::createEditor()
{
    return new GainMonoAudioProcessorEditor(*this);
}