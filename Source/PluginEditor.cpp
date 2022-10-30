/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ICUSonificationAudioProcessorEditor::ICUSonificationAudioProcessorEditor(ICUSonificationAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(800, 100);

    addAndMakeVisible(delaySlider);
    delaySlider.setRange(0.0, 1.0);
    delaySlider.setValue(0.5);
    delaySlider.onValueChange = [this] {
        audioProcessor.setDelay(delaySlider.getValue());
    };

    addAndMakeVisible(delayLabel);
    delayLabel.setText("Delay (s)", juce::dontSendNotification);
    delayLabel.attachToComponent(&delaySlider, true);

    addAndMakeVisible(feedbackSlider);
    feedbackSlider.setRange(0.0, 1.0);
    feedbackSlider.setValue(0.5);
    feedbackSlider.onValueChange = [this] {
        audioProcessor.setFeedback(feedbackSlider.getValue());
    };

    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent(&feedbackSlider, true);
}

ICUSonificationAudioProcessorEditor::~ICUSonificationAudioProcessorEditor()
{
}

//==============================================================================
void ICUSonificationAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ICUSonificationAudioProcessorEditor::resized()
{
    const int sliderLeft = 80;
    delaySlider.setBounds(sliderLeft, 10, getWidth() - sliderLeft - 20, 20);
    feedbackSlider.setBounds(sliderLeft, 40, getWidth() - sliderLeft - 20, 20);
}