/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainxAudioProcessorEditor::GainxAudioProcessorEditor (GainxAudioProcessor& p)
    : juce::AudioProcessorEditor (&p),
    audioProcessor (p)
{
    setSize (200, 300);
    setResizable(true, true);

    gainSlider.setSliderStyle(juce::Slider::LinearVertical);
    gainSlider.setRange(-12.0, 12.0, 0.1);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 48, 24);
    gainSlider.setDoubleClickReturnValue(true, 0.0);
    gainSlider.setTextValueSuffix(" Db");
    gainSlider.addListener(this);

    addAndMakeVisible(gainSlider);
}

GainxAudioProcessorEditor::~GainxAudioProcessorEditor()
{
}

//==============================================================================
void GainxAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GainxAudioProcessorEditor::resized()
{
    auto width = getWidth();
    auto height = getHeight();

    gainSlider.setBounds(width * 0.06, height * 0.08,
                         width * 0.24, height * 0.8);
}

void GainxAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    audioProcessor.setGainValue(slider->getValue());
}
