#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    #ifndef NDEBUG
    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    #endif
    };
    
    // ================= PARAMETER CONTROLS ========================================
    // Header parameters
    setSliderComponent(inputKnob, inputKnobAttachment, "INPUT", "Rot");
    setSliderComponent(outputKnob, outputKnobAttachment, "OUTPUT", "Rot");
    setSliderComponent(gateKnob, gateKnobAttachment, "GATE", "Rot");
    setSliderComponent(mixKnob, mixKnobAttachment, "MIX", "Rot");
    
    // PreEQ parameters
    setToggleComponent(preEqToggle, preEqToggleAttachment, "PRE_ON");
    
    setSliderComponent(preHPFKnob, preHPFKnobAttachment, "PRE_HPF", "Rot");
    setSliderComponent(preLPFKnob, preLPFKnobAttachment, "PRE_LPF", "Rot");
    
    setSliderComponent(preLowGainKnob, preLowGainKnobAttachment, "PRE_LOW_GAIN", "Rot");
    setSliderComponent(preLowQKnob, preLowQKnobAttachment, "PRE_LOW_Q", "Rot");
    setSliderComponent(preLowFreqKnob, preLowFreqKnobAttachment, "PRE_LOW_FREQ", "Rot");
    
    setSliderComponent(preMidGainKnob, preMidGainKnobAttachment, "PRE_MID_GAIN", "Rot");
    setSliderComponent(preMidQKnob, preMidQKnobAttachment, "PRE_MID_Q", "Rot");
    setSliderComponent(preMidFreqKnob, preMidFreqKnobAttachment, "PRE_MID_FREQ", "Rot");
    
    setSliderComponent(preHighGainKnob, preHighGainKnobAttachment, "PRE_HIGH_GAIN", "Rot");
    setSliderComponent(preHighQKnob, preHighQKnobAttachment, "PRE_HIGH_Q", "Rot");
    setSliderComponent(preHighFreqKnob, preHighFreqKnobAttachment, "PRE_HIGH_FREQ", "Rot");
    
    // Distortion general parameters
    setSliderComponent(driveGainKnob, driveGainKnobAttachment, "DRIVE", "Rot");
    setSliderComponent(driveBiasKnob, driveBiasKnobAttachment, "BIAS", "Rot");
    setSliderComponent(driveSymKnob, driveSymKnobAttachment, "SYM", "Lin");
    
    // Distortion positive voltage parameters
    setSliderComponent(posCorrKnob, posCorrKnobAttachment, "POS_CORR", "Rot");
    setSliderComponent(posFxKnob, posFxKnobAttachment, "POS_FX", "Rot");
    
    // Distortion negative voltage parameters
    setSliderComponent(negCorrKnob, negCorrKnobAttachment, "NEG_CORR", "Rot");
    setSliderComponent(negFxKnob, negFxKnobAttachment, "NEG_FX", "Rot");
    
    // Listeners
    driveSymKnob.addListener(this);
    posFxKnob.addListener(this);
    negFxKnob.addListener(this);
    
    // =========== DISTORTION FUNCTION BUTTONS LOGIC ===============================

    setButtonComponent(posSoftButton);
    setButtonComponent(posHardButton);
    setButtonComponent(posArcTanButton);
    setButtonComponent(posTanhButton);
    setButtonComponent(posSinFoldButton);
    setButtonComponent(posCustomButton);
    
    setButtonComponent(negSoftButton);
    setButtonComponent(negHardButton);
    setButtonComponent(negArcTanButton);
    setButtonComponent(negTanhButton);
    setButtonComponent(negSinFoldButton);
    setButtonComponent(negCustomButton);
    
    setHiddenSliderComponent(posFuncHiddenSlider, posFuncHiddenSliderAttachment, "POS_FUNC");
    setHiddenSliderComponent(negFuncHiddenSlider, negFuncHiddenSliderAttachment, "NEG_FUNC");
    
    // Compression parameters
    setToggleComponent(compToggle, compToggleAttachment, "COMP_ON");
    
    setSliderComponent(compThresKnob, compThresKnobAttachment, "COMP_THRES", "Rot");
    setSliderComponent(compRatioKnob, compRatioKnobAttachment, "COMP_RATIO", "Rot");
    setSliderComponent(compAttKnob, compAttKnobAttachment, "COMP_ATT", "Rot");
    setSliderComponent(compRelKnob, compRelKnobAttachment, "COMP_REL", "Rot");
    
    // PostEQ parameters
    setToggleComponent(postEqToggle, postEqToggleAttachment, "POST_ON");
    
    setSliderComponent(postHPFKnob, postHPFKnobAttachment, "POST_HPF", "Rot");
    setSliderComponent(postLPFKnob, postLPFKnobAttachment, "POST_LPF", "Rot");
    
    setSliderComponent(postLowGainKnob, postLowGainKnobAttachment, "POST_LOW_GAIN", "Rot");
    setSliderComponent(postLowQKnob, postLowQKnobAttachment, "POST_LOW_Q", "Rot");
    setSliderComponent(postLowFreqKnob, postLowFreqKnobAttachment, "POST_LOW_FREQ", "Rot");
    
    setSliderComponent(postMidGainKnob, postMidGainKnobAttachment, "POST_MID_GAIN", "Rot");
    setSliderComponent(postMidQKnob, postMidQKnobAttachment, "POST_MID_Q", "Rot");
    setSliderComponent(postMidFreqKnob, postMidFreqKnobAttachment, "POST_MID_FREQ", "Rot");
    
    setSliderComponent(postHighGainKnob, postHighGainKnobAttachment, "POST_HIGH_GAIN", "Rot");
    setSliderComponent(postHighQKnob, postHighQKnobAttachment, "POST_HIGH_Q", "Rot");
    setSliderComponent(postHighFreqKnob, postHighFreqKnobAttachment, "POST_HIGH_FREQ", "Rot");
    
    // ==================== IMAGE ASSETS ===========================================
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    lightOnImage = juce::ImageCache::getFromMemory(BinaryData::lightOn_png, BinaryData::lightOn_pngSize);
    knobBlackImage = juce::ImageCache::getFromMemory(BinaryData::knobBlack_png, BinaryData::knobBlack_pngSize);
    knobBlueImage = juce::ImageCache::getFromMemory(BinaryData::knobBlue_png, BinaryData::knobBlue_pngSize);
    knobRedImage = juce::ImageCache::getFromMemory(BinaryData::knobRed_png, BinaryData::knobRed_pngSize);
    knobWhiteImage = juce::ImageCache::getFromMemory(BinaryData::knobWhite_png, BinaryData::knobWhite_pngSize);
    switchImage = juce::ImageCache::getFromMemory(BinaryData::switch_png, BinaryData::switch_pngSize);
    
    // Size should be set in the Wrapper to allow resizing
    setSize (980, 580);
}

PluginEditor::~PluginEditor()
{
    posSoftButton.removeListener(this);
    posHardButton.removeListener(this);
    posArcTanButton.removeListener(this);
    posTanhButton.removeListener(this);
    posSinFoldButton.removeListener(this);
    posCustomButton.removeListener(this);
    //
    negSoftButton.removeListener(this);
    negHardButton.removeListener(this);
    negArcTanButton.removeListener(this);
    negTanhButton.removeListener(this);
    negSinFoldButton.removeListener(this);
    negCustomButton.removeListener(this);
    //
    driveSymKnob.removeListener(this);
    posFxKnob.removeListener(this);
    negFxKnob.removeListener(this);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // Background
    g.drawImageWithin(backgroundImage, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
    
    // ============== On/Off states =======================
    if (preEqToggle.getToggleState())
        g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 44, 104));
    
    if (postEqToggle.getToggleState())
        g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 674, 104));
    
    if (compToggle.getToggleState())
        g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 873, 342));
    
    // ============== Symmetry mode =======================
    if (driveSymKnob.getValue())
        g.drawImageTransformed(switchImage, scaledDown(0.5f, 122, 430));
    else
        g.drawImageTransformed(switchImage, scaledDown(0.5f, 122, 460));
    
    // ============ Drive function lights  ================
    switch ((int) posFuncHiddenSlider.getValue()) {
        case 0:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 366, 364));
            break;
        case 1:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 366, 410));
            break;
        case 2:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 480, 364));
            break;
        case 3:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 480, 410));
            break;
        case 4:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 594, 364));
            break;
        case 5:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 594, 410));
            break;
            
        default:
            break;
    }
    
    switch ((int) negFuncHiddenSlider.getValue()) {
        case 0:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 366, 472));
            break;
        case 1:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 366, 520));
            break;
        case 2:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 480, 472));
            break;
        case 3:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 480, 520));
            break;
        case 4:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 594, 472));
            break;
        case 5:
            g.drawImageTransformed(lightOnImage, scaledDown(0.5f, 594, 520));
            break;
            
        default:
            break;
    }
    
    // ===== Header parameter knobs angle in radians ======
    auto inputRadians = rotationRadians(inputKnob.getValue(), -30.f, 30.f);
    auto gateRadians = rotationRadians(gateKnob.getValue(), -100.f, 0.f);
    auto mixRadians = rotationRadians(mixKnob.getValue(), 0.f, 100.f);
    auto outputRadians = rotationRadians(outputKnob.getValue(), -30.f, 30.f);
    
    g.drawImageTransformed(knobWhiteImage, knobRotation(inputRadians, 0.21f, 402, 12));
    g.drawImageTransformed(knobWhiteImage, knobRotation(gateRadians, 0.21f, 550, 12));
    g.drawImageTransformed(knobWhiteImage, knobRotation(mixRadians, 0.21f, 698, 12));
    g.drawImageTransformed(knobWhiteImage, knobRotation(outputRadians, 0.21f, 846, 12));
    
    // ====== PreEQ parameter knobs angle in radians ======
    auto preHPFRadians = rotationRadians(preHPFKnob.getValue(), 10.f, 200.f);
    auto preLPFRadians = rotationRadians(preLPFKnob.getValue(), 2000.f, 20000.f);
    auto preLowGainRadians = rotationRadians(preLowGainKnob.getValue(), -12.f, 12.f);
    auto preLowQRadians = rotationRadians(preLowQKnob.getValue(), 0.5f, 4.f);
    auto preLowFreqRadians = rotationRadians(preLowFreqKnob.getValue(), 60.f, 200.f);
    auto preMidGainRadians = rotationRadians(preMidGainKnob.getValue(), -12.f, 12.f);
    auto preMidQRadians = rotationRadians(preMidQKnob.getValue(), 0.5f, 4.f);
    auto preMidFreqRadians = rotationRadians(preMidFreqKnob.getValue(), 250.f, 1000.f);
    auto preHighGainRadians = rotationRadians(preHighGainKnob.getValue(), -12.f, 12.f);
    auto preHighQRadians = rotationRadians(preHighQKnob.getValue(), 0.5f, 4.f);
    auto preHighFreqRadians = rotationRadians(preHighFreqKnob.getValue(), 750.f, 3000.f);
    
    g.drawImageTransformed(knobBlueImage, knobRotation(preHPFRadians, 0.48f, 128, 70));
    g.drawImageTransformed(knobBlueImage, knobRotation(preLPFRadians, 0.48f, 240, 70));
    g.drawImageTransformed(knobBlueImage, knobRotation(preLowGainRadians, 0.4f, 17, 177));
    g.drawImageTransformed(knobBlackImage, knobRotation(preLowQRadians, 0.3f, 22, 252));
    g.drawImageTransformed(knobBlackImage, knobRotation(preLowFreqRadians, 0.3f, 74, 228));
    g.drawImageTransformed(knobBlueImage, knobRotation(preMidGainRadians, 0.4f, 131, 177));
    g.drawImageTransformed(knobBlackImage, knobRotation(preMidQRadians, 0.3f, 136, 252));
    g.drawImageTransformed(knobBlackImage, knobRotation(preMidFreqRadians, 0.3f, 188, 228));
    g.drawImageTransformed(knobBlueImage, knobRotation(preHighGainRadians, 0.4f, 245, 177));
    g.drawImageTransformed(knobBlackImage, knobRotation(preHighQRadians, 0.3f, 250, 252));
    g.drawImageTransformed(knobBlackImage, knobRotation(preHighFreqRadians, 0.3f, 302, 228));
    
    // ===== PostEQ parameter knobs angle in radians ======
    auto postHPFRadians = rotationRadians(postHPFKnob.getValue(), 10.f, 200.f);
    auto postLPFRadians = rotationRadians(postLPFKnob.getValue(), 2000.f, 20000.f);
    auto postLowGainRadians = rotationRadians(postLowGainKnob.getValue(), -12.f, 12.f);
    auto postLowQRadians = rotationRadians(postLowQKnob.getValue(), 0.5f, 4.f);
    auto postLowFreqRadians = rotationRadians(postLowFreqKnob.getValue(), 60.f, 200.f);
    auto postMidGainRadians = rotationRadians(postMidGainKnob.getValue(), -12.f, 12.f);
    auto postMidQRadians = rotationRadians(postMidQKnob.getValue(), 0.5f, 4.f);
    auto postMidFreqRadians = rotationRadians(postMidFreqKnob.getValue(), 250.f, 1000.f);
    auto postHighGainRadians = rotationRadians(postHighGainKnob.getValue(), -12.f, 12.f);
    auto postHighQRadians = rotationRadians(postHighQKnob.getValue(), 0.5f, 4.f);
    auto postHighFreqRadians = rotationRadians(postHighFreqKnob.getValue(), 750.f, 3000.f);
    
    g.drawImageTransformed(knobBlueImage, knobRotation(postHPFRadians, 0.48f, 758, 70));
    g.drawImageTransformed(knobBlueImage, knobRotation(postLPFRadians, 0.48f, 870, 70));
    g.drawImageTransformed(knobBlueImage, knobRotation(postLowGainRadians, 0.4f, 647, 177));
    g.drawImageTransformed(knobBlackImage, knobRotation(postLowQRadians, 0.3f, 652, 252));
    g.drawImageTransformed(knobBlackImage, knobRotation(postLowFreqRadians, 0.3f, 704, 228));
    g.drawImageTransformed(knobBlueImage, knobRotation(postMidGainRadians, 0.4f, 761, 177));
    g.drawImageTransformed(knobBlackImage, knobRotation(postMidQRadians, 0.3f, 766, 252));
    g.drawImageTransformed(knobBlackImage, knobRotation(postMidFreqRadians, 0.3f, 818, 228));
    g.drawImageTransformed(knobBlueImage, knobRotation(postHighGainRadians, 0.4f, 875, 177));
    g.drawImageTransformed(knobBlackImage, knobRotation(postHighQRadians, 0.3f, 880, 252));
    g.drawImageTransformed(knobBlackImage, knobRotation(postHighFreqRadians, 0.3f, 932, 228));
    
    // ====== Drive parameter knobs angle in radians ======
    auto driveRadians = rotationRadians(driveGainKnob.getValue(), -18.f, 30.f);
    auto biasRadians = rotationRadians(driveBiasKnob.getValue(), -1.f, 1.f);
    auto posCorrRadians = rotationRadians(posCorrKnob.getValue(), 0.f, 1.f);
    auto negCorrRadians = rotationRadians(negCorrKnob.getValue(), 0.f, 1.f);
    auto posFxRadians = rotationRadians(posFxKnob.getValue(), -1.f, 1.f);
    auto negFxRadians = rotationRadians(negFxKnob.getValue(), -1.f, 1.f);

    g.drawImageTransformed(knobRedImage, knobRotation(driveRadians, 0.51f, 40, 355));
    g.drawImageTransformed(knobRedImage, knobRotation(biasRadians, 0.51f, 40, 473));
    g.drawImageTransformed(knobRedImage, knobRotation(posCorrRadians, 0.45f, 220, 377));
    g.drawImageTransformed(knobRedImage, knobRotation(negCorrRadians, 0.45f, 220, 487));
    g.drawImageTransformed(knobRedImage, knobRotation(posFxRadians, 0.45f, 653, 375));
    g.drawImageTransformed(knobRedImage, knobRotation(negFxRadians, 0.45f, 653, 484));
    
    // ====== Comp parameter knobs angle in radians =======
    auto compRatioRadians = rotationRadians(compRatioKnob.getValue(), 2.f, 40.f);
    auto compThresRadians = rotationRadians(compThresKnob.getValue(), -20.f, 0.f);
    auto compAttRadians = rotationRadians(compAttKnob.getValue(), 0.1f, 50.f);
    auto compRelRadians = rotationRadians(compRelKnob.getValue(), 5.f, 2500.f);
    
    g.drawImageTransformed(knobWhiteImage, knobRotation(compRatioRadians, 0.45f, 788, 390));
    g.drawImageTransformed(knobWhiteImage, knobRotation(compThresRadians, 0.45f, 895, 390));
    g.drawImageTransformed(knobWhiteImage, knobRotation(compAttRadians, 0.45f, 788, 483));
    g.drawImageTransformed(knobWhiteImage, knobRotation(compRelRadians, 0.45f, 893, 483));
    
    // FIXME: Laggy response when changing clipping function
    // Draw f(x) in the graph panel
    functionGrapher(g, driveGainKnob.getValue(), driveBiasKnob.getValue());
    
    //
    // ====================================================================
    // TODO: WHEN MY EDITOR IS READY, I SHOULD DELETE EVERYTHING DOWN BELOW
    // ====================================================================
    //
    #ifndef NDEBUG
    auto area = getLocalBounds();
    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
    auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
    g.drawText (helloWorld, area.removeFromTop (150), juce::Justification::centred, false);
    #endif
}

void PluginEditor::resized()
{
    // ========= Header parameters =========
    inputKnob.setBounds(394, 6, 40, 40);
    gateKnob.setBounds(542, 6, 40, 40);
    mixKnob.setBounds(690, 6, 40, 40);
    outputKnob.setBounds(838, 6, 40, 40);
    
    // ========= PreEQ parameters ==========
    preEqToggle.setBounds(35, 75, 50, 20);
    preHPFKnob.setBounds(120, 60, 75, 75);
    preLPFKnob.setBounds(230, 60, 75, 75);
    preLowGainKnob.setBounds(6, 165, 65, 65);
    preLowQKnob.setBounds(15, 245, 50, 50);
    preLowFreqKnob.setBounds(66, 220, 50, 50);
    preMidGainKnob.setBounds(120, 165, 65, 65);
    preMidQKnob.setBounds(130, 245, 50, 50);
    preMidFreqKnob.setBounds(180, 220, 50, 50);
    preHighGainKnob.setBounds(235, 165, 65, 65);
    preHighQKnob.setBounds(245, 245, 50, 50);
    preHighFreqKnob.setBounds(295, 220, 50, 50);
    
    // ======== PostEQ parameters ==========
    postEqToggle.setBounds(665, 75, 50, 20);
    postHPFKnob.setBounds(750, 60, 75, 75);
    postLPFKnob.setBounds(860, 60, 75, 75);
    postLowGainKnob.setBounds(638, 165, 65, 65);
    postLowQKnob.setBounds(645, 245, 50, 50);
    postLowFreqKnob.setBounds(697, 220, 50, 50);
    postMidGainKnob.setBounds(750, 165, 65, 65);
    postMidQKnob.setBounds(760, 245, 50, 50);
    postMidFreqKnob.setBounds(810, 220, 50, 50);
    postHighGainKnob.setBounds(865, 165, 65, 65);
    postHighQKnob.setBounds(875, 245, 50, 50);
    postHighFreqKnob.setBounds(925, 220, 50, 50);
    
    // ====== Distortion parameters ========
    driveGainKnob.setBounds(30, 345, 80, 80);
    driveBiasKnob.setBounds(30, 465, 80, 80);
    driveSymKnob.setBounds(116, 420, 35, 70);
    
    posCorrKnob.setBounds(210, 365, 75, 75);
    posTanhButton.setBounds(310, 370, 50, 20);
    posArcTanButton.setBounds(310, 415, 50, 20);
    posHardButton.setBounds(425, 370, 50, 20);
    posSoftButton.setBounds(425, 415, 50, 20);
    posSinFoldButton.setBounds(540, 370, 50, 20);
    posCustomButton.setBounds(540, 415, 50, 20);
    posFxKnob.setBounds(640, 365, 75, 75);
    
    negCorrKnob.setBounds(210, 476, 75, 75);
    negTanhButton.setBounds(310, 480, 50, 20);
    negArcTanButton.setBounds(310, 520, 50, 20);
    negHardButton.setBounds(425, 480, 50, 20);
    negSoftButton.setBounds(425, 520, 50, 20);
    negSinFoldButton.setBounds(540, 480, 50, 20);
    negCustomButton.setBounds(540, 520, 50, 20);
    negFxKnob.setBounds(640, 472, 75, 75);
    
    // ====== Compression parameters =======
    compToggle.setBounds(815, 345, 50, 20);
    compRatioKnob.setBounds(780, 380, 75, 75);
    compThresKnob.setBounds(880, 380, 75, 75);
    compAttKnob.setBounds(780, 470, 75, 75);
    compRelKnob.setBounds(880, 470, 75, 75);
    
    //
    // ====================================================================
    // TODO: WHEN MY EDITOR IS READY, I SHOULD DELETE EVERYTHING DOWN BELOW
    // ====================================================================
    //
    // layout the positions of your child components here
    #ifndef NDEBUG
    auto area = getLocalBounds();
    area.removeFromBottom(50);
    inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));
    #endif
}

//=================== PARAMETER MANIPULATION UTILS ==============================
void PluginEditor::setSliderComponent(juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment, juce::String paramName, juce::String style)
{
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.state, paramName, slider);
    
    if (style == "Lin")
        slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    else
        slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(slider);
    slider.setAlpha(0);
}

void PluginEditor::setHiddenSliderComponent(juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment, juce::String paramName)
{
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.state, paramName, slider);
    
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addChildComponent(slider);
}

void PluginEditor::setToggleComponent(juce::ToggleButton& button, std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& buttonAttachment, juce::String paramName)
{
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.state, paramName, button);
    addAndMakeVisible(button);
    button.setAlpha(0);
}

void PluginEditor::setButtonComponent(juce::TextButton& button)
{
    addAndMakeVisible(button);
    button.addListener(this);
    button.setAlpha(0);
}

void PluginEditor::buttonClicked(juce::Button* button)
{
    // Is this a programming war crime ???
    if (button == &posTanhButton) {
        posFuncHiddenSlider.setValue(0);
        if (driveSymKnob.getValue())
            negFuncHiddenSlider.setValue(0);
    } else if (button == &posArcTanButton) {
        posFuncHiddenSlider.setValue(1);
        if (driveSymKnob.getValue())
            negFuncHiddenSlider.setValue(1);
    } else if (button == &posHardButton) {
        posFuncHiddenSlider.setValue(2);
        if (driveSymKnob.getValue())
            negFuncHiddenSlider.setValue(2);
    } else if (button == &posSoftButton) {
        posFuncHiddenSlider.setValue(3);
        if (driveSymKnob.getValue())
            negFuncHiddenSlider.setValue(3);
    } else if (button == &posSinFoldButton) {
        posFuncHiddenSlider.setValue(4);
        if (driveSymKnob.getValue())
            negFuncHiddenSlider.setValue(4);
    } else if (button == &posCustomButton) {
        posFuncHiddenSlider.setValue(5);
        if (driveSymKnob.getValue())
            negFuncHiddenSlider.setValue(5);
    //
    } else if (button == &negTanhButton) {
        negFuncHiddenSlider.setValue(0);
        if (driveSymKnob.getValue())
            posFuncHiddenSlider.setValue(0);
    } else if (button == &negArcTanButton) {
        negFuncHiddenSlider.setValue(1);
        if (driveSymKnob.getValue())
            posFuncHiddenSlider.setValue(1);
    } else if (button == &negHardButton) {
        negFuncHiddenSlider.setValue(2);
        if (driveSymKnob.getValue())
            posFuncHiddenSlider.setValue(2);
    } else if (button == &negSoftButton) {
        negFuncHiddenSlider.setValue(3);
        if (driveSymKnob.getValue())
            posFuncHiddenSlider.setValue(3);
    } else if (button == &negSinFoldButton) {
        negFuncHiddenSlider.setValue(4);
        if (driveSymKnob.getValue())
            posFuncHiddenSlider.setValue(4);
    } else if (button == &negCustomButton) {
        negFuncHiddenSlider.setValue(5);
        if (driveSymKnob.getValue())
            posFuncHiddenSlider.setValue(5);
    }
}

void PluginEditor::sliderValueChanged(juce::Slider *slider)
{
    // Smaller programming war crime ???
    if (slider == &driveSymKnob) {
        negFuncHiddenSlider.setValue( posFuncHiddenSlider.getValue() );
        negFxKnob.setValue( posFxKnob.getValue() );
    } else if (slider == &posFxKnob && driveSymKnob.getValue()) {
        negFxKnob.setValue( posFxKnob.getValue() );
    } else if (slider == &negFxKnob && driveSymKnob.getValue()) {
        posFxKnob.setValue( negFxKnob.getValue() );
    }
}

float PluginEditor::rotationRadians(float actualVal, float minVal, float maxVal)
{
    // Knob mapping function: y = (x-A)/(B-A) * (D-C) + C
    // x = {A, B} = {minVal, maxVal}
    // y = {C, D} = {-150, 150} * PI / 180
    float radians = ((actualVal - minVal) / (maxVal - minVal) * 300.0f - 150.0f) * DEG2RADS;
    return radians;
}

juce::AffineTransform PluginEditor::knobRotation(float radians, float scaleFactor, float posX, float posY)
{
    juce::AffineTransform t;
    t = t.rotated(radians, 57.f, 57.f);
    t = t.scaled(scaleFactor);
    t = t.translated(posX, posY);
    return t;
}

juce::AffineTransform PluginEditor::scaledDown(float scaleFactor, float posX, float posY)
{
    juce::AffineTransform t;
    t = t.scaled(scaleFactor);
    t = t.translated(posX, posY);
    return t;
}

void PluginEditor::functionGrapher(juce::Graphics& g, float driveVal, float biasVal)
{
    float gainLinear = juce::Decibels::decibelsToGain(driveVal);
    
    // Draw f(x)
    g.setColour(juce::Colours::coral);
    juce::Path fxPath;
    for (int x = 350; x < 630; ++x) {
        float xVal = juce::jmap((float) x, 350.f, 630.f, -1.f, 1.f);
        float yVal = processorRef.shapeBlender(xVal * gainLinear);
        int y = (int) juce::jmap(yVal, -1.f, 1.f, 330.f, 50.f);
        
        juce::Point<float> p (x, y);
        x == 350 ? fxPath.startNewSubPath(p) : fxPath.lineTo(p);
    }
    g.strokePath(fxPath, juce::PathStrokeType(2.0f));
    
    // Draw bias line
    int xBias = (int) juce::jmap((float) biasVal, -1.f, 1.f, 350.f, 630.f);
    float dashLengths[] = {10.f, 5.f};
    g.setColour(juce::Colours::peachpuff);
    g.drawDashedLine(juce::Line<float>(xBias, 50, xBias, 330), dashLengths, 2, 1.5f);
}
