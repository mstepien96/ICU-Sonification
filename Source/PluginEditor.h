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
        
        // Clearing after loading
        textContent->clear();
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
                textContent->insertTextAtCaret("Reading finished");
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
        
        textContent->clear();
        
        juce::int64 counter = 0;
        
        while(!inputStream.isExhausted()) {
            auto line = inputStream.readNextLine();
            juce::String trimmed = line.trim();
            
            float floatValue = trimmed.getFloatValue();
            
            audioProcessor.dataVector.push_back(floatValue);
        
            counter++;
            
            if (counter > 100000) {
                audioProcessor.dataRead = true;
                textContent->insertTextAtCaret("Reading finished");
                break;
            }
        }
        
        audioProcessor.dataRead = true;
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
        
        textContent->clear();
        
        juce::int64 counter = 0;
        
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
        
        audioProcessor.dataReadTwo = true;
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
        } else if (slider == &hiPass) {
            //audioProcessor.calculateHPFButterWorthCoeffs(hiPass.getValue());
            audioProcessor.calculateHPFBIQCoeff(hiPass.getValue(), 0.7);
        } else if (slider == &threshold) {
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
            float HI_LOPassData1 = audioProcessor.HIfilterData(LOPassData1);
            
            juce::String outputString1 = juce::String(HI_LOPassData1);
            
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
            float HI_LOPassData2 = audioProcessor.HIfilterData(LOPassData2);
            
            juce::String outputString2 = juce::String(HI_LOPassData2);
            
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
    
    void feedDataSelector() {
        dataSelector.addItem("Lol", 1);
        dataSelector.addItem("Second", 2);
        
        dataSelector.setSelectedId (1);
    }
    
    void selectDataset() {
        auto resources = juce::File::getSpecialLocation (juce::File::currentApplicationFile)
                .getChildFile ("Contents")
                .getChildFile ("Resources");
        
        auto resources2 = juce::File::getSpecialLocation(juce::File::hostApplicationPath);
        
        DBG(resources.getFullPathName());
        DBG(resources2.getFullPathName());
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ICUSonificationAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ICUSonificationAudioProcessorEditor)
    
    /// File reader
    std::unique_ptr<juce::FilenameComponent> fileComp;
    std::unique_ptr<juce::FilenameComponent> fileComp2;

    /// Text field
    std::unique_ptr<juce::TextEditor> textContent;

    /// Timer
    std::unique_ptr<juce::HighResolutionTimer> timer;
    
    /// Visualization
    TextButton currentTimeWindow;
    Label currentTimeLabel;
    TextButton ECGAmpWindow;
    Label ECGAmpLabel;
    TextButton lengthWindow;
    Label lengthLabel;

    /// Player Buttons
    TextButton playPauseBtn;
    TextButton fastForwardBtn;
    TextButton rewindBtn;

    /// Timesteps
    String currentTime;
    String recordingLength;
    String ECGamplitude;

    /// Buttons
    TextButton stateChangeBtn;
    TextButton printDataBtn;
    
    /// Button Labels
    Label stateLabel;
    Label printLabel;
    
    /// Sliders
    Slider loPass;
    Slider hiPass;
    Slider threshold;
    
    /// Labels for Sliders
    Label loPassLabel;
    Label hiPassLabel;
    Label thresholdLabel;
    
    Label dataSetLabel;
    Label dataSet2Label;
    
    ComboBox dataSelector;
};
