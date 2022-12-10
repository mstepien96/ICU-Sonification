/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

//==============================================================================
ICUSonificationAudioProcessorEditor::ICUSonificationAudioProcessorEditor (ICUSonificationAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    // File reader initialization
    fileComp.reset(new juce::FilenameComponent("fileComp",
                                                 {},
                                                 false,
                                                 false,
                                                 false,
                                                 {},
                                                 {},
                                                 "Select file to open"));
    
    fileComp2.reset(new juce::FilenameComponent("fileComp2",
                                                {},
                                                false,
                                                false,
                                                false,
                                                {},
                                                {},
                                                "Select file to open"));
    
    addAndMakeVisible(fileComp.get());
    addAndMakeVisible(fileComp2.get());
    fileComp->addListener(this);
    fileComp2->addListener(this);
    
    addAndMakeVisible(dataSetLabel);
    dataSetLabel.setText("1st Dataset", juce::dontSendNotification);
    dataSetLabel.attachToComponent(&dataSelector, false);
    
    addAndMakeVisible(dataSet2Label);
    dataSet2Label.setText("2nd Dataset", juce::dontSendNotification);
    dataSet2Label.attachToComponent(&dataSelector2, false);
    
    // Text Field initialization
    textContent.reset(new juce::TextEditor());
    addAndMakeVisible(textContent.get());
    textContent->setMultiLine(true);
    textContent->setReadOnly(true);
    textContent->setCaretVisible(false);
    setSize (800, 600);
    
    /// Windows
    addAndMakeVisible(currentTimeWindow);
    currentTimeWindow.setEnabled(false);
    currentTimeWindow.setButtonText("0 s");
    addAndMakeVisible(currentTimeLabel);
    currentTimeLabel.setText("Time:", juce::dontSendNotification);
    currentTimeLabel.attachToComponent(&currentTimeWindow, false);
    
    addAndMakeVisible(ECGAmpWindow);
    ECGAmpWindow.setEnabled(false);
    ECGAmpWindow.setButtonText("0 mV");
    addAndMakeVisible(ECGAmpLabel);
    ECGAmpLabel.setText("Amp.:", juce::dontSendNotification);
    ECGAmpLabel.attachToComponent(&ECGAmpWindow, false);
    
    addAndMakeVisible(lengthWindow);
    lengthWindow.setEnabled(false);
    lengthWindow.setButtonText("0 s");
    addAndMakeVisible(lengthLabel);
    lengthLabel.setText("Dur.:", juce::dontSendNotification);
    lengthLabel.attachToComponent(&lengthWindow, false);
    
    /// Player Section
    addAndMakeVisible(playPauseBtn);
    playPauseBtn.setButtonText("Play");
    playPauseBtn.onClick = [this] {
        audioProcessor.isPlaying = !audioProcessor.isPlaying;
        audioProcessor.setGate(audioProcessor.isPlaying);
        if (!audioProcessor.isPlaying) {
            playPauseBtn.setButtonText("Play");
        } else {
            playPauseBtn.setButtonText("Pause");
        }
        
    };
    
    addAndMakeVisible(fastForwardBtn);
    fastForwardBtn.setButtonText("+ 1 sec.");
    fastForwardBtn.onClick = [this] {
        audioProcessor.ECGcounter = std::min(int(audioProcessor.dataVector.size() - 1), int(audioProcessor.ECGcounter + 1 / audioProcessor.samplingRate));
    };
    
    addAndMakeVisible(rewindBtn);
    rewindBtn.setButtonText("- 1 sec.");
    rewindBtn.onClick = [this] {
        audioProcessor.ECGcounter = std::max(0, int(audioProcessor.ECGcounter - 1 / audioProcessor.samplingRate));
    };
    
    /// Button changing between the state of the healhy and unhealthy dataset
    addAndMakeVisible(stateChangeBtn);
    stateChangeBtn.setButtonText("Normal data");
    stateChangeBtn.onClick = [this] {
        audioProcessor.streamPicker = !audioProcessor.streamPicker;
        if (audioProcessor.streamPicker) {
            stateChangeBtn.setButtonText("ST-elevation");
        } else {
            stateChangeBtn.setButtonText("Normal data");
        }
    };
    
    addAndMakeVisible(stateLabel);
    stateLabel.setText("Current data, click to change:", juce::dontSendNotification);
    stateLabel.attachToComponent(&stateChangeBtn, false);
    
    /// Button printing the filtered data sets to the directory of the program
    addAndMakeVisible(printDataBtn);
    printDataBtn.setButtonText("Print Data");
    printDataBtn.onClick = [this] {
        printData();
    };
    
    addAndMakeVisible(printLabel);
    printLabel.setText("Click to print data in directory", juce::dontSendNotification);
    printLabel.attachToComponent(&printDataBtn, true);
    
    /// LO Pass Slider
    addAndMakeVisible(loPass);
    loPass.setRange(0.5, 15);
    loPass.setValue(2);
    loPass.setTextValueSuffix(" Hz");
    loPass.addListener(this);
    
    addAndMakeVisible(loPassLabel);
    loPassLabel.setText("LO Pass", juce::dontSendNotification);
    loPassLabel.attachToComponent(&loPass, true);
    
    /// HI Pass Slider
    addAndMakeVisible(hiPass);
    hiPass.setRange(0.0, 0.5);
    hiPass.setValue(0.25);
    hiPass.setTextValueSuffix(" Hz");
    hiPass.addListener(this);
    
    addAndMakeVisible(hiPassLabel);
    hiPassLabel.setText("HI Pass", juce::dontSendNotification);
    hiPassLabel.attachToComponent(&hiPass, true);
    
    /// Threshold slider
    addAndMakeVisible(threshold);
    threshold.setRange(-1, 1);
    threshold.setValue(0.5);
    threshold.setTextValueSuffix(" mV");
    threshold.addListener(this);
    
    addAndMakeVisible(thresholdLabel);
    thresholdLabel.setText("Threshold", juce::dontSendNotification);
    thresholdLabel.attachToComponent(&threshold, true);
    
    addAndMakeVisible(dataSelector);
    feedDataSelectors();
    dataSelector.onChange = [this] {
        selectDataset();
        
        if (audioProcessor.dataRead) {
            textContent->clear();
            currentTime = juce::String(audioProcessor.ECGcounter * audioProcessor.samplingRate);
            textContent->insertTextAtCaret("Current Time: " + currentTime + " s" + juce::newLine);
            recordingLength = juce::String(int(audioProcessor.dataVector.size() * audioProcessor.samplingRate));
            textContent->insertTextAtCaret("Recording Length: " + recordingLength + " s" + juce::newLine);
            ECGamplitude = juce::String(audioProcessor.dataVector[audioProcessor.ECGcounter]);
            textContent->insertTextAtCaret("ECG amplitude: " + ECGamplitude + " mV");
            
            currentTimeWindow.setButtonText(currentTime + " s");
            ECGAmpWindow.setButtonText(ECGamplitude + " mV");
            lengthWindow.setButtonText(recordingLength + " s");
        }
        
        
    };
    
    addAndMakeVisible(dataSelector2);
    dataSelector2.onChange = [this] {
        selectDataset2();
    };
    
    readDefaultData();    
}

ICUSonificationAudioProcessorEditor::~ICUSonificationAudioProcessorEditor()
{
}

//==============================================================================
void ICUSonificationAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    // g.drawText("Some Text", getLocalBounds(), juce::Justification::centred, false);
}

void ICUSonificationAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    fileComp->setBounds(30, 520, 300, 20);
    fileComp2->setBounds(30, 560, 300, 20);
    textContent->setBounds(450, 10, 300, 70);
    
    currentTimeWindow.setBounds(350, 160, 60, 30);
    ECGAmpWindow.setBounds(270, 160, 60, 30);
    lengthWindow.setBounds(430, 160, 60, 30);
    
    stateChangeBtn.setBounds(310, 250, 150, 30);
    
    playPauseBtn.setBounds(350, 300, 60, 30);
    fastForwardBtn.setBounds(430, 300, 60, 30);
    rewindBtn.setBounds(270, 300, 60, 30);
    
    loPass.setBounds(300, 350, 200, 30);
    hiPass.setBounds(300, 400, 200, 30);
    threshold.setBounds(300, 450, 200, 30);
    
    printDataBtn.setBounds(730, 550, 60, 30);
    
    dataSelector.setBounds(30, 30, 150, 20);
    dataSelector2.setBounds(30, 80, 150, 20);
}
