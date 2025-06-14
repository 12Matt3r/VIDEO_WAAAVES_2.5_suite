#include "GuiApp.h"
#include <iostream>

// Define constants used in this file
const float LFO_MAX_RATE_GUI = 20.0f;
const int FB_DELAY_MAX_CONST_GUI = 30; // Assuming fbob from ofApp.cpp was meant to be this

// Helper to generate unique ImGui IDs
std::string gen_ui_id(const std::string& base, int idx = -1, const std::string& suffix = "") {
    std::string id = base;
    if (idx != -1) id += "_" + std::to_string(idx);
    id += "##" + base + suffix;
    if (idx != -1) id += "_" + std::to_string(idx) + suffix;
    return id;
}

void GuiApp::setup(){
    ofBackground(0);
    imgui_instance.setup();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Default member initializers in GuiApp.h handle most setup.
    // Specific initial setup for availableWindows if needed:
    if (availableWindowTitles.empty()) {
        availableWindowTitles.push_back("No windows listed yet");
    }
    selectedWindowIndex = 0; // Default to first item or -1 if truly empty
}

void GuiApp::update() {}

void GuiApp::drawChannelControls(int ch_idx, float lfom) {
    std::string title = "Channel " + std::to_string(ch_idx + 1);
    if (ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
        ChannelSettings& cs = channel_settings[ch_idx];

        if (ImGui::CollapsingHeader(gen_ui_id("InputSource", ch_idx).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            const char* items[] = { "Cam 1", "Cam 2", "NDI", "Video File", "Window Capture", "Noise Generator" };
            // cs.select is 1-indexed (1=Cam1, 2=Cam2, ..., 6=Noise Generator)
            // current_selection for ImGui::Combo is 0-indexed
            int current_selection = (cs.select > 0 && cs.select <= IM_ARRAYSIZE(items)) ? cs.select - 1 : 0;
            if (ImGui::Combo(gen_ui_id("SourceCombo", ch_idx).c_str(), &current_selection, items, IM_ARRAYSIZE(items))) {
                cs.select = current_selection + 1; // Update to 1-indexed
            }
             if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select input source for this channel.");
        }
        ImGui::Separator();
        if (ch_idx == 1) {
             if (ImGui::CollapsingHeader(gen_ui_id("Mixing", ch_idx).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderFloat(gen_ui_id("MixVal", ch_idx).c_str(), &cs.mix_value, -2.0f, 2.0f);
                 if (ImGui::IsItemHovered()) ImGui::SetTooltip("Channel mix amount.");
                ImGui::SliderFloat(gen_ui_id("KeyVal", ch_idx).c_str(), &cs.key_value, .0f, 1.0f);
                 if (ImGui::IsItemHovered()) ImGui::SetTooltip("Channel keying value.");
            }
            ImGui::Separator();
        }
        if (ImGui::CollapsingHeader(gen_ui_id("ColorAdjust", ch_idx).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SliderFloat(gen_ui_id("Hue", ch_idx).c_str(), &cs.hue, -5.0f, 5.0f);
            ImGui::SliderFloat(gen_ui_id("Saturation", ch_idx).c_str(), &cs.saturation, -5.0f, 5.0f);
            ImGui::SliderFloat(gen_ui_id("Brightness", ch_idx).c_str(), &cs.bright, -5.0f, 5.0f);
            ImGui::Spacing();
            ImGui::Checkbox(gen_ui_id("HueInv", ch_idx).c_str(), &cs.hue_alt_invert_toggle); ImGui::SameLine();
            ImGui::Checkbox(gen_ui_id("SatInv", ch_idx).c_str(), &cs.saturation_alt_invert_toggle); ImGui::SameLine();
            ImGui::Checkbox(gen_ui_id("BrightInv", ch_idx).c_str(), &cs.bright_alt_invert_toggle);
            ImGui::Spacing();
            ImGui::Checkbox(gen_ui_id("SatWrap", ch_idx).c_str(), &cs.saturation_wrap); ImGui::SameLine();
            ImGui::Checkbox(gen_ui_id("BrightWrap", ch_idx).c_str(), &cs.bright_wrap);
            ImGui::Spacing();
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader(gen_ui_id("PowMap", ch_idx).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SliderFloat(gen_ui_id("HuePm", ch_idx).c_str(), &cs.hue_powmap, -5.0f, 5.0f);
             if (ImGui::IsItemHovered()) ImGui::SetTooltip("Apply power curve to hue (val^powmap).");
            ImGui::SliderFloat(gen_ui_id("SatPm", ch_idx).c_str(), &cs.saturation_powmap, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Apply power curve to saturation (val^powmap).");
            ImGui::SliderFloat(gen_ui_id("BrightPm", ch_idx).c_str(), &cs.bright_powmap, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Apply power curve to brightness (val^powmap).");
            ImGui::Spacing();
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader(gen_ui_id("LFOs", ch_idx).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Hue LFO"); ImGui::Indent();
            ImGui::SliderFloat(gen_ui_id("HueLFORate", ch_idx).c_str(), &cs.hue_lfo.theta,  .0f, LFO_MAX_RATE_GUI);
             if (ImGui::IsItemHovered()) ImGui::SetTooltip("LFO rate for Hue modulation.");
            ImGui::SliderFloat(gen_ui_id("HueLFOAmp", ch_idx).c_str(), &cs.hue_lfo.amp, -5.0f, 5.0f);
             if (ImGui::IsItemHovered()) ImGui::SetTooltip("LFO amplitude for Hue modulation.");
            ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Saturation LFO"); ImGui::Indent();
            ImGui::SliderFloat(gen_ui_id("SatLFORate", ch_idx).c_str(), &cs.saturation_lfo.theta, .0f, LFO_MAX_RATE_GUI);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("LFO rate for Saturation modulation.");
            ImGui::SliderFloat(gen_ui_id("SatLFOAmp", ch_idx).c_str(), &cs.saturation_lfo.amp, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("LFO amplitude for Saturation modulation.");
            ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Brightness LFO"); ImGui::Indent();
            ImGui::SliderFloat(gen_ui_id("BrightLFORate", ch_idx).c_str(), &cs.bright_lfo.theta, .0f, LFO_MAX_RATE_GUI);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("LFO rate for Brightness modulation.");
            ImGui::SliderFloat(gen_ui_id("BrightLFOAmp", ch_idx).c_str(), &cs.bright_lfo.amp, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("LFO amplitude for Brightness modulation.");
            ImGui::Unindent(); ImGui::Spacing();
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void GuiApp::drawFeedbackControls(int fb_idx, float lfom) {
    std::string title = "Feedback " + std::to_string(fb_idx);
    if (ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);
        FeedbackSettings& fbs = feedback_settings[fb_idx];

        if (ImGui::CollapsingHeader(gen_ui_id("Parameters", fb_idx, "FBParams").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            ImGui::Checkbox(gen_ui_id("TexModActive", fb_idx, "FBTxMd").c_str(), &fbs.tex_mod_enabled);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable texture modification for this feedback channel.");

            ImGui::SameLine();
            const char* items_src[] = { "Channel 1","Channel 2" };
            // Sync ImGui's static current item with the actual struct state
            static int item_current_fb_texmod_src[MAX_FB_CHANNELS_GUI]; // MAX_FB_CHANNELS_GUI should be defined
            if (fbs.texmod_select >= 1 && fbs.texmod_select <= IM_ARRAYSIZE(items_src)) {
                item_current_fb_texmod_src[fb_idx] = fbs.texmod_select -1;
            } else {
                 item_current_fb_texmod_src[fb_idx] = 0; // Default if out of bounds
            }

            if (ImGui::Combo(gen_ui_id("TexModSrc", fb_idx, "FBTxSrc").c_str(), &item_current_fb_texmod_src[fb_idx], items_src, IM_ARRAYSIZE(items_src))) {
                fbs.texmod_select = item_current_fb_texmod_src[fb_idx] + 1; // Update struct state from ImGui
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select source for texture modification.");
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(gen_ui_id("MixDelay", fb_idx, "FBMix").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                if(!fbs.tex_mod_enabled){
                    ImGui::SliderFloat(gen_ui_id("Mix", fb_idx, "FBMixVal").c_str(), &fbs.mix, -2.0f, 2.0f);
                    ImGui::SliderFloat(gen_ui_id("KeyVal", fb_idx, "FBKeyVal").c_str(), &fbs.key_value, .0f, 1.0f);
                    ImGui::SliderFloat(gen_ui_id("KeyThresh", fb_idx, "FBKeyThr").c_str(), &fbs.key_threshold, .0f, 1.0f);
                } else {
                    ImGui::SliderFloat(gen_ui_id("TexFbMix", fb_idx, "FBTxMix").c_str(), &fbs.tex_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat(gen_ui_id("TexFbKeyVal", fb_idx, "FBTxKeyV").c_str(), &fbs.tex_key_value, .0f, 1.0f);
                    ImGui::SliderFloat(gen_ui_id("TexFbKeyThresh", fb_idx, "FBTxKeyT").c_str(), &fbs.tex_key_threshold, .0f, 1.0f);
                }
                ImGui::SliderInt(gen_ui_id("DelayAmt", fb_idx, "FBDelAmt").c_str(), &fbs.delay_amount, 0, FB_DELAY_MAX_CONST_GUI - 1);
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(gen_ui_id("Switches", fb_idx, "FBSwitch").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                const char* items_overflow[] = { "Clamp","Toroid","Mirror" };
                static int item_current_fb_overflow[MAX_FB_CHANNELS_GUI];
                if (fbs.toroid_switch >= 0 && fbs.toroid_switch < IM_ARRAYSIZE(items_overflow)) {
                    item_current_fb_overflow[fb_idx] = fbs.toroid_switch;
                } else {
                    item_current_fb_overflow[fb_idx] = 0; // Default
                }
                if(ImGui::Combo(gen_ui_id("Overflow", fb_idx, "FBOver").c_str(), &item_current_fb_overflow[fb_idx], items_overflow, IM_ARRAYSIZE(items_overflow))) {
                    fbs.toroid_switch = item_current_fb_overflow[fb_idx]; // Update struct state
                }
                ImGui::Checkbox(gen_ui_id("HFlip", fb_idx, "FBHFlip").c_str(), &fbs.hflip_switch); ImGui::SameLine();
                ImGui::Checkbox(gen_ui_id("VFlip", fb_idx, "FBVFlip").c_str(), &fbs.vflip_switch);
                ImGui::Spacing();
                ImGui::Checkbox(gen_ui_id("HueInv", fb_idx, "FBHueInv").c_str(), &fbs.hue_invert); ImGui::SameLine();
                ImGui::Checkbox(gen_ui_id("SatInv", fb_idx, "FBSatInv").c_str(), &fbs.saturation_invert); ImGui::SameLine();
                ImGui::Checkbox(gen_ui_id("BrightInv", fb_idx, "FBBrtInv").c_str(), &fbs.bright_invert);
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(gen_ui_id("Ranges", fb_idx, "FBRanges").c_str())) {
                ImGui::SliderFloat(gen_ui_id("XDispRange", fb_idx, "FBXDR").c_str(), &fbs.x_displace_range, .0001f, 100.0f);
                ImGui::SliderFloat(gen_ui_id("YDispRange", fb_idx, "FBYDR").c_str(), &fbs.y_displace_range, .0001f, 100.0f);
                ImGui::SliderFloat(gen_ui_id("ZDispRange", fb_idx, "FBZDR").c_str(), &fbs.z_displace_range, .0001f, 100.0f);
                ImGui::SliderFloat(gen_ui_id("RotRange", fb_idx, "FBRotR").c_str(), &fbs.rotate_range, .0001f, TWO_PI);
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(gen_ui_id("ColorSpace", fb_idx, "FBColor").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                  if(!fbs.tex_mod_enabled){
                      ImGui::SliderFloat(gen_ui_id("Hue", fb_idx, "FBHue").c_str(), &fbs.hue, 8.0f, 12.0f);
                      ImGui::SliderFloat(gen_ui_id("Saturation", fb_idx, "FBSat").c_str(), &fbs.saturation, 8.0f, 12.0f);
                      ImGui::SliderFloat(gen_ui_id("Brightness", fb_idx, "FBBright").c_str(), &fbs.bright, 8.0f, 12.0f);
                      ImGui::SliderFloat(gen_ui_id("HueXMod", fb_idx, "FBHXM").c_str(), &fbs.huex_mod, 0.0f, 10.0f);
                      ImGui::SliderFloat(gen_ui_id("HueXOffset", fb_idx, "FBHXO").c_str(), &fbs.huex_offset, -20.0f, 20.0f);
                      ImGui::SliderFloat(gen_ui_id("HueXLFOVal", fb_idx, "FBHXLFO").c_str(), &fbs.huex_lfo_val, -20.0f, 20.0f);
                  } else {
                     ImGui::SliderFloat(gen_ui_id("TexHue", fb_idx, "FBTxHue").c_str(), &fbs.tex_hue, -.25f, .25f);
                     ImGui::SliderFloat(gen_ui_id("TexSat", fb_idx, "FBTxSat").c_str(), &fbs.tex_saturation, -.25f, .25f);
                     ImGui::SliderFloat(gen_ui_id("TexBright", fb_idx, "FBTxBright").c_str(), &fbs.tex_bright, -.25f, .25f);
                     ImGui::SliderFloat(gen_ui_id("TexHueXMod", fb_idx, "FBTxHXM").c_str(), &fbs.tex_huex_mod, -1.0f, 0.0f);
                     ImGui::SliderFloat(gen_ui_id("TexHueXOffset", fb_idx, "FBTxHXO").c_str(), &fbs.tex_huex_offset, -2.0f, 2.0f);
                     ImGui::SliderFloat(gen_ui_id("TexHueXLFOVal", fb_idx, "FBTxHXLFO").c_str(), &fbs.tex_huex_lfo_val, -2.0f, 2.0f);
                 }
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(gen_ui_id("Geometry", fb_idx, "FBGeom").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                if(!fbs.tex_mod_enabled){
                    ImGui::SliderFloat(gen_ui_id("XDisp", fb_idx, "FBXDisp").c_str(), &fbs.x_displace, -fbs.x_displace_range, fbs.x_displace_range);
                    ImGui::SliderFloat(gen_ui_id("YDisp", fb_idx, "FBYDisp").c_str(), &fbs.y_displace, -fbs.y_displace_range, fbs.y_displace_range);
                    ImGui::SliderFloat(gen_ui_id("ZDisp", fb_idx, "FBZDisp").c_str(), &fbs.z_displace,  100.0f-fbs.z_displace_range, 100.0f+fbs.z_displace_range);
                    ImGui::SliderFloat(gen_ui_id("Rotate", fb_idx, "FBRot").c_str(), &fbs.rotate, -fbs.rotate_range, fbs.rotate_range);
                } else {
                    ImGui::SliderFloat(gen_ui_id("TexXDisp", fb_idx, "FBTxXDisp").c_str(), &fbs.tex_x_displace, -fbs.x_displace_range, fbs.x_displace_range);
                    ImGui::SliderFloat(gen_ui_id("TexYDisp", fb_idx, "FBTxYDisp").c_str(), &fbs.tex_y_displace,  -fbs.y_displace_range, fbs.y_displace_range);
                    ImGui::SliderFloat(gen_ui_id("TexZDisp", fb_idx, "FBTxZDisp").c_str(), &fbs.tex_z_displace,  -fbs.z_displace_range, fbs.z_displace_range);
                    ImGui::SliderFloat(gen_ui_id("TexRotate", fb_idx, "FBTxRot").c_str(), &fbs.tex_rotate, -fbs.rotate_range, fbs.rotate_range);
                }
                ImGui::TreePop();
            }
            ImGui::Unindent();
        }
        ImGui::Separator();

        if (ImGui::CollapsingHeader(gen_ui_id("LFOs", fb_idx, "FBLFOs").c_str(), ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::Indent();
            if (ImGui::TreeNodeEx(gen_ui_id("LfoMixDelay", fb_idx, "FBLFOMD").c_str())) {
                ImGui::Text("Mix LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "MixLFORate").c_str(), &fbs.mix_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "MixLFOAmp").c_str(), &fbs.mix_lfo.amp, -2.0f, 2.0f); ImGui::Unindent();
                ImGui::Text("Key Value LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "KVLFORate").c_str(), &fbs.lumakeyvalue_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "KVLFOAmp").c_str(), &fbs.lumakeyvalue_lfo.amp, -1.0f, 1.0f); ImGui::Unindent();
                ImGui::Text("Key Threshold LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "KTLFORate").c_str(), &fbs.lumakeythreshold_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "KTLFOAmp").c_str(), &fbs.lumakeythreshold_lfo.amp, -1.0f, 1.0f); ImGui::Unindent();
                ImGui::Text("Delay LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "DelayLFORate").c_str(), &fbs.delay_lfo.theta, .0f, 10.0f);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "DelayLFOAmp").c_str(), &fbs.delay_lfo.amp, -1.0f, 1.0f); ImGui::Unindent();
                ImGui::TreePop();
            }
            ImGui::Spacing();
            if (ImGui::TreeNodeEx(gen_ui_id("LfoColor", fb_idx, "FBLFOCS").c_str())) {
                ImGui::Text("Hue LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "HueLFORate").c_str(), &fbs.hue_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "HueLFOAmp").c_str(), &fbs.hue_lfo.amp, -2.0f, 2.0f); ImGui::Unindent();
                ImGui::Text("Saturation LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "SatLFORate").c_str(), &fbs.saturation_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "SatLFOAmp").c_str(), &fbs.saturation_lfo.amp, -2.0f, 2.0f); ImGui::Unindent();
                ImGui::Text("Brightness LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "BrightLFORate").c_str(), &fbs.bright_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "BrightLFOAmp").c_str(), &fbs.bright_lfo.amp, -2.0f, 2.0f); ImGui::Unindent();
                ImGui::Text("HueX Mod LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "HXMLFORate").c_str(), &fbs.huexmod_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "HXMLFOAmp").c_str(), &fbs.huexmod_lfo.amp, -10.0f, 10.0f); ImGui::Unindent();
                ImGui::Text("HueX Offset LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "HXOLFORate").c_str(), &fbs.huexoffset_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "HXOLFOAmp").c_str(), &fbs.huexoffset_lfo.amp, -10.0f, 10.0f); ImGui::Unindent();
                ImGui::Text("HueX LFO Val LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "HXLFOVLFORate").c_str(), &fbs.huexlfo_val_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "HXLFOVLFOAmp").c_str(), &fbs.huexlfo_val_lfo.amp, -10.0f, 10.0f); ImGui::Unindent();
                ImGui::TreePop();
            }
            ImGui::Spacing();
            if (ImGui::TreeNodeEx(gen_ui_id("LfoGeom", fb_idx, "FBLFOGeom").c_str())) {
                ImGui::Text("X Displace LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "XDispLFORate").c_str(), &fbs.x_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "XDispLFOAmp").c_str(), &fbs.x_lfo.amp, -fbs.x_displace_range, fbs.x_displace_range); ImGui::Unindent();
                ImGui::Text("Y Displace LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "YDispLFORate").c_str(), &fbs.y_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "YDispLFOAmp").c_str(), &fbs.y_lfo.amp, -fbs.y_displace_range, fbs.y_displace_range); ImGui::Unindent();
                ImGui::Text("Z Displace LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "ZDispLFORate").c_str(), &fbs.z_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "ZDispLFOAmp").c_str(), &fbs.z_lfo.amp, -fbs.z_displace_range, fbs.z_displace_range); ImGui::Unindent();
                ImGui::Text("Rotate LFO"); ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("Rate", fb_idx, "RotLFORate").c_str(), &fbs.rotate_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(gen_ui_id("Amp", fb_idx, "RotLFOAmp").c_str(), &fbs.rotate_lfo.amp, -fbs.rotate_range, fbs.rotate_range); ImGui::Unindent();
                ImGui::TreePop();
            }
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void GuiApp::drawGlobalControls(float lfom) {
    if (ImGui::Begin("Global Controls", nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
        GlobalSettings& gs = global_settings;

        if (ImGui::CollapsingHeader(gen_ui_id("GlobalMainControls", -1, "Header").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            if (ImGui::TreeNodeEx(gen_ui_id("General", -1, "GlobalGeneralNode").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Checkbox(gen_ui_id("FBClear", -1, "GlobalFBClearCb").c_str(), &gs.framebuffer_clear_trigger);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Clears all feedback buffers on the next frame.");
                ImGui::Checkbox(gen_ui_id("CtrlReset", -1, "GlobalCtrlResetCb").c_str(), &gs.control_reset_trigger);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Resets all controls to their default values.");
                ImGui::Spacing();
                ImGui::TreePop();
            }
            ImGui::Spacing();
            if (ImGui::TreeNodeEx(gen_ui_id("TexMods", -1, "GlobalTexModsNode").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Checkbox(gen_ui_id("TexModActive", -1, "GlobalTexModActiveCb" ).c_str(), &gs.texmod_enabled);
                const char* items_global_texmod_src[] = { "Cam 1","Cam 2","NDI" };
                static int item_current_g_texmod_src = 0;
                if (gs.global_texmod_select >= 0 && gs.global_texmod_select < IM_ARRAYSIZE(items_global_texmod_src)) { item_current_g_texmod_src = gs.global_texmod_select; }
                if(ImGui::Combo(gen_ui_id("TexModSrc", -1, "GlobalTexModSrcCb" ).c_str(), &item_current_g_texmod_src, items_global_texmod_src, IM_ARRAYSIZE(items_global_texmod_src))) { gs.global_texmod_select = item_current_g_texmod_src; }
                if (gs.texmod_enabled){ ImGui::Indent(); ImGui::Text("TexMod Effects");
                    ImGui::SliderFloat(gen_ui_id("BlurAmt", -1, "GlobalTexBlurAmtS" ).c_str(), &gs.texmod_blur_amount, -4.0f, 4.0f);
                    ImGui::SliderFloat(gen_ui_id("BlurRad", -1, "GlobalTexBlurRadS" ).c_str(), &gs.texmod_blur_radius, -2.0f, 2.0f);
                    ImGui::SliderFloat(gen_ui_id("SharpAmt", -1, "GlobalTexSharpAmtS" ).c_str(), &gs.texmod_sharpen_amount, -.6f, .6f);
                    ImGui::SliderFloat(gen_ui_id("SharpRad", -1, "GlobalTexSharpRadS" ).c_str(), &gs.texmod_sharpen_radius, .0f, 10.f);
                    ImGui::SliderFloat(gen_ui_id("SharpBoost", -1, "GlobalTexSharpBoostS" ).c_str(), &gs.texmod_sharpen_boost, -.4f, .4f); ImGui::Unindent(); }
                ImGui::TreePop();
            }
            ImGui::Spacing();
            if (ImGui::TreeNodeEx(gen_ui_id("PostProc", -1, "GlobalPostProcNode").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                if(!gs.texmod_enabled) { ImGui::Text("Main Effects (active if Global TexMod is off)");
                    ImGui::SliderFloat(gen_ui_id("BlurAmtMain", -1, "GlobalBlurAmtS_Main").c_str(), &gs.blur_amount, -2.0f, 2.0f);
                    ImGui::SliderFloat(gen_ui_id("BlurRadMain", -1, "GlobalBlurRadS_Main").c_str(), &gs.blur_radius, .0f, 10.0f);
                    ImGui::SliderFloat(gen_ui_id("SharpAmtMain", -1, "GlobalSharpAmtS_Main").c_str(), &gs.sharpen_amount, -.6f, .6f);
                    ImGui::SliderFloat(gen_ui_id("SharpRadMain", -1, "GlobalSharpRadS_Main").c_str(), &gs.sharpen_radius, .0f, 10.f);
                    ImGui::SliderFloat(gen_ui_id("SharpBoostMain", -1, "GlobalSharpBoostS_Main").c_str(), &gs.sharpen_boost, .0f, 1.0f);
                } else { ImGui::TextWrapped("Main post-processing effects are overridden by Global Texture Modifier when active."); }
                ImGui::TreePop();
            }
            ImGui::Spacing();
            if (ImGui::TreeNodeEx(gen_ui_id("CamNDISet", -1, "GlobalCamNDINode").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderFloat(gen_ui_id("Cam1Scale", -1, "GlobalCam1ScaleS" ).c_str(), &gs.cam1_scale, .0f, 2.0f);
                ImGui::Checkbox(gen_ui_id("Cam1HFlip", -1, "GlobalCam1HFlipCb" ).c_str(), &gs.cam1_hflip_switch); ImGui::SameLine(); ImGui::Checkbox(gen_ui_id("Cam1VFlip", -1, "GlobalCam1VFlipCb" ).c_str(), &gs.cam1_vflip_switch);
                ImGui::SliderFloat(gen_ui_id("Cam2Scale", -1, "GlobalCam2ScaleS" ).c_str(), &gs.cam2_scale, .0f, 2.0f);
                ImGui::Checkbox(gen_ui_id("Cam2HFlip", -1, "GlobalCam2HFlipCb" ).c_str(), &gs.cam2_hflip_switch); ImGui::SameLine(); ImGui::Checkbox(gen_ui_id("Cam2VFlip", -1, "GlobalCam2VFlipCb" ).c_str(), &gs.cam2_vflip_switch);
                ImGui::SliderFloat(gen_ui_id("NDIScale", -1, "GlobalNDIScaleS" ).c_str(), &gs.ndi_scale, -1000.0f, 500.0f); if (ImGui::IsItemHovered()) ImGui::SetTooltip("NDI scale (often Z-depth).");
                ImGui::TreePop();
            }
            ImGui::Spacing();
            if (ImGui::TreeNodeEx(gen_ui_id("SkewGeo", -1, "GlobalSkewGeoNode").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderFloat(gen_ui_id("XSkew", -1, "GlobalXSkewS" ).c_str(), &gs.x_skew, -PI, PI); ImGui::SliderFloat(gen_ui_id("YSkew", -1, "GlobalYSkewS" ).c_str(), &gs.y_skew, -PI, PI);
                ImGui::Checkbox(gen_ui_id("Tetra", -1, "GlobalTetraCb" ).c_str(), &gs.tetrahedron_switch); ImGui::SameLine(); ImGui::Checkbox(gen_ui_id("Hyper", -1, "GlobalHyperCb" ).c_str(), &gs.hypercube_switch);
                if (gs.hypercube_switch) { ImGui::Indent(); ImGui::SliderFloat(gen_ui_id("HyperThetaR", -1, "GlobalHyperThetaS" ).c_str(), &gs.hypercube_theta_rate, -.1f,.1f); ImGui::SliderFloat(gen_ui_id("HyperPhiR", -1, "GlobalHyperPhiS" ).c_str(), &gs.hypercube_phi_rate, -.1f,.1f); ImGui::Unindent(); }
                ImGui::TreePop();
            }
            ImGui::Unindent();
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader(gen_ui_id("GlobalLFOs", -1, "Header").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            ImGui::Text("Blur Amount LFO"); ImGui::Indent(); ImGui::SliderFloat(gen_ui_id("Rate", -1, "GlobalBlurLFORateS" ).c_str(), &gs.blur_amount_lfo.theta, .0f, LFO_MAX_RATE_GUI); ImGui::SliderFloat(gen_ui_id("Amp", -1, "GlobalBlurLFOAmpS" ).c_str(), &gs.blur_amount_lfo.amp, -10.0f, 10.0f); ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Blur Radius LFO"); ImGui::Indent(); ImGui::SliderFloat(gen_ui_id("Rate", -1, "GlobalBlurRadLFORateS" ).c_str(), &gs.blur_radius_lfo.theta, .0f, LFO_MAX_RATE_GUI); ImGui::SliderFloat(gen_ui_id("Amp", -1, "GlobalBlurRadLFOAmpS" ).c_str(), &gs.blur_radius_lfo.amp, -10.0f, 10.0f); ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Sharpen Amount LFO"); ImGui::Indent(); ImGui::SliderFloat(gen_ui_id("Rate", -1, "GlobalSharpAmtLFORateS" ).c_str(), &gs.sharpen_amount_lfo.theta, .0f, LFO_MAX_RATE_GUI); ImGui::SliderFloat(gen_ui_id("Amp", -1, "GlobalSharpAmtLFOAmpS" ).c_str(), &gs.sharpen_amount_lfo.amp, -.6f, .6f); ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Sharpen Radius LFO"); ImGui::Indent(); ImGui::SliderFloat(gen_ui_id("Rate", -1, "GlobalSharpRadLFORateS" ).c_str(), &gs.sharpen_radius_lfo.theta, .0f, LFO_MAX_RATE_GUI); ImGui::SliderFloat(gen_ui_id("Amp", -1, "GlobalSharpRadLFOAmpS" ).c_str(), &gs.sharpen_radius_lfo.amp, -10.0f, 10.f); ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Sharpen Boost LFO"); ImGui::Indent(); ImGui::SliderFloat(gen_ui_id("Rate", -1, "GlobalSharpBoostLFORateS" ).c_str(), &gs.sharpen_boost_lfo.theta, .0f, LFO_MAX_RATE_GUI); ImGui::SliderFloat(gen_ui_id("Amp", -1, "GlobalSharpBoostLFOAmpS" ).c_str(), &gs.sharpen_boost_lfo.amp, -1.0f, 1.0f); ImGui::Unindent();
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void GuiApp::drawPixelateControls(const std::string& panel_name_id, float lfom) {
    if (ImGui::Begin(panel_name_id.c_str(), nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);
        if (ImGui::CollapsingHeader(gen_ui_id("Settings", -1, panel_name_id).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            if (panel_name_id == "cam1_pixelate") {
                ImGui::Checkbox(gen_ui_id("Active", -1, panel_name_id).c_str(), &global_settings.cam1_pixel_switch);
                ImGui::SliderInt(gen_ui_id("ScaleX", -1, panel_name_id).c_str(), &global_settings.cam1_pixel_scale_x, 1, 256);
                ImGui::SliderInt(gen_ui_id("ScaleY", -1, panel_name_id).c_str(), &global_settings.cam1_pixel_scale_y, 1, 256);
                ImGui::SliderFloat(gen_ui_id("Mix", -1, panel_name_id).c_str(), &global_settings.cam1_pixel_mix, -2.0f, 2.0f);
                ImGui::SliderFloat(gen_ui_id("BrightScale", -1, panel_name_id).c_str(), &global_settings.cam1_pixel_brightscale, -2.0f, 2.0f);
            } else if (panel_name_id == "cam2_pixelate") {
                ImGui::Checkbox(gen_ui_id("Active", -1, panel_name_id).c_str(), &global_settings.cam2_pixel_switch);
                // ... (similar controls for cam2)
            } else if (panel_name_id == "ndi_pixelate") {
                ImGui::Checkbox(gen_ui_id("Active", -1, panel_name_id).c_str(), &global_settings.ndi_pixel_switch);
                // ... (similar controls for ndi)
            } else { // Must be feedback pixelate
                for(int i=0; i<MAX_FB_CHANNELS_GUI; ++i) {
                    if (panel_name_id == "fb" + std::to_string(i) + "_pixelate") {
                        FeedbackSettings& fbs = feedback_settings[i];
                        ImGui::Checkbox(gen_ui_id("Active", i, panel_name_id).c_str(), &fbs.pixel_switch);
                        ImGui::Checkbox(gen_ui_id("TexModPx", i, panel_name_id).c_str(), &fbs.pixel_texmod_enabled); ImGui::SameLine();
                        const char* items_fb_px_src[] = { "Channel 1", "Channel 2"};
                        static int item_current_fb_px_src[MAX_FB_CHANNELS_GUI] = {0};
                        if (fbs.pixel_texmod_select >=0 && fbs.pixel_texmod_select < IM_ARRAYSIZE(items_fb_px_src)) item_current_fb_px_src[i] = fbs.pixel_texmod_select; else item_current_fb_px_src[i] = 0;
                        if(ImGui::Combo(gen_ui_id("TexModSrcPx", i, panel_name_id).c_str(), &item_current_fb_px_src[i], items_fb_px_src, IM_ARRAYSIZE(items_fb_px_src))) { fbs.pixel_texmod_select = item_current_fb_px_src[i];}

                        if(!fbs.pixel_texmod_enabled){
                            ImGui::SliderInt(gen_ui_id("ScaleX", i, panel_name_id).c_str(), &fbs.pixel_scale_x, 1, 256);
                            // ... (other non-texmod pixel controls for fbs)
                        } else {
                            ImGui::SliderInt(gen_ui_id("TexModScaleX", i, panel_name_id).c_str(), &fbs.texmod_pixel_scale_x, -256, 256);
                            // ... (other texmod pixel controls for fbs)
                        }
                        break;
                    }
                }
            }
            ImGui::Unindent();
        }
        ImGui::Spacing();
        if (ImGui::CollapsingHeader(gen_ui_id("LFOs", -1, panel_name_id).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
             // ... (LFO controls for the specific panel_name_id, using global_settings or feedback_settings[idx])
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void GuiApp::drawVideoControls() {
    if (ImGui::Begin("Video Player", nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
        if (ImGui::Button(gen_ui_id("LoadVideoBtn").c_str())) { loadVideoTrigger = true; }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Open a dialog to load a video file.");
        ImGui::TextWrapped("Path: %s", loadedVideoPath.c_str());
        ImGui::Spacing();
        if (videoPlayingState) { if (ImGui::Button(gen_ui_id("PauseBtn").c_str())) { pauseVideoTrigger = true; } }
        else { if (ImGui::Button(gen_ui_id("PlayBtn").c_str())) { playVideoTrigger = true; } }
        ImGui::SameLine(); if (ImGui::Button(gen_ui_id("StopBtn").c_str())) { stopVideoTrigger = true; }
        ImGui::SameLine();
        if (ImGui::Checkbox(gen_ui_id("LoopCb").c_str(), &videoLoopingState)) {
            loopVideoToggleTrigger = true;
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle video looping.");
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void GuiApp::drawWindowCaptureControls() {
    if (ImGui::Begin("Window Capture", nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.7f);
        if (ImGui::Button(gen_ui_id("RefreshWinListBtn").c_str())) { listWindowsTrigger = true; }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Update the list of available windows to capture.");

        std::vector<const char*> cstrings; cstrings.reserve(availableWindowTitles.size());
        for(const auto& title : availableWindowTitles) { cstrings.push_back(title.c_str()); }

        if (availableWindowTitles.empty() || (availableWindowTitles.size()==1 && availableWindowTitles[0] == "No windows listed yet")) {
            ImGui::Text("No windows listed. Press Refresh.");
        } else {
            // Ensure selectedWindowIndex is valid before using it
            if (selectedWindowIndex < 0 || selectedWindowIndex >= cstrings.size()) {
                selectedWindowIndex = 0;
            }
            if (ImGui::Combo(gen_ui_id("WinCapCombo").c_str(), &selectedWindowIndex, cstrings.data(), cstrings.size())){
                // selectedWindowIndex is already updated by ImGui::Combo
            }
        }
        ImGui::Spacing();
        if (isWindowCurrentlyCapturing) {
            if (ImGui::Button(gen_ui_id("StopCapBtn").c_str())) { stopCaptureTrigger = true; }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Stop the current window capture.");
        } else {
            if (ImGui::Button(gen_ui_id("StartCapBtn").c_str())) {
                 if (!availableWindowTitles.empty() && availableWindowTitles[0] != "No windows listed yet" && selectedWindowIndex >= 0 && selectedWindowIndex < availableWindowTitles.size()) {
                    startCaptureTrigger = true;
                } else {
                     ofLogWarning("GuiApp") << "Cannot start capture: No valid window selected or list empty/not refreshed.";
                }
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Start capturing the selected window.");
        }
        ImGui::Spacing();
        ImGui::TextWrapped("Capturing: %s", capturedWindowTitleDisplay.c_str());
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void GuiApp::drawMoreEffectsControls() {
    if (ImGui::Begin("More Effects", nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f); // Adjust item width for this panel

        if (ImGui::CollapsingHeader("Particle Feedback##PFXHeader", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            ParticleFeedbackSettings& pfx_settings = particle_feedback_settings; // Shorter alias

            ImGui::Checkbox(gen_ui_id("EnablePFX", -1, "Cb").c_str(), &pfx_settings.enableParticleFeedback);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable the particle feedback system.");

            ImGui::Separator();
            ImGui::Text("Spawning");
            const char* input_sources_pfx[] = { "Main Output", "Cam 1", "Cam 2", "NDI", "Video File", "Window Capture", "Noise Generator" };
            // pfx_settings.inputSource is 0-indexed (0=MainOutput, 1=Cam1, ..., 6=NoiseGenerator)
            int current_source_idx_pfx = (pfx_settings.inputSource >= 0 && pfx_settings.inputSource < IM_ARRAYSIZE(input_sources_pfx)) ? pfx_settings.inputSource : 0;
            if (ImGui::Combo(gen_ui_id("PFXInputSrc", -1, "Cb").c_str(), &current_source_idx_pfx, input_sources_pfx, IM_ARRAYSIZE(input_sources_pfx))) {
                pfx_settings.inputSource = current_source_idx_pfx;
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select the source image for particle generation.");

            ImGui::SliderFloat(gen_ui_id("PFXSpawnThresh", -1, "Sld").c_str(), &pfx_settings.spawnThreshold, 0.0f, 1.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Brightness threshold (0-1) to spawn particles.");

            ImGui::SliderInt(gen_ui_id("PFXMaxParticles", -1, "SldInt").c_str(), &pfx_settings.maxParticles, 100, 10000);
             if (ImGui::IsItemHovered()) ImGui::SetTooltip("Maximum number of concurrent particles.");
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::Text("Particle Behavior");
            ImGui::SliderFloat(gen_ui_id("PFXLife", -1, "Sld").c_str(), &pfx_settings.particleInitialLife, 0.1f, 10.0f, "%.2f s");
             if (ImGui::IsItemHovered()) ImGui::SetTooltip("Initial lifespan of particles in seconds.");
            ImGui::SliderFloat(gen_ui_id("PFXSpeed", -1, "Sld").c_str(), &pfx_settings.particleInitialSpeed, 0.0f, 200.0f, "%.0f px/s");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Initial speed of particles in pixels per second.");
            ImGui::SliderFloat(gen_ui_id("PFXDrag", -1, "Sld").c_str(), &pfx_settings.particleDrag, 0.0f, 0.5f, "%.3f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Drag/damping factor applied to particle velocity.");
            ImGui::SliderFloat(gen_ui_id("PFXSize", -1, "Sld").c_str(), &pfx_settings.particleSize, 0.1f, 20.0f, "%.1f px");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Size of the particles.");

            ImGui::Checkbox(gen_ui_id("PFXVelFromBright", -1, "Cb").c_str(), &pfx_settings.enableVelocityFromBrightness);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("If enabled, particle initial speed is scaled by spawn pixel brightness.");
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::Text("Particle Appearance");
            ImGui::Checkbox(gen_ui_id("PFXInheritColor", -1, "Cb").c_str(), &pfx_settings.inheritColorFromSpawn);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("If enabled, particles inherit color from their spawn pixel. Otherwise, use base color.");

            float color_arr[4] = {
                pfx_settings.particleBaseColor.r / 255.0f,
                pfx_settings.particleBaseColor.g / 255.0f,
                pfx_settings.particleBaseColor.b / 255.0f,
                pfx_settings.particleBaseColor.a / 255.0f
            };
            if (ImGui::ColorEdit4(gen_ui_id("PFXBaseColor", -1, "ClrEdit").c_str(), color_arr)) {
                pfx_settings.particleBaseColor.set(color_arr[0] * 255, color_arr[1] * 255, color_arr[2] * 255, color_arr[3] * 255);
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Base color for particles if not inheriting from spawn pixel.");
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::Text("Forces & Environment");
            ImGui::SliderFloat(gen_ui_id("PFXNoiseForce", -1, "Sld").c_str(), &pfx_settings.noiseForceAmount, 0.0f, 100.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Strength of Perlin noise field affecting particle movement.");
            ImGui::SliderFloat(gen_ui_id("PFXNoiseScale", -1, "Sld").c_str(), &pfx_settings.noiseFieldScale, 0.001f, 0.1f, "%.4f");
             if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scale of the noise field (smaller value = larger patterns).");
            ImGui::SliderFloat(gen_ui_id("PFXNoiseTimeSpd", -1, "Sld").c_str(), &pfx_settings.noiseTimeSpeed, 0.0f, 1.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("How fast the noise field evolves over time.");
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::Text("Blending");
            ImGui::SliderFloat(gen_ui_id("PFXFeedbackMix", -1, "Sld").c_str(), &pfx_settings.feedbackMix, 0.0f, 1.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Mix amount when blending particle FBO back to main output (controls trail persistence). 0 = no trails, 1 = infinite trails.");

            ImGui::Unindent();
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Slit-Scan##SlitScanEffect", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            SlitScanSettings& ss_settings = this->slit_scan_settings;

            ImGui::Checkbox(gen_ui_id("EnableSS", -1, "SSCb").c_str(), &ss_settings.enableSlitScan);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable the Slit-Scan effect.");

            ImGui::Separator();
            ImGui::Text("Source & Slit Configuration");

            const char* input_sources_ss[] = { "Main Output", "Cam 1", "Cam 2", "NDI", "Video File", "Window Capture", "Noise Generator" };
            // ss_settings.inputSource is 0-indexed (0=MainOutput, 1=Cam1, ..., 6=NoiseGenerator)
            int current_source_idx_ss = (ss_settings.inputSource >= 0 && ss_settings.inputSource < IM_ARRAYSIZE(input_sources_ss)) ? ss_settings.inputSource : 0;
            if (ImGui::Combo(gen_ui_id("SSInputSrc", -1, "SSCombo").c_str(), &current_source_idx_ss, input_sources_ss, IM_ARRAYSIZE(input_sources_ss))) {
                ss_settings.inputSource = current_source_idx_ss;
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select the source image for the slit-scan effect.");

            ImGui::SliderInt(gen_ui_id("SSDelayFrames", -1, "SSSliderInt").c_str(), &ss_settings.delayFrames, 0, 120);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delay the input source by this many frames before scanning.");

            const char* slit_directions[] = { "Vertical Slit (Scan X-axis)", "Horizontal Slit (Scan Y-axis)" };
            int current_direction_idx = (ss_settings.slitDirection >= 0 && ss_settings.slitDirection < IM_ARRAYSIZE(slit_directions)) ? ss_settings.slitDirection : 0;
            if (ImGui::Combo(gen_ui_id("SSDirection", -1, "SSDirCombo").c_str(), &current_direction_idx, slit_directions, IM_ARRAYSIZE(slit_directions))) {
                ss_settings.slitDirection = current_direction_idx;
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Direction of the slit: Vertical slit samples a column, Horizontal slit samples a row.");

            ImGui::SliderFloat(gen_ui_id("SSPosition", -1, "SSSliderF").c_str(), &ss_settings.slitPosition, 0.0f, 1.0f, "%.3f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Normalized position of the slit across the source image (0.0 to 1.0).");

            ImGui::SliderInt(gen_ui_id("SSThickness", -1, "SSThickSldInt").c_str(), &ss_settings.slitThickness, 1, 100);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Thickness of the slit in pixels.");
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::Text("Accumulation & Output Blending");
            ImGui::SliderFloat(gen_ui_id("SSAccumSpeed", -1, "SSAccumSldF").c_str(), &ss_settings.accumulationSpeed, -10.0f, 10.0f, "%.1f px/frame");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("How many pixels the write position in the output moves per frame. Negative values reverse direction.");

            ImGui::Checkbox(gen_ui_id("SSWrapAccum", -1, "SSWrapCb").c_str(), &ss_settings.wrapAccumulation);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("If the accumulation wraps around the output image edges.");

            ImGui::SliderFloat(gen_ui_id("SSOutputMix", -1, "SSMixSldF").c_str(), &ss_settings.outputMix, 0.0f, 1.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Blend factor of the slit-scan output when drawing to main scene (0=transparent, 1=opaque).");

            const char* blend_modes_ss[] = { "Alpha Blend", "Additive", "Screen" };
            int current_blend_idx_ss = (ss_settings.blendMode >= 0 && ss_settings.blendMode < IM_ARRAYSIZE(blend_modes_ss)) ? ss_settings.blendMode : 0;
            if (ImGui::Combo(gen_ui_id("SSBlendMode", -1, "SSBlendCombo").c_str(), &current_blend_idx_ss, blend_modes_ss, IM_ARRAYSIZE(blend_modes_ss))) {
                ss_settings.blendMode = current_blend_idx_ss;
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Blend mode for combining slit-scan output with the main scene.");

            ImGui::Unindent();
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Noise Generator##NoiseGenEffect", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            NoiseGeneratorSettings& ng_settings = this->noise_generator_settings;

            ImGui::Checkbox(gen_ui_id("EnableNoise", -1, "NGCb").c_str(), &ng_settings.enableNoise);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable the Noise Generator. This will make a noise texture available as an input source.");

            ImGui::Separator();
            ImGui::Text("Noise Parameters");

            ImGui::Text("Type: Perlin (using ofNoise)"); // Placeholder, actual type selection commented out for now

            ImGui::SliderFloat(gen_ui_id("NGScale", -1, "NGSldF").c_str(), &ng_settings.noiseScale, 0.001f, 0.1f, "%.4f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scale of the noise pattern. Smaller values = larger patterns.");

            ImGui::Checkbox(gen_ui_id("NGAnimTime", -1, "NGCb").c_str(), &ng_settings.noiseAnimateTime);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Automatically animate the 'time' parameter of the noise.");
            if (ng_settings.noiseAnimateTime) {
                ImGui::SliderFloat(gen_ui_id("NGSpeed", -1, "NGSldF").c_str(), &ng_settings.noiseSpeed, 0.0f, 1.0f);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Speed of time animation for the noise.");
            } else {
                ImGui::SliderFloat(gen_ui_id("NGTime", -1, "NGSldF").c_str(), &ng_settings.noiseTime, 0.0f, 1000.0f);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Manual 'time' parameter for the noise (Z-axis for 3D noise).");
            }

            ImGui::SliderInt(gen_ui_id("NGOctaves", -1, "NGSldI").c_str(), &ng_settings.noiseOctaves, 1, 8);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Number of noise layers (octaves) for detail (requires shader support).");
            ImGui::SliderFloat(gen_ui_id("NGPersistence", -1, "NGSldF").c_str(), &ng_settings.noisePersistence, 0.0f, 1.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Persistence of higher octaves (requires shader support).");
            ImGui::Spacing();

            ImGui::Separator();
            ImGui::Text("Noise Output Mapping");
            ImGui::SliderFloat(gen_ui_id("NGRangeMin", -1, "NGSldF").c_str(), &ng_settings.noiseRangeMin, -1.0f, 1.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Expected minimum output of raw noise function (for remapping to 0-1).");
            ImGui::SliderFloat(gen_ui_id("NGRangeMax", -1, "NGSldF").c_str(), &ng_settings.noiseRangeMax, -1.0f, 1.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Expected maximum output of raw noise function (for remapping to 0-1).");

            ImGui::Checkbox(gen_ui_id("NGApplyContrast", -1, "NGCb").c_str(), &ng_settings.noiseApplyContrast);
            if (ng_settings.noiseApplyContrast) {
                ImGui::Indent();
                ImGui::SliderFloat(gen_ui_id("NGContrast", -1, "NGSldF").c_str(), &ng_settings.noiseContrast, 0.1f, 5.0f);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Contrast factor. >1 increases, <1 decreases.");
                ImGui::SliderFloat(gen_ui_id("NGBrightness", -1, "NGSldF").c_str(), &ng_settings.noiseBrightness, -0.5f, 0.5f);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Additive brightness adjustment (applied after contrast).");
                ImGui::Unindent();
            }
            ImGui::Spacing();

            ImGui::Checkbox(gen_ui_id("NGColorEnable", -1, "NGCb").c_str(), &ng_settings.noiseColorEnable);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable color mapping for the noise.");
            if (ng_settings.noiseColorEnable) {
                ImGui::Indent();
                // Corrected initialization for color_arr using ng_settings
                float color1_arr[4] = { ng_settings.noiseColor1.r / 255.0f, ng_settings.noiseColor1.g / 255.0f, ng_settings.noiseColor1.b / 255.0f, ng_settings.noiseColor1.a / 255.0f };
                if (ImGui::ColorEdit4(gen_ui_id("NGColor1", -1, "NGClr1").c_str(), color1_arr)) {
                    ng_settings.noiseColor1.set(color1_arr[0] * 255, color1_arr[1] * 255, color1_arr[2] * 255, color1_arr[3] * 255);
                }
                // Corrected initialization for color2_arr using ng_settings
                float color2_arr[4] = { ng_settings.noiseColor2.r / 255.0f, ng_settings.noiseColor2.g / 255.0f, ng_settings.noiseColor2.b / 255.0f, ng_settings.noiseColor2.a / 255.0f };
                if (ImGui::ColorEdit4(gen_ui_id("NGColor2", -1, "NGClr2").c_str(), color2_arr)) {
                    ng_settings.noiseColor2.set(color2_arr[0] * 255, color2_arr[1] * 255, color2_arr[2] * 255, color2_arr[3] * 255);
                }
                ImGui::Unindent();
            }
            ImGui::Unindent();
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Pixel Sorting##PixelSortEffect", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            PixelSortSettings& ps_settings = this->pixel_sort_settings;

            ImGui::Checkbox(gen_ui_id("EnablePS", -1, "PSCb").c_str(), &ps_settings.enablePixelSort);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable the Pixel Sorting effect.");

            ImGui::Separator();
            ImGui::Text("Configuration");

            const char* input_sources_ps[] = { "Main Output", "Cam 1", "Cam 2", "NDI", "Video File", "Window Capture", "Noise Generator" };
            int current_source_idx_ps = (ps_settings.inputSource >= 0 && ps_settings.inputSource < IM_ARRAYSIZE(input_sources_ps)) ? ps_settings.inputSource : 0;
            if (ImGui::Combo(gen_ui_id("PSInputSrc", -1, "PSCmb").c_str(), &current_source_idx_ps, input_sources_ps, IM_ARRAYSIZE(input_sources_ps))) {
                ps_settings.inputSource = current_source_idx_ps;
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select the source image for the pixel sorting effect.");

            const char* sort_modes[] = { "Horizontal Lines", "Vertical Columns" };
            int current_sort_mode_idx = (ps_settings.sortMode >= 0 && ps_settings.sortMode < IM_ARRAYSIZE(sort_modes)) ? ps_settings.sortMode : 0;
            if (ImGui::Combo(gen_ui_id("PSSortMode", -1, "PSCmb").c_str(), &current_sort_mode_idx, sort_modes, IM_ARRAYSIZE(sort_modes))) {
                ps_settings.sortMode = current_sort_mode_idx;
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Sort pixels along horizontal lines or vertical columns.");

            const char* sort_criteria[] = { "Brightness", "Hue", "Red", "Green", "Blue", "Luminance" };
            int current_criteria_idx = (ps_settings.sortCriteria >= 0 && ps_settings.sortCriteria < IM_ARRAYSIZE(sort_criteria)) ? ps_settings.sortCriteria : 0;
            if (ImGui::Combo(gen_ui_id("PSSortCrit", -1, "PSCmb").c_str(), &current_criteria_idx, sort_criteria, IM_ARRAYSIZE(sort_criteria))) {
                ps_settings.sortCriteria = current_criteria_idx;
            }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Metric used for sorting pixels.");

            ImGui::SliderFloat(gen_ui_id("PSThreshMin", -1, "PSSldF").c_str(), &ps_settings.thresholdMin, 0.0f, 1.0f, "%.2f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Minimum threshold to start/include pixels in a sortable segment.");
            ImGui::SliderFloat(gen_ui_id("PSThreshMax", -1, "PSSldF").c_str(), &ps_settings.thresholdMax, 0.0f, 1.0f, "%.2f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Maximum threshold to end/include pixels in a sortable segment.");

            ImGui::Checkbox(gen_ui_id("PSSortAsc", -1, "PSCb").c_str(), &ps_settings.sortAscending);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Sort in ascending order. If unchecked, sorts in descending order.");

            ImGui::SliderFloat(gen_ui_id("PSEffectMix", -1, "PSSldF").c_str(), &ps_settings.effectMix, 0.0f, 1.0f, "%.2f");
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Blend between original (0.0) and sorted (1.0) image.");

            ImGui::Unindent();
        }

        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void GuiApp::drawOutputWindow(ofTexture* output_texture) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    if (ImGui::Begin("Output Window", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse)) {
        if (output_texture && output_texture->isAllocated()) {
            ImVec2 window_size = ImGui::GetContentRegionAvail();
            float tex_w = output_texture->getWidth();
            float tex_h = output_texture->getHeight();
            if (tex_w > 0 && tex_h > 0) {
                float aspect_ratio = tex_h / tex_w;
                float img_width = window_size.x;
                float img_height = window_size.x * aspect_ratio;
                if (img_height > window_size.y) {
                    img_height = window_size.y;
                    img_width = window_size.y / aspect_ratio;
                }
                float off_x = (window_size.x - img_width) * 0.5f;
                float off_y = (window_size.y - img_height) * 0.5f;
                if (off_x < 0) off_x = 0; if (off_y < 0) off_y = 0; // Ensure offset is not negative
                ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + off_x, ImGui::GetCursorPosY() + off_y));
                ImGui::Image(reinterpret_cast<ImTextureID>(output_texture->getTextureData().textureID),
                            ImVec2(img_width, img_height));
            } else {
                 ImGui::TextWrapped("Output texture has invalid dimensions.");
            }
        } else { ImGui::TextWrapped("Output display area. Waiting for texture from ofApp..."); }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void GuiApp::draw() {
    imgui_instance.begin();
    static bool first_time_docking = true;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos()); ImGui::SetNextWindowSize(viewport->GetWorkSize()); ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                  ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpaceWindow", nullptr, window_flags); ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    if (first_time_docking) {
        first_time_docking = false;
        ImGui::DockBuilderRemoveNode(dockspace_id); ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->GetWorkSize());
        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
        ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);

        ImGui::DockBuilderDockWindow("Output Window", dock_main_id);
        ImGui::DockBuilderDockWindow("Channel 1", dock_left_id);
        ImGui::DockBuilderDockWindow("Channel 2", dock_left_id);
        ImGui::DockBuilderDockWindow("Video Player", dock_left_id);
        ImGui::DockBuilderDockWindow("Window Capture", dock_left_id);
        ImGui::DockBuilderDockWindow("Global Controls", dock_left_id);
        ImGui::DockBuilderDockWindow("More Effects", dock_left_id);

        ImGuiID dock_right_feedback_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Top, 0.5f, nullptr, &dock_right_id);
        for(int i=0; i<MAX_FB_CHANNELS_GUI; ++i) ImGui::DockBuilderDockWindow(("Feedback " + std::to_string(i)).c_str(), dock_right_feedback_id);

        ImGuiID dock_right_pixelate_id = dock_right_id;
        ImGui::DockBuilderDockWindow("cam1_pixelate", dock_right_pixelate_id);
        ImGui::DockBuilderDockWindow("cam2_pixelate", dock_right_pixelate_id);
        ImGui::DockBuilderDockWindow("ndi_pixelate", dock_right_pixelate_id);
        for(int i=0; i<MAX_FB_CHANNELS_GUI; ++i) ImGui::DockBuilderDockWindow(("fb" + std::to_string(i) + "_pixelate").c_str(), dock_right_pixelate_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    if (ImGui::BeginMenuBar()) { if (ImGui::BeginMenu("Layout")) { if (ImGui::MenuItem("Reset Docking")) { first_time_docking = true; } ImGui::EndMenu(); } ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate); ImGui::EndMenuBar(); }

    float lfom = LFO_MAX_RATE_GUI;
    drawChannelControls(0, lfom); drawChannelControls(1, lfom);
    for (int i = 0; i < MAX_FB_CHANNELS_GUI; ++i) { drawFeedbackControls(i, lfom); }
    drawGlobalControls(lfom);
    drawVideoControls();
    drawWindowCaptureControls();
    drawMoreEffectsControls();

    drawPixelateControls("cam1_pixelate", lfom);
    drawPixelateControls("cam2_pixelate", lfom);
    drawPixelateControls("ndi_pixelate", lfom);
    for (int i = 0; i < MAX_FB_CHANNELS_GUI; ++i) { drawPixelateControls("fb" + std::to_string(i) + "_pixelate", lfom); }

    ImGui::End();
    imgui_instance.end();
}
```
