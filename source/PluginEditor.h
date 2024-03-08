#pragma once

#include "PluginProcessor.h"
#include "BinaryData.h"
// DELETE WHEN PLUGIN FINISHED
#include "melatonin_inspector/melatonin_inspector.h"

#define DEG2RADS 0.0174533f

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //=================== PARAMETER MANIPULATION UTILS =============================
    void setSliderComponent(juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment, juce::String paramName, juce::String style);
    
    void setToggleComponent(juce::ToggleButton& button, std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& buttonAttachment, juce::String paramName);
    
    void setButtonComponent(juce::TextButton& button);
    
    void buttonClicked(juce::Button* button) override;
    
    float rotationRadians (float actualVal, float minVal, float maxVal);
    
    juce::AffineTransform knobRotation(float radians, float scaleFactor, float posX, float posY);
    
    juce::AffineTransform scaledDown(float scaleFactor, float posX, float posY);

private:
    // ================= PARAMETER CONTROLS ========================================
    // Header parameters
    juce::Slider inputKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputKnobAttachment;
    
    juce::Slider gateKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gateKnobAttachment;
    
    juce::Slider mixKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixKnobAttachment;
    
    juce::Slider outputKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputKnobAttachment;
    
    // PreEQ parameters
    juce::ToggleButton preEqToggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> preEqToggleAttachment;
    
    juce::Slider preHPFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preHPFKnobAttachment;
    
    juce::Slider preLPFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preLPFKnobAttachment;
    
    juce::Slider preLowGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preLowGainKnobAttachment;
    
    juce::Slider preLowQKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preLowQKnobAttachment;
    
    juce::Slider preLowFreqKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preLowFreqKnobAttachment;
    
    juce::Slider preMidGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preMidGainKnobAttachment;
    
    juce::Slider preMidQKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preMidQKnobAttachment;
    
    juce::Slider preMidFreqKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preMidFreqKnobAttachment;
    
    juce::Slider preHighGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preHighGainKnobAttachment;
    
    juce::Slider preHighQKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preHighQKnobAttachment;
    
    juce::Slider preHighFreqKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preHighFreqKnobAttachment;
    
    // Distortion generic parameters
    juce::Slider driveGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveGainKnobAttachment;
    
    juce::Slider driveBiasKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveBiasKnobAttachment;
    
    juce::Slider driveSymKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveSymKnobAttachment;
    
    // Distortion positive voltage parameters
    juce::Slider posCorrKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> posCorrKnobAttachment;
    
    juce::Slider posFxKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> posFxKnobAttachment;
    
    juce::TextButton posSoftButton;
    juce::TextButton posHardButton;
    juce::TextButton posArcTanButton;
    juce::TextButton posTanhButton;
    juce::TextButton posSinFoldButton;
    juce::TextButton posCustomButton;
    
    /*
    juce::ToggleButton posSoftButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> posSoftButtonAttachment;
    
    juce::ToggleButton posHardButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> posHardButtonAttachment;
    
    juce::ToggleButton posArcTanButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> posArcTanButtonAttachment;
    
    juce::ToggleButton posTanhButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> posTanhButtonAttachment;
    
    juce::ToggleButton posSinFoldButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> posSinFoldButtonAttachment;
    
    juce::ToggleButton posCustomButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> posCustomButtonAttachment;
     */
    
    // Distortion negative voltage parameters
    juce::Slider negCorrKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> negCorrKnobAttachment;
    
    juce::Slider negFxKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> negFxKnobAttachment;
    
    juce::TextButton negSoftButton;
    juce::TextButton negHardButton;
    juce::TextButton negArcTanButton;
    juce::TextButton negTanhButton;
    juce::TextButton negSinFoldButton;
    juce::TextButton negCustomButton;
    
    /*
    juce::ToggleButton negSoftButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> negSoftButtonAttachment;
    
    juce::ToggleButton negHardButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> negHardButtonAttachment;
    
    juce::ToggleButton negArcTanButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> negArcTanButtonAttachment;
    
    juce::ToggleButton negTanhButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> negTanhButtonAttachment;
    
    juce::ToggleButton negSinFoldButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> negSinFoldButtonAttachment;
    
    juce::ToggleButton negCustomButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> negCustomButtonAttachment;
     */
    
    // Compression parameters
    juce::ToggleButton compToggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compToggleAttachment;
    
    juce::Slider compThresKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compThresKnobAttachment;
    
    juce::Slider compRatioKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compRatioKnobAttachment;
    
    juce::Slider compAttKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compAttKnobAttachment;
    
    juce::Slider compRelKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compRelKnobAttachment;
    
    // PostEQ parameters
    juce::ToggleButton postEqToggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> postEqToggleAttachment;
    
    juce::Slider postHPFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postHPFKnobAttachment;
    
    juce::Slider postLPFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postLPFKnobAttachment;
    
    juce::Slider postLowGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postLowGainKnobAttachment;
    
    juce::Slider postLowQKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postLowQKnobAttachment;
    
    juce::Slider postLowFreqKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postLowFreqKnobAttachment;
    
    juce::Slider postMidGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postMidGainKnobAttachment;
    
    juce::Slider postMidQKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postMidQKnobAttachment;
    
    juce::Slider postMidFreqKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postMidFreqKnobAttachment;
    
    juce::Slider postHighGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postHighGainKnobAttachment;
    
    juce::Slider postHighQKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postHighQKnobAttachment;
    
    juce::Slider postHighFreqKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> postHighFreqKnobAttachment;
    
    // ======================= IMAGE ASSETS ========================================
    juce::Image backgroundImage;
    juce::Image lightOnImage;
    juce::Image knobBlackImage;
    juce::Image knobBlueImage;
    juce::Image knobRedImage;
    juce::Image knobWhiteImage;
    juce::Image switchImage;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    
    // DELETE WHEN PLUGIN FINISHED
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
