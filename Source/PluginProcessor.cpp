/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainxAudioProcessor::GainxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

GainxAudioProcessor::~GainxAudioProcessor()
{
}

//==============================================================================
const juce::String GainxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GainxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainxAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GainxAudioProcessor::getProgramName (int index)
{
    return {};
}

void GainxAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GainxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void GainxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GainxAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sampleIdx= 0; sampleIdx< buffer.getNumSamples(); ++sampleIdx) {
          auto sample = buffer.getSample(channel, sampleIdx);
          auto sampleDb = juce::Decibels::gainToDecibels(sample) + gainValue;
          
          channelData[sampleIdx] = juce::Decibels::decibelsToGain(sampleDb);
        }
    }
}

//==============================================================================
bool GainxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GainxAudioProcessor::createEditor()
{
    return new GainxAudioProcessorEditor (*this);
}

//==============================================================================
void GainxAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GainxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//void GainxAudioProcessor::prepareListeners() {
//  ;
//
//}

//juce::AudioProcessorValueTreeState::ParameterLayout GainxAudioProcessor::createParameterLayout() {
//  auto gainParam = std::make_unique<juce::AudioParameterFloat>(
//      "gain",
//      "Gain",
//      juce::NormalisableRange<float>(0.0, 12.0,0.1),
//      1.0,
//      juce::String(),
//      juce::AudioProcessorParameter::genericParameter,
//      [](float value, float)
//      {return juce::String(value, 1) + " dB ";}
//    );
//
//  paramsLayout.add(gainParam);
//
//  return paramsLayout;
//}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainxAudioProcessor();
}

void GainxAudioProcessor::setGainValue(float newValue) {
    this->gainValue = newValue;
    
}
