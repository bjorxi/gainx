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
  juce::LookAndFeel_V4::setDefaultLookAndFeel(&gainxLAF);
  setSize (200, 300);
  setResizable(true, true);
  
  gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  gainSlider.setRange(0 , 12.0, 0.1);
  gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 48, 24);
  gainSlider.setDoubleClickReturnValue(true, 0.0);
  gainSlider.setTextValueSuffix(" Db");
  gainSlider.addListener(this);
  gainSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(239, 146, 35));
  gainSlider.setColour(juce::Slider::thumbColourId, juce::Colour(239, 146, 35));
  gainSlider.setColour(juce::Slider::textBoxTextColourId , juce::Colour(0, 0, 0));
  gainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(226, 226, 226));

  addAndMakeVisible(gainSlider);
}

GainxAudioProcessorEditor::~GainxAudioProcessorEditor()
{
}

//==============================================================================
void GainxAudioProcessorEditor::paint (juce::Graphics& g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
  g.fillAll(juce::Colour(226, 226, 226));

  g.setColour (juce::Colours::white);
  g.setFont (15.0f);
    // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GainxAudioProcessorEditor::resized()
{
    auto width = getWidth();
    auto height = getHeight();

    gainSlider.setBounds(width * 0.06, height * 0.08,
                         width * 0.5, height * 0.8);
}

void GainxAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    audioProcessor.setGainValue(slider->getValue());
}
