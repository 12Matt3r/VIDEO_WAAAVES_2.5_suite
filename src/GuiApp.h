#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include <string>
#include <vector>

class GuiApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void drawOutputWindow(ofTexture* output_texture);

    ofxImGui::Gui imgui_instance;

    struct LFOSettings {
        float amp {0.0f};
        float theta {0.0f};
    };

    struct ChannelSettings {
        int select {1};
        float bright {1.0f};
        float hue {1.0f};
        float saturation {1.0f};
        bool hue_alt_invert_toggle {false};
        bool saturation_alt_invert_toggle {false};
        bool bright_alt_invert_toggle {false};
        bool saturation_wrap {false};
        bool bright_wrap {false};
        float bright_powmap {1.0f};
        float hue_powmap {1.0f};
        float saturation_powmap {1.0f};
        LFOSettings hue_lfo;
        LFOSettings saturation_lfo;
        LFOSettings bright_lfo;
        float mix_value {0.0f};
        float key_value {0.0f};
    };
    ChannelSettings channel_settings[2];

    struct FeedbackSettings {
        float key_value {0.0f};
        float key_threshold {0.0f};
        int delay_amount {0};
        float mix {0.0f};
        bool hflip_switch {false};
        bool vflip_switch {false};
        int toroid_switch {0};
        float hue {10.0f};
        float saturation {10.0f};
        float bright {10.0f};
        bool hue_invert {false};
        bool saturation_invert {false};
        bool bright_invert {false};
        float huex_mod {10.0f};
        float huex_offset {0.0f};
        float huex_lfo_val {0.0f};
        float x_displace {0.0f};
        float y_displace {0.0f};
        float z_displace {100.0f};
        float rotate {0.0f};

        float x_displace_range {40.0f};
        float y_displace_range {40.0f};
        float z_displace_range {10.0f};
        float rotate_range {PI};

        int texmod_select {1};
        bool tex_mod_enabled {false};
        float tex_key_value {0.0f}; // tex_fb_key_value
        float tex_key_threshold {0.0f}; // tex_fb_key_threshold
        float tex_mix {0.0f}; // tex_fb_mix
        float tex_hue {0.0f}; // tex_fb_hue
        float tex_saturation {0.0f}; // tex_fb_saturation
        float tex_bright {0.0f}; // tex_fb_bright
        float tex_huex_mod {0.0f}; // tex_fb_huex_mod
        float tex_huex_offset {0.0f}; // tex_fb_huex_offset
        float tex_huex_lfo_val {0.0f}; // tex_fb_huex_lfo_val
        float tex_x_displace {0.0f}; // tex_fb_x_displace
        float tex_y_displace {0.0f}; // tex_fb_y_displace
        float tex_z_displace {0.0f}; // tex_fb_z_displace
        float tex_rotate {0.0f}; // tex_fb_rotate

        LFOSettings lumakeyvalue_lfo; LFOSettings lumakeythreshold_lfo;
        LFOSettings x_lfo; LFOSettings y_lfo; LFOSettings z_lfo; LFOSettings rotate_lfo;
        LFOSettings mix_lfo; LFOSettings delay_lfo;
        LFOSettings hue_lfo; LFOSettings saturation_lfo; LFOSettings bright_lfo;
        LFOSettings huexmod_lfo; LFOSettings huexoffset_lfo; LFOSettings huexlfo_val_lfo; // Matches huex_lfo_val

        bool pixel_switch {false};
        int pixel_scale_x {64}; int pixel_scale_y {64};
        float pixel_mix {1.0f}; float pixel_brightscale {0.0f};
        int pixel_texmod_select {0};
        bool pixel_texmod_enabled {false};
        int texmod_pixel_scale_x {0}; int texmod_pixel_scale_y {0};
        float texmod_pixel_mix {0.0f}; float texmod_pixel_brightscale {0.0f};
        LFOSettings pixel_scale_x_lfo; LFOSettings pixel_scale_y_lfo;
        LFOSettings pixel_mix_lfo; LFOSettings pixel_brightscale_lfo;
    };
    FeedbackSettings feedback_settings[4];

    struct GlobalSettings {
        int global_texmod_select {0};
        bool texmod_enabled {false};
        bool control_reset_trigger {false};
        bool framebuffer_clear_trigger {false};
        float blur_amount {0.0f}; float blur_radius {1.0f};
        float sharpen_amount {0.0f}; float sharpen_radius {1.0f}; float sharpen_boost {0.0f};
        float cam1_scale {1.0f}; float cam2_scale {1.0f}; float ndi_scale {0.0f};
        bool cam1_hflip_switch {false}; bool cam1_vflip_switch {false};
        bool cam2_hflip_switch {false}; bool cam2_vflip_switch {false};
        float x_skew {0.0f}; float y_skew {0.0f};
        bool tetrahedron_switch {false}; bool hypercube_switch {false};
        float hypercube_theta_rate {0.01f}; float hypercube_phi_rate {0.01f};
        bool ndiOutput_enabled {false};

        float texmod_blur_amount {0.0f}; float texmod_blur_radius {1.0f};
        float texmod_sharpen_amount {0.0f}; float texmod_sharpen_radius {1.0f}; float texmod_sharpen_boost {0.0f};

        LFOSettings blur_amount_lfo; LFOSettings blur_radius_lfo;
        LFOSettings sharpen_amount_lfo; LFOSettings sharpen_radius_lfo; LFOSettings sharpen_boost_lfo;

        bool cam1_pixel_switch {false}; int cam1_pixel_scale_x {64}; int cam1_pixel_scale_y {64}; float cam1_pixel_mix {1.0f}; float cam1_pixel_brightscale {0.0f};
        LFOSettings cam1_pixel_scale_x_lfo; LFOSettings cam1_pixel_scale_y_lfo; LFOSettings cam1_pixel_mix_lfo; LFOSettings cam1_pixel_brightscale_lfo;

        bool cam2_pixel_switch {false}; int cam2_pixel_scale_x {64}; int cam2_pixel_scale_y {64}; float cam2_pixel_mix {1.0f}; float cam2_pixel_brightscale {0.0f};
        LFOSettings cam2_pixel_scale_x_lfo; LFOSettings cam2_pixel_scale_y_lfo; LFOSettings cam2_pixel_mix_lfo; LFOSettings cam2_pixel_brightscale_lfo;

        bool ndi_pixel_switch {false}; int ndi_pixel_scale_x {64}; int ndi_pixel_scale_y {64}; float ndi_pixel_mix {1.0f}; float ndi_pixel_brightscale {0.0f};
        LFOSettings ndi_pixel_scale_x_lfo; LFOSettings ndi_pixel_scale_y_lfo; LFOSettings ndi_pixel_mix_lfo; LFOSettings ndi_pixel_brightscale_lfo;
    };
    GlobalSettings global_settings;

    std::string loadedVideoPath {"No video loaded"};
    bool videoPlayingState {false};
    bool videoLoopingState {true};
    bool loadVideoTrigger {false}; bool playVideoTrigger {false}; bool pauseVideoTrigger {false}; bool stopVideoTrigger {false}; bool loopVideoToggleTrigger {false};

    std::vector<std::string> availableWindowTitles;
    int selectedWindowIndex {-1};
    bool isWindowCurrentlyCapturing {false};
    std::string capturedWindowTitleDisplay {"None"};
    bool listWindowsTrigger {false}; bool startCaptureTrigger {false}; bool stopCaptureTrigger {false};

    // Updated draw function declarations
    void drawChannelControls(int channel_idx, float lfom);
    void drawFeedbackControls(int fb_idx, float lfom);
    void drawGlobalControls(float lfom);
    void drawPixelateControls(const std::string& panel_name_id, float lfom);
    void drawVideoControls();
    void drawWindowCaptureControls();
    void drawMoreEffectsControls();
};
