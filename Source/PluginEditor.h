/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iostream>
#include "PluginProcessor.h"
using namespace juce;

//==============================================================================
// public juce::HighResolutionTimer
class ICUSonificationAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::FilenameComponentListener, public juce::Slider::Listener
{
public:
    ICUSonificationAudioProcessorEditor (ICUSonificationAudioProcessor&);
    ~ICUSonificationAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void readFile (const juce::File& fileToRead) {
        if (!fileToRead.existsAsFile()) {
            return;
        }
        
        auto fileText = fileToRead.loadFileAsString();
        
        juce::FileInputStream inputStream(fileToRead);
        
        if (!inputStream.openedOk()) {
            return;
        }
        
        // Counter used in ensuring we don't load the 12 million rows
        juce::int64 counter = 0;
        // Counter for array indexing
        juce::int64 valueIndex = 0;
        
        while(!inputStream.isExhausted()) {
            auto line = inputStream.readNextLine();
            juce::String trimmed = line.trim();
            juce::StringArray tokens;
            
            // Splitting String into array of subStrings
            tokens.addTokens(trimmed, "\t");
            
            // Removing annotations from the input
            if (counter > 1) {
                // Increments through every entry in our StringArray
                for (int i = 0; i < tokens.size() - 1; i++) {
                    // Char to String
                    juce::String token = tokens[i].toStdString();
                    
                    if (token != "") {
                        // String to Float
                        float floatValue = token.getFloatValue();
                        
                        // If value index is 1 we will insert the mV value in our dynamic vector array
                        if (valueIndex == 1) {
                            audioProcessor.dataVector.push_back(floatValue);
                        }
                        valueIndex++;
                    }
                }
                valueIndex = 0;
            }
            
            // Breaker
            counter++;
            if (counter > 10000) {  // was 252 & 2502
                audioProcessor.dataRead = true;
                
                //audioProcessor.dataMax1 = *std::max_element(audioProcessor.dataVector.begin(), audioProcessor.dataVector.end());
                //audioProcessor.dataMin1 = *std::min_element(audioProcessor.dataVector.begin(), audioProcessor.dataVector.end());
                break;
            }
        }
        
        audioProcessor.dataRead = true;
    }
    
    void readFileST(const juce::File& fileToRead) {
        if (!fileToRead.existsAsFile()) {
            return;
        }
        
        auto fileText = fileToRead.loadFileAsString();
        
        juce::FileInputStream inputStream(fileToRead);
        
        if (!inputStream.openedOk()) {
            return;
        }
        
        juce::int64 counter = 0;
        
        if (!audioProcessor.dataVector.empty()) {
            audioProcessor.dataVector.clear();
        }
        
        while(!inputStream.isExhausted()) {
            auto line = inputStream.readNextLine();
            juce::String trimmed = line.trim();
            
            float floatValue = trimmed.getFloatValue();
            
            audioProcessor.dataVector.push_back(floatValue);
        
            counter++;
            
            if (counter > 100000) {
                audioProcessor.dataRead = true;
                break;
            }
        }
        
        if (!audioProcessor.dataVector.empty() && !audioProcessor.dataVector2.empty()) {
            playPauseBtn.setEnabled(true);
            fastForwardBtn.setEnabled(true);
            rewindBtn.setEnabled(true);
            
            audioProcessor.dataRead = true;
        }
        
        readyLabel.setText("Ready", juce::dontSendNotification);
    }
    
    void readFileST2(const juce::File& fileToRead) {
        if (!fileToRead.existsAsFile()) {
            return;
        }
        
        auto fileText = fileToRead.loadFileAsString();
        
        juce::FileInputStream inputStream(fileToRead);
        
        if (!inputStream.openedOk()) {
            return;
        }
        
        juce::int64 counter = 0;
        
        if (!audioProcessor.dataVector2.empty()) {
            audioProcessor.dataVector2.clear();
        }
        
        while(!inputStream.isExhausted()) {
            auto line = inputStream.readNextLine();
            juce::String trimmed = line.trim();
            
            float floatValue = trimmed.getFloatValue();
            
            audioProcessor.dataVector2.push_back(floatValue);
            
            counter++;
            
            if (counter > 100000) {
                break;
            }
        }
        
        if (!audioProcessor.dataVector.empty() && !audioProcessor.dataVector2.empty()) {
            playPauseBtn.setEnabled(true);
            fastForwardBtn.setEnabled(true);
            rewindBtn.setEnabled(true);
            
            audioProcessor.dataRead = true;
        }
        
        readyLabel2.setText("Ready", juce::dontSendNotification);
    }
    
    
    // Changing path for file and update the filename
    void filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged) override {
        if (fileComponentThatHasChanged == fileComp.get()) {
            readFileST(fileComp->getCurrentFileText());
        }
        
        if (fileComponentThatHasChanged == fileComp2.get()) {
            readFileST2(fileComp2->getCurrentFileText());
        }
    }
    
    void sliderValueChanged(Slider* slider) override {
        if (slider == &loPass) {
            //audioProcessor.calculateBIQCoeff(freqCutOff.getValue() , 0.7);
            audioProcessor.calculateLPFButterWorthCoeffs(loPass.getValue());
        } /*else if (slider == &hiPass) {
            //audioProcessor.calculateHPFButterWorthCoeffs(hiPass.getValue());
            audioProcessor.calculateHPFBIQCoeff(hiPass.getValue(), 0.7);
        }*/ else if (slider == &threshold) {
            audioProcessor.thresholdValue = threshold.getValue();
        }
    }
    
    void readDefaultData() {
        auto bundle = juce::File::getSpecialLocation (juce::File::currentExecutableFile).getParentDirectory().getParentDirectory().getParentDirectory()
            .getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
        
        String OSSeperator = juce::File::getSeparatorString();
        
        juce::File normalData = bundle.getChildFile("Source" + OSSeperator + "normalData.txt");
        juce::File STElevation = bundle.getChildFile("Source" + OSSeperator + "ST-elevatedData.txt");

        readFileST(normalData);
        readFileST2(STElevation);
    }
    
    void printData() {
        audioProcessor.resetCoeffs();
        // Calculating coefficients
        audioProcessor.calculateLPFButterWorthCoeffs(loPass.getValue());
        audioProcessor.calculateHPFBIQCoeff(hiPass.getValue(), 0.707);
        
        //audioProcessor.calculateLPFBIQCoeff(loPass.getValue(), 0.707);
        //audioProcessor.calculateHPButterWorthCoeffs(hiPass.getValue());
        
        // Finding the root directory
        juce::File myfile = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
        
        // Take Operating System seperator into account
        String OSSeperator = juce::File::getSeparatorString();
        
        // Retrieving directory path
        juce::File log1 = myfile.getChildFile("Source" + OSSeperator + "normalData_Output.txt");
        juce::File log2 = myfile.getChildFile("Source" + OSSeperator + "ST-ElevatedData_Output.txt");
        
        // Creating necessary .txt files
        log1.create();
        log2.create();
        
        // Opening temporary file to offload data
        TemporaryFile tempFile(log1);
        TemporaryFile tempFile2(log2);
        
        FileOutputStream output1(tempFile.getFile());
        FileOutputStream output2(tempFile2.getFile());
        
        if(!output1.openedOk() || !output2.openedOk()) {
            DBG("Failed to create or locate the data.txt");
            return;
        }
        
        // Putting every entry through a Low Pass Filter
        for (int i = 0; i < audioProcessor.dataVector.size() - 1; i++) {
            
            float LOPassData1 = audioProcessor.LOfilterData(audioProcessor.dataVector[i]);
            // float HI_LOPassData1 = audioProcessor.HIfilterData(LOPassData1);
            
            juce::String outputString1 = juce::String(LOPassData1);
            
            output1.writeString(outputString1 + "\n");
            output1.flush();
            
            if (output1.getStatus().failed()) {
                return;
            }
            
        }
        
        tempFile.overwriteTargetFileWithTemporary();
        
        // Resetting
        audioProcessor.resetCoeffs();
        
        audioProcessor.calculateLPFButterWorthCoeffs(loPass.getValue());
        audioProcessor.calculateHPFBIQCoeff(hiPass.getValue(), 0.707);
        
        // Putting every entry through a High Pass Filter
        for (int i = 0; i < audioProcessor.dataVector2.size() - 1; i++) {
            float LOPassData2 = audioProcessor.LOfilterData(audioProcessor.dataVector2[i]);
            // float HI_LOPassData2 = audioProcessor.HIfilterData(LOPassData2);
            
            juce::String outputString2 = juce::String(LOPassData2);
            
            output2.writeString(outputString2 + "\n");
            output2.flush();
            
            if (output2.getStatus().failed()) {
                return;
            }
        }
        
        tempFile2.overwriteTargetFileWithTemporary();
        
        audioProcessor.resetCoeffs();
        
        audioProcessor.calculateLPFButterWorthCoeffs(loPass.getValue());
        audioProcessor.calculateHPFBIQCoeff(hiPass.getValue(), 0.707);
    }
    
    void feedDataSelectors() {
        dataSelector.addItem("normalData.txt", 1);
        dataSelector.addItem("MOESM1_noise.txt", 2);
        dataSelector.addItem("MOESM2_noise.txt", 3);
        dataSelector.addItem("MOESM3_noise.txt", 4);
        dataSelector.addItem("MOESM4_noise.txt", 5);
        dataSelector.addItem("MOESM5_noise.txt", 6);
        dataSelector.addItem("MOESM6_noise.txt", 7);
        dataSelector.addItem("MOESM7_noise.txt", 8);
        dataSelector.addItem("MOESM8_noise.txt", 9);
        dataSelector.addItem("MOESM9_noise.txt", 10);
        dataSelector.addItem("MOESM10_noise.txt", 11);
        dataSelector.addItem("MOESM11_noise.txt", 12);
        dataSelector.addItem("MOESM12_noise.txt", 13);
        dataSelector.addItem("MOESM13_noise.txt", 14);
        dataSelector.addItem("MOESM14_noise.txt", 15);
        dataSelector.addItem("MOESM15_noise.txt", 16);
        dataSelector.addItem("MOESM16_noise.txt", 17);
        dataSelector.addItem("MOESM17_noise.txt", 18);
        dataSelector.addItem("MOESM18_noise.txt", 19);
        dataSelector.addItem("MOESM19_noise.txt", 20);
        dataSelector.addItem("MOESM20_noise.txt", 21);
        
        dataSelector2.addItem("ST-elevatedData.txt", 1);
        dataSelector2.addItem("MOESM1_noise.txt", 2);
        dataSelector2.addItem("MOESM2_noise.txt", 3);
        dataSelector2.addItem("MOESM3_noise.txt", 4);
        dataSelector2.addItem("MOESM4_noise.txt", 5);
        dataSelector2.addItem("MOESM5_noise.txt", 6);
        dataSelector2.addItem("MOESM6_noise.txt", 7);
        dataSelector2.addItem("MOESM7_noise.txt", 8);
        dataSelector2.addItem("MOESM8_noise.txt", 9);
        dataSelector2.addItem("MOESM9_noise.txt", 10);
        dataSelector2.addItem("MOESM10_noise.txt", 11);
        dataSelector2.addItem("MOESM11_noise.txt", 12);
        dataSelector2.addItem("MOESM12_noise.txt", 13);
        dataSelector2.addItem("MOESM13_noise.txt", 14);
        dataSelector2.addItem("MOESM14_noise.txt", 15);
        dataSelector2.addItem("MOESM15_noise.txt", 16);
        dataSelector2.addItem("MOESM16_noise.txt", 17);
        dataSelector2.addItem("MOESM17_noise.txt", 18);
        dataSelector2.addItem("MOESM18_noise.txt", 19);
        dataSelector2.addItem("MOESM19_noise.txt", 20);
        dataSelector2.addItem("MOESM20_noise.txt", 21);
    }
    
    void selectDataset() {
        auto resources = juce::File::getSpecialLocation (juce::File::currentApplicationFile).getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
        
        auto dataPath = resources.getChildFile("Data");
        
        int selectedIndex = dataSelector.getSelectedId();
        juce::String selectedPath = dataSelector.getItemText(selectedIndex - 1);
        
        juce::File selectedDataset = dataPath.getChildFile(selectedPath);
        
        readFileST(selectedDataset);
    }
    
    void selectDataset2() {
        auto resources = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory();
        
        auto dataPath = resources.getChildFile("Data");
        
        int selectedIndex = dataSelector2.getSelectedId();
        juce::String selectedPath = dataSelector2.getItemText(selectedIndex - 1);
        
        juce::File selectedDataset = dataPath.getChildFile(selectedPath);
        
        readFileST2(selectedDataset);
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ICUSonificationAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ICUSonificationAudioProcessorEditor)
    
    /// File reader
    std::unique_ptr<juce::FilenameComponent> fileComp;
    std::unique_ptr<juce::FilenameComponent> fileComp2;

    /// Timer
    std::unique_ptr<juce::HighResolutionTimer> timer;
    
    /// Dataset Selectors
    ComboBox dataSelector;
    ComboBox dataSelector2;
    Label dataSetLabel;
    Label dataSet2Label;
    
    /// Visualization
    TextButton currentTimeWindow;
    Label currentTimeLabel;
    TextButton ECGAmpWindow;
    Label ECGAmpLabel;
    TextButton lengthWindow;
    Label lengthLabel;
    Label readyLabel;
    Label readyLabel2;
    
    TextButton ECGAmpWindow2;
    TextButton lengthWindow2;

    /// Player Buttons
    TextButton playPauseBtn;
    TextButton fastForwardBtn;
    TextButton rewindBtn;

    /// Timesteps
    String currentTime;
    String recordingLength;
    String recordingLength2;
    String ECGamplitude;
    String ECGamplitude2;

    /// Buttons
    TextButton stateChangeBtn;
    TextButton printDataBtn;
    TextButton changeSound;
    
    /// Button Labels
    Label stateLabel;
    Label printLabel;
    Label changeSoundLabel;
    
    /// Sliders
    Slider loPass;
    Slider hiPass;
    Slider threshold;
    
    /// Labels for Sliders
    Label loPassLabel;
    Label hiPassLabel;
    Label thresholdLabel;
    
    Label selectInfoLabel;
};
