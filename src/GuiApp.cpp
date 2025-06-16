#include "GuiApp.h"
#include "ofJson.h"
#include <iostream>
#include <fstream>
#include <algorithm> // For std::fill

const float LFO_MAX_RATE_GUI = 20.0f;
const int FB_DELAY_MAX_CONST_GUI = 30;

std::string gen_ui_id(const std::string& base, int idx = -1, const std::string& suffix = "") {
    std::string id = base;
    if (idx != -1) id += "_" + std::to_string(idx);
    id += "##" + base + suffix;
    if (idx != -1) id += "_" + std::to_string(idx) + suffix;
    return id;
}

// --- Start ofJson Serialization ---
void to_json(ofJson& j, const ofColor& c) {
    j = ofJson{{"r", c.r}, {"g", c.g}, {"b", c.b}, {"a", c.a}};
}
void from_json(const ofJson& j, ofColor& c) {
    c.r = j.value("r", 255.0f);
    c.g = j.value("g", 255.0f);
    c.b = j.value("b", 255.0f);
    c.a = j.value("a", 255.0f);
}

void to_json(ofJson& j, const GuiApp::LFOSettings& s) {
    j = ofJson{{"amp", s.amp}, {"theta", s.theta}};
}
void from_json(const ofJson& j, GuiApp::LFOSettings& s) {
    s.amp = j.value("amp", 0.0f);
    s.theta = j.value("theta", 0.0f);
}

void to_json(ofJson& j, const GuiApp::ReactionDiffusionSettings& s) {
    j = ofJson{
        {"enableRD", s.enableRD},
        {"feedRate", s.feedRate}, {"killRate", s.killRate},
        {"diffuseRateU", s.diffuseRateU}, {"diffuseRateV", s.diffuseRateV},
        {"timeStep", s.timeStep}, {"iterationsPerFrame", s.iterationsPerFrame},
        {"seedPattern", s.seedPattern}, {"seedRadius", s.seedRadius}, {"seedNoiseDensity", s.seedNoiseDensity},
        {"outputMix", s.outputMix}, {"colorMode", s.colorMode},
        {"colorU", s.colorU}, {"colorV", s.colorV},
        {"smoothDisplay", s.smoothDisplay}
    };
}
void from_json(const ofJson& j, GuiApp::ReactionDiffusionSettings& s) {
    s.enableRD = j.value("enableRD", false);
    s.feedRate = j.value("feedRate", 0.037f);
    s.killRate = j.value("killRate", 0.060f);
    s.diffuseRateU = j.value("diffuseRateU", 1.0f);
    s.diffuseRateV = j.value("diffuseRateV", 0.5f);
    s.timeStep = j.value("timeStep", 1.0f);
    s.iterationsPerFrame = j.value("iterationsPerFrame", 10);
    s.seedPattern = j.value("seedPattern", 0);
    s.seedRadius = j.value("seedRadius", 10.0f);
    s.seedNoiseDensity = j.value("seedNoiseDensity", 0.1f);
    s.outputMix = j.value("outputMix", 1.0f);
    s.colorMode = j.value("colorMode", 0);
    if (j.count("colorU")) from_json(j["colorU"], s.colorU); else s.colorU = ofColor::blue;
    if (j.count("colorV")) from_json(j["colorV"], s.colorV); else s.colorV = ofColor::yellow;
    s.smoothDisplay = j.value("smoothDisplay", false);
}

void to_json(ofJson& j, const GuiApp::ParticleFeedbackSettings& s) {
    j = ofJson{
        {"enableParticleFeedback", s.enableParticleFeedback},
        {"inputSource", s.inputSource},
        {"spawnThreshold", s.spawnThreshold},
        {"maxParticles", s.maxParticles},
        {"particleInitialLife", s.particleInitialLife},
        {"particleInitialSpeed", s.particleInitialSpeed},
        {"particleDrag", s.particleDrag},
        {"particleSize", s.particleSize},
        {"enableVelocityFromBrightness", s.enableVelocityFromBrightness},
        {"inheritColorFromSpawn", s.inheritColorFromSpawn},
        {"particleBaseColor", s.particleBaseColor},
        {"noiseForceAmount", s.noiseForceAmount},
        {"noiseFieldScale", s.noiseFieldScale},
        {"noiseTimeSpeed", s.noiseTimeSpeed},
        {"feedbackMix", s.feedbackMix}
    };
}
void from_json(const ofJson& j, GuiApp::ParticleFeedbackSettings& s) {
    s.enableParticleFeedback = j.value("enableParticleFeedback", false);
    s.inputSource = j.value("inputSource", 0);
    s.spawnThreshold = j.value("spawnThreshold", 0.5f);
    s.maxParticles = j.value("maxParticles", 1000);
    s.particleInitialLife = j.value("particleInitialLife", 2.0f);
    s.particleInitialSpeed = j.value("particleInitialSpeed", 50.0f);
    s.particleDrag = j.value("particleDrag", 0.05f);
    s.particleSize = j.value("particleSize", 2.0f);
    s.enableVelocityFromBrightness = j.value("enableVelocityFromBrightness", true);
    s.inheritColorFromSpawn = j.value("inheritColorFromSpawn", true);
    if (j.count("particleBaseColor")) from_json(j["particleBaseColor"], s.particleBaseColor); else s.particleBaseColor = ofColor::white;
    s.noiseForceAmount = j.value("noiseForceAmount", 0.0f);
    s.noiseFieldScale = j.value("noiseFieldScale", 0.01f);
    s.noiseTimeSpeed = j.value("noiseTimeSpeed", 0.1f);
    s.feedbackMix = j.value("feedbackMix", 0.1f);
}

void to_json(ofJson& j, const GuiApp::SlitScanSettings& s) {
    j = ofJson{
        {"enableSlitScan", s.enableSlitScan},
        {"inputSource", s.inputSource},
        {"slitDirection", s.slitDirection},
        {"slitPosition", s.slitPosition},
        {"slitThickness", s.slitThickness},
        {"accumulationSpeed", s.accumulationSpeed},
        {"wrapAccumulation", s.wrapAccumulation},
        {"delayFrames", s.delayFrames},
        {"outputMix", s.outputMix},
        {"blendMode", s.blendMode}
    };
}
void from_json(const ofJson& j, GuiApp::SlitScanSettings& s) {
    s.enableSlitScan = j.value("enableSlitScan", false);
    s.inputSource = j.value("inputSource", 0);
    s.slitDirection = j.value("slitDirection", 0);
    s.slitPosition = j.value("slitPosition", 0.5f);
    s.slitThickness = j.value("slitThickness", 2);
    s.accumulationSpeed = j.value("accumulationSpeed", 1.0f);
    s.wrapAccumulation = j.value("wrapAccumulation", true);
    s.delayFrames = j.value("delayFrames", 0);
    s.outputMix = j.value("outputMix", 1.0f);
    s.blendMode = j.value("blendMode", 0);
}

void to_json(ofJson& j, const GuiApp::NoiseGeneratorSettings& s) {
    j = ofJson{
        {"enableNoise", s.enableNoise},
        {"noiseType", s.noiseType},
        {"noiseScale", s.noiseScale},
        {"noiseTime", s.noiseTime},
        {"noiseSpeed", s.noiseSpeed},
        {"noiseAnimateTime", s.noiseAnimateTime},
        {"noiseOctaves", s.noiseOctaves},
        {"noisePersistence", s.noisePersistence},
        {"noiseColorEnable", s.noiseColorEnable},
        {"noiseColor1", s.noiseColor1},
        {"noiseColor2", s.noiseColor2},
        {"noiseRangeMin", s.noiseRangeMin},
        {"noiseRangeMax", s.noiseRangeMax},
        {"noiseApplyContrast", s.noiseApplyContrast},
        {"noiseContrast", s.noiseContrast},
        {"noiseBrightness", s.noiseBrightness}
    };
}
void from_json(const ofJson& j, GuiApp::NoiseGeneratorSettings& s) {
    s.enableNoise = j.value("enableNoise", false);
    s.noiseType = j.value("noiseType", 0);
    s.noiseScale = j.value("noiseScale", 0.02f);
    s.noiseTime = j.value("noiseTime", 0.0f);
    s.noiseSpeed = j.value("noiseSpeed", 0.05f);
    s.noiseAnimateTime = j.value("noiseAnimateTime", true);
    s.noiseOctaves = j.value("noiseOctaves", 4);
    s.noisePersistence = j.value("noisePersistence", 0.5f);
    s.noiseColorEnable = j.value("noiseColorEnable", false);
    if (j.count("noiseColor1")) from_json(j["noiseColor1"], s.noiseColor1); else s.noiseColor1 = ofColor::black;
    if (j.count("noiseColor2")) from_json(j["noiseColor2"], s.noiseColor2); else s.noiseColor2 = ofColor::white;
    s.noiseRangeMin = j.value("noiseRangeMin", 0.0f);
    s.noiseRangeMax = j.value("noiseRangeMax", 1.0f);
    s.noiseApplyContrast = j.value("noiseApplyContrast", false);
    s.noiseContrast = j.value("noiseContrast", 1.0f);
    s.noiseBrightness = j.value("noiseBrightness", 0.0f);
}

void to_json(ofJson& j, const GuiApp::PixelSortSettings& s) {
    j = ofJson{
        {"enablePixelSort", s.enablePixelSort},
        {"inputSource", s.inputSource},
        {"sortMode", s.sortMode},
        {"sortCriteria", s.sortCriteria},
        {"thresholdMin", s.thresholdMin},
        {"thresholdMax", s.thresholdMax},
        {"sortAscending", s.sortAscending},
        {"smartThresholding", s.smartThresholding},
        {"sortWindowSize", s.sortWindowSize},
        {"effectMix", s.effectMix}
    };
}
void from_json(const ofJson& j, GuiApp::PixelSortSettings& s) {
    s.enablePixelSort = j.value("enablePixelSort", false);
    s.inputSource = j.value("inputSource", 0);
    s.sortMode = j.value("sortMode", 0);
    s.sortCriteria = j.value("sortCriteria", 0);
    s.thresholdMin = j.value("thresholdMin", 0.1f);
    s.thresholdMax = j.value("thresholdMax", 0.9f);
    s.sortAscending = j.value("sortAscending", true);
    s.smartThresholding = j.value("smartThresholding", true);
    s.sortWindowSize = j.value("sortWindowSize", 5);
    s.effectMix = j.value("effectMix", 1.0f);
}
// --- End ofJson Serialization ---

namespace {
template<typename SettingsType>
bool loadSettingsPresetsFromFile(const std::string& filepath,
                                 std::vector<SettingsType>& presets_vec,
                                 std::vector<std::string>& preset_names_vec) {
    ofJson presets_json;
    std::string full_path = ofToDataPath(filepath, true);
    ofFile file(full_path);

    if (file.exists() && file.getSize() > 0) {
        try {
            file >> presets_json;
            if (presets_json.is_array()) {
                presets_vec.clear();
                preset_names_vec.clear();
                for (auto& item_json : presets_json) {
                    SettingsType s;
                    from_json(item_json["settings"], s);
                    presets_vec.push_back(s);
                    preset_names_vec.push_back(item_json.value("name", "Unnamed Preset"));
                }
                return true;
            }
        } catch (ofJson::parse_error& e) {
            ofLogError("GuiApp::PresetHelpers") << "Failed to parse " << filepath << ": " << e.what();
        }
    }
    return false;
}

template<typename SettingsType>
bool saveSettingsPresetsToFile(const std::string& filepath,
                               const std::vector<SettingsType>& presets_vec,
                               const std::vector<std::string>& preset_names_vec) {
    ofJson presets_json = ofJson::array();
    for (size_t i = 0; i < presets_vec.size(); ++i) {
        if (i < preset_names_vec.size()) {
            ofJson item_json;
            item_json["name"] = preset_names_vec[i];
            item_json["settings"] = presets_vec[i];
            presets_json.push_back(item_json);
        }
    }
    std::string full_path = ofToDataPath(filepath, true);
    if (!ofSaveJson(full_path, presets_json)) {
        ofLogError("GuiApp::PresetHelpers") << "Failed to save " << filepath;
        return false;
    }
    return true;
}
}

void GuiApp::setup(){
    ofBackground(0);
    imgui_instance.setup();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    if (availableWindowTitles.empty()) {
        availableWindowTitles.push_back("No windows listed yet");
    }
    selectedWindowIndex = 0;
    setupPresets();
}

void GuiApp::update() {}

void GuiApp::setupPresets() {
    loadRDPresets();
    loadParticlePresets();
    loadSlitScanPresets();
    loadNoiseGeneratorPresets();
    loadPixelSortPresets();
}

// RD Presets
void GuiApp::loadRDPresets() {
    if (!loadSettingsPresetsFromFile(rd_presets_filepath, rd_presets, rd_preset_names)) {
        addDefaultRDPresets();
        saveRDPresets();
    }
    current_rd_preset_index = -1;
}
void GuiApp::saveRDPresets() {
    saveSettingsPresetsToFile(rd_presets_filepath, rd_presets, rd_preset_names);
}
void GuiApp::applyRDPreset(int preset_index) {
    if (preset_index >= 0 && preset_index < rd_presets.size()) {
        reaction_diffusion_settings = rd_presets[preset_index];
        current_rd_preset_index = preset_index;
        reaction_diffusion_settings.clearCanvasTrigger = true;
    }
}
void GuiApp::addCurrentSettingsAsRDPreset(const std::string& name) {
    if (name.empty()) { ofLogWarning("GuiApp") << "RD preset name empty."; return; }
    for (size_t i = 0; i < rd_preset_names.size(); ++i) {
        if (rd_preset_names[i] == name) {
            rd_presets[i] = reaction_diffusion_settings;
            current_rd_preset_index = i;
            saveRDPresets();
            ofLogNotice("GuiApp") << "Overwrote existing RD preset: " << name;
            return;
        }
    }
    rd_presets.push_back(reaction_diffusion_settings);
    rd_preset_names.push_back(name);
    current_rd_preset_index = rd_presets.size() - 1;
    saveRDPresets();
    std::fill(rd_new_preset_name_buffer, rd_new_preset_name_buffer + sizeof(rd_new_preset_name_buffer), 0);
    ofLogNotice("GuiApp") << "Saved new RD preset: " << name;
}
void GuiApp::deleteRDPreset(int preset_index) {
    if (preset_index >= 0 && preset_index < rd_presets.size()) {
        rd_presets.erase(rd_presets.begin() + preset_index);
        rd_preset_names.erase(rd_preset_names.begin() + preset_index);
        if (current_rd_preset_index == preset_index) current_rd_preset_index = -1;
        else if (current_rd_preset_index > preset_index) current_rd_preset_index--;
        saveRDPresets();
    }
}
void GuiApp::addDefaultRDPresets() {
    rd_presets.clear(); rd_preset_names.clear();
    ReactionDiffusionSettings s;
    s = ReactionDiffusionSettings();
    s.feedRate = 0.037f; s.killRate = 0.060f; s.diffuseRateU = 1.0f; s.diffuseRateV = 0.5f; s.timeStep = 1.0f; s.iterationsPerFrame = 16; s.colorMode = 1;
    rd_presets.push_back(s); rd_preset_names.push_back("Coral Growth");
    s = ReactionDiffusionSettings();
    s.feedRate = 0.030f; s.killRate = 0.062f; s.diffuseRateU = 1.0f; s.diffuseRateV = 0.5f; s.iterationsPerFrame = 20; s.colorMode = 2; s.colorU = ofColor::fromHex(0x222244); s.colorV = ofColor::fromHex(0x88DDEE);
    rd_presets.push_back(s); rd_preset_names.push_back("Mitosis Blue");
    s = ReactionDiffusionSettings();
    s.feedRate = 0.014f; s.killRate = 0.054f; s.diffuseRateU = 1.0f; s.diffuseRateV = 0.5f; s.iterationsPerFrame = 10; s.colorMode = 2; s.colorU = ofColor::black; s.colorV = ofColor::orangeRed;
    rd_presets.push_back(s); rd_preset_names.push_back("Worms & Spots");
}

// Particle Feedback Presets
void GuiApp::loadParticlePresets() {
    if (!loadSettingsPresetsFromFile(particle_presets_filepath, particle_presets, particle_preset_names)) {
        addDefaultParticlePresets();
        saveParticlePresets();
    }
    current_particle_preset_index = -1;
}
void GuiApp::saveParticlePresets() {
    saveSettingsPresetsToFile(particle_presets_filepath, particle_presets, particle_preset_names);
}
void GuiApp::applyParticlePreset(int preset_index) {
    if (preset_index >= 0 && preset_index < particle_presets.size()) {
        particle_feedback_settings = particle_presets[preset_index];
        current_particle_preset_index = preset_index;
    }
}
void GuiApp::addCurrentSettingsAsParticlePreset(const std::string& name) {
    if (name.empty()) { ofLogWarning("GuiApp") << "Particle preset name empty."; return; }
    for (size_t i = 0; i < particle_preset_names.size(); ++i) {
        if (particle_preset_names[i] == name) {
            particle_presets[i] = particle_feedback_settings;
            current_particle_preset_index = i;
            saveParticlePresets();
            ofLogNotice("GuiApp") << "Overwrote existing Particle preset: " << name;
            return;
        }
    }
    particle_presets.push_back(particle_feedback_settings);
    particle_preset_names.push_back(name);
    current_particle_preset_index = particle_presets.size() - 1;
    saveParticlePresets();
    std::fill(particle_new_preset_name_buffer, particle_new_preset_name_buffer + sizeof(particle_new_preset_name_buffer), 0);
    ofLogNotice("GuiApp") << "Saved new Particle preset: " << name;
}
void GuiApp::deleteParticlePreset(int preset_index) {
    if (preset_index >= 0 && preset_index < particle_presets.size()) {
        particle_presets.erase(particle_presets.begin() + preset_index);
        particle_preset_names.erase(particle_preset_names.begin() + preset_index);
        if (current_particle_preset_index == preset_index) current_particle_preset_index = -1;
        else if (current_particle_preset_index > preset_index) current_particle_preset_index--;
        saveParticlePresets();
    }
}
void GuiApp::addDefaultParticlePresets() {
    particle_presets.clear(); particle_preset_names.clear();
    ParticleFeedbackSettings s;
    s = ParticleFeedbackSettings();
    s.spawnThreshold = 0.6f; s.maxParticles = 2000; s.particleInitialLife = 1.5f; s.particleInitialSpeed = 30.f; s.particleDrag=0.1f; s.particleSize = 1.5f; s.feedbackMix = 0.05f;
    particle_presets.push_back(s); particle_preset_names.push_back("Basic Sparkles");
    s = ParticleFeedbackSettings();
    s.spawnThreshold = 0.3f; s.maxParticles = 5000; s.particleInitialLife = 3.0f; s.particleInitialSpeed = 10.f; s.particleDrag=0.01f; s.particleSize = 1.0f; s.noiseForceAmount = 20.f; s.noiseFieldScale = 0.005f; s.feedbackMix = 0.03f;
    particle_presets.push_back(s); particle_preset_names.push_back("Noise Flow");
}

// Slit-Scan Presets
void GuiApp::loadSlitScanPresets() {
    if (!loadSettingsPresetsFromFile(slit_scan_presets_filepath, slit_scan_presets, slit_scan_preset_names)) {
        addDefaultSlitScanPresets();
        saveSlitScanPresets();
    }
    current_slit_scan_preset_index = -1;
}
void GuiApp::saveSlitScanPresets() {
    saveSettingsPresetsToFile(slit_scan_presets_filepath, slit_scan_presets, slit_scan_preset_names);
}
void GuiApp::applySlitScanPreset(int preset_index) {
    if (preset_index >= 0 && preset_index < slit_scan_presets.size()) {
        slit_scan_settings = slit_scan_presets[preset_index];
        current_slit_scan_preset_index = preset_index;
    }
}
void GuiApp::addCurrentSettingsAsSlitScanPreset(const std::string& name) {
    if (name.empty()) { ofLogWarning("GuiApp") << "Slit-Scan preset name empty."; return; }
    for (size_t i = 0; i < slit_scan_preset_names.size(); ++i) {
        if (slit_scan_preset_names[i] == name) {
            slit_scan_presets[i] = slit_scan_settings;
            current_slit_scan_preset_index = i;
            saveSlitScanPresets();
            ofLogNotice("GuiApp") << "Overwrote existing Slit-Scan preset: " << name;
            return;
        }
    }
    slit_scan_presets.push_back(slit_scan_settings);
    slit_scan_preset_names.push_back(name);
    current_slit_scan_preset_index = slit_scan_presets.size() - 1;
    saveSlitScanPresets();
    std::fill(slit_scan_new_preset_name_buffer, slit_scan_new_preset_name_buffer + sizeof(slit_scan_new_preset_name_buffer), 0);
    ofLogNotice("GuiApp") << "Saved new Slit-Scan preset: " << name;
}
void GuiApp::deleteSlitScanPreset(int preset_index) {
    if (preset_index >= 0 && preset_index < slit_scan_presets.size()) {
        slit_scan_presets.erase(slit_scan_presets.begin() + preset_index);
        slit_scan_preset_names.erase(slit_scan_preset_names.begin() + preset_index);
        if (current_slit_scan_preset_index == preset_index) current_slit_scan_preset_index = -1;
        else if (current_slit_scan_preset_index > preset_index) current_slit_scan_preset_index--;
        saveSlitScanPresets();
    }
}
void GuiApp::addDefaultSlitScanPresets() {
    slit_scan_presets.clear(); slit_scan_preset_names.clear();
    SlitScanSettings s;
    s = SlitScanSettings();
    s.slitDirection = 1; s.accumulationSpeed = 1.0f;
    slit_scan_presets.push_back(s); slit_scan_preset_names.push_back("Horizontal Scan");
    s = SlitScanSettings();
    s.slitDirection = 0; s.delayFrames = 30; s.accumulationSpeed = 2.0f; s.slitThickness = 4;
    slit_scan_presets.push_back(s); slit_scan_preset_names.push_back("Delayed Vertical");
}

// Noise Generator Presets
void GuiApp::loadNoiseGeneratorPresets() {
    if (!loadSettingsPresetsFromFile(noise_generator_presets_filepath, noise_generator_presets, noise_generator_preset_names)) {
        addDefaultNoiseGeneratorPresets();
        saveNoiseGeneratorPresets();
    }
    current_noise_generator_preset_index = -1;
}
void GuiApp::saveNoiseGeneratorPresets() {
    saveSettingsPresetsToFile(noise_generator_presets_filepath, noise_generator_presets, noise_generator_preset_names);
}
void GuiApp::applyNoiseGeneratorPreset(int preset_index) {
    if (preset_index >= 0 && preset_index < noise_generator_presets.size()) {
        noise_generator_settings = noise_generator_presets[preset_index];
        current_noise_generator_preset_index = preset_index;
    }
}
void GuiApp::addCurrentSettingsAsNoiseGeneratorPreset(const std::string& name) {
    if (name.empty()) { ofLogWarning("GuiApp") << "Noise Generator preset name empty."; return; }
    for (size_t i = 0; i < noise_generator_preset_names.size(); ++i) {
        if (noise_generator_preset_names[i] == name) {
            noise_generator_presets[i] = noise_generator_settings;
            current_noise_generator_preset_index = i;
            saveNoiseGeneratorPresets();
            ofLogNotice("GuiApp") << "Overwrote existing Noise Generator preset: " << name;
            return;
        }
    }
    noise_generator_presets.push_back(noise_generator_settings);
    noise_generator_preset_names.push_back(name);
    current_noise_generator_preset_index = noise_generator_presets.size() - 1;
    saveNoiseGeneratorPresets();
    std::fill(noise_generator_new_preset_name_buffer, noise_generator_new_preset_name_buffer + sizeof(noise_generator_new_preset_name_buffer), 0);
    ofLogNotice("GuiApp") << "Saved new Noise Generator preset: " << name;
}
void GuiApp::deleteNoiseGeneratorPreset(int preset_index) {
    if (preset_index >= 0 && preset_index < noise_generator_presets.size()) {
        noise_generator_presets.erase(noise_generator_presets.begin() + preset_index);
        noise_generator_preset_names.erase(noise_generator_preset_names.begin() + preset_index);
        if (current_noise_generator_preset_index == preset_index) current_noise_generator_preset_index = -1;
        else if (current_noise_generator_preset_index > preset_index) current_noise_generator_preset_index--;
        saveNoiseGeneratorPresets();
    }
}
void GuiApp::addDefaultNoiseGeneratorPresets() {
    noise_generator_presets.clear(); noise_generator_preset_names.clear();
    NoiseGeneratorSettings s;
    s = NoiseGeneratorSettings();
    s.noiseScale = 0.01f; s.noiseSpeed = 0.02f; s.noiseOctaves = 6; s.noisePersistence = 0.6f;
    noise_generator_presets.push_back(s); noise_generator_preset_names.push_back("Slow Clouds");
    s = NoiseGeneratorSettings();
    s.noiseScale = 0.08f; s.noiseSpeed = 0.5f; s.noiseOctaves = 2; s.noisePersistence = 0.3f;
    noise_generator_presets.push_back(s); noise_generator_preset_names.push_back("Fast Grainy");
    s = NoiseGeneratorSettings();
    s.noiseScale = 0.03f; s.noiseSpeed = 0.03f; s.noiseOctaves = 5; s.noiseColorEnable = true;
    s.noiseColor1 = ofColor::magenta; s.noiseColor2 = ofColor::cyan;
    noise_generator_presets.push_back(s); noise_generator_preset_names.push_back("Psychedelic Clouds");
}

// Pixel Sorting Presets
void GuiApp::loadPixelSortPresets() {
    if (!loadSettingsPresetsFromFile(pixel_sort_presets_filepath, pixel_sort_presets, pixel_sort_preset_names)) {
        addDefaultPixelSortPresets();
        savePixelSortPresets();
    }
    current_pixel_sort_preset_index = -1;
}
void GuiApp::savePixelSortPresets() {
    saveSettingsPresetsToFile(pixel_sort_presets_filepath, pixel_sort_presets, pixel_sort_preset_names);
}
void GuiApp::applyPixelSortPreset(int preset_index) {
    if (preset_index >= 0 && preset_index < pixel_sort_presets.size()) {
        pixel_sort_settings = pixel_sort_presets[preset_index];
        current_pixel_sort_preset_index = preset_index;
    }
}
void GuiApp::addCurrentSettingsAsPixelSortPreset(const std::string& name) {
    if (name.empty()) { ofLogWarning("GuiApp") << "Pixel Sort preset name empty."; return; }
    for (size_t i = 0; i < pixel_sort_preset_names.size(); ++i) {
        if (pixel_sort_preset_names[i] == name) {
            pixel_sort_presets[i] = pixel_sort_settings;
            current_pixel_sort_preset_index = i;
            savePixelSortPresets();
            ofLogNotice("GuiApp") << "Overwrote existing Pixel Sort preset: " << name;
            return;
        }
    }
    pixel_sort_presets.push_back(pixel_sort_settings);
    pixel_sort_preset_names.push_back(name);
    current_pixel_sort_preset_index = pixel_sort_presets.size() - 1;
    savePixelSortPresets();
    std::fill(pixel_sort_new_preset_name_buffer, pixel_sort_new_preset_name_buffer + sizeof(pixel_sort_new_preset_name_buffer), 0);
    ofLogNotice("GuiApp") << "Saved new Pixel Sort preset: " << name;
}
void GuiApp::deletePixelSortPreset(int preset_index) {
    if (preset_index >= 0 && preset_index < pixel_sort_presets.size()) {
        pixel_sort_presets.erase(pixel_sort_presets.begin() + preset_index);
        pixel_sort_preset_names.erase(pixel_sort_preset_names.begin() + preset_index);
        if (current_pixel_sort_preset_index == preset_index) current_pixel_sort_preset_index = -1;
        else if (current_pixel_sort_preset_index > preset_index) current_pixel_sort_preset_index--;
        savePixelSortPresets();
    }
}
void GuiApp::addDefaultPixelSortPresets() {
    pixel_sort_presets.clear(); pixel_sort_preset_names.clear();
    PixelSortSettings s;
    s = PixelSortSettings();
    s.sortCriteria = 0; s.thresholdMin = 0.2f; s.thresholdMax = 0.7f; s.sortWindowSize = 10;
    pixel_sort_presets.push_back(s); pixel_sort_preset_names.push_back("Bright Streaks");
    s = PixelSortSettings();
    s.sortMode = 1; s.sortCriteria = 1; s.thresholdMin = 0.0f; s.thresholdMax = 1.0f;
    s.sortWindowSize = 7; s.smartThresholding = false;
    pixel_sort_presets.push_back(s); pixel_sort_preset_names.push_back("Vertical Hue Sort");
}

void GuiApp::drawChannelControls(int ch_idx, float lfom) {
    std::string title = "Channel " + std::to_string(ch_idx + 1);
    if (ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
        ChannelSettings& cs = channel_settings[ch_idx];

        if (ImGui::CollapsingHeader(gen_ui_id("InputSource", ch_idx).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            const char* items[] = { "Cam 1", "Cam 2", "NDI", "Video File", "Window Capture", "Noise Generator", "Reaction-Diffusion" };
            int current_selection = (cs.select > 0 && cs.select <= IM_ARRAYSIZE(items)) ? cs.select - 1 : 0;
            if (ImGui::Combo(gen_ui_id("SourceCombo", ch_idx).c_str(), &current_selection, items, IM_ARRAYSIZE(items))) {
                cs.select = current_selection + 1;
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
            ImGui::SliderFloat(gen_ui_id("SatPm", ch_idx).c_str(), &cs.saturation_powmap, -5.0f, 5.0f);
            ImGui::SliderFloat(gen_ui_id("BrightPm", ch_idx).c_str(), &cs.bright_powmap, -5.0f, 5.0f);
            ImGui::Spacing();
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader(gen_ui_id("LFOs", ch_idx).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Hue LFO"); ImGui::Indent();
            ImGui::SliderFloat(gen_ui_id("HueLFORate", ch_idx).c_str(), &cs.hue_lfo.theta,  .0f, LFO_MAX_RATE_GUI);
            ImGui::SliderFloat(gen_ui_id("HueLFOAmp", ch_idx).c_str(), &cs.hue_lfo.amp, -5.0f, 5.0f);
            ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Saturation LFO"); ImGui::Indent();
            ImGui::SliderFloat(gen_ui_id("SatLFORate", ch_idx).c_str(), &cs.saturation_lfo.theta, .0f, LFO_MAX_RATE_GUI);
            ImGui::SliderFloat(gen_ui_id("SatLFOAmp", ch_idx).c_str(), &cs.saturation_lfo.amp, -5.0f, 5.0f);
            ImGui::Unindent(); ImGui::Spacing();
            ImGui::Text("Brightness LFO"); ImGui::Indent();
            ImGui::SliderFloat(gen_ui_id("BrightLFORate", ch_idx).c_str(), &cs.bright_lfo.theta, .0f, LFO_MAX_RATE_GUI);
            ImGui::SliderFloat(gen_ui_id("BrightLFOAmp", ch_idx).c_str(), &cs.bright_lfo.amp, -5.0f, 5.0f);
            ImGui::Unindent(); ImGui::Spacing();
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void GuiApp::drawFeedbackControls(int fb_idx, float lfom) { /* ... (Omitted for brevity - assumed unchanged) ... */ }
void GuiApp::drawGlobalControls(float lfom) { /* ... (Omitted for brevity - assumed unchanged) ... */ }
void GuiApp::drawPixelateControls(const std::string& panel_name_id, float lfom) { /* ... (Omitted for brevity - assumed unchanged) ... */ }
void GuiApp::drawVideoControls() { /* ... (Omitted for brevity - assumed unchanged) ... */ }
void GuiApp::drawWindowCaptureControls() { /* ... (Omitted for brevity - assumed unchanged) ... */ }

void GuiApp::drawMoreEffectsControls() {
    if (ImGui::Begin("More Effects", nullptr, ImGuiWindowFlags_None)) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);

        if (ImGui::CollapsingHeader("Particle Feedback##PFXHeader", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            ParticleFeedbackSettings& pfx_settings = particle_feedback_settings;
            ImGui::Separator(); ImGui::Text("Presets"); ImGui::Indent();
            std::vector<const char*> p_preset_cnames; for(const auto& name : particle_preset_names) { p_preset_cnames.push_back(name.c_str()); }
            if (p_preset_cnames.empty()) { ImGui::Text("No presets available."); }
            else {
                int temp_idx = current_particle_preset_index;
                if (ImGui::Combo(gen_ui_id("PFXPresetCombo",-1,"PFCmb").c_str(), &temp_idx, p_preset_cnames.data(), p_preset_cnames.size())) {
                    if(temp_idx >= 0) applyParticlePreset(temp_idx);
                }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Load a preset.");
                ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
                if (current_particle_preset_index != -1 && static_cast<size_t>(current_particle_preset_index) < particle_preset_names.size()) {
                     ImGui::TextDisabled(("(Active: " + particle_preset_names[current_particle_preset_index] + ")").c_str());
                } else if (!particle_presets.empty()) { ImGui::TextDisabled("(Custom Settings)");}

                if (current_particle_preset_index != -1) {
                    ImGui::SameLine(); if (ImGui::Button(gen_ui_id("PFXDelPreset",-1,"PFBtnDel").c_str())) { deleteParticlePreset(current_particle_preset_index); }
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete selected preset.");
                }
            }
            ImGui::InputText(gen_ui_id("PFXNewPresetName",-1,"PFInTxt").c_str(), particle_new_preset_name_buffer, IM_ARRAYSIZE(particle_new_preset_name_buffer)); ImGui::SameLine();
            if (ImGui::Button(gen_ui_id("PFXSavePreset",-1,"PFBtnSave").c_str())) { addCurrentSettingsAsParticlePreset(std::string(particle_new_preset_name_buffer)); }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save current settings as new preset.");
            ImGui::Unindent(); ImGui::Separator();

            if(ImGui::Checkbox(gen_ui_id("EnablePFX", -1, "Cb").c_str(), &pfx_settings.enableParticleFeedback)) {current_particle_preset_index = -1;}
            ImGui::Text("Spawning");
            const char* input_sources_pfx[] = { "Main Output", "Cam 1", "Cam 2", "NDI", "Video File", "Window Capture", "Noise Generator", "Reaction-Diffusion" };
            int current_source_idx_pfx = (pfx_settings.inputSource >= 0 && pfx_settings.inputSource < IM_ARRAYSIZE(input_sources_pfx)) ? pfx_settings.inputSource : 0;
            if (ImGui::Combo(gen_ui_id("PFXInputSrc", -1, "Cb").c_str(), &current_source_idx_pfx, input_sources_pfx, IM_ARRAYSIZE(input_sources_pfx))) {
                pfx_settings.inputSource = current_source_idx_pfx; current_particle_preset_index = -1;
            }
            if(ImGui::SliderFloat(gen_ui_id("PFXSpawnThresh", -1, "Sld").c_str(), &pfx_settings.spawnThreshold, 0.0f, 1.0f)) {current_particle_preset_index = -1;}
            if(ImGui::SliderInt(gen_ui_id("PFXMaxParticles", -1, "SldInt").c_str(), &pfx_settings.maxParticles, 100, 10000)) {current_particle_preset_index = -1;}
            ImGui::Text("Particle Behavior");
            if(ImGui::SliderFloat(gen_ui_id("PFXLife", -1, "Sld").c_str(), &pfx_settings.particleInitialLife, 0.1f, 10.0f, "%.2f s")) {current_particle_preset_index = -1;}
            if(ImGui::SliderFloat(gen_ui_id("PFXSpeed", -1, "Sld").c_str(), &pfx_settings.particleInitialSpeed, 0.0f, 200.0f, "%.0f px/s")) {current_particle_preset_index = -1;}
            if(ImGui::SliderFloat(gen_ui_id("PFXDrag", -1, "Sld").c_str(), &pfx_settings.particleDrag, 0.0f, 0.5f, "%.3f")) {current_particle_preset_index = -1;}
            if(ImGui::SliderFloat(gen_ui_id("PFXSize", -1, "Sld").c_str(), &pfx_settings.particleSize, 0.1f, 20.0f, "%.1f px")) {current_particle_preset_index = -1;}
            if(ImGui::Checkbox(gen_ui_id("PFXVelFromBright", -1, "Cb").c_str(), &pfx_settings.enableVelocityFromBrightness)) {current_particle_preset_index = -1;}
            ImGui::Text("Particle Appearance");
            if(ImGui::Checkbox(gen_ui_id("PFXInheritColor", -1, "Cb").c_str(), &pfx_settings.inheritColorFromSpawn)) {current_particle_preset_index = -1;}
            float color_arr[4] = { pfx_settings.particleBaseColor.r / 255.0f, pfx_settings.particleBaseColor.g / 255.0f, pfx_settings.particleBaseColor.b / 255.0f, pfx_settings.particleBaseColor.a / 255.0f };
            if (ImGui::ColorEdit4(gen_ui_id("PFXBaseColor", -1, "ClrEdit").c_str(), color_arr)) {
                pfx_settings.particleBaseColor.set(color_arr[0] * 255, color_arr[1] * 255, color_arr[2] * 255, color_arr[3] * 255);
                current_particle_preset_index = -1;
            }
            ImGui::Text("Forces & Environment");
            if(ImGui::SliderFloat(gen_ui_id("PFXNoiseForce", -1, "Sld").c_str(), &pfx_settings.noiseForceAmount, 0.0f, 100.0f)) {current_particle_preset_index = -1;}
            if(ImGui::SliderFloat(gen_ui_id("PFXNoiseScale", -1, "Sld").c_str(), &pfx_settings.noiseFieldScale, 0.001f, 0.1f, "%.4f")) {current_particle_preset_index = -1;}
            if(ImGui::SliderFloat(gen_ui_id("PFXNoiseTimeSpd", -1, "Sld").c_str(), &pfx_settings.noiseTimeSpeed, 0.0f, 1.0f)) {current_particle_preset_index = -1;}
            ImGui::Text("Blending");
            if(ImGui::SliderFloat(gen_ui_id("PFXFeedbackMix", -1, "Sld").c_str(), &pfx_settings.feedbackMix, 0.0f, 1.0f)) {current_particle_preset_index = -1;}
            ImGui::Unindent();
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Slit-Scan##SlitScanEffect", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            SlitScanSettings& ss_settings = this->slit_scan_settings;
            ImGui::Separator(); ImGui::Text("Presets"); ImGui::Indent();
            std::vector<const char*> preset_cnames_ss; for(const auto& name : slit_scan_preset_names) { preset_cnames_ss.push_back(name.c_str()); }
            if (preset_cnames_ss.empty()) { ImGui::Text("No presets available."); }
            else {
                int temp_idx_ss = current_slit_scan_preset_index;
                if (ImGui::Combo(gen_ui_id("SSPresetCombo", -1, "SSCmb").c_str(), &temp_idx_ss, preset_cnames_ss.data(), preset_cnames_ss.size())) {
                    if (temp_idx_ss >= 0) applySlitScanPreset(temp_idx_ss);
                }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Load a preset.");
                ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
                if (current_slit_scan_preset_index != -1 && static_cast<size_t>(current_slit_scan_preset_index) < slit_scan_preset_names.size()) {
                     ImGui::TextDisabled(("(Active: " + slit_scan_preset_names[current_slit_scan_preset_index] + ")").c_str());
                } else if (!slit_scan_presets.empty()) { ImGui::TextDisabled("(Custom Settings)");}

                if (current_slit_scan_preset_index != -1) {
                    ImGui::SameLine(); if (ImGui::Button(gen_ui_id("SSDelPreset", -1, "SSBtnDel").c_str())) { deleteSlitScanPreset(current_slit_scan_preset_index); }
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete selected preset.");
                }
            }
            ImGui::InputText(gen_ui_id("SSNewPresetName", -1, "SSInTxt").c_str(), slit_scan_new_preset_name_buffer, IM_ARRAYSIZE(slit_scan_new_preset_name_buffer)); ImGui::SameLine();
            if (ImGui::Button(gen_ui_id("SSSavePreset", -1, "SSBtnSave").c_str())) { addCurrentSettingsAsSlitScanPreset(std::string(slit_scan_new_preset_name_buffer)); }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save current settings as new preset.");
            ImGui::Unindent(); ImGui::Separator();

            if(ImGui::Checkbox(gen_ui_id("EnableSS", -1, "SSCb").c_str(), &ss_settings.enableSlitScan)) { current_slit_scan_preset_index = -1; }
            ImGui::Text("Source & Slit Configuration");
            const char* input_sources_ss[] = { "Main Output", "Cam 1", "Cam 2", "NDI", "Video File", "Window Capture", "Noise Generator", "Reaction-Diffusion" };
            int current_source_idx_ss = (ss_settings.inputSource >= 0 && ss_settings.inputSource < IM_ARRAYSIZE(input_sources_ss)) ? ss_settings.inputSource : 0;
            if (ImGui::Combo(gen_ui_id("SSInputSrc", -1, "SSCombo").c_str(), &current_source_idx_ss, input_sources_ss, IM_ARRAYSIZE(input_sources_ss))) {
                ss_settings.inputSource = current_source_idx_ss; current_slit_scan_preset_index = -1;
            }
            if(ImGui::SliderInt(gen_ui_id("SSDelayFrames", -1, "SSSliderInt").c_str(), &ss_settings.delayFrames, 0, 120)) { current_slit_scan_preset_index = -1; }
            const char* slit_directions[] = { "Vertical Slit (Scan X-axis)", "Horizontal Slit (Scan Y-axis)" };
            int current_direction_idx = (ss_settings.slitDirection >= 0 && ss_settings.slitDirection < IM_ARRAYSIZE(slit_directions)) ? ss_settings.slitDirection : 0;
            if (ImGui::Combo(gen_ui_id("SSDirection", -1, "SSDirCombo").c_str(), &current_direction_idx, slit_directions, IM_ARRAYSIZE(slit_directions))) {
                ss_settings.slitDirection = current_direction_idx; current_slit_scan_preset_index = -1;
            }
            if(ImGui::SliderFloat(gen_ui_id("SSPosition", -1, "SSSliderF").c_str(), &ss_settings.slitPosition, 0.0f, 1.0f, "%.3f")) { current_slit_scan_preset_index = -1; }
            if(ImGui::SliderInt(gen_ui_id("SSThickness", -1, "SSThickSldInt").c_str(), &ss_settings.slitThickness, 1, 100)) { current_slit_scan_preset_index = -1; }
            ImGui::Text("Accumulation & Output Blending");
            if(ImGui::SliderFloat(gen_ui_id("SSAccumSpeed", -1, "SSAccumSldF").c_str(), &ss_settings.accumulationSpeed, -10.0f, 10.0f, "%.1f px/frame")) { current_slit_scan_preset_index = -1; }
            if(ImGui::Checkbox(gen_ui_id("SSWrapAccum", -1, "SSWrapCb").c_str(), &ss_settings.wrapAccumulation)) { current_slit_scan_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("SSOutputMix", -1, "SSMixSldF").c_str(), &ss_settings.outputMix, 0.0f, 1.0f)) { current_slit_scan_preset_index = -1; }
            const char* blend_modes_ss[] = { "Alpha Blend", "Additive", "Screen" };
            int current_blend_idx_ss = (ss_settings.blendMode >= 0 && ss_settings.blendMode < IM_ARRAYSIZE(blend_modes_ss)) ? ss_settings.blendMode : 0;
            if (ImGui::Combo(gen_ui_id("SSBlendMode", -1, "SSBlendCombo").c_str(), &current_blend_idx_ss, blend_modes_ss, IM_ARRAYSIZE(blend_modes_ss))) {
                ss_settings.blendMode = current_blend_idx_ss; current_slit_scan_preset_index = -1;
            }
            ImGui::Unindent();
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Noise Generator##NoiseGenEffect", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            NoiseGeneratorSettings& ng_settings = this->noise_generator_settings;
            ImGui::Separator(); ImGui::Text("Presets"); ImGui::Indent();
            std::vector<const char*> preset_cnames_ng; for(const auto& name : noise_generator_preset_names) { preset_cnames_ng.push_back(name.c_str()); }
            if (preset_cnames_ng.empty()) { ImGui::Text("No presets available."); }
            else {
                int temp_idx_ng = current_noise_generator_preset_index;
                if (ImGui::Combo(gen_ui_id("NGPresetCombo", -1, "NGCmb").c_str(), &temp_idx_ng, preset_cnames_ng.data(), preset_cnames_ng.size())) {
                    if (temp_idx_ng >= 0) applyNoiseGeneratorPreset(temp_idx_ng);
                }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Load a preset.");
                ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
                if (current_noise_generator_preset_index != -1 && static_cast<size_t>(current_noise_generator_preset_index) < noise_generator_preset_names.size()) {
                     ImGui::TextDisabled(("(Active: " + noise_generator_preset_names[current_noise_generator_preset_index] + ")").c_str());
                } else if (!noise_generator_presets.empty()) { ImGui::TextDisabled("(Custom Settings)");}

                if (current_noise_generator_preset_index != -1) {
                    ImGui::SameLine(); if (ImGui::Button(gen_ui_id("NGDelPreset", -1, "NGBtnDel").c_str())) { deleteNoiseGeneratorPreset(current_noise_generator_preset_index); }
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete selected preset.");
                }
            }
            ImGui::InputText(gen_ui_id("NGNewPresetName", -1, "NGInTxt").c_str(), noise_generator_new_preset_name_buffer, IM_ARRAYSIZE(noise_generator_new_preset_name_buffer)); ImGui::SameLine();
            if (ImGui::Button(gen_ui_id("NGSavePreset", -1, "NGBtnSave").c_str())) { addCurrentSettingsAsNoiseGeneratorPreset(std::string(noise_generator_new_preset_name_buffer)); }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save current settings as new preset.");
            ImGui::Unindent(); ImGui::Separator();

            if(ImGui::Checkbox(gen_ui_id("EnableNoise", -1, "NGCb").c_str(), &ng_settings.enableNoise)) { current_noise_generator_preset_index = -1; }
            ImGui::Text("Noise Parameters"); ImGui::Text("Type: Perlin (using ofNoise)");
            if(ImGui::SliderFloat(gen_ui_id("NGScale", -1, "NGSldF").c_str(), &ng_settings.noiseScale, 0.001f, 0.1f, "%.4f")) { current_noise_generator_preset_index = -1; }
            if(ImGui::Checkbox(gen_ui_id("NGAnimTime", -1, "NGCb").c_str(), &ng_settings.noiseAnimateTime)) { current_noise_generator_preset_index = -1; }
            if (ng_settings.noiseAnimateTime) {
                if(ImGui::SliderFloat(gen_ui_id("NGSpeed", -1, "NGSldF").c_str(), &ng_settings.noiseSpeed, 0.0f, 1.0f)) { current_noise_generator_preset_index = -1; }
            } else {
                if(ImGui::SliderFloat(gen_ui_id("NGTime", -1, "NGSldF").c_str(), &ng_settings.noiseTime, 0.0f, 1000.0f)) { current_noise_generator_preset_index = -1; }
            }
            if(ImGui::SliderInt(gen_ui_id("NGOctaves", -1, "NGSldI").c_str(), &ng_settings.noiseOctaves, 1, 8)) { current_noise_generator_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("NGPersistence", -1, "NGSldF").c_str(), &ng_settings.noisePersistence, 0.0f, 1.0f)) { current_noise_generator_preset_index = -1; }
            ImGui::Text("Noise Output Mapping");
            if(ImGui::SliderFloat(gen_ui_id("NGRangeMin", -1, "NGSldF").c_str(), &ng_settings.noiseRangeMin, -1.0f, 1.0f)) { current_noise_generator_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("NGRangeMax", -1, "NGSldF").c_str(), &ng_settings.noiseRangeMax, -1.0f, 1.0f)) { current_noise_generator_preset_index = -1; }
            if(ImGui::Checkbox(gen_ui_id("NGApplyContrast", -1, "NGCb").c_str(), &ng_settings.noiseApplyContrast)) { current_noise_generator_preset_index = -1; }
            if (ng_settings.noiseApplyContrast) {
                ImGui::Indent();
                if(ImGui::SliderFloat(gen_ui_id("NGContrast", -1, "NGSldF").c_str(), &ng_settings.noiseContrast, 0.1f, 5.0f)) { current_noise_generator_preset_index = -1; }
                if(ImGui::SliderFloat(gen_ui_id("NGBrightness", -1, "NGSldF").c_str(), &ng_settings.noiseBrightness, -0.5f, 0.5f)) { current_noise_generator_preset_index = -1; }
                ImGui::Unindent();
            }
            if(ImGui::Checkbox(gen_ui_id("NGColorEnable", -1, "NGCb").c_str(), &ng_settings.noiseColorEnable)) { current_noise_generator_preset_index = -1; }
            if (ng_settings.noiseColorEnable) {
                ImGui::Indent();
                float color1_arr_ng[4] = { ng_settings.noiseColor1.r / 255.0f, ng_settings.noiseColor1.g / 255.0f, ng_settings.noiseColor1.b / 255.0f, ng_settings.noiseColor1.a / 255.0f };
                if (ImGui::ColorEdit4(gen_ui_id("NGColor1", -1, "NGClr1").c_str(), color1_arr_ng)) {
                    ng_settings.noiseColor1.set(color1_arr_ng[0] * 255, color1_arr_ng[1] * 255, color1_arr_ng[2] * 255, color1_arr_ng[3] * 255);
                    current_noise_generator_preset_index = -1;
                }
                float color2_arr_ng[4] = { ng_settings.noiseColor2.r / 255.0f, ng_settings.noiseColor2.g / 255.0f, ng_settings.noiseColor2.b / 255.0f, ng_settings.noiseColor2.a / 255.0f };
                if (ImGui::ColorEdit4(gen_ui_id("NGColor2", -1, "NGClr2").c_str(), color2_arr_ng)) {
                    ng_settings.noiseColor2.set(color2_arr_ng[0] * 255, color2_arr_ng[1] * 255, color2_arr_ng[2] * 255, color2_arr_ng[3] * 255);
                    current_noise_generator_preset_index = -1;
                }
                ImGui::Unindent();
            }
            ImGui::Unindent();
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Pixel Sorting##PixelSortEffect", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            PixelSortSettings& ps_settings = this->pixel_sort_settings;
            ImGui::Separator(); ImGui::Text("Presets"); ImGui::Indent();
            std::vector<const char*> preset_cnames_ps; for(const auto& name : pixel_sort_preset_names) { preset_cnames_ps.push_back(name.c_str()); }
            if (preset_cnames_ps.empty()) { ImGui::Text("No presets available."); }
            else {
                int temp_idx_ps = current_pixel_sort_preset_index;
                if (ImGui::Combo(gen_ui_id("PSPresetCombo", -1, "PSCmb").c_str(), &temp_idx_ps, preset_cnames_ps.data(), preset_cnames_ps.size())) {
                    if (temp_idx_ps >= 0) applyPixelSortPreset(temp_idx_ps);
                }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Load a preset.");
                ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
                if (current_pixel_sort_preset_index != -1 && static_cast<size_t>(current_pixel_sort_preset_index) < pixel_sort_preset_names.size()) {
                     ImGui::TextDisabled(("(Active: " + pixel_sort_preset_names[current_pixel_sort_preset_index] + ")").c_str());
                } else if (!pixel_sort_presets.empty()) { ImGui::TextDisabled("(Custom Settings)");}

                if (current_pixel_sort_preset_index != -1) {
                    ImGui::SameLine(); if (ImGui::Button(gen_ui_id("PSDelPreset", -1, "PSBtnDel").c_str())) { deletePixelSortPreset(current_pixel_sort_preset_index); }
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete selected preset.");
                }
            }
            ImGui::InputText(gen_ui_id("PSNewPresetName", -1, "PSInTxt").c_str(), pixel_sort_new_preset_name_buffer, IM_ARRAYSIZE(pixel_sort_new_preset_name_buffer)); ImGui::SameLine();
            if (ImGui::Button(gen_ui_id("PSSavePreset", -1, "PSBtnSave").c_str())) { addCurrentSettingsAsPixelSortPreset(std::string(pixel_sort_new_preset_name_buffer)); }
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save current settings as new preset.");
            ImGui::Unindent(); ImGui::Separator();

            if(ImGui::Checkbox(gen_ui_id("EnablePS", -1, "PSCb").c_str(), &ps_settings.enablePixelSort)) { current_pixel_sort_preset_index = -1; }
            ImGui::Text("Configuration");
            const char* input_sources_ps[] = { "Main Output", "Cam 1", "Cam 2", "NDI", "Video File", "Window Capture", "Noise Generator", "Reaction-Diffusion" };
            int current_source_idx_ps = (ps_settings.inputSource >= 0 && ps_settings.inputSource < IM_ARRAYSIZE(input_sources_ps)) ? ps_settings.inputSource : 0;
            if (ImGui::Combo(gen_ui_id("PSInputSrc", -1, "PSCmb").c_str(), &current_source_idx_ps, input_sources_ps, IM_ARRAYSIZE(input_sources_ps))) {
                ps_settings.inputSource = current_source_idx_ps; current_pixel_sort_preset_index = -1;
            }
            const char* sort_modes[] = { "Horizontal Lines", "Vertical Columns" };
            int current_sort_mode_idx = (ps_settings.sortMode >= 0 && ps_settings.sortMode < IM_ARRAYSIZE(sort_modes)) ? ps_settings.sortMode : 0;
            if (ImGui::Combo(gen_ui_id("PSSortMode", -1, "PSCmb").c_str(), &current_sort_mode_idx, sort_modes, IM_ARRAYSIZE(sort_modes))) {
                ps_settings.sortMode = current_sort_mode_idx; current_pixel_sort_preset_index = -1;
            }
            const char* sort_criteria[] = { "Brightness", "Hue", "Red", "Green", "Blue", "Luminance" };
            int current_criteria_idx = (ps_settings.sortCriteria >= 0 && ps_settings.sortCriteria < IM_ARRAYSIZE(sort_criteria)) ? ps_settings.sortCriteria : 0;
            if (ImGui::Combo(gen_ui_id("PSSortCrit", -1, "PSCmb").c_str(), &current_criteria_idx, sort_criteria, IM_ARRAYSIZE(sort_criteria))) {
                ps_settings.sortCriteria = current_criteria_idx; current_pixel_sort_preset_index = -1;
            }
            if(ImGui::SliderFloat(gen_ui_id("PSThreshMin", -1, "PSSldF").c_str(), &ps_settings.thresholdMin, 0.0f, 1.0f, "%.2f")) { current_pixel_sort_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("PSThreshMax", -1, "PSSldF").c_str(), &ps_settings.thresholdMax, 0.0f, 1.0f, "%.2f")) { current_pixel_sort_preset_index = -1; }
            if(ImGui::Checkbox(gen_ui_id("PSSortAsc", -1, "PSCb").c_str(), &ps_settings.sortAscending)) { current_pixel_sort_preset_index = -1; }
            if(ImGui::Checkbox(gen_ui_id("PSSmartThresh", -1, "PSCb").c_str(), &ps_settings.smartThresholding)) { current_pixel_sort_preset_index = -1; }
            if(ImGui::SliderInt(gen_ui_id("PSSortWinSize", -1, "PSSldI").c_str(), &ps_settings.sortWindowSize, 3, 15)) { current_pixel_sort_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("PSEffectMix", -1, "PSSldF").c_str(), &ps_settings.effectMix, 0.0f, 1.0f, "%.2f")) { current_pixel_sort_preset_index = -1; }
            ImGui::Unindent();
        }

        ImGui::Separator();
        if (ImGui::CollapsingHeader("Reaction-Diffusion##RDEffect", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            ReactionDiffusionSettings& rd_settings = this->reaction_diffusion_settings;
            ImGui::Separator(); ImGui::Text("Presets"); ImGui::Indent();
            std::vector<const char*> preset_cnames_rd; for(const auto& name : rd_preset_names) { preset_cnames_rd.push_back(name.c_str()); }
            if (preset_cnames_rd.empty()) { ImGui::Text("No presets available."); }
            else {
                int temp_preset_idx_rd = current_rd_preset_index;
                if (ImGui::Combo(gen_ui_id("RDPresetCombo", -1, "RDCmb").c_str(), &temp_preset_idx_rd, preset_cnames_rd.data(), preset_cnames_rd.size())) {
                    if (temp_preset_idx_rd >= 0) applyRDPreset(temp_preset_idx_rd);
                }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Load a preset.");
                ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
                if (current_rd_preset_index != -1 && static_cast<size_t>(current_rd_preset_index) < rd_preset_names.size()) {
                     ImGui::TextDisabled(("(Active: " + rd_preset_names[current_rd_preset_index] + ")").c_str());
                } else if (!rd_presets.empty()) { ImGui::TextDisabled("(Custom Settings)");}

                if (current_rd_preset_index != -1) {
                    ImGui::SameLine(); if (ImGui::Button(gen_ui_id("RDDelPreset", -1, "RDBtnDel").c_str())) { deleteRDPreset(current_rd_preset_index); }
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete selected preset.");
                }
            }
            ImGui::InputText(gen_ui_id("RDPresetName", -1, "RDInTxt").c_str(), rd_new_preset_name_buffer, IM_ARRAYSIZE(rd_new_preset_name_buffer)); ImGui::SameLine();
            if (ImGui::Button(gen_ui_id("RDSavePreset", -1, "RDBtnSave").c_str())) { addCurrentSettingsAsRDPreset(std::string(rd_new_preset_name_buffer));}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save current settings as new preset.");
            ImGui::Unindent(); ImGui::Separator();

            if(ImGui::Checkbox(gen_ui_id("EnableRD", -1, "RDCb").c_str(), &rd_settings.enableRD)) { current_rd_preset_index = -1; }
            ImGui::Text("Simulation Parameters");
            if(ImGui::SliderFloat(gen_ui_id("RDFeed", -1, "RDSldF").c_str(), &rd_settings.feedRate, 0.0f, 0.1f, "%.4f")) { current_rd_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("RDKill", -1, "RDSldF").c_str(), &rd_settings.killRate, 0.0f, 0.1f, "%.4f")) { current_rd_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("RDDiffU", -1, "RDSldF").c_str(), &rd_settings.diffuseRateU, 0.0f, 2.0f, "%.2f")) { current_rd_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("RDDiffV", -1, "RDSldF").c_str(), &rd_settings.diffuseRateV, 0.0f, 2.0f, "%.2f")) { current_rd_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("RDTimeStep", -1, "RDSldF").c_str(), &rd_settings.timeStep, 0.1f, 2.0f, "%.2f")) { current_rd_preset_index = -1; }
            if(ImGui::SliderInt(gen_ui_id("RDIterations", -1, "RDSldI").c_str(), &rd_settings.iterationsPerFrame, 1, 50)) { current_rd_preset_index = -1; }
            ImGui::Text("Canvas & Seeding");
            if (ImGui::Button(gen_ui_id("RDClearSeed", -1, "RDBtn").c_str())) { rd_settings.clearCanvasTrigger = true; }
            const char* seed_patterns[] = { "Center Spot", "Random Noise Patch", "Full Random Noise", "Mouse Click Area" };
            int current_seed_idx = rd_settings.seedPattern;
            if (ImGui::Combo(gen_ui_id("RDSeedPattern", -1, "RDCmb").c_str(), &current_seed_idx, seed_patterns, IM_ARRAYSIZE(seed_patterns))) {
                rd_settings.seedPattern = current_seed_idx; current_rd_preset_index = -1;
            }
            if (rd_settings.seedPattern == 0 || rd_settings.seedPattern == 3) {
               if(ImGui::SliderFloat(gen_ui_id("RDSeedRadius", -1, "RDSldF").c_str(), &rd_settings.seedRadius, 1.0f, 50.0f, "%.0f px")) { current_rd_preset_index = -1; }
            } else if (rd_settings.seedPattern == 1) {
               if(ImGui::SliderFloat(gen_ui_id("RDSeedNoiseDensity", -1, "RDSldF").c_str(), &rd_settings.seedNoiseDensity, 0.01f, 1.0f, "%.2f")) { current_rd_preset_index = -1; }
            }
            ImGui::Text("Display & Output");
            const char* color_modes[] = { "U as Grayscale", "V as Grayscale", "U/V to Colors" };
            int current_cm_idx = rd_settings.colorMode;
            if (ImGui::Combo(gen_ui_id("RDColorMode", -1, "RDCmb").c_str(), &current_cm_idx, color_modes, IM_ARRAYSIZE(color_modes))) {
                rd_settings.colorMode = current_cm_idx; current_rd_preset_index = -1;
            }
            if (rd_settings.colorMode == 2) {
                ImGui::Indent();
                float cU[4] = {{ rd_settings.colorU.r/255.f, rd_settings.colorU.g/255.f, rd_settings.colorU.b/255.f, rd_settings.colorU.a/255.f }};
                if(ImGui::ColorEdit4(gen_ui_id("RDColorU",-1,"RDClrU").c_str(), cU)) { rd_settings.colorU.set(cU[0]*255,cU[1]*255,cU[2]*255,cU[3]*255); current_rd_preset_index = -1; }
                float cV[4] = {{ rd_settings.colorV.r/255.f, rd_settings.colorV.g/255.f, rd_settings.colorV.b/255.f, rd_settings.colorV.a/255.f }};
                if(ImGui::ColorEdit4(gen_ui_id("RDColorV",-1,"RDClrV").c_str(), cV)) { rd_settings.colorV.set(cV[0]*255,cV[1]*255,cV[2]*255,cV[3]*255); current_rd_preset_index = -1; }
                ImGui::Unindent();
            }
            if(ImGui::Checkbox(gen_ui_id("RDSmoothDisp",-1,"RDCb").c_str(), &rd_settings.smoothDisplay)) { current_rd_preset_index = -1; }
            if(ImGui::SliderFloat(gen_ui_id("RDOutputMix", -1, "RDSldF").c_str(), &rd_settings.outputMix, 0.0f, 1.0f)) { current_rd_preset_index = -1; }
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
                if (off_x < 0) off_x = 0; if (off_y < 0) off_y = 0;
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
