/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"
#include <iostream>


int fbob=30;

int guiscale=275;

void GuiApp::setup(){
    
    ofBackground(0);
   
    //gui.setup(); // ofxImGui::Gui::setup() usually calls ImGui::CreateContext() and ImGui_Impl*.cpp setup.
                 // We need to set IO flags after ImGui::CreateContext() and before ImGui_Impl*.cpp newFrame().
                 // Assuming gui.setup() handles the context, we'll call it and then set flags.
                 // If ofxImGui has a way to pass config flags or a custom setup, that would be better.
    gui.setup();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // Optional: Enable viewport support if you want ImGui windows to be able to float outside the main OF window
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    //     ImGui::StyleColorsDark(); // or ImGui::StyleColorsClassic();
    //     ImGuiStyle& style = ImGui::GetStyle();
    //     if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    //         style.WindowRounding = 0.0f;
    //         style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    //     }
    // }
    
  }


//----------------------------------
    void GuiApp::update() {
       
    }


//------------------------------

void GuiApp::drawChannel1Controls(ofxImGui::Settings &settings, float lfom) {
    // No longer using settings for window positioning if docking is primary
    if (ImGui::Begin("channel1", nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f); // Adjust item width

        if (ImGui::CollapsingHeader("Input Source##Ch1Source", ImGuiTreeNodeFlags_DefaultOpen)) {
            const char* items[] = { "cam1", "cam2", "ndi" };
            static int item_current_ch1_src = 0;
            ImGui::Combo("Source##Ch1SourceCombo", &item_current_ch1_src, items, IM_ARRAYSIZE(items));
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select the input source for Channel 1.");
            if(item_current_ch1_src == 0) { channel1_select = 1; }
            else if(item_current_ch1_src == 1) { channel1_select = 2; }
            else { channel1_select = 3; }
            ImGui::Spacing();
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Color Adjustments##Ch1Color", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SliderFloat("Hue##Ch1Hue", &channel_settings[0].hue, -5.0f, 5.0f);
            ImGui::SliderFloat("Saturation##Ch1Sat", &channel_settings[0].saturation, -5.0f, 5.0f);
            ImGui::SliderFloat("Brightness##Ch1Bright", &channel_settings[0].bright, -5.0f, 5.0f);
            ImGui::Spacing();
            ImGui::Checkbox("Hue Invert##Ch1HueInv", &channel_settings[0].hue_alt_invert_toggle); ImGui::SameLine();
            ImGui::Checkbox("Sat Invert##Ch1SatInv", &channel_settings[0].saturation_alt_invert_toggle); ImGui::SameLine();
            ImGui::Checkbox("Bright Invert##Ch1BrightInv", &channel_settings[0].bright_alt_invert_toggle);
            ImGui::Spacing();
            ImGui::Checkbox("Saturation Wrap##Ch1SatWrap", &channel_settings[0].saturation_wrap); ImGui::SameLine();
            ImGui::Checkbox("Brightness Wrap##Ch1BrightWrap", &channel_settings[0].bright_wrap);
            ImGui::Spacing();
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Power Mapping##Ch1PowMap", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SliderFloat("Hue Powmap##Ch1HuePm", &channel_settings[0].hue_powmap, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Apply power curve to hue adjustments (value^powmap).");
            ImGui::SliderFloat("Saturation Powmap##Ch1SatPm", &channel_settings[0].saturation_powmap, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Apply power curve to saturation adjustments (value^powmap).");
            ImGui::SliderFloat("Brightness Powmap##Ch1BrightPm", &channel_settings[0].bright_powmap, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Apply power curve to brightness adjustments (value^powmap).");
            ImGui::Spacing();
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("LFOs##Ch1LFOs", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Hue LFO");
            ImGui::Indent();
            ImGui::SliderFloat("Rate##Ch1HueLFORate", &channel_settings[0].hue_lfo_theta,  .0f, lfom);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Hue LFO rate.");
            ImGui::SliderFloat("Amplitude##Ch1HueLFOAmp", &channel_settings[0].hue_lfo_amp, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Hue LFO amplitude.");
            ImGui::Unindent();
            ImGui::Spacing();

            ImGui::Text("Saturation LFO");
            ImGui::Indent();
            ImGui::SliderFloat("Rate##Ch1SatLFORate", &channel_settings[0].saturation_lfo_theta, .0f, lfom);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Saturation LFO rate.");
            ImGui::SliderFloat("Amplitude##Ch1SatLFOAmp", &channel_settings[0].saturation_lfo_amp, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Saturation LFO amplitude.");
            ImGui::Unindent();
            ImGui::Spacing();

            ImGui::Text("Brightness LFO");
            ImGui::Indent();
            ImGui::SliderFloat("Rate##Ch1BrightLFORate", &channel_settings[0].bright_lfo_theta, .0f, lfom);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Brightness LFO rate.");
            ImGui::SliderFloat("Amplitude##Ch1BrightLFOAmp", &channel_settings[0].bright_lfo_amp, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Brightness LFO amplitude.");
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void GuiApp::drawChannel2Controls(ofxImGui::Settings &settings, float lfom) {
    if (ImGui::Begin("channel2", nullptr, ImGuiWindowFlags_None))
    {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);

        if (ImGui::CollapsingHeader("Input Source##Ch2Source", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const char* items[] = { "cam1","cam2", "ndi" };
            static int item_current_ch2 = 0;
            ImGui::Combo("Source##ch2", &item_current_ch2, items, IM_ARRAYSIZE(items));
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select the input source for Channel 2.");
            if(item_current_ch2==0){channel2_select=1;}
            if(item_current_ch2==1){channel2_select=2;}
            if(item_current_ch2==2){channel2_select=3;}
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Mixing##ch2"))
        {
            ImGui::SliderFloat("Mix##ch2", &channel_settings[1].mix, -2.0f, 2.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Channel 2 mix amount.");
            ImGui::SliderFloat("Key Value##ch2", &channel_settings[1].key_value, .0f, 1.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Channel 2 keying value.");
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Color Adjustments##ch2"))
        {
            ImGui::SliderFloat("Hue##ch2", &channel_settings[1].hue, -5.0f, 5.0f);
            ImGui::SliderFloat("Saturation##ch2", &channel_settings[1].saturation, -5.0f, 5.0f);
            ImGui::SliderFloat("Brightness##ch2", &channel_settings[1].bright, -5.0f, 5.0f);
            ImGui::Spacing();
            ImGui::Checkbox("Hue Invert##ch2_alt", &channel_settings[1].hue_alt_invert_toggle); ImGui::SameLine();
            ImGui::Checkbox("Sat Invert##ch2_alt", &channel_settings[1].saturation_alt_invert_toggle); ImGui::SameLine();
            ImGui::Checkbox("Bright Invert##ch2_alt", &channel_settings[1].bright_alt_invert_toggle);
            ImGui::Spacing();
            ImGui::Checkbox("Saturation Wrap##ch2", &channel_settings[1].saturation_wrap); ImGui::SameLine();
            ImGui::Checkbox("Brightness Wrap##ch2", &channel_settings[1].bright_wrap);
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Power Mapping##ch2"))
        {
            ImGui::SliderFloat("Hue Powmap##ch2", &channel_settings[1].hue_powmap, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Apply power curve to hue adjustments.");
            ImGui::SliderFloat("Saturation Powmap##ch2", &channel_settings[1].saturation_powmap, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Apply power curve to saturation adjustments.");
            ImGui::SliderFloat("Brightness Powmap##ch2", &channel_settings[1].bright_powmap, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Apply power curve to brightness adjustments.");
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("LFOs##ch2"))
        {
            ImGui::Text("Hue LFO");
            ImGui::SliderFloat("Rate##ch2_hue_lfo", &channel_settings[1].hue_lfo_theta,  .0f, lfom);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Hue LFO rate.");
            ImGui::SliderFloat("Amplitude##ch2_hue_lfo", &channel_settings[1].hue_lfo_amp, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Hue LFO amplitude.");
            ImGui::Spacing();
            ImGui::Text("Saturation LFO");
            ImGui::SliderFloat("Rate##ch2_sat_lfo", &channel_settings[1].saturation_lfo_theta, .0f, lfom);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Saturation LFO rate.");
            ImGui::SliderFloat("Amplitude##ch2_sat_lfo", &channel_settings[1].saturation_lfo_amp, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Saturation LFO amplitude.");
            ImGui::Spacing();
            ImGui::Text("Brightness LFO");
            ImGui::SliderFloat("Rate##ch2_br_lfo", &channel_settings[1].bright_lfo_theta, .0f, lfom);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Brightness LFO rate.");
            ImGui::SliderFloat("Amplitude##ch2_br_lfo", &channel_settings[1].bright_lfo_amp, -5.0f, 5.0f);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Brightness LFO amplitude.");
        }
        ImGui::PopItemWidth();
    }
    ofxImGui::EndWindow(settings);
}

void GuiApp::draw() {
    gui.begin(); // This usually calls ImGui::NewFrame()

    static bool first_time_docking = true;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    // Make it transparent for OF background to show, or remove for solid color if preferred
    // For a solid background, remove ImGuiWindowFlags_NoBackground and ensure a theme is set or set ImGuiCol_WindowBg
    window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
    ImGui::PopStyleVar(3); // Pop for WindowRounding, WindowBorderSize, WindowPadding

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    if (first_time_docking) {
        first_time_docking = false;

        ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add back the dockspace node
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID dock_main_id = dockspace_id; // This is the central node
        ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.25f, nullptr, &dock_main_id);
        ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.33f, nullptr, &dock_main_id); // 0.33f of remaining space (so 0.25 relative to total)
        
        // Dock "Output Window" to the central area (dock_main_id)
        ImGui::DockBuilderDockWindow("Output Window", dock_main_id);

        // Dock Channel and Global controls to the left
        ImGui::DockBuilderDockWindow("channel1", dock_left_id);
        ImGui::DockBuilderDockWindow("channel2", dock_left_id); // Will tab with channel1
        ImGui::DockBuilderDockWindow("global", dock_left_id);   // Will tab/stack

        // Dock Feedback controls to the right
        ImGui::DockBuilderDockWindow("fb0", dock_right_id);
        ImGui::DockBuilderDockWindow("fb1", dock_right_id);
        ImGui::DockBuilderDockWindow("fb2", dock_right_id);
        ImGui::DockBuilderDockWindow("fb3", dock_right_id);

        // Dock Pixelate controls to the right (will also tab/stack with feedback)
        ImGui::DockBuilderDockWindow("cam1_pixelate", dock_right_id);
        ImGui::DockBuilderDockWindow("cam2_pixelate", dock_right_id);
        ImGui::DockBuilderDockWindow("fb0_pixelate", dock_right_id);
        ImGui::DockBuilderDockWindow("fb1_pixelate", dock_right_id);
        ImGui::DockBuilderDockWindow("fb2_pixelate", dock_right_id);
        ImGui::DockBuilderDockWindow("fb3_pixelate", dock_right_id);
        ImGui::DockBuilderDockWindow("ndi_pixelate", dock_right_id);

        ImGui::DockBuilderFinish(dockspace_id);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Layout")) {
            if (ImGui::MenuItem("Reset Docking Layout")) {
                first_time_docking = true; // Trigger layout reset on next frame
            }
            ImGui::EndMenu();
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::EndMenuBar();
    }

    auto mainSettings = ofxImGui::Settings();
    float lfom = 10; // Assuming this was a local variable, ensure it's defined.
    // gui_hscaler and gui_vscaler are no longer needed for main panel positioning
    // int gui_hscaler=170;
    // int gui_vscaler=80;

    // Call drawing functions for control panels - they will now be dockable windows
    // Pass nullptr for settings if it's not used by ImGui::Begin directly
    drawChannel1Controls(mainSettings, lfom); // mainSettings might be unused now
    drawChannel2Controls(mainSettings, lfom); // mainSettings might be unused now

    drawFeedbackControls(lfom, 0); // Removed mainSettings, hscaler, vscaler
    drawFeedbackControls(lfom, 1);
    drawFeedbackControls(lfom, 2);
    drawFeedbackControls(lfom, 3);

    drawGlobalControls(lfom); // mainSettings might be unused now // Removed mainSettings

    // Conceptual Output Window (simple example)
    ImGui::Begin("Output Window");
    ImGui::Text("This is where the main output would be rendered.");
    // In a real scenario, you'd get the ofFbo texture and draw it here using ImGui::Image
    ImGui::End();

    // Pixelate controls
    // id and group_index are no longer needed for this simplified signature
    drawPixelateControls(lfom, "cam1_pixelate");
    drawPixelateControls(lfom, "cam2_pixelate");
    drawPixelateControls(lfom, "fb0_pixelate");
    drawPixelateControls(lfom, "fb1_pixelate");
    drawPixelateControls(lfom, "fb2_pixelate");
    drawPixelateControls(lfom, "fb3_pixelate");
    drawPixelateControls(lfom, "ndi_pixelate");


    ImGui::End(); // End DockSpaceWindow
    gui.end();   // This usually calls ImGui::Render() and ImGui_Impl*.cpp RenderDrawData()
}

void GuiApp::drawFeedbackControls(float lfom, int index) { // Removed settings
    std::string fb_name = "fb" + std::to_string(index);
    // Using fb_name for window title for now, can be changed to more descriptive like "Feedback 0"
    if (ImGui::Begin(fb_name.c_str(), nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f); // Adjust item width

        if (ImGui::CollapsingHeader(("Parameters##Params_" + fb_name).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();

            ImGui::Checkbox(("Texture Mod##" + fb_name + "_texmod_cb").c_str(), &feedback_settings[index].tex_mod);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable texture modification for this feedback channel.");
            ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);

            const char* items_src[] = { "Channel 1","Channel 2" };
            static int item_current_fb_texmod_src[4] = {0,0,0,0};
            ImGui::PushID(("TexModSrcCombo" + fb_name).c_str());
            ImGui::Combo("Source", &item_current_fb_texmod_src[index], items_src, IM_ARRAYSIZE(items_src));
            ImGui::PopID();
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select source for texture modification.");
            if(item_current_fb_texmod_src[index]==0){feedback_settings[index].texmod_select=1;}
            else {feedback_settings[index].texmod_select=2;}
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(("Mix & Delay##MixDelay_" + fb_name).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                if(feedback_settings[index].tex_mod==false){
                    ImGui::SliderFloat(("Mix##Mix_" + fb_name).c_str(), &feedback_settings[index].mix, -2.0f, 2.0f);
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Feedback mix amount.");
                    ImGui::SliderFloat(("Key Value##KeyVal_" + fb_name).c_str(), &feedback_settings[index].key_value, .0f, 1.0f);
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Luma key value.");
                    ImGui::SliderFloat(("Key Threshold##KeyThresh_" + fb_name).c_str(), &feedback_settings[index].key_threshold, .0f, 1.0f);
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Luma key threshold.");
                } else {
                    ImGui::SliderFloat(("TexMod Mix##TexMix_" + fb_name).c_str(), &feedback_settings[index].tex_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat(("TexMod Key Val##TexKeyVal_" + fb_name).c_str(), &feedback_settings[index].tex_key_value, .0f, 1.0f);
                    ImGui::SliderFloat(("TexMod Key Thresh##TexKeyThresh_" + fb_name).c_str(), &feedback_settings[index].tex_key_threshold, .0f, 1.0f);
                }
                ImGui::SliderInt(("Delay Amount##Delay_" + fb_name).c_str(), &feedback_settings[index].delay_amount, 0, fbob-1);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Feedback delay in frames.");
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(("Switches##Switches_" + fb_name).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                const char* items_overflow[] = { "Clamp","Toroid","Mirror" };
                static int item_current_fb_overflow[4] = {0,0,0,0};
                ImGui::PushID(("OverflowCombo" + fb_name).c_str());
                ImGui::Combo("Mode", &item_current_fb_overflow[index], items_overflow, IM_ARRAYSIZE(items_overflow));
                ImGui::PopID();
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select coordinate overflow handling method.");
                if(item_current_fb_overflow[index]==0){feedback_settings[index].toroid_switch=0;}
                else if(item_current_fb_overflow[index]==1){feedback_settings[index].toroid_switch=1;}
                else {feedback_settings[index].toroid_switch=2;}

                ImGui::Checkbox(("H Mirror##HMir_" + fb_name).c_str(), &feedback_settings[index].hflip_switch); ImGui::SameLine();
                ImGui::Checkbox(("V Mirror##VMir_" + fb_name).c_str(), &feedback_settings[index].vflip_switch);
                ImGui::Spacing();
                ImGui::Checkbox(("Hue Invert##HueInv_" + fb_name).c_str(), &feedback_settings[index].hue_invert); ImGui::SameLine();
                ImGui::Checkbox(("Sat Invert##SatInv_" + fb_name).c_str(), &feedback_settings[index].saturation_invert); ImGui::SameLine();
                ImGui::Checkbox(("Bright Invert##BrInv_" + fb_name).c_str(), &feedback_settings[index].bright_invert);
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(("Ranges##Ranges_" + fb_name).c_str())) { // Default closed
                ImGui::SliderFloat(("X Disp Range##XDispR_" + fb_name).c_str(), &feedback_settings[index].x_displace_range, .0001f, 100.0f);
                ImGui::SliderFloat(("Y Disp Range##YDispR_" + fb_name).c_str(), &feedback_settings[index].y_displace_range, .0001f, 100.0f);
                ImGui::SliderFloat(("Z Disp Range##ZDispR_" + fb_name).c_str(), &feedback_settings[index].z_displace_range, .0001f, 100.0f);
                ImGui::SliderFloat(("Rotate Range##RotR_" + fb_name).c_str(), &feedback_settings[index].rotate_range, .0001f, TWO_PI);
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(("Color Space##ColorSpace_" + fb_name).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                  if(feedback_settings[index].tex_mod==false){
                      ImGui::SliderFloat(("Hue##ColHue_" + fb_name).c_str(), &feedback_settings[index].hue, 8.0f, 12.0f);
                      ImGui::SliderFloat(("Saturation##ColSat_" + fb_name).c_str(), &feedback_settings[index].saturation, 8.0f, 12.0f);
                      ImGui::SliderFloat(("Brightness##ColBr_" + fb_name).c_str(), &feedback_settings[index].bright, 8.0f, 12.0f);
                      ImGui::SliderFloat(("HueX Mod##ColHMod_" + fb_name).c_str(), &feedback_settings[index].huex_mod, 0.0f, 10.0f);
                      ImGui::SliderFloat(("HueX Offset##ColHOff_" + fb_name).c_str(), &feedback_settings[index].huex_offset, -20.0f, 20.0f);
                      ImGui::SliderFloat(("HueX LFO Val##ColHLFO_" + fb_name).c_str(), &feedback_settings[index].huex_lfo, -20.0f, 20.0f);
                       if (ImGui::IsItemHovered()) ImGui::SetTooltip("Raw LFO value for HueX modulation.");
                  } else {
                     ImGui::SliderFloat(("TexMod Hue##TexColHue_" + fb_name).c_str(), &feedback_settings[index].tex_hue, -.25f, .25f);
                     ImGui::SliderFloat(("TexMod Sat##TexColSat_" + fb_name).c_str(), &feedback_settings[index].tex_saturation, -.25f, .25f);
                     ImGui::SliderFloat(("TexMod Bright##TexColBr_" + fb_name).c_str(), &feedback_settings[index].tex_bright, -.25f, .25f);
                     ImGui::SliderFloat(("TexMod HueX Mod##TexColHMod_" + fb_name).c_str(), &feedback_settings[index].tex_huex_mod, -1.0f, 0.0f);
                     ImGui::SliderFloat(("TexMod HueX Offset##TexColHOff_" + fb_name).c_str(), &feedback_settings[index].tex_huex_offset, -2.0f, 2.0f);
                     ImGui::SliderFloat(("TexMod HueX LFO Val##TexColHLFO_" + fb_name).c_str(), &feedback_settings[index].tex_huex_lfo, -2.0f, 2.0f);
                 }
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx(("Geometry##Geom_" + fb_name).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                if(feedback_settings[index].tex_mod==false){
                    ImGui::SliderFloat(("X Displace##GeomX_" + fb_name).c_str(), &feedback_settings[index].x_displace, -feedback_settings[index].x_displace_range, feedback_settings[index].x_displace_range);
                    ImGui::SliderFloat(("Y Displace##GeomY_" + fb_name).c_str(), &feedback_settings[index].y_displace, -feedback_settings[index].y_displace_range, feedback_settings[index].y_displace_range);
                    ImGui::SliderFloat(("Z Displace##GeomZ_" + fb_name).c_str(), &feedback_settings[index].z_displace,  100.0f-feedback_settings[index].z_displace_range, 100.0f+feedback_settings[index].z_displace_range);
                    ImGui::SliderFloat(("Rotate##GeomRot_" + fb_name).c_str(), &feedback_settings[index].rotate, -feedback_settings[index].rotate_range, feedback_settings[index].rotate_range);
                } else {
                    ImGui::SliderFloat(("TexMod X Displace##TexGeomX_" + fb_name).c_str(), &feedback_settings[index].tex_x_displace, -feedback_settings[index].x_displace_range, feedback_settings[index].x_displace_range);
                    ImGui::SliderFloat(("TexMod Y Displace##TexGeomY_" + fb_name).c_str(), &feedback_settings[index].tex_y_displace,  -feedback_settings[index].y_displace_range, feedback_settings[index].y_displace_range);
                    ImGui::SliderFloat(("TexMod Z Displace##TexGeomZ_" + fb_name).c_str(), &feedback_settings[index].tex_z_displace,  -feedback_settings[index].y_displace_range, feedback_settings[index].y_displace_range);
                    ImGui::SliderFloat(("TexMod Rotate##TexGeomRot_" + fb_name).c_str(), &feedback_settings[index].tex_rotate, -feedback_settings[index].rotate_range, feedback_settings[index].rotate_range);
                }
                ImGui::TreePop();
            }
            ImGui::Unindent();
        } // End Parameters CollapsingHeader
        ImGui::Separator();

        if (ImGui::CollapsingHeader(("LFOs##LFOs_" + fb_name).c_str(), ImGuiTreeNodeFlags_DefaultOpen)){
            ImGui::Indent();
            if (ImGui::TreeNodeEx(("Mix & Delay LFOs##LfoMixDelay_" + fb_name).c_str())) {
                ImGui::Text("Mix LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateMix_" + fb_name).c_str(), &feedback_settings[index].mix_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpMix_" + fb_name).c_str(), &feedback_settings[index].mix_lfo.amp, -2.0f, 2.0f); ImGui::Unindent();
                ImGui::Text("Key Value LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateKV_" + fb_name).c_str(), &feedback_settings[index].lumakeyvalue_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpKV_" + fb_name).c_str(), &feedback_settings[index].lumakeyvalue_lfo.amp, -1.0f, 1.0f); ImGui::Unindent();
                ImGui::Text("Key Threshold LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateKT_" + fb_name).c_str(), &feedback_settings[index].lumakeythreshold_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpKT_" + fb_name).c_str(), &feedback_settings[index].lumakeythreshold_lfo.amp, -1.0f, 1.0f); ImGui::Unindent();
                ImGui::Text("Delay LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateDelay_" + fb_name).c_str(), &feedback_settings[index].delay_lfo.theta, .0f, 10.0f);
                ImGui::SliderFloat(("Amplitude##LFOAmpDelay_" + fb_name).c_str(), &feedback_settings[index].delay_lfo.amp, -1.0f, 1.0f); ImGui::Unindent();
                ImGui::TreePop();
            }
            ImGui::Spacing();
            if (ImGui::TreeNodeEx(("Color Space LFOs##LfoColor_" + fb_name).c_str())) {
                ImGui::Text("Hue LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateHue_" + fb_name).c_str(), &feedback_settings[index].hue_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpHue_" + fb_name).c_str(), &feedback_settings[index].hue_lfo.amp, -2.0f, 2.0f); ImGui::Unindent();
                ImGui::Text("Saturation LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateSat_" + fb_name).c_str(), &feedback_settings[index].saturation_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpSat_" + fb_name).c_str(), &feedback_settings[index].saturation_lfo.amp, -2.0f, 2.0f); ImGui::Unindent();
                ImGui::Text("Brightness LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateBr_" + fb_name).c_str(), &feedback_settings[index].bright_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpBr_" + fb_name).c_str(), &feedback_settings[index].bright_lfo.amp, -2.0f, 2.0f); ImGui::Unindent();
                ImGui::Text("HueX Mod LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateHMod_" + fb_name).c_str(), &feedback_settings[index].huexmod_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpHMod_" + fb_name).c_str(), &feedback_settings[index].huexmod_lfo.amp, -10.0f, 10.0f); ImGui::Unindent();
                ImGui::Text("HueX Offset LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateHOff_" + fb_name).c_str(), &feedback_settings[index].huexoffset_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpHOff_" + fb_name).c_str(), &feedback_settings[index].huexoffset_lfo.amp, -10.0f, 10.0f); ImGui::Unindent();
                ImGui::Text("HueX LFO (Self) LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateHLFO_" + fb_name).c_str(), &feedback_settings[index].huexlfo_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpHLFO_" + fb_name).c_str(), &feedback_settings[index].huexlfo_lfo.amp, -10.0f, 10.0f); ImGui::Unindent();
                ImGui::TreePop();
            }
            ImGui::Spacing();
            if (ImGui::TreeNodeEx(("Geometry LFOs##LfoGeom_" + fb_name).c_str())) {
                ImGui::Text("X Displace LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateX_" + fb_name).c_str(), &feedback_settings[index].x_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpX_" + fb_name).c_str(), &feedback_settings[index].x_lfo.amp, -feedback_settings[index].x_displace_range, feedback_settings[index].x_displace_range); ImGui::Unindent();
                ImGui::Text("Y Displace LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateY_" + fb_name).c_str(), &feedback_settings[index].y_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpY_" + fb_name).c_str(), &feedback_settings[index].y_lfo.amp, -feedback_settings[index].y_displace_range, feedback_settings[index].y_displace_range); ImGui::Unindent();
                ImGui::Text("Z Displace LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateZ_" + fb_name).c_str(), &feedback_settings[index].z_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpZ_" + fb_name).c_str(), &feedback_settings[index].z_lfo.amp, -feedback_settings[index].z_displace_range, feedback_settings[index].z_displace_range); ImGui::Unindent();
                ImGui::Text("Rotate LFO"); ImGui::Indent();
                ImGui::SliderFloat(("Rate##LFORateRot_" + fb_name).c_str(), &feedback_settings[index].rotate_lfo.theta, .0f, lfom);
                ImGui::SliderFloat(("Amplitude##LFOAmpRot_" + fb_name).c_str(), &feedback_settings[index].rotate_lfo.amp, -feedback_settings[index].rotate_range, feedback_settings[index].rotate_range); ImGui::Unindent();
                ImGui::TreePop();
            }
            ImGui::Unindent();
        }//end LFOs CollapsingHeader
        ImGui::PopItemWidth();
    }//end ImGui::Begin
    ImGui::End();
}

void GuiApp::drawGlobalControls(float lfom) {
    if (ImGui::Begin("global", nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f); // Standardized item width

        if (ImGui::CollapsingHeader("Global Controls##GlobalMain", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();

            if (ImGui::TreeNodeEx("General##GlobalGeneral", ImGuiTreeNodeFlags_DefaultOpen)) {
                // The framebuffer_clear and control_reset are set to false each frame in ofApp.cpp
                // This makes them act like momentary buttons.
                ImGui::Checkbox("Framebuffer Clear##GlobalFBClear", &global_settings.framebuffer_clear);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Clears all feedback buffers next frame.");
                ImGui::Checkbox("Control Reset##GlobalCtrlReset", &global_settings.control_reset);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Resets all controls to default values (not implemented yet).");
                ImGui::Spacing();
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx("Texture Modifiers##GlobalTexMods", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Checkbox(("Texture Mod Active##GlobalTexModActive" ), &global_settings.texmod);
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Enable global texture modification effects.");
                const char* items_global_texmod_src[] = { "cam1","cam2","ndi" };
                static int item_current_global_texmod_src = 0;
                ImGui::Combo(("TexMod Source##GlobalTexModSrcCombo" ), &item_current_global_texmod_src, items_global_texmod_src, IM_ARRAYSIZE(items_global_texmod_src));
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select source for global texture modification.");
                if(item_current_global_texmod_src==0){global_settings.texmod_select=0;}
                else if(item_current_global_texmod_src==1){global_settings.texmod_select=1;}
                else {global_settings.texmod_select=2;}
                ImGui::Spacing();

                if(global_settings.texmod){
                    ImGui::Indent();
                    ImGui::Text("TexMod Effects");
                    ImGui::SliderFloat(("Blur Amount##GlobalTexBlurAmt" ), &global_settings.texmod_blur_amount, -4.0f, 4.0f);
                    ImGui::SliderFloat(("Blur Radius##GlobalTexBlurRad" ), &global_settings.texmod_blur_radius, -2.0f, 2.0f);
                    ImGui::SliderFloat(("Sharpen Amount##GlobalTexSharpAmt" ), &global_settings.texmod_sharpen_amount, -.6f, .6f);
                    ImGui::SliderFloat(("Sharpen Radius##GlobalTexSharpRad" ), &global_settings.texmod_sharpen_radius, .0f, 10.f);
                    ImGui::SliderFloat(("Sharpen Boost##GlobalTexSharpBoost" ), &global_settings.texmod_sharpen_boost, -.4f, .4f);
                    ImGui::Unindent();
                }
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx("Post Processing##GlobalPostProc", ImGuiTreeNodeFlags_DefaultOpen)) {
                if(!global_settings.texmod) { // Only show if global texmod is NOT active
                    ImGui::Text("Main Effects (when TexMod is off)");
                    ImGui::SliderFloat(("Blur Amount##GlobalBlurAmt" ), &global_settings.blur_amount, -2.0f, 2.0f);
                    ImGui::SliderFloat(("Blur Radius##GlobalBlurRad" ), &global_settings.blur_radius, .0f, 10.0f);
                    ImGui::SliderFloat(("Sharpen Amount##GlobalSharpAmt" ), &global_settings.sharpen_amount, -.6f, .6f);
                    ImGui::SliderFloat(("Sharpen Radius##GlobalSharpRad" ), &global_settings.sharpen_radius, .0f, 10.f);
                    ImGui::SliderFloat(("Sharpen Boost##GlobalSharpBoost" ), &global_settings.sharpen_boost, .0f, 1.0f);
                } else {
                    ImGui::TextWrapped("Main post-processing effects are overridden by Global Texture Modifier when active.");
                }
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx("Camera & NDI Settings##GlobalCamNDI", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderFloat(("Cam 1 Scale##GlobalCam1Scale" ), &global_settings.cam1_scale, .0f, 2.0f);
                ImGui::Checkbox(("Cam 1 H-Flip##GlobalCam1HFlip" ), &global_settings.cam1_hflip_switch); ImGui::SameLine();
                ImGui::Checkbox(("Cam 1 V-Flip##GlobalCam1VFlip" ), &global_settings.cam1_vflip_switch);

                ImGui::SliderFloat(("Cam 2 Scale##GlobalCam2Scale" ), &global_settings.cam2_scale, .0f, 2.0f);
                ImGui::Checkbox(("Cam 2 H-Flip##GlobalCam2HFlip" ), &global_settings.cam2_hflip_switch); ImGui::SameLine();
                ImGui::Checkbox(("Cam 2 V-Flip##GlobalCam2VFlip" ), &global_settings.cam2_vflip_switch);
                
                ImGui::SliderFloat(("NDI Scale##GlobalNDIScale" ), &global_settings.ndi_scale, -1000.0f, 500.0f);
                 if (ImGui::IsItemHovered()) ImGui::SetTooltip("NDI scale is often used for Z-depth displacement.");
                ImGui::TreePop();
            }
            ImGui::Spacing();

            if (ImGui::TreeNodeEx("Skew & Geometric Shapes##GlobalSkewGeo", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderFloat(("X Skew##GlobalXSkew" ), &global_settings.x_skew, -3.14f, 3.14f);
                ImGui::SliderFloat(("Y Skew##GlobalYSkew" ), &global_settings.y_skew, -3.14f, 3.14f);
                ImGui::Checkbox(("Tetrahedron##GlobalTetra" ), &global_settings.tetrahedron_switch); ImGui::SameLine();
                ImGui::Checkbox(("Hypercube##GlobalHyper" ), &global_settings.hypercube_switch);
                if (global_settings.hypercube_switch) {
                    ImGui::Indent();
                    ImGui::SliderFloat(("Hypercube Theta Rate##GlobalHyperTheta" ), &global_settings.hypercube_theta_rate, -.1,.1);
                    ImGui::SliderFloat(("Hypercube Phi Rate##GlobalHyperPhi" ), &global_settings.hypercube_phi_rate, -.1,.1);
                    ImGui::Unindent();
                }
                ImGui::TreePop();
            }
            ImGui::Unindent();
        } // End Global Controls CollapsingHeader
        ImGui::Separator();

        if (ImGui::CollapsingHeader(("Global LFOs##GlobalLFOs" ), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            ImGui::Text("Blur Amount LFO"); ImGui::Indent();
            ImGui::SliderFloat(("Rate##GlobalBlurLFORate" ), &global_settings.blur_amount_lfo.theta, .0f, 20.0f);
            ImGui::SliderFloat(("Amplitude##GlobalBlurLFOAmp" ), &global_settings.blur_amount_lfo.amp, -10.0f, 10.0f); ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Blur Radius LFO"); ImGui::Indent();
            ImGui::SliderFloat(("Rate##GlobalBlurRadLFORate" ), &global_settings.blur_radius_lfo.theta, .0f, 20.0f);
            ImGui::SliderFloat(("Amplitude##GlobalBlurRadLFOAmp" ), &global_settings.blur_radius_lfo.amp, -10.0f, 10.0f); ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Sharpen Amount LFO"); ImGui::Indent();
            ImGui::SliderFloat(("Rate##GlobalSharpAmtLFORate" ), &global_settings.sharpen_amount_lfo.theta, .0f, 20.0f);
            ImGui::SliderFloat(("Amplitude##GlobalSharpAmtLFOAmp" ), &global_settings.sharpen_amount_lfo.amp, -.6f, .6f); ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Sharpen Radius LFO"); ImGui::Indent();
            ImGui::SliderFloat(("Rate##GlobalSharpRadLFORate" ), &global_settings.sharpen_radius_lfo.theta, .0f, 20.0f);
            ImGui::SliderFloat(("Amplitude##GlobalSharpRadLFOAmp" ), &global_settings.sharpen_radius_lfo.amp, -10.0f, 10.f); ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Sharpen Boost LFO"); ImGui::Indent();
            ImGui::SliderFloat(("Rate##GlobalSharpBoostLFORate" ), &global_settings.sharpen_boost_lfo.theta, .0f, 20.0f);
            ImGui::SliderFloat(("Amplitude##GlobalSharpBoostLFOAmp" ), &global_settings.sharpen_boost_lfo.amp, -1.0f, 1.0f); ImGui::Unindent();
            ImGui::Unindent();
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

// Forward declaration for pixelate LFOs, assuming they will also be refactored if they exist
// For now, direct access or specific LFOSettings members will be used if defined in GuiApp.h

void GuiApp::drawPixelateControls(float lfom, const std::string& name) { // Signature updated
    if (ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_None)) { // Changed to ImGui::Begin
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);

        if (ImGui::CollapsingHeader(("Pixelate Settings##" + name).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            // Generic handling based on name, assuming variables like cam1_pixel_switch, fb0_pixel_scale_x etc.
            // This will require careful mapping or a more robust way to access these variables.
            // For this step, I will hardcode based on the names provided in the original file.

            if (name == "cam1_pixelate") {
                ImGui::Checkbox("pixelate", &cam1_pixel_switch);
                ImGui::SliderInt("pixel scale_x", &cam1_pixel_scale_x, 0, 256);
                ImGui::SliderInt("pixel scale_y", &cam1_pixel_scale_y, 0, 256);
                ImGui::SliderFloat("pixel mix", &cam1_pixel_mix, -2.0f, 2.0f);
                ImGui::SliderFloat("bright_scale", &cam1_pixel_brightscale, -2.0f, 2.0f);
            } else if (name == "cam2_pixelate") {
                ImGui::Checkbox("pixelate", &cam2_pixel_switch);
                ImGui::SliderInt("pixel scale_x", &cam2_pixel_scale_x, 0, 256);
                ImGui::SliderInt("pixel scale_y", &cam2_pixel_scale_y, 0, 256);
                ImGui::SliderFloat("pixel mix", &cam2_pixel_mix, -2.0f, 2.0f);
                ImGui::SliderFloat("bright_scale", &cam2_pixel_brightscale, -2.0f, 2.0f);
            } else if (name == "fb0_pixelate") {
                ImGui::Checkbox("tex_mod", &fb0_pixel_texmod);
                const char* items[] = { "channel1", "channel2"};
                static int item_current = 0; ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
                if(item_current==0){fb0_pixel_texmod_select=0;} else {fb0_pixel_texmod_select=1;}
                ImGui::Checkbox("pixelate", &fb0_pixel_switch);
                if(!fb0_pixel_texmod){
                    ImGui::SliderInt("pixel scale x", &fb0_pixel_scale_x, 0, 256);
                    ImGui::SliderInt("pixel scale y", &fb0_pixel_scale_y, 0, 256);
                    ImGui::SliderFloat("pixel mix", &fb0_pixel_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat("bright_scale", &fb0_pixel_brightscale, -2.0f, 2.0f);
                } else {
                    ImGui::SliderInt("texmod pixel scale x", &texmod_fb0_pixel_scale_x, -256, 256);
                    ImGui::SliderInt("texmod pixel scale y", &texmod_fb0_pixel_scale_y, -256, 256);
                    ImGui::SliderFloat("texmod pixel mix", &texmod_fb0_pixel_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat("texmod bright_scale", &texmod_fb0_pixel_brightscale, -2.0f, 2.0f);
                }
            } else if (name == "fb1_pixelate") {
                ImGui::Checkbox("tex_mod", &fb1_pixel_texmod);
                const char* items[] = { "channel1", "channel2"};
                static int item_current = 0; ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
                if(item_current==0){fb1_pixel_texmod_select=0;} else {fb1_pixel_texmod_select=1;}
                ImGui::Checkbox("pixelate", &fb1_pixel_switch);
                if(!fb1_pixel_texmod){
                    ImGui::SliderInt("pixel scale x", &fb1_pixel_scale_x, 0, 256);
                    ImGui::SliderInt("pixel scale y", &fb1_pixel_scale_y, 0, 256);
                    ImGui::SliderFloat("pixel mix", &fb1_pixel_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat("bright_scale", &fb1_pixel_brightscale, -2.0f, 2.0f);
                } else {
                    ImGui::SliderInt("texmod pixel scale x", &texmod_fb1_pixel_scale_x, -256, 256);
                    ImGui::SliderInt("texmod pixel scale y", &texmod_fb1_pixel_scale_y, -256, 256);
                    ImGui::SliderFloat("texmod pixel mix", &texmod_fb1_pixel_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat("texmod bright_scale", &texmod_fb1_pixel_brightscale, -2.0f, 2.0f);
                }
            } else if (name == "fb2_pixelate") {
                ImGui::Checkbox("tex_mod", &fb2_pixel_texmod);
                const char* items[] = { "channel1", "channel2"};
                static int item_current = 0; ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
                if(item_current==0){fb2_pixel_texmod_select=0;} else {fb2_pixel_texmod_select=1;}
                ImGui::Checkbox("pixelate", &fb2_pixel_switch);
                if(!fb2_pixel_texmod){
                    ImGui::SliderInt("pixel scale x", &fb2_pixel_scale_x, 0, 256);
                    ImGui::SliderInt("pixel scale y", &fb2_pixel_scale_y, 0, 256);
                    ImGui::SliderFloat("pixel mix", &fb2_pixel_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat("bright_scale", &fb2_pixel_brightscale, -2.0f, 2.0f);
                } else {
                    ImGui::SliderInt("texmod pixel scale x", &texmod_fb2_pixel_scale_x, -256, 256);
                    ImGui::SliderInt("texmod pixel scale y", &texmod_fb2_pixel_scale_y, -256, 256);
                    ImGui::SliderFloat("texmod pixel mix", &texmod_fb2_pixel_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat("texmod bright_scale", &texmod_fb2_pixel_brightscale, -2.0f, 2.0f);
                }
            } else if (name == "fb3_pixelate") {
                ImGui::Checkbox("tex_mod", &fb3_pixel_texmod);
                const char* items[] = { "channel1", "channel2"};
                static int item_current = 0; ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
                if(item_current==0){fb3_pixel_texmod_select=0;} else {fb3_pixel_texmod_select=1;}
                ImGui::Checkbox("pixelate", &fb3_pixel_switch);
                if(!fb3_pixel_texmod){
                    ImGui::SliderInt("pixel scale x", &fb3_pixel_scale_x, 0, 256);
                    ImGui::SliderInt("pixel scale y", &fb3_pixel_scale_y, 0, 256);
                    ImGui::SliderFloat("pixel mix", &fb3_pixel_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat("bright_scale", &fb3_pixel_brightscale, -2.0f, 2.0f);
                } else {
                    ImGui::SliderInt("texmod pixel scale x", &texmod_fb3_pixel_scale_x, -256, 256);
                    ImGui::SliderInt("texmod pixel scale y", &texmod_fb3_pixel_scale_y, -256, 256);
                    ImGui::SliderFloat("texmod pixel mix", &texmod_fb3_pixel_mix, -2.0f, 2.0f);
                    ImGui::SliderFloat("texmod bright_scale", &texmod_fb3_pixel_brightscale, -2.0f, 2.0f);
                }
            } else if (name == "ndi_pixelate") {
                 ImGui::Checkbox("pixelate", &ndi_pixel_switch);
                 ImGui::SliderInt("pixel scale_x", &ndi_pixel_scale_x, 0, 256);
                 ImGui::SliderInt("pixel scale_y", &ndi_pixel_scale_y, 0, 256);
                 ImGui::SliderFloat("pixel mix", &ndi_pixel_mix, -2.0f, 2.0f);
                 ImGui::SliderFloat("bright_scale", &ndi_pixel_brightscale, -2.0f, 2.0f);
            }
        }

        if (ImGui::CollapsingHeader("L F O s")) {
            ImGui::Text("Pixel LFOs"); // Placeholder text
            // Assuming LFO variables like cam1_pixel_scale_x_lfo_amp, etc.
            // This section will also need to be generalized or use a map/struct access
            if (name == "cam1_pixelate") {
                ImGui::SliderFloat("pixel scale_x_amp", &cam1_pixel_scale_x_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_x_rate", &cam1_pixel_scale_x_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel scale_y_amp", &cam1_pixel_scale_y_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_y_rate", &cam1_pixel_scale_y_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel mix_amp", &cam1_pixel_mix_lfo_amp, -20.0f, 20.0f);
                ImGui::SliderFloat("pixel mix_rate", &cam1_pixel_mix_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel bright scale amp", &cam1_pixel_brightscale_lfo_amp, -2.0f, 2.0f);
                ImGui::SliderFloat("pixel bright scale rate", &cam1_pixel_brightscale_lfo_theta, .0f, 20.0f);
            } else if (name == "cam2_pixelate") {
                ImGui::SliderFloat("pixel scale_x_amp", &cam2_pixel_scale_x_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_x_rate", &cam2_pixel_scale_x_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel scale_y_amp", &cam2_pixel_scale_y_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_y_rate", &cam2_pixel_scale_y_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel mix_amp", &cam2_pixel_mix_lfo_amp, -256.0f, 256.0f); // Original had -256 to 256
                ImGui::SliderFloat("pixel mix_rate", &cam2_pixel_mix_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel bright scale amp", &cam2_pixel_brightscale_lfo_amp, -2.0f, 2.0f); // Original had -256 to 256
                ImGui::SliderFloat("pixel bright scale rate", &cam2_pixel_brightscale_lfo_theta, .0f, 20.0f);
            } else if (name == "fb0_pixelate") {
                ImGui::SliderFloat("pixel scale_x_amp", &fb0_pixel_scale_x_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_x_rate", &fb0_pixel_scale_x_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel scale_y_amp", &fb0_pixel_scale_y_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_y_rate", &fb0_pixel_scale_y_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel mix_amp", &fb0_pixel_mix_lfo_amp, -20.0f, 20.0f);
                ImGui::SliderFloat("pixel mix_rate", &fb0_pixel_mix_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel bright scale amp", &fb0_pixel_brightscale_lfo_amp, -2.0f, 2.0f);
                ImGui::SliderFloat("pixel bright scale rate", &fb0_pixel_brightscale_lfo_theta, .0f, 20.0f);
            } else if (name == "fb1_pixelate") {
                ImGui::SliderFloat("pixel scale_x_amp", &fb1_pixel_scale_x_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_x_rate", &fb1_pixel_scale_x_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel scale_y_amp", &fb1_pixel_scale_y_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_y_rate", &fb1_pixel_scale_y_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel mix_amp", &fb1_pixel_mix_lfo_amp, -20.0f, 20.0f);
                ImGui::SliderFloat("pixel mix_rate", &fb1_pixel_mix_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel bright scale amp", &fb1_pixel_brightscale_lfo_amp, -2.0f, 2.0f);
                ImGui::SliderFloat("pixel bright scale rate", &fb1_pixel_brightscale_lfo_theta, .0f, 20.0f);
            } else if (name == "fb2_pixelate") {
                ImGui::SliderFloat("pixel scale_x_amp", &fb2_pixel_scale_x_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_x_rate", &fb2_pixel_scale_x_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel scale_y_amp", &fb2_pixel_scale_y_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_y_rate", &fb2_pixel_scale_y_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel mix_amp", &fb2_pixel_mix_lfo_amp, -20.0f, 20.0f);
                ImGui::SliderFloat("pixel mix_rate", &fb2_pixel_mix_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel bright scale amp", &fb2_pixel_brightscale_lfo_amp, -2.0f, 2.0f);
                ImGui::SliderFloat("pixel bright scale rate", &fb2_pixel_brightscale_lfo_theta, .0f, 20.0f);
            } else if (name == "fb3_pixelate") {
                ImGui::SliderFloat("pixel scale_x_amp", &fb3_pixel_scale_x_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_x_rate", &fb3_pixel_scale_x_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel scale_y_amp", &fb3_pixel_scale_y_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_y_rate", &fb3_pixel_scale_y_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel mix_amp", &fb3_pixel_mix_lfo_amp, -20.0f, 20.0f);
                ImGui::SliderFloat("pixel mix_rate", &fb3_pixel_mix_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel bright scale amp", &fb3_pixel_brightscale_lfo_amp, -2.0f, 2.0f);
                ImGui::SliderFloat("pixel bright scale rate", &fb3_pixel_brightscale_lfo_theta, .0f, 20.0f);
            } else if (name == "ndi_pixelate") {
                ImGui::SliderFloat("pixel scale_x_amp", &ndi_pixel_scale_x_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_x_rate", &ndi_pixel_scale_x_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel scale_y_amp", &ndi_pixel_scale_y_lfo_amp, -256.0f, 256.0f);
                ImGui::SliderFloat("pixel scale_y_rate", &ndi_pixel_scale_y_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel mix_amp", &ndi_pixel_mix_lfo_amp, -20.0f, 20.0f);
                ImGui::SliderFloat("pixel mix_rate", &ndi_pixel_mix_lfo_theta, .0f, 20.0f);
                ImGui::SliderFloat("pixel bright scale amp", &ndi_pixel_brightscale_lfo_amp, -2.0f, 2.0f);
                ImGui::SliderFloat("pixel bright scale rate", &ndi_pixel_brightscale_lfo_theta, .0f, 20.0f);
            }
        }
    }
    ofxImGui::EndWindow(settings);
}

         mainSettings.windowPos=ImVec2(ImVec2(1*gui_hscaler, 0*gui_vscaler));
        drawPixelateControls(mainSettings, lfom, "cam1_pixelate", 0, gui_hscaler, gui_vscaler, 0);
        drawPixelateControls(mainSettings, lfom, "cam2_pixelate", 1, gui_hscaler, gui_vscaler, 0);
        drawPixelateControls(mainSettings, lfom, "fb0_pixelate", 0, gui_hscaler, gui_vscaler, 1);
        drawPixelateControls(mainSettings, lfom, "fb1_pixelate", 1, gui_hscaler, gui_vscaler, 1);
        drawPixelateControls(mainSettings, lfom, "fb2_pixelate", 2, gui_hscaler, gui_vscaler, 1);
        drawPixelateControls(mainSettings, lfom, "fb3_pixelate", 3, gui_hscaler, gui_vscaler, 1);
        drawPixelateControls(mainSettings, lfom, "ndi_pixelate", 0, gui_hscaler, gui_vscaler, 2); // id=0 for ndi as it's singular
       
        
        gui.end();
        
    }


//---------------------------




