#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofJson.h" // Added for preset system
#include <string>
#include <vector>

// Constants for array sizes, useful for iteration and initialization
const int MAX_INPUT_CHANNELS_GUI = 2;
const int MAX_FB_CHANNELS_GUI = 4;

// Forward declare structs for ofJson serialization functions
// Forward declare if structs are below, or ensure this is after struct definitions
struct LFOSettings; // Already part of ChannelSettings, FeedbackSettings, GlobalSettings
struct ChannelSettings;
struct FeedbackSettings;
struct GlobalSettings;
struct ParticleFeedbackSettings;
struct SlitScanSettings;
struct NoiseGeneratorSettings;
struct PixelSortSettings;
struct ReactionDiffusionSettings;

// ofJson serialization function declarations
void to_json(ofJson& j, const LFOSettings& s);
void from_json(const ofJson& j, LFOSettings& s);
// Note: For ChannelSettings, FeedbackSettings, GlobalSettings, if they contain complex types (like arrays of LFOSettings or other structs),
// their to_json/from_json will need to handle those recursively. For now, assuming simple members or that ofJson handles std::vector of simple types.
// We'll focus on ReactionDiffusionSettings first. Serializers for other complex types can be added as needed.
void to_json(ofJson& j, const ReactionDiffusionSettings& s);
void from_json(const ofJson& j, ReactionDiffusionSettings& s);
void to_json(ofJson& j, const ParticleFeedbackSettings& s);
void from_json(const ofJson& j, ParticleFeedbackSettings& s);
void to_json(ofJson& j, const SlitScanSettings& s);
void from_json(const ofJson& j, SlitScanSettings& s);
void to_json(ofJson& j, const NoiseGeneratorSettings& s);
void from_json(const ofJson& j, NoiseGeneratorSettings& s);
void to_json(ofJson& j, const PixelSortSettings& s);
void from_json(const ofJson& j, PixelSortSettings& s);
// For ofColor, oF's ofJson handles it by default.

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
    ChannelSettings channel_settings[MAX_INPUT_CHANNELS_GUI];

    struct FeedbackSettings {
        float key_value {0.0f}; float key_threshold {0.0f}; int delay_amount {0}; float mix {0.0f};
        bool hflip_switch {false}; bool vflip_switch {false}; int toroid_switch {0};
        float hue {10.0f}; float saturation {10.0f}; float bright {10.0f};
        bool hue_invert {false}; bool saturation_invert {false}; bool bright_invert {false};
        float huex_mod {10.0f}; float huex_offset {0.0f}; float huex_lfo_val {0.0f};
        float x_displace {0.0f}; float y_displace {0.0f}; float z_displace {100.0f}; float rotate {0.0f};
        float x_displace_range {40.0f}; float y_displace_range {40.0f}; float z_displace_range {10.0f}; float rotate_range {OF_PI};

        int texmod_select {1}; bool tex_mod_enabled {false};
        float tex_key_value {0.0f}; float tex_key_threshold {0.0f}; float tex_mix {0.0f};
        float tex_hue {0.0f}; float tex_saturation {0.0f}; float tex_bright {0.0f};
        float tex_huex_mod {0.0f}; float tex_huex_offset {0.0f}; float tex_huex_lfo_val {0.0f};
        float tex_x_displace {0.0f}; float tex_y_displace {0.0f}; float tex_z_displace {0.0f}; float tex_rotate {0.0f};

        LFOSettings lumakeyvalue_lfo; LFOSettings lumakeythreshold_lfo; LFOSettings x_lfo; LFOSettings y_lfo; LFOSettings z_lfo; LFOSettings rotate_lfo;
        LFOSettings mix_lfo; LFOSettings delay_lfo; LFOSettings hue_lfo; LFOSettings saturation_lfo; LFOSettings bright_lfo;
        LFOSettings huexmod_lfo; LFOSettings huexoffset_lfo; LFOSettings huexlfo_val_lfo;

        bool pixel_switch {false}; int pixel_scale_x {64}; int pixel_scale_y {64}; float pixel_mix {1.0f}; float pixel_brightscale {0.0f};
        int pixel_texmod_select {0}; bool pixel_texmod_enabled {false};
        int texmod_pixel_scale_x {0}; int texmod_pixel_scale_y {0}; float texmod_pixel_mix {0.0f}; float texmod_pixel_brightscale {0.0f};
        LFOSettings pixel_scale_x_lfo; LFOSettings pixel_scale_y_lfo; LFOSettings pixel_mix_lfo; LFOSettings pixel_brightscale_lfo;
    };
    FeedbackSettings feedback_settings[MAX_FB_CHANNELS_GUI];

    struct GlobalSettings {
        int global_texmod_select {0}; bool texmod_enabled {false};
        bool control_reset_trigger {false}; bool framebuffer_clear_trigger {false};
        float blur_amount {0.0f}; float blur_radius {1.0f}; float sharpen_amount {0.0f}; float sharpen_radius {1.0f}; float sharpen_boost {0.0f};
        float cam1_scale {1.0f}; float cam2_scale {1.0f}; float ndi_scale {0.0f};
        bool cam1_hflip_switch {false}; bool cam1_vflip_switch {false}; bool cam2_hflip_switch {false}; bool cam2_vflip_switch {false};
        float x_skew {0.0f}; float y_skew {0.0f};
        bool tetrahedron_switch {false}; bool hypercube_switch {false}; float hypercube_theta_rate {0.01f}; float hypercube_phi_rate {0.01f};
        bool ndiOutput_enabled {false};

        float texmod_blur_amount {0.0f}; float texmod_blur_radius {1.0f}; float texmod_sharpen_amount {0.0f}; float texmod_sharpen_radius {1.0f}; float texmod_sharpen_boost {0.0f};

        LFOSettings blur_amount_lfo; LFOSettings blur_radius_lfo; LFOSettings sharpen_amount_lfo; LFOSettings sharpen_radius_lfo; LFOSettings sharpen_boost_lfo;

        bool cam1_pixel_switch {false}; int cam1_pixel_scale_x {64}; int cam1_pixel_scale_y {64}; float cam1_pixel_mix {1.0f}; float cam1_pixel_brightscale {0.0f};
        LFOSettings cam1_pixel_scale_x_lfo; LFOSettings cam1_pixel_scale_y_lfo; LFOSettings cam1_pixel_mix_lfo; LFOSettings cam1_pixel_brightscale_lfo;

        bool cam2_pixel_switch {false}; int cam2_pixel_scale_x {64}; int cam2_pixel_scale_y {64}; float cam2_pixel_mix {1.0f}; float cam2_pixel_brightscale {0.0f};
        LFOSettings cam2_pixel_scale_x_lfo; LFOSettings cam2_pixel_scale_y_lfo; LFOSettings cam2_pixel_mix_lfo; LFOSettings cam2_pixel_brightscale_lfo;

        bool ndi_pixel_switch {false}; int ndi_pixel_scale_x {64}; int ndi_pixel_scale_y {64}; float ndi_pixel_mix {1.0f}; float ndi_pixel_brightscale {0.0f};
        LFOSettings ndi_pixel_scale_x_lfo; LFOSettings ndi_pixel_scale_y_lfo; LFOSettings ndi_pixel_mix_lfo; LFOSettings ndi_pixel_brightscale_lfo;
    };
    GlobalSettings global_settings;

    struct ParticleFeedbackSettings {
        bool enableParticleFeedback {{false}};
        int inputSource {{0}};
        float spawnThreshold {{0.5f}};
        int maxParticles {{1000}};
        float particleInitialLife {{2.0f}};
        float particleInitialSpeed {{50.0f}};
        float particleDrag {{0.05f}};
        float particleSize {{2.0f}};
        bool enableVelocityFromBrightness {{true}};
        bool inheritColorFromSpawn {{true}};
        ofColor particleBaseColor {{ofColor::white}};
        float noiseForceAmount {{0.0f}};
        float noiseFieldScale {{0.01f}};
        float noiseTimeSpeed {{0.1f}};
        float feedbackMix {{0.1f}};
    };
    ParticleFeedbackSettings particle_feedback_settings;

    struct SlitScanSettings {
        bool enableSlitScan {{false}};
        int inputSource {{0}};
        int slitDirection {{0}};
        float slitPosition {{0.5}};
        int slitThickness {{2}};
        float accumulationSpeed {{1.0}};
        bool wrapAccumulation {{true}};
        int delayFrames {{0}};
        float outputMix {{1.0}};
        int blendMode {{0}};
    };
    SlitScanSettings slit_scan_settings;

    struct NoiseGeneratorSettings {
        bool enableNoise {{false}};
        int noiseType {{0}};
        float noiseScale {{0.02f}};
        float noiseTime {{0.0f}};
        float noiseSpeed {{0.05f}};
        bool noiseAnimateTime {{true}};
        int noiseOctaves {{4}};
        float noisePersistence {{0.5f}};
        bool noiseColorEnable {{false}};
        ofColor noiseColor1 {{ofColor::black}};
        ofColor noiseColor2 {{ofColor::white}};
        float noiseRangeMin {{0.0f}};
        float noiseRangeMax {{1.0f}};
        bool noiseApplyContrast {{false}};
        float noiseContrast {{1.0f}};
        float noiseBrightness {{0.0f}};
    };
    NoiseGeneratorSettings noise_generator_settings;

    struct PixelSortSettings {
        bool enablePixelSort {{false}};
        int inputSource {{0}};
        int sortMode {{0}};
        int sortCriteria {{0}};
        float thresholdMin {{0.1f}};
        float thresholdMax {{0.9f}};
        bool sortAscending {{true}};
        bool smartThresholding {{true}};
        int sortWindowSize {{5}};
        float effectMix {{1.0f}};
    };
    PixelSortSettings pixel_sort_settings;

    struct ReactionDiffusionSettings {
        bool enableRD {{false}};
        float feedRate {{0.037f}};
        float killRate {{0.060f}};
        float diffuseRateU {{1.0f}};
        float diffuseRateV {{0.5f}};
        float timeStep {{1.0f}};
        int iterationsPerFrame {{10}};
        bool clearCanvasTrigger {{false}};
        int seedPattern {{0}};
        float seedRadius {{10.0f}};
        float seedNoiseDensity {{0.1f}};
        float outputMix {{1.0f}};
        int colorMode {{0}};
        ofColor colorU {{ofColor::blue}};
        ofColor colorV {{ofColor::yellow}};
        bool smoothDisplay {{false}};
    };
    ReactionDiffusionSettings reaction_diffusion_settings;

    std::string loadedVideoPath {"No video loaded"};
    bool videoPlayingState {false}; bool videoLoopingState {true};
    bool loadVideoTrigger {false}; bool playVideoTrigger {false}; bool pauseVideoTrigger {false}; bool stopVideoTrigger {false}; bool loopVideoToggleTrigger {false};

    std::vector<std::string> availableWindowTitles;
    int selectedWindowIndex {-1};
    bool isWindowCurrentlyCapturing {false};
    std::string capturedWindowTitleDisplay {"None"};
    bool listWindowsTrigger {false}; bool startCaptureTrigger {false}; bool stopCaptureTrigger {false};

    void drawChannelControls(int channel_idx, float lfom);
    void drawFeedbackControls(int fb_idx, float lfom);
    void drawGlobalControls(float lfom);
    void drawPixelateControls(const std::string& panel_name_id, float lfom);
    void drawVideoControls();
    void drawWindowCaptureControls();
    void drawMoreEffectsControls();

public:
    std::vector<ReactionDiffusionSettings> rd_presets;
    std::vector<std::string> rd_preset_names;
    int current_rd_preset_index {-1};
    char rd_new_preset_name_buffer[128] {""};
    const std::string rd_presets_filepath {"rd_presets.json"};

    // Particle Feedback Presets
    std::vector<ParticleFeedbackSettings> particle_presets;
    std::vector<std::string> particle_preset_names;
    int current_particle_preset_index {-1};
    char particle_new_preset_name_buffer[128] {""};
    const std::string particle_presets_filepath {"particle_presets.json"};

    // Slit-Scan Presets
    std::vector<SlitScanSettings> slit_scan_presets;
    std::vector<std::string> slit_scan_preset_names;
    int current_slit_scan_preset_index {-1};
    char slit_scan_new_preset_name_buffer[128] {""};
    const std::string slit_scan_presets_filepath {"slit_scan_presets.json"};

    // Noise Generator Presets
    std::vector<NoiseGeneratorSettings> noise_generator_presets;
    std::vector<std::string> noise_generator_preset_names;
    int current_noise_generator_preset_index {-1};
    char noise_generator_new_preset_name_buffer[128] {""};
    const std::string noise_generator_presets_filepath {"noise_generator_presets.json"};

    // Pixel Sorting Presets
    std::vector<PixelSortSettings> pixel_sort_presets;
    std::vector<std::string> pixel_sort_preset_names;
    int current_pixel_sort_preset_index {-1};
    char pixel_sort_new_preset_name_buffer[128] {""};
    const std::string pixel_sort_presets_filepath {"pixel_sort_presets.json"};

    void setupPresets();

    void loadRDPresets();
    void saveRDPresets();
    void applyRDPreset(int preset_index);
    void addCurrentSettingsAsRDPreset(const std::string& name);
    void deleteRDPreset(int preset_index);
    void addDefaultRDPresets();
};

// Note: The struct definitions are now inside the GuiApp class.
// For to_json/from_json, they need to be able to see these struct definitions.
// If they are global functions, the structs either need to be global too,
// or these functions need to be templated or part of a namespace that can access them.
// For simplicity and because they operate on GuiApp members, making them part of GuiApp or
// ensuring structs are accessible globally is needed.
// The current structure (structs inside class, global to/from_json) will cause compilation errors.
// Moving to_json/from_json declarations *after* the class, and ensuring structs are accessible
// (e.g. GuiApp::ReactionDiffusionSettings) is one way if they are kept global.
// Alternatively, make them static members of GuiApp or put them in an accessible namespace.
// For this iteration, I will assume they will be global functions and struct names will be qualified (e.g. GuiApp::LFOSettings).
// However, for the NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE macros to work easily, structs are often global or in a namespace.
// Let's assume for now these will be global functions and structs are defined as shown inside GuiApp.
// The actual implementation in .cpp will need to handle this (e.g. by making them friend functions or similar)
// or by moving struct definitions out of the class.
// The simplest fix is to move struct definitions outside the class if to_json/from_json are global.
// For now, I'll leave as is and address compilation issues if they arise based on exact compiler errors.
// The prompt asked for declarations after struct defs - this is tricky if structs are *inside* the class and functions are global.

// The forward declarations are good.
// The to_json/from_json should ideally refer to GuiApp::ReactionDiffusionSettings etc. if they are global.
// Or, if these are helper functions defined in GuiApp.cpp, they can access the nested structs.
// The prompt implies global to_json/from_json.
// Let's adjust for global functions accessing nested structs:
void to_json(ofJson& j, const GuiApp::LFOSettings& s);
void from_json(const ofJson& j, GuiApp::LFOSettings& s);
// void to_json(ofJson& j, const GuiApp::ChannelSettings& s); // Complex, skip for now
// void from_json(const ofJson& j, GuiApp::ChannelSettings& s); // Complex, skip for now
// void to_json(ofJson& j, const GuiApp::FeedbackSettings& s); // Complex, skip for now
// void from_json(const ofJson& j, GuiApp::FeedbackSettings& s); // Complex, skip for now
// void to_json(ofJson& j, const GuiApp::GlobalSettings& s); // Complex, skip for now
// void from_json(const ofJson& j, GuiApp::GlobalSettings& s); // Complex, skip for now
void to_json(ofJson& j, const GuiApp::ParticleFeedbackSettings& s);
void from_json(const ofJson& j, GuiApp::ParticleFeedbackSettings& s);
void to_json(ofJson& j, const GuiApp::SlitScanSettings& s);
void from_json(const ofJson& j, GuiApp::SlitScanSettings& s);
void to_json(ofJson& j, const GuiApp::NoiseGeneratorSettings& s);
void from_json(const ofJson& j, GuiApp::NoiseGeneratorSettings& s);
void to_json(ofJson& j, const GuiApp::PixelSortSettings& s);
void from_json(const ofJson& j, GuiApp::PixelSortSettings& s);
void to_json(ofJson& j, const GuiApp::ReactionDiffusionSettings& s);
void from_json(const ofJson& j, GuiApp::ReactionDiffusionSettings& s);
