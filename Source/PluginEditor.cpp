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
    
    setSize (800, 600);
    
    addAndMakeVisible(fileComp.get());
    addAndMakeVisible(fileComp2.get());
    fileComp->addListener(this);
    fileComp2->addListener(this);
    addAndMakeVisible(selectInfoLabel);
    selectInfoLabel.setText("Manually select if data not loaded", juce::dontSendNotification);
    
    /// Windows
    addAndMakeVisible(currentTimeWindow);
    currentTimeWindow.setButtonText("0 s");
    addAndMakeVisible(currentTimeLabel);
    currentTimeLabel.setText("Time:", juce::dontSendNotification);
    currentTimeLabel.attachToComponent(&currentTimeWindow, false);
    
    addAndMakeVisible(ECGAmpWindow);
    ECGAmpWindow.setButtonText("0 mV");
    addAndMakeVisible(ECGAmpLabel);
    ECGAmpLabel.setText("Amp.:", juce::dontSendNotification);
    ECGAmpLabel.attachToComponent(&ECGAmpWindow, false);
    
    addAndMakeVisible(ECGAmpWindow2);
    ECGAmpWindow2.setButtonText("0 mV");
    
    addAndMakeVisible(lengthWindow);
    lengthWindow.setButtonText("0 s");
    addAndMakeVisible(lengthLabel);
    lengthLabel.setText("Dur.:", juce::dontSendNotification);
    lengthLabel.attachToComponent(&lengthWindow, false);
    
    addAndMakeVisible(lengthWindow2);
    lengthWindow2.setButtonText("0 s");
    
    addAndMakeVisible(readyLabel);
    readyLabel.setText("Not Ready", juce::dontSendNotification);
    addAndMakeVisible(readyLabel2);
    readyLabel2.setText("Not Ready", juce::dontSendNotification);
    
    /// Player Section
    addAndMakeVisible(playPauseBtn);
    playPauseBtn.setButtonText("Play");
    playPauseBtn.setEnabled(false);
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
    fastForwardBtn.setEnabled(false);
    fastForwardBtn.onClick = [this] {
        audioProcessor.ECGcounter = std::min(int(audioProcessor.dataVector.size() - 1), int(audioProcessor.ECGcounter + 1 / audioProcessor.samplingRate));
        currentTime = juce::String(audioProcessor.ECGcounter * audioProcessor.samplingRate);
        currentTimeWindow.setButtonText(currentTime + " s");
    };
    
    addAndMakeVisible(rewindBtn);
    rewindBtn.setButtonText("- 1 sec.");
    rewindBtn.setEnabled(false);
    rewindBtn.onClick = [this] {
        audioProcessor.ECGcounter = std::max(0, int(audioProcessor.ECGcounter - 1 / audioProcessor.samplingRate));
        currentTime = juce::String(audioProcessor.ECGcounter * audioProcessor.samplingRate);
        currentTimeWindow.setButtonText(currentTime + " s");
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
    loPass.setRange(0.5, 70);
    loPass.setValue(20);
    loPass.setTextValueSuffix(" Hz");
    loPass.addListener(this);
    
    addAndMakeVisible(loPassLabel);
    loPassLabel.setText("LO Pass", juce::dontSendNotification);
    loPassLabel.attachToComponent(&loPass, true);
    
    /// HI Pass Slider
    /*
    addAndMakeVisible(hiPass);
    hiPass.setRange(0.0, 0.5);
    hiPass.setValue(0.25);
    hiPass.setTextValueSuffix(" Hz");
    hiPass.addListener(this);
    
    addAndMakeVisible(hiPassLabel);
    hiPassLabel.setText("HI Pass", juce::dontSendNotification);
    hiPassLabel.attachToComponent(&hiPass, true);
     */
    
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
    addAndMakeVisible(dataSelector2);
    
    addAndMakeVisible(dataSetLabel);
    dataSetLabel.setText("1st Dataset", juce::dontSendNotification);
    dataSetLabel.attachToComponent(&dataSelector, false);
    
    addAndMakeVisible(dataSet2Label);
    dataSet2Label.setText("2nd Dataset", juce::dontSendNotification);
    dataSet2Label.attachToComponent(&dataSelector2, false);
    feedDataSelectors();
    
    dataSelector.onChange = [this] {
        selectDataset();
        
        currentTime = juce::String(audioProcessor.ECGcounter * audioProcessor.samplingRate);
        recordingLength = juce::String(int(audioProcessor.dataVector.size() * audioProcessor.samplingRate));
        ECGamplitude = juce::String(audioProcessor.dataVector[audioProcessor.ECGcounter]);
        
        currentTimeWindow.setButtonText(currentTime + " s");
        ECGAmpWindow.setButtonText(ECGamplitude + " mV");
        lengthWindow.setButtonText(recordingLength + " s");
    };
    
    dataSelector2.onChange = [this] {
        selectDataset2();
        
        ECGamplitude2 = juce::String(audioProcessor.dataVector2[audioProcessor.ECGcounter]);
        recordingLength2 = juce::String(int(audioProcessor.dataVector2.size() * audioProcessor.samplingRate));
        
        ECGAmpWindow2.setButtonText(ECGamplitude2 + " mV");
        lengthWindow2.setButtonText(recordingLength2 + " s");
    };
    
    dataSelector.setSelectedId(1);
    dataSelector2.setSelectedId(1);
    
    addAndMakeVisible(changeSound);
    changeSound.setButtonText("Sine");
    changeSound.onClick = [this] {
        audioProcessor.gateSoundChoice = !audioProcessor.gateSoundChoice;
        audioProcessor.setGateSound(audioProcessor.gateSoundChoice);
        
        if (audioProcessor.gateSoundChoice) {
            changeSound.setButtonText("Vocal");
        } else {
            changeSound.setButtonText("Sine");
        }
    };
    
    addAndMakeVisible(changeSoundLabel);
    changeSoundLabel.setText("Click to change sound", juce::dontSendNotification);
    changeSoundLabel.attachToComponent(&changeSound, false);
    
    // readDefaultData();
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
    
    if (audioProcessor.isPlaying) {
        currentTime = juce::String(audioProcessor.ECGcounter * audioProcessor.samplingRate);
        
        recordingLength = juce::String(int(audioProcessor.dataVector.size() * audioProcessor.samplingRate));
        recordingLength2 = juce::String(int(audioProcessor.dataVector2.size() * audioProcessor.samplingRate));
        
        ECGamplitude = juce::String(audioProcessor.dataVector[audioProcessor.ECGcounter]);
        ECGamplitude2 = juce::String(audioProcessor.dataVector2[audioProcessor.ECGcounter]);
        
        currentTimeWindow.setButtonText(currentTime + " s");
        ECGAmpWindow.setButtonText(ECGamplitude + " mV");
        lengthWindow.setButtonText(recordingLength + " s");
        
        ECGAmpWindow2.setButtonText(ECGamplitude2 + " mV");
        lengthWindow2.setButtonText(recordingLength2 + " s");
    }
}

void ICUSonificationAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    dataSelector.setBounds(170, 35, 150, 20);
    dataSelector2.setBounds(170, 85, 150, 20);
    
    ECGAmpWindow.setBounds(330, 30, 100, 30);
    lengthWindow.setBounds(440, 30, 100, 30);
    readyLabel.setBounds(550, 30, 100, 30);
    
    ECGAmpWindow2.setBounds(330, 80, 100, 30);
    lengthWindow2.setBounds(440, 80, 100, 30);
    readyLabel2.setBounds(550, 80, 100, 30);
    
    currentTimeWindow.setBounds(270, 250, 60, 30);
    stateChangeBtn.setBounds(350, 250, 150, 30);
    
    playPauseBtn.setBounds(350, 300, 60, 30);
    fastForwardBtn.setBounds(430, 300, 60, 30);
    rewindBtn.setBounds(270, 300, 60, 30);
    changeSound.setBounds(10, 200, 150, 20);
    
    loPass.setBounds(300, 350, 200, 30);
    hiPass.setBounds(300, 400, 200, 30);
    threshold.setBounds(300, 450, 200, 30);
    
    printDataBtn.setBounds(730, 550, 60, 30);
    
    fileComp->setBounds(10, 550, 180, 20);
    fileComp2->setBounds(10, 575, 180, 20);
    selectInfoLabel.setBounds(10, 525, 200, 20);
}
