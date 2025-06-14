/*
 re the midi biz
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */

/*thanks to ed tannenbaum for help in fixing up the framebuffer code!*/

#include "ofApp.h"
#include <iostream>
// #include "ofVideoPlayer.h" // Already in ofApp.h

// Renamed to avoid conflict if fbob is a global in other files (it's a const int here, so not strictly necessary but good practice)
const int fbob_const = 30;

// Global LFO theta accumulators (legacy from original structure)
// These will be updated based on gui->struct.lfo.theta values
float ch1_theta_hue, ch1_theta_saturation, ch1_theta_bright;
float ch2_theta_hue, ch2_theta_saturation, ch2_theta_bright;

float fb0_theta_mix, fb0_theta_lumakeyvalue, fb0_theta_lumakeythreshold, fb0_theta_delay, fb0_theta_hue, fb0_theta_saturation, fb0_theta_bright, fb0_theta_huexmod, fb0_theta_huexoffset, fb0_theta_huexlfo, fb0_theta_x, fb0_theta_y, fb0_theta_z, fb0_theta_rotate;
float fb1_theta_mix, fb1_theta_lumakeyvalue, fb1_theta_lumakeythreshold, fb1_theta_delay, fb1_theta_hue, fb1_theta_saturation, fb1_theta_bright, fb1_theta_huexmod, fb1_theta_huexoffset, fb1_theta_huexlfo, fb1_theta_x, fb1_theta_y, fb1_theta_z, fb1_theta_rotate;
float fb2_theta_mix, fb2_theta_lumakeyvalue, fb2_theta_lumakeythreshold, fb2_theta_delay, fb2_theta_hue, fb2_theta_saturation, fb2_theta_bright, fb2_theta_huexmod, fb2_theta_huexoffset, fb2_theta_huexlfo, fb2_theta_x, fb2_theta_y, fb2_theta_z, fb2_theta_rotate;
float fb3_theta_mix, fb3_theta_lumakeyvalue, fb3_theta_lumakeythreshold, fb3_theta_delay, fb3_theta_hue, fb3_theta_saturation, fb3_theta_bright, fb3_theta_huexmod, fb3_theta_huexoffset, fb3_theta_huexlfo, fb3_theta_x, fb3_theta_y, fb3_theta_z, fb3_theta_rotate;

float blur_theta_amount, blur_theta_radius, sharpen_theta_amount, sharpen_theta_radius, sharpen_theta_boost;

// Pixel LFO thetas (legacy) - to be updated from structs in lfo_update
float cam1_theta_pixel_scale_x, cam1_theta_pixel_scale_y, cam1_theta_pixel_mix, cam1_theta_pixel_brightscale;
float cam2_theta_pixel_scale_x, cam2_theta_pixel_scale_y, cam2_theta_pixel_mix, cam2_theta_pixel_brightscale;
float ndi_theta_pixel_scale_x, ndi_theta_pixel_scale_y, ndi_theta_pixel_mix, ndi_theta_pixel_brightscale;
float fb0_theta_pixel_scale_x, fb0_theta_pixel_scale_y, fb0_theta_pixel_mix, fb0_theta_pixel_brightscale;
float fb1_theta_pixel_scale_x, fb1_theta_pixel_scale_y, fb1_theta_pixel_mix, fb1_theta_pixel_brightscale;
float fb2_theta_pixel_scale_x, fb2_theta_pixel_scale_y, fb2_theta_pixel_mix, fb2_theta_pixel_brightscale;
float fb3_theta_pixel_scale_x, fb3_theta_pixel_scale_y, fb3_theta_pixel_mix, fb3_theta_pixel_brightscale;


// MIDI controller variables (c1-c31, legacy)
float c1=0, c2=0, c3=0, c4=0, c5=0, c6=0, c7=0, c8=0, c9=0, c10=0, c11=0, c12=0, c13=0, c14=0, c15=0, c16=0;
float c17=0, c18=0, c19=0, c20=0, c21=0, c22=0, c23=0, c24=0, c25=0, c26=0, c27=0, c28=0, c29=0, c30=0, c31=0;

ofPolyline tetrahedron;
// Other legacy global variables (aa, ss, dd etc.) - these should ideally be moved into GuiApp or ofApp members if they are settings
float aa=0.0, ss=0.0, dd=0.0, ff=0.0, gg=0.0, hh=0.0; // Used in pastFrames transform
float qq=0.0, ee=0.0, oo=1.0; int ii=1; // Used in pastFrames transform and shader_sharpen

int framedelayoffset=0;
unsigned int framecount=0;
ofFbo pastFrames[fbob_const];

void incIndex() { framecount++; framedelayoffset = framecount % fbob_const; }

//--------------------------------------------------------------
void ofApp::setupVideoPlayer() {
    videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
    videoLoaded = false;
    videoPlaying = false;
    loadedVideoPathString = "No video loaded";
    if(gui) {
        gui->loadedVideoPath = loadedVideoPathString;
        gui->videoPlayingState = videoPlaying;
        // Initialize GUI loop state from video player's default (usually OF_LOOP_NONE)
        // or a desired default like true (OF_LOOP_NORMAL)
        gui->videoLoopingState = true; // Default to looping
        videoPlayer.setLoopState(gui->videoLoopingState ? OF_LOOP_NORMAL : OF_LOOP_NONE);
    }
}

void ofApp::handleVideoEvents() {
    if (!gui) return;

    if (gui->loadVideoTrigger) {
        loadVideoFromFile();
        gui->loadVideoTrigger = false;
    }
    if (gui->playVideoTrigger) {
        playVideo();
        gui->playVideoTrigger = false;
    }
    if (gui->pauseVideoTrigger) {
        pauseVideo();
        gui->pauseVideoTrigger = false;
    }
    if (gui->stopVideoTrigger) {
        stopVideo();
        gui->stopVideoTrigger = false;
    }
    if (gui->loopVideoToggleTrigger) {
        toggleLoop();
        gui->loopVideoToggleTrigger = false;
    }
}

void ofApp::loadVideoFromFile() {
    ofFileDialogResult result = ofSystemLoadDialog("Load video file");
    if (result.bSuccess) {
        loadedVideoPathString = result.getPath();
        videoLoaded = videoPlayer.load(loadedVideoPathString);
        if (videoLoaded) {
            videoPlayer.setLoopState(gui->videoLoopingState ? OF_LOOP_NORMAL : OF_LOOP_NONE);
            videoPlaying = true;
            videoPlayer.play();
            ofLogNotice("ofApp::loadVideoFromFile") << "Successfully loaded video: " << loadedVideoPathString;
        } else {
            ofLogError("ofApp::loadVideoFromFile") << "Failed to load video: " << loadedVideoPathString;
            loadedVideoPathString = "Failed to load video";
            videoPlaying = false;
        }
    }
}

void ofApp::playVideo() {
    if (videoLoaded && videoPlayer.isLoaded()) {
        videoPlayer.play();
        videoPlaying = true;
    }
}

void ofApp::pauseVideo() {
    if (videoLoaded && videoPlayer.isLoaded() && videoPlayer.isPlaying()) {
        videoPlayer.setPaused(true);
        videoPlaying = false;
    }
}

void ofApp::stopVideo() {
    if (videoLoaded && videoPlayer.isLoaded()) {
        videoPlayer.stop();
        videoPlaying = false;
    }
}

void ofApp::toggleLoop() {
    if (videoLoaded && videoPlayer.isLoaded()) {
        videoPlayer.setLoopState(gui->videoLoopingState ? OF_LOOP_NORMAL : OF_LOOP_NONE);
    }
}

void ofApp::updateVideoPlayerData() {
    if (!gui) return;
    gui->loadedVideoPath = loadedVideoPathString;
    if (videoLoaded && videoPlayer.isLoaded()) {
        gui->videoPlayingState = videoPlayer.isPlaying();
        gui->videoLoopingState = (videoPlayer.getLoopState() == OF_LOOP_NORMAL);
    } else {
        gui->videoPlayingState = false;
    }
}

// Window Capture Placeholders
void ofApp::handleWindowCaptureEvents() {
    if (!gui) return;
    if (gui->listWindowsTrigger) { listAvailableWindows(); gui->listWindowsTrigger = false; }
    if (gui->startCaptureTrigger) {
        if (gui->selectedWindowIndex >= 0 && gui->selectedWindowIndex < gui->availableWindowTitles.size()) {
             startWindowCapture(gui->selectedWindowIndex);
        } else {
            ofLogWarning("ofApp") << "No valid window selected for capture.";
        }
        gui->startCaptureTrigger = false;
    }
    if (gui->stopCaptureTrigger) { stopWindowCapture(); gui->stopCaptureTrigger = false; }
}
void ofApp::listAvailableWindows() {
    if (!gui) return;
    // Placeholder: In a real app, this would query the OS for window titles.
    gui->availableWindowTitles.clear();
    gui->availableWindowTitles.push_back("Dummy Window 1 (Example)");
    gui->availableWindowTitles.push_back("Another Application (Example)");
    gui->availableWindowTitles.push_back("Desktop (Example)");
    if (gui->availableWindowTitles.empty()) {
        gui->availableWindowTitles.push_back("No windows found");
    }
    gui->selectedWindowIndex = 0; // Reset selection
    ofLogNotice("ofApp") << "Placeholder: listAvailableWindows() called.";
}
void ofApp::startWindowCapture(int windowIndex) {
    if (!gui) return;
    isCapturingWindow = true;
    currentCaptureTargetIndex = windowIndex;
    if (windowIndex >= 0 && windowIndex < gui->availableWindowTitles.size()) {
        gui->capturedWindowTitleDisplay = gui->availableWindowTitles[windowIndex];
    } else {
        gui->capturedWindowTitleDisplay = "Error: Invalid Index";
    }
    ofLogNotice("ofApp") << "Placeholder: startWindowCapture() for: " << gui->capturedWindowTitleDisplay;
    // Actual capture implementation would start here (e.g. using ofWindowGrabber or platform specific code)
}
void ofApp::stopWindowCapture() {
    if (!gui) return;
    isCapturingWindow = false;
    currentCaptureTargetIndex = -1;
    gui->capturedWindowTitleDisplay = "None";
    ofLogNotice("ofApp") << "Placeholder: stopWindowCapture() called.";
    // Actual capture implementation would stop here
}
void ofApp::updateWindowCaptureData() {
    if (!gui) return;
    gui->isWindowCurrentlyCapturing = isCapturingWindow;
    // If capturing, this is where you'd grab a frame and put it into an ofTexture/ofImage
    // e.g., if (isCapturingWindow) { /* capturedFrame = grabber.grab(); */ }
    // Then this texture would be passed to gui->drawOutputWindow or used in ndi_fbo
}


void ofApp::setup() {
    ofSetFrameRate(30); ofSetVerticalSync(true); ofBackground(0);
	midiIn.listInPorts(); midiIn.openPort(1); midiIn.ignoreTypes(false, false, false); midiIn.addListener(this); midiIn.setVerbose(true);
    NDI_reciever_setup("Scan Converter");

#ifdef TARGET_OPENGLES
    shader_mixer.load("shadersES2/shader_mixer"); // Assuming ES2 shaders exist
    shader_blur.load("shadersES2/shader_blur");
    shader_sharpen.load("shadersES2/shader_sharpen");
#else
    if(ofIsGLProgrammableRenderer()){
        // This path might not be used if only GL2 specified. Consider revising shader loading.
        shader_mixer.load("shadersGL3/shader_mixer");
        shader_blur.load("shadersGL3/shader_blur");
        shader_sharpen.load("shadersGL3/shader_sharpen");
    }else{
        shader_mixer.load("shadersGL2/shader_mixer");
        shader_blur.load("shadersGL2/shader_blur");
        shader_sharpen.load("shadersGL2/shader_sharpen");
    }
#endif

    fbo_draw.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA); // Added GL_RGBA for alpha
    fbo_feedback.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    fbo_blur.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    ndi_fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    fbo_feedback.begin(); ofClear(0,0,0,0); fbo_feedback.end(); // Clear with alpha
    fbo_draw.begin(); ofClear(0,0,0,0); fbo_draw.end();
    fbo_blur.begin(); ofClear(0,0,0,0); fbo_blur.end();
    ndi_fbo.begin(); ofClear(0,0,0,0); ndi_fbo.end();

    for(int i=0;i<fbob_const;i++){ pastFrames[i].allocate(ofGetWidth(), ofGetHeight(), GL_RGBA); pastFrames[i].begin(); ofClear(0,0,0,0); pastFrames[i].end(); }

    cam1.setVerbose(true); cam1.listDevices(); cam1.setDeviceID(0); cam1.setup(640, 480);
    cam2.setDeviceID(1); cam2.setup(640, 480);

    tetrahedron_setup();
    setupVideoPlayer();
    isCapturingWindow = false; currentCaptureTargetIndex = -1; // Init window capture state
}

void ofApp::update() {
    cam1.update(); cam2.update();
    midibiz();
    NDI_reciever_update(); // This now also handles video player drawing to ndi_fbo
    lfo_update();

    handleVideoEvents();
    if (videoLoaded && videoPlayer.isLoaded()) { videoPlayer.update(); }
    updateVideoPlayerData();

    handleWindowCaptureEvents();
    updateWindowCaptureData();

    if(gui && gui->global_settings.framebuffer_clear_trigger){
        for(int i=0;i<fbob_const;i++){ pastFrames[i].begin(); ofClear(0,0,0,255); pastFrames[i].end(); }
        fbo_draw.begin(); ofClear(0,0,0,255); fbo_draw.end();
        gui->global_settings.framebuffer_clear_trigger = false;
    }
     if(gui && gui->global_settings.control_reset_trigger){
        ofLogNotice("ofApp") << "Control Reset Triggered (logic not implemented in detail)";
        // Here you would reset all relevant variables in ofApp and GuiApp to defaults
        gui->global_settings.control_reset_trigger = false;
    }
}

void ofApp::draw() {
    fbo_draw.begin();
    ofClear(0,0,0,0); // Clear with alpha for proper blending if video/NDI is not full screen

    shader_mixer.begin();
    fbo_feedback.draw(0,0);

    // ndi_fbo contains video if playing, or NDI input.
    // If isCapturingWindow is true, it could also contain the window capture.
    // This logic needs to be in NDI_reciever_update or a new function.
    shader_mixer.setUniformTexture("ndi", ndi_fbo.getTexture(), 1);
    shader_mixer.setUniformTexture("cam1", cam1.getTexture(), 2);
    shader_mixer.setUniformTexture("cam2", cam2.getTexture(), 3);

    if (!gui) { shader_mixer.end(); fbo_draw.end(); if(gui) gui->drawOutputWindow(&fbo_draw.getTexture()); return; } // Early exit if gui is null

    // Pass videoPlayer texture to shader if loaded, otherwise pass a black texture or cam1/cam2 as fallback
    // This specific uniform "videoSource" might not be used if ndi_fbo handles video.
    // if (videoLoaded && videoPlayer.isAllocated()) {
    //      shader_mixer.setUniformTexture("videoSource", videoPlayer.getTexture(), 10);
    // } else {
    //      shader_mixer.setUniformTexture("videoSource", cam1.getTexture(), 10);
    // }
    // shader_mixer.setUniform1i("useVideoSource", (videoLoaded && videoPlayer.isAllocated()) ? 1 : 0);

    shader_mixer.setUniformTexture("fb0",pastFrames[(abs(framedelayoffset-fbob_const-gui->feedback_settings[0].delay_amount-(int)(fbob_const*c8))-abs((int)(fbob_const*lfo(gui->feedback_settings[0].delay_lfo.amp,fb0_theta_delay,0))))-1)%fbob_const].getTexture(),4);
    shader_mixer.setUniformTexture("fb1",pastFrames[(abs(framedelayoffset-fbob_const-gui->feedback_settings[1].delay_amount-(int)(fbob_const*c21))-abs((int)(fbob_const*lfo(gui->feedback_settings[1].delay_lfo.amp,fb1_theta_delay,0))))-1)%fbob_const].getTexture(),5);
    shader_mixer.setUniformTexture("fb2",pastFrames[(abs(framedelayoffset-fbob_const-gui->feedback_settings[2].delay_amount-abs((int)(fbob_const*lfo(gui->feedback_settings[2].delay_lfo.amp,fb2_theta_delay,0))))-1)%fbob_const].getTexture(),6);
    shader_mixer.setUniformTexture("fb3",pastFrames[(abs(framedelayoffset-fbob_const-gui->feedback_settings[3].delay_amount-abs((int)(fbob_const*lfo(gui->feedback_settings[3].delay_lfo.amp,fb3_theta_delay,0))))-1)%fbob_const].getTexture(),7);

    shader_mixer.setUniform1f("width", ofGetWidth()); shader_mixer.setUniform1f("height", ofGetHeight());
    shader_mixer.setUniform1f("cam1_scale", gui->global_settings.cam1_scale); shader_mixer.setUniform1f("cam2_scale", gui->global_settings.cam2_scale);

    ofVec3f hsb_x_vec;
    ofVec3f hue_x_params_vec;
    ofVec3f fb_modswitch_vec;

    // fb0
    shader_mixer.setUniform1f("fb0lumakeyvalue", gui->feedback_settings[0].key_value+c6+lfo(gui->feedback_settings[0].lumakeyvalue_lfo.amp,fb0_theta_lumakeyvalue,0));
    shader_mixer.setUniform1f("fb0lumakeythresh", gui->feedback_settings[0].key_threshold+lfo(gui->feedback_settings[0].lumakeythreshold_lfo.amp,fb0_theta_lumakeythreshold,0));
    shader_mixer.setUniform1f("fb0blend", gui->feedback_settings[0].mix+lfo(gui->feedback_settings[0].mix_lfo.amp,fb0_theta_mix,0)+c7*2.0f);
    shader_mixer.setUniform1i("fb0_toroid_switch", gui->feedback_settings[0].toroid_switch);
    hsb_x_vec.set((gui->feedback_settings[0].hue+lfo(gui->feedback_settings[0].hue_lfo.amp,fb0_theta_hue,0))/10+c13*.2f, (gui->feedback_settings[0].saturation+lfo(gui->feedback_settings[0].saturation_lfo.amp,fb0_theta_saturation,0))/10+c14*.2f, (gui->feedback_settings[0].bright+lfo(gui->feedback_settings[0].bright_lfo.amp,fb0_theta_bright,0))/10+c15*.2f);
    shader_mixer.setUniform3f("fb0_hsb_x",hsb_x_vec);
    hue_x_params_vec.set((gui->feedback_settings[0].huex_mod+lfo(gui->feedback_settings[0].huexmod_lfo.amp,fb0_theta_huexmod,0))/10-c16, (gui->feedback_settings[0].huex_offset+lfo(gui->feedback_settings[0].huexoffset_lfo.amp,fb0_theta_huexoffset,0))/10+c17*2.0f, (gui->feedback_settings[0].huex_lfo_val+lfo(gui->feedback_settings[0].huexlfo_val_lfo.amp,fb0_theta_huexlfo,0))/10+c18*2.0f);
    shader_mixer.setUniform3f("fb0_hue_x",hue_x_params_vec);
    fb_modswitch_vec.set(gui->feedback_settings[0].hue_invert,gui->feedback_settings[0].saturation_invert,gui->feedback_settings[0].bright_invert);
    shader_mixer.setUniform3f("fb0_modswitch",fb_modswitch_vec);
    shader_mixer.setUniform3f("fb0_rescale",ofVec3f(gui->feedback_settings[0].x_displace+lfo(gui->feedback_settings[0].x_lfo.amp,fb0_theta_x,0)+c9*40.0f, gui->feedback_settings[0].y_displace+lfo(gui->feedback_settings[0].y_lfo.amp,fb0_theta_y,0)+c10*40.0f, gui->feedback_settings[0].z_displace/100.0f+lfo(gui->feedback_settings[0].z_lfo.amp,fb0_theta_z,0)/100.0f+c11));
    shader_mixer.setUniform1f("fb0_rotate",(gui->feedback_settings[0].rotate)+lfo(gui->feedback_settings[0].rotate_lfo.amp,fb0_theta_rotate,0)+c12);
    shader_mixer.setUniform1f("tex_fb0lumakeyvalue", gui->feedback_settings[0].tex_key_value);
    shader_mixer.setUniform1f("tex_fb0lumakeythresh", gui->feedback_settings[0].tex_key_threshold);
    shader_mixer.setUniform1f("tex_fb0blend", gui->feedback_settings[0].tex_mix);
    shader_mixer.setUniform3f("tex_fb0_hsb_x",ofVec3f(gui->feedback_settings[0].tex_hue,gui->feedback_settings[0].tex_saturation,gui->feedback_settings[0].tex_bright));
    shader_mixer.setUniform3f("tex_fb0_hue_x",ofVec3f(gui->feedback_settings[0].tex_huex_mod,gui->feedback_settings[0].tex_huex_offset,gui->feedback_settings[0].tex_huex_lfo_val));
    shader_mixer.setUniform3f("tex_fb0_rescale",ofVec3f(gui->feedback_settings[0].tex_x_displace, gui->feedback_settings[0].tex_y_displace, gui->feedback_settings[0].tex_z_displace/100.0f));
    shader_mixer.setUniform1f("tex_fb0_rotate",(gui->feedback_settings[0].tex_rotate));
    ofVec2f fb0_texmod_logic_vec_shader; if(gui->feedback_settings[0].texmod_select==1){ fb0_texmod_logic_vec_shader.set(1,0); } else if(gui->feedback_settings[0].texmod_select==2){ fb0_texmod_logic_vec_shader.set(0,1); } else {fb0_texmod_logic_vec_shader.set(0,0);} shader_mixer.setUniform2f("fb0_texmod_logic",fb0_texmod_logic_vec_shader);

    // fb1
    shader_mixer.setUniform1f("fb1lumakeyvalue", gui->feedback_settings[1].key_value+c19+lfo(gui->feedback_settings[1].lumakeyvalue_lfo.amp,fb1_theta_lumakeyvalue,0));
    shader_mixer.setUniform1f("fb1lumakeythresh", gui->feedback_settings[1].key_threshold+lfo(gui->feedback_settings[1].lumakeythreshold_lfo.amp,fb1_theta_lumakeythreshold,0));
    shader_mixer.setUniform1f("fb1blend", gui->feedback_settings[1].mix+lfo(gui->feedback_settings[1].mix_lfo.amp,fb1_theta_mix,0)+c20*2.0f);
    shader_mixer.setUniform1i("fb1_toroid_switch", gui->feedback_settings[1].toroid_switch);
    hsb_x_vec.set((gui->feedback_settings[1].hue+lfo(gui->feedback_settings[1].hue_lfo.amp,fb1_theta_hue,0))/10+c26*.2f, (gui->feedback_settings[1].saturation+lfo(gui->feedback_settings[1].saturation_lfo.amp,fb1_theta_saturation,0))/10+c27*.2f, (gui->feedback_settings[1].bright+lfo(gui->feedback_settings[1].bright_lfo.amp,fb1_theta_bright,0))/10+c28*.2f);
    shader_mixer.setUniform3f("fb1_hsb_x",hsb_x_vec);
    hue_x_params_vec.set((gui->feedback_settings[1].huex_mod+lfo(gui->feedback_settings[1].huexmod_lfo.amp,fb1_theta_huexmod,0))/10-c29, (gui->feedback_settings[1].huex_offset+lfo(gui->feedback_settings[1].huexoffset_lfo.amp,fb1_theta_huexoffset,0))/10+c30*2.0f, (gui->feedback_settings[1].huex_lfo_val+lfo(gui->feedback_settings[1].huexlfo_val_lfo.amp,fb1_theta_huexlfo,0))/10+c31*2.0f);
    shader_mixer.setUniform3f("fb1_hue_x",hue_x_params_vec);
    fb_modswitch_vec.set(gui->feedback_settings[1].hue_invert,gui->feedback_settings[1].saturation_invert,gui->feedback_settings[1].bright_invert);
    shader_mixer.setUniform3f("fb1_modswitch",fb_modswitch_vec);
    shader_mixer.setUniform3f("fb1_rescale",ofVec3f(gui->feedback_settings[1].x_displace+lfo(gui->feedback_settings[1].x_lfo.amp,fb1_theta_x,0)+c22*40.0f, gui->feedback_settings[1].y_displace+lfo(gui->feedback_settings[1].y_lfo.amp,fb1_theta_y,0)+c23*40.0f, gui->feedback_settings[1].z_displace/100.0f+lfo(gui->feedback_settings[1].z_lfo.amp,fb1_theta_z,0)/100.0f+c24));
    shader_mixer.setUniform1f("fb1_rotate",(gui->feedback_settings[1].rotate)+lfo(gui->feedback_settings[1].rotate_lfo.amp,fb1_theta_rotate,0)+c25*6.18f);
    shader_mixer.setUniform1f("tex_fb1lumakeyvalue", gui->feedback_settings[1].tex_key_value);
    shader_mixer.setUniform1f("tex_fb1lumakeythresh", gui->feedback_settings[1].tex_key_threshold);
    shader_mixer.setUniform1f("tex_fb1blend", gui->feedback_settings[1].tex_mix);
    shader_mixer.setUniform3f("tex_fb1_hsb_x",ofVec3f(gui->feedback_settings[1].tex_hue,gui->feedback_settings[1].tex_saturation,gui->feedback_settings[1].tex_bright));
    shader_mixer.setUniform3f("tex_fb1_hue_x",ofVec3f(gui->feedback_settings[1].tex_huex_mod,gui->feedback_settings[1].tex_huex_offset,gui->feedback_settings[1].tex_huex_lfo_val));
    shader_mixer.setUniform3f("tex_fb1_rescale",ofVec3f(gui->feedback_settings[1].tex_x_displace, gui->feedback_settings[1].tex_y_displace, gui->feedback_settings[1].tex_z_displace/100.0f));
    shader_mixer.setUniform1f("tex_fb1_rotate",(gui->feedback_settings[1].tex_rotate));
    ofVec2f fb1_texmod_logic_vec_shader; if(gui->feedback_settings[1].texmod_select==1){ fb1_texmod_logic_vec_shader.set(1,0); } else if(gui->feedback_settings[1].texmod_select==2){ fb1_texmod_logic_vec_shader.set(0,1); }  else {fb1_texmod_logic_vec_shader.set(0,0);} shader_mixer.setUniform2f("fb1_texmod_logic",fb1_texmod_logic_vec_shader);

    // fb2
    shader_mixer.setUniform1f("fb2lumakeyvalue", gui->feedback_settings[2].key_value+lfo(gui->feedback_settings[2].lumakeyvalue_lfo.amp,fb2_theta_lumakeyvalue,0));
    shader_mixer.setUniform1f("fb2lumakeythresh", gui->feedback_settings[2].key_threshold+lfo(gui->feedback_settings[2].lumakeythreshold_lfo.amp,fb2_theta_lumakeythreshold,0));
    shader_mixer.setUniform1f("fb2blend", gui->feedback_settings[2].mix+lfo(gui->feedback_settings[2].mix_lfo.amp,fb2_theta_mix,0));
    shader_mixer.setUniform1i("fb2_toroid_switch", gui->feedback_settings[2].toroid_switch);
    hsb_x_vec.set((gui->feedback_settings[2].hue+lfo(gui->feedback_settings[2].hue_lfo.amp,fb2_theta_hue,0))/10, (gui->feedback_settings[2].saturation+lfo(gui->feedback_settings[2].saturation_lfo.amp,fb2_theta_saturation,0))/10, (gui->feedback_settings[2].bright+lfo(gui->feedback_settings[2].bright_lfo.amp,fb2_theta_bright,0))/10);
    shader_mixer.setUniform3f("fb2_hsb_x",hsb_x_vec);
    hue_x_params_vec.set((gui->feedback_settings[2].huex_mod+lfo(gui->feedback_settings[2].huexmod_lfo.amp,fb2_theta_huexmod,0))/10, (gui->feedback_settings[2].huex_offset+lfo(gui->feedback_settings[2].huexoffset_lfo.amp,fb2_theta_huexoffset,0))/10, (gui->feedback_settings[2].huex_lfo_val+lfo(gui->feedback_settings[2].huexlfo_val_lfo.amp,fb2_theta_huexlfo,0))/10);
    shader_mixer.setUniform3f("fb2_hue_x",hue_x_params_vec);
    fb_modswitch_vec.set(gui->feedback_settings[2].hue_invert,gui->feedback_settings[2].saturation_invert,gui->feedback_settings[2].bright_invert);
    shader_mixer.setUniform3f("fb2_modswitch",fb_modswitch_vec);
    shader_mixer.setUniform3f("fb2_rescale",ofVec3f(gui->feedback_settings[2].x_displace+lfo(gui->feedback_settings[2].x_lfo.amp,fb2_theta_x,0), gui->feedback_settings[2].y_displace+lfo(gui->feedback_settings[2].y_lfo.amp,fb2_theta_y,0), gui->feedback_settings[2].z_displace/100.0f+lfo(gui->feedback_settings[2].z_lfo.amp,fb2_theta_z,0)/100.0f));
    shader_mixer.setUniform1f("fb2_rotate",(gui->feedback_settings[2].rotate)+lfo(gui->feedback_settings[2].rotate_lfo.amp,fb2_theta_rotate,0));
    shader_mixer.setUniform1f("tex_fb2lumakeyvalue", gui->feedback_settings[2].tex_key_value);
    shader_mixer.setUniform1f("tex_fb2lumakeythresh", gui->feedback_settings[2].tex_key_threshold);
    shader_mixer.setUniform1f("tex_fb2blend", gui->feedback_settings[2].tex_mix);
    shader_mixer.setUniform3f("tex_fb2_hsb_x",ofVec3f(gui->feedback_settings[2].tex_hue,gui->feedback_settings[2].tex_saturation,gui->feedback_settings[2].tex_bright));
    shader_mixer.setUniform3f("tex_fb2_hue_x",ofVec3f(gui->feedback_settings[2].tex_huex_mod,gui->feedback_settings[2].tex_huex_offset,gui->feedback_settings[2].tex_huex_lfo_val));
    shader_mixer.setUniform3f("tex_fb2_rescale",ofVec3f(gui->feedback_settings[2].tex_x_displace, gui->feedback_settings[2].tex_y_displace, gui->feedback_settings[2].tex_z_displace/100.0f));
    shader_mixer.setUniform1f("tex_fb2_rotate",(gui->feedback_settings[2].tex_rotate));
    ofVec2f fb2_texmod_logic_vec_shader; if(gui->feedback_settings[2].texmod_select==1){ fb2_texmod_logic_vec_shader.set(1,0); } else if(gui->feedback_settings[2].texmod_select==2){ fb2_texmod_logic_vec_shader.set(0,1); } else {fb2_texmod_logic_vec_shader.set(0,0);} shader_mixer.setUniform2f("fb2_texmod_logic",fb2_texmod_logic_vec_shader);

    // fb3
    shader_mixer.setUniform1f("fb3lumakeyvalue", gui->feedback_settings[3].key_value+lfo(gui->feedback_settings[3].lumakeyvalue_lfo.amp,fb3_theta_lumakeyvalue,0));
    shader_mixer.setUniform1f("fb3lumakeythresh", gui->feedback_settings[3].key_threshold+lfo(gui->feedback_settings[3].lumakeythreshold_lfo.amp,fb3_theta_lumakeythreshold,0));
    shader_mixer.setUniform1f("fb3blend", gui->feedback_settings[3].mix+lfo(gui->feedback_settings[3].mix_lfo.amp,fb3_theta_mix,0));
    shader_mixer.setUniform1i("fb3_toroid_switch", gui->feedback_settings[3].toroid_switch);
    hsb_x_vec.set((gui->feedback_settings[3].hue+lfo(gui->feedback_settings[3].hue_lfo.amp,fb3_theta_hue,0))/10, (gui->feedback_settings[3].saturation+lfo(gui->feedback_settings[3].saturation_lfo.amp,fb3_theta_saturation,0))/10, (gui->feedback_settings[3].bright+lfo(gui->feedback_settings[3].bright_lfo.amp,fb3_theta_bright,0))/10);
    shader_mixer.setUniform3f("fb3_hsb_x",hsb_x_vec);
    hue_x_params_vec.set((gui->feedback_settings[3].huex_mod+lfo(gui->feedback_settings[3].huexmod_lfo.amp,fb3_theta_huexmod,0))/10, (gui->feedback_settings[3].huex_offset+lfo(gui->feedback_settings[3].huexoffset_lfo.amp,fb3_theta_huexoffset,0))/10, (gui->feedback_settings[3].huex_lfo_val+lfo(gui->feedback_settings[3].huexlfo_val_lfo.amp,fb3_theta_huexlfo,0))/10);
    shader_mixer.setUniform3f("fb3_hue_x",hue_x_params_vec);
    fb_modswitch_vec.set(gui->feedback_settings[3].hue_invert,gui->feedback_settings[3].saturation_invert,gui->feedback_settings[3].bright_invert);
    shader_mixer.setUniform3f("fb3_modswitch",fb_modswitch_vec);
    shader_mixer.setUniform3f("fb3_rescale",ofVec3f(gui->feedback_settings[3].x_displace+lfo(gui->feedback_settings[3].x_lfo.amp,fb3_theta_x,0), gui->feedback_settings[3].y_displace+lfo(gui->feedback_settings[3].y_lfo.amp,fb3_theta_y,0), gui->feedback_settings[3].z_displace/100.0f+lfo(gui->feedback_settings[3].z_lfo.amp,fb3_theta_z,0)/100.0f));
    shader_mixer.setUniform1f("fb3_rotate",(gui->feedback_settings[3].rotate)+lfo(gui->feedback_settings[3].rotate_lfo.amp,fb3_theta_rotate,0));
    shader_mixer.setUniform1f("tex_fb3lumakeyvalue", gui->feedback_settings[3].tex_key_value);
    shader_mixer.setUniform1f("tex_fb3lumakeythresh", gui->feedback_settings[3].tex_key_threshold);
    shader_mixer.setUniform1f("tex_fb3blend", gui->feedback_settings[3].tex_mix);
    shader_mixer.setUniform3f("tex_fb3_hsb_x",ofVec3f(gui->feedback_settings[3].tex_hue,gui->feedback_settings[3].tex_saturation,gui->feedback_settings[3].tex_bright));
    shader_mixer.setUniform3f("tex_fb3_hue_x",ofVec3f(gui->feedback_settings[3].tex_huex_mod,gui->feedback_settings[3].tex_huex_offset,gui->feedback_settings[3].tex_huex_lfo_val));
    shader_mixer.setUniform3f("tex_fb3_rescale",ofVec3f(gui->feedback_settings[3].tex_x_displace, gui->feedback_settings[3].tex_y_displace, gui->feedback_settings[3].tex_z_displace/100.0f));
    shader_mixer.setUniform1f("tex_fb3_rotate",(gui->feedback_settings[3].tex_rotate));
    ofVec2f fb3_texmod_logic_vec_shader; if(gui->feedback_settings[3].texmod_select==1){ fb3_texmod_logic_vec_shader.set(1,0); } else if(gui->feedback_settings[3].texmod_select==2){ fb3_texmod_logic_vec_shader.set(0,1); } else {fb3_texmod_logic_vec_shader.set(0,0);} shader_mixer.setUniform2f("fb3_texmod_logic",fb3_texmod_logic_vec_shader);

    shader_mixer.setUniform1f("ee",ee);

    shader_mixer.setUniform1i("channel1", gui->channel_settings[0].select);
    shader_mixer.setUniform1i("channel2", gui->channel_settings[1].select);
    shader_mixer.setUniform1f("mix1blend1", gui->channel_settings[1].mix_value);
    shader_mixer.setUniform1f("mix1keybright",gui->channel_settings[1].key_value);

    shader_mixer.setUniform1f("channel1hue_x", gui->channel_settings[0].hue+c1*5.0f+lfo(gui->channel_settings[0].hue_lfo.amp,ch1_theta_hue,0));
    shader_mixer.setUniform1f("channel1saturation_x", gui->channel_settings[0].saturation+c2*5.0f+lfo(gui->channel_settings[0].saturation_lfo.amp,ch1_theta_saturation,0));
    shader_mixer.setUniform1f("channel1bright_x", gui->channel_settings[0].bright+c3*5.0f+abs(lfo(gui->channel_settings[0].bright_lfo.amp,ch1_theta_bright,0)));
    shader_mixer.setUniform1i("channel1satwrap", gui->channel_settings[0].saturation_wrap);
    shader_mixer.setUniform1i("channel1brightwrap", gui->channel_settings[0].bright_wrap);
    shader_mixer.setUniform1i("ch1hue_inverttoggle", gui->channel_settings[0].hue_alt_invert_toggle);
    shader_mixer.setUniform1i("ch1sat_inverttoggle", gui->channel_settings[0].saturation_alt_invert_toggle);
    shader_mixer.setUniform1i("ch1bright_inverttoggle", gui->channel_settings[0].bright_alt_invert_toggle);
    shader_mixer.setUniform1f("channel1bright_powmap", gui->channel_settings[0].bright_powmap);
    shader_mixer.setUniform1f("channel1hue_powmap", gui->channel_settings[0].hue_powmap);
    shader_mixer.setUniform1f("channel1sat_powmap", gui->channel_settings[0].saturation_powmap);

    shader_mixer.setUniform1f("channel2bright_x", gui->channel_settings[1].bright+lfo(gui->channel_settings[1].bright_lfo.amp,ch2_theta_bright,0));
    shader_mixer.setUniform1f("channel2hue_x", gui->channel_settings[1].hue+lfo(gui->channel_settings[1].hue_lfo.amp,ch2_theta_hue,0));
    shader_mixer.setUniform1f("channel2saturation_x", gui->channel_settings[1].saturation+lfo(gui->channel_settings[1].saturation_lfo.amp,ch2_theta_saturation,0));
    shader_mixer.setUniform1i("channel2satwrap", gui->channel_settings[1].saturation_wrap);
    shader_mixer.setUniform1i("channel2brightwrap", gui->channel_settings[1].bright_wrap);
    shader_mixer.setUniform1i("ch2hue_inverttoggle", gui->channel_settings[1].hue_alt_invert_toggle);
    shader_mixer.setUniform1i("ch2sat_inverttoggle", gui->channel_settings[1].saturation_alt_invert_toggle);
    shader_mixer.setUniform1i("ch2bright_inverttoggle", gui->channel_settings[1].bright_alt_invert_toggle);
    shader_mixer.setUniform1f("channel2bright_powmap", gui->channel_settings[1].bright_powmap);
    shader_mixer.setUniform1f("channel2hue_powmap", gui->channel_settings[1].hue_powmap);
    shader_mixer.setUniform1f("channel2sat_powmap", gui->channel_settings[1].saturation_powmap);

    shader_mixer.setUniform2f("cam1dimensions",ofVec2f(cam1.getWidth(),cam1.getHeight()));
    shader_mixer.setUniform2f("cam2dimensions",ofVec2f(cam2.getWidth(),cam2.getHeight()));

    shader_mixer.setUniform1i("cam1_hflip_switch", gui->global_settings.cam1_hflip_switch);
    shader_mixer.setUniform1i("cam1_vflip_switch", gui->global_settings.cam1_vflip_switch);
    shader_mixer.setUniform1i("cam2_hflip_switch", gui->global_settings.cam2_hflip_switch);
    shader_mixer.setUniform1i("cam2_vflip_switch", gui->global_settings.cam2_vflip_switch);
    shader_mixer.setUniform1i("fb0_hflip_switch", gui->feedback_settings[0].hflip_switch);
    shader_mixer.setUniform1i("fb0_vflip_switch", gui->feedback_settings[0].vflip_switch);
    shader_mixer.setUniform1i("fb1_hflip_switch", gui->feedback_settings[1].hflip_switch);
    shader_mixer.setUniform1i("fb1_vflip_switch", gui->feedback_settings[1].vflip_switch);
    shader_mixer.setUniform1i("fb2_hflip_switch", gui->feedback_settings[2].hflip_switch);
    shader_mixer.setUniform1i("fb2_vflip_switch", gui->feedback_settings[2].vflip_switch);
    shader_mixer.setUniform1i("fb3_hflip_switch", gui->feedback_settings[3].hflip_switch);
    shader_mixer.setUniform1i("fb3_vflip_switch", gui->feedback_settings[3].vflip_switch);

    shader_mixer.setUniform1i("cam1_pixel_switch",gui->global_settings.cam1_pixel_switch);
    shader_mixer.setUniform1i("cam1_pixel_scale_x",gui->global_settings.cam1_pixel_scale_x+ (int)lfo(gui->global_settings.cam1_pixel_scale_x_lfo.amp,cam1_theta_pixel_scale_x,0));
    shader_mixer.setUniform1i("cam1_pixel_scale_y",gui->global_settings.cam1_pixel_scale_y+ (int)lfo(gui->global_settings.cam1_pixel_scale_y_lfo.amp,cam1_theta_pixel_scale_y,0));
    shader_mixer.setUniform1f("cam1_pixel_mix",gui->global_settings.cam1_pixel_mix+lfo(gui->global_settings.cam1_pixel_mix_lfo.amp,cam1_theta_pixel_mix,0));
    shader_mixer.setUniform1f("cam1_pixel_brightscale",gui->global_settings.cam1_pixel_brightscale+lfo(gui->global_settings.cam1_pixel_brightscale_lfo.amp,cam1_theta_pixel_brightscale,0));

    shader_mixer.setUniform1i("cam2_pixel_switch",gui->global_settings.cam2_pixel_switch);
    shader_mixer.setUniform1i("cam2_pixel_scale_x",gui->global_settings.cam2_pixel_scale_x+ (int)lfo(gui->global_settings.cam2_pixel_scale_x_lfo.amp,cam2_theta_pixel_scale_x,0));
    shader_mixer.setUniform1i("cam2_pixel_scale_y",gui->global_settings.cam2_pixel_scale_y+ (int)lfo(gui->global_settings.cam2_pixel_scale_y_lfo.amp,cam2_theta_pixel_scale_y,0));
    shader_mixer.setUniform1f("cam2_pixel_mix",gui->global_settings.cam2_pixel_mix+lfo(gui->global_settings.cam2_pixel_mix_lfo.amp,cam2_theta_pixel_mix,0));
    shader_mixer.setUniform1f("cam2_pixel_brightscale",gui->global_settings.cam2_pixel_brightscale+lfo(gui->global_settings.cam2_pixel_brightscale_lfo.amp,cam2_theta_pixel_brightscale,0));

    shader_mixer.setUniform1i("ndi_pixel_switch",gui->global_settings.ndi_pixel_switch);
    shader_mixer.setUniform1i("ndi_pixel_scale_x",gui->global_settings.ndi_pixel_scale_x+ (int)lfo(gui->global_settings.ndi_pixel_scale_x_lfo.amp,ndi_theta_pixel_scale_x,0));
    shader_mixer.setUniform1i("ndi_pixel_scale_y",gui->global_settings.ndi_pixel_scale_y+ (int)lfo(gui->global_settings.ndi_pixel_scale_y_lfo.amp,ndi_theta_pixel_scale_y,0));
    shader_mixer.setUniform1f("ndi_pixel_mix",gui->global_settings.ndi_pixel_mix+lfo(gui->global_settings.ndi_pixel_mix_lfo.amp,ndi_theta_pixel_mix,0));
    shader_mixer.setUniform1f("ndi_pixel_brightscale",gui->global_settings.ndi_pixel_brightscale+lfo(gui->global_settings.ndi_pixel_brightscale_lfo.amp,ndi_theta_pixel_brightscale,0));

    for(int i=0; i<4; ++i) {
        string fb_idx_str = "fb" + ofToString(i);
        shader_mixer.setUniform1i(fb_idx_str + "_pixel_switch", gui->feedback_settings[i].pixel_switch);
        shader_mixer.setUniform1i(fb_idx_str + "_pixel_scale_x", gui->feedback_settings[i].pixel_scale_x + (int)lfo(gui->feedback_settings[i].pixel_scale_x_lfo.amp, (i==0?fb0_theta_pixel_scale_x:(i==1?fb1_theta_pixel_scale_x:(i==2?fb2_theta_pixel_scale_x:fb3_theta_pixel_scale_x))),0));
        shader_mixer.setUniform1i(fb_idx_str + "_pixel_scale_y", gui->feedback_settings[i].pixel_scale_y + (int)lfo(gui->feedback_settings[i].pixel_scale_y_lfo.amp, (i==0?fb0_theta_pixel_scale_y:(i==1?fb1_theta_pixel_scale_y:(i==2?fb2_theta_pixel_scale_y:fb3_theta_pixel_scale_y))),0));
        shader_mixer.setUniform1f(fb_idx_str + "_pixel_mix", gui->feedback_settings[i].pixel_mix + lfo(gui->feedback_settings[i].pixel_mix_lfo.amp, (i==0?fb0_theta_pixel_mix:(i==1?fb1_theta_pixel_mix:(i==2?fb2_theta_pixel_mix:fb3_theta_pixel_mix))),0));
        shader_mixer.setUniform1f(fb_idx_str + "_pixel_brightscale", gui->feedback_settings[i].pixel_brightscale + lfo(gui->feedback_settings[i].pixel_brightscale_lfo.amp, (i==0?fb0_theta_pixel_brightscale:(i==1?fb1_theta_pixel_brightscale:(i==2?fb2_theta_pixel_brightscale:fb3_theta_pixel_brightscale))),0));

        ofVec2f fb_pixel_texmod_logic_vec_shader;
        if(gui->feedback_settings[i].pixel_texmod_select==1){ fb_pixel_texmod_logic_vec_shader.set(1,0); }
        else if(gui->feedback_settings[i].pixel_texmod_select==2){ fb_pixel_texmod_logic_vec_shader.set(0,1); }
        else {fb_pixel_texmod_logic_vec_shader.set(0,0);} // Default if not 1 or 2
        shader_mixer.setUniform2f(fb_idx_str + "_pixel_texmod_logic", fb_pixel_texmod_logic_vec_shader);
        shader_mixer.setUniform1i("texmod_" + fb_idx_str + "_pixel_scale_x", gui->feedback_settings[i].texmod_pixel_scale_x);
        shader_mixer.setUniform1i("texmod_" + fb_idx_str + "_pixel_scale_y", gui->feedback_settings[i].texmod_pixel_scale_y);
        shader_mixer.setUniform1f("texmod_" + fb_idx_str + "_pixel_mix", gui->feedback_settings[i].texmod_pixel_mix);
        shader_mixer.setUniform1f("texmod_" + fb_idx_str + "_pixel_brightscale", gui->feedback_settings[i].texmod_pixel_brightscale);
    }

    shader_mixer.end();

    if(gui->global_settings.hypercube_switch){ hypercube_draw(); }
    if(gui->global_settings.tetrahedron_switch){
        ofSetColor(127+127*(sin(ofGetElapsedTimef())),127+127*(cos(ofGetElapsedTimef()/7)),127-127*(sin(ofGetElapsedTimef()/19)),255);
        ofNoFill();
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofRotateZRad(ofGetElapsedTimef()/5);
        ofRotateYRad(ofGetElapsedTimef()/13);
        ofRotateXRad(ofGetElapsedTimef()/11);
        tetrahedron.draw();
        ofPopMatrix();
    }
    fbo_draw.end();

    fbo_blur.begin();
    shader_blur.begin();
    fbo_draw.draw(0,0);
    if(gui->global_settings.global_texmod_select==0){ shader_blur.setUniformTexture("texmod",cam1.getTexture(),8); }
    if(gui->global_settings.global_texmod_select==1){ shader_blur.setUniformTexture("texmod",cam2.getTexture(),8); }
    if(gui->global_settings.global_texmod_select==2){ shader_blur.setUniformTexture("texmod",ndi_fbo.getTexture(),8); }
    shader_blur.setUniform1f("blur_amount",gui->global_settings.blur_amount + c4*10+lfo(gui->global_settings.blur_amount_lfo.amp,blur_theta_amount,0));
    shader_blur.setUniform1f("blur_radius",gui->global_settings.blur_radius+lfo(gui->global_settings.blur_radius_lfo.amp,blur_theta_radius,0));
    shader_blur.setUniform1f("texmod_blur_amount",gui->global_settings.texmod_blur_amount);
    shader_blur.setUniform1f("texmod_blur_radius",gui->global_settings.texmod_blur_radius);
    shader_blur.end();
    fbo_blur.end();

    fbo_draw.begin();
    shader_sharpen.begin();
    fbo_blur.draw(0,0);
    if(gui->global_settings.global_texmod_select==0){ shader_sharpen.setUniformTexture("texmod",cam1.getTexture(),9); }
    if(gui->global_settings.global_texmod_select==1){ shader_sharpen.setUniformTexture("texmod",cam2.getTexture(),9); }
    if(gui->global_settings.global_texmod_select==2){ shader_sharpen.setUniformTexture("texmod",ndi_fbo.getTexture(),9); }
    shader_sharpen.setUniform1f("texmod_sharpen_amount",gui->global_settings.texmod_sharpen_amount);
    shader_sharpen.setUniform1f("texmod_sharpen_radius",gui->global_settings.texmod_sharpen_radius);
    shader_sharpen.setUniform1f("texmod_sharpen_boost",gui->global_settings.texmod_sharpen_boost);
    shader_sharpen.setUniform1f("sharpen_amount",gui->global_settings.sharpen_amount+c5*.3+lfo(gui->global_settings.sharpen_amount_lfo.amp,sharpen_theta_amount,0));
    shader_sharpen.setUniform1f("sharpen_radius",gui->global_settings.sharpen_radius+lfo(gui->global_settings.sharpen_radius_lfo.amp,sharpen_theta_radius,0));
    shader_sharpen.setUniform1f("sharpen_boost",gui->global_settings.sharpen_boost+c5+lfo(gui->global_settings.sharpen_boost_lfo.amp,sharpen_theta_boost,0));
    shader_sharpen.setUniform1f("qq",qq);
    shader_sharpen.end();
    fbo_draw.end();

    // Final draw to screen is now handled by GuiApp drawing the "Output Window"
    // The ofFbo that GuiApp::drawOutputWindow receives should be fbo_draw
    // So, no direct fbo_draw.draw(0,0) here.

    pastFrames[abs(fbob_const-framedelayoffset)-1].begin();
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2,0);
    ofTranslate(ff,gg,hh);
    ofRotateYRad(ss+gui->global_settings.y_skew);
    ofRotateXRad(aa+gui->global_settings.x_skew);
    ofRotateZRad(dd); ofRotateZRad(oo*TWO_PI/ii);
    fbo_draw.draw(-ofGetWidth()/2,-ofGetHeight()/2);
    ofPopMatrix();
    pastFrames[abs(fbob_const-framedelayoffset)-1].end();

    incIndex();

    if (gui) {
        gui->drawOutputWindow(&fbo_draw.getTexture()); // Pass the final texture to GuiApp
    }
}

//-------------------------------------------------------

float ofApp::lfo(float amp, float rate,int shape){
    return amp*sin(rate);
}

//--------------------------------------------------------------
void ofApp::NDI_reciever_setup(string reciever_name){
    auto findSource = [](const string &name_or_url) {
        auto sources = ofxNDI::listSources();
        if(name_or_url == "") { return make_pair(ofxNDI::Source(), false); }
        auto found = find_if(begin(sources), end(sources), [name_or_url](const ofxNDI::Source &s) {
            return ofIsStringInString(s.p_ndi_name, name_or_url) || ofIsStringInString(s.p_url_address, name_or_url);
        });
        if(found == end(sources)) { ofLogWarning("ofxNDI") << "no NDI source found by string:" << name_or_url; return make_pair(ofxNDI::Source(), false); }
        return make_pair(*found, true);
    };
    string name_or_url = reciever_name;
    auto result = findSource(name_or_url);
    if(result.second ? ndi_receiver_.setup(result.first) : ndi_receiver_.setup()) {
        ndi_video_.setup(ndi_receiver_);
    }
}
//-------------------------

void ofApp::NDI_reciever_update(){
    if (isCapturingWindow) {
        // Placeholder: If actual window capture is implemented, update ndi_fbo with capturedFrame
        // For now, it might show the last NDI frame or video frame if capture isn't drawing here.
        // Example:
        // if (capturedFrame.isAllocated()) {
        //    ndi_fbo.begin();
        //    ofClear(0,0,0,0);
        //    capturedFrame.draw(0,0,ndi_fbo.getWidth(), ndi_fbo.getHeight());
        //    ndi_fbo.end();
        // }
        // return; // Skip NDI/Video processing if window capture is active and drawing to ndi_fbo
    }

    if(ndi_receiver_.isConnected()) {
        ndi_video_.update();
        if(ndi_video_.isFrameNew()) {
            ndi_video_.decodeTo(ndi_pixels);
        }
    }
    ndi_fbo.begin();
    ofClear(0,0,0,0);
    if (videoLoaded && videoPlayer.isLoaded() && videoPlaying) {
        videoPlayer.draw(0,0, ndi_fbo.getWidth(), ndi_fbo.getHeight());
    } else if(ndi_pixels.isAllocated()) {
        ofPushMatrix();
        ofTranslate(ndi_fbo.getWidth()/2,ndi_fbo.getHeight()/2);
        ofTranslate(0,0,gui->global_settings.ndi_scale);
        ofImage(ndi_pixels).draw(-ofImage(ndi_pixels).getWidth()/2,-ofImage(ndi_pixels).getHeight()/2);
        ofPopMatrix();
    }
    ndi_fbo.end();
}

//------------------------------------------------

void ofApp::NDI_sender_setup(string app_name){
    if(sender_.setup(app_name)) {
        ndi_send_video_.setup(sender_);
        ndi_send_video_.setAsync(true);
    }
}

//----

void ofApp:: NDI_sender_update(){
    ofPixels fbo_pixels;
    fbo_draw.readToPixels(fbo_pixels);
    ndi_send_video_.send(fbo_pixels);
}

//- ---

void ofApp::lfo_update(){
    if (!gui) return;
    // Channel LFOs
    ch1_theta_hue += 0.01f * gui->channel_settings[0].hue_lfo.theta;
    ch1_theta_saturation += 0.01f * gui->channel_settings[0].saturation_lfo.theta;
    ch1_theta_bright += 0.01f * gui->channel_settings[0].bright_lfo.theta;
    ch2_theta_hue += 0.01f * gui->channel_settings[1].hue_lfo.theta;
    ch2_theta_saturation += 0.01f * gui->channel_settings[1].saturation_lfo.theta;
    ch2_theta_bright += 0.01f * gui->channel_settings[1].bright_lfo.theta;

    // Feedback LFOs
    float* fb_thetas[] = {
        &fb0_theta_mix, &fb0_theta_lumakeyvalue, &fb0_theta_lumakeythreshold, &fb0_theta_delay, &fb0_theta_hue, &fb0_theta_saturation, &fb0_theta_bright, &fb0_theta_huexmod, &fb0_theta_huexoffset, &fb0_theta_huexlfo, &fb0_theta_x, &fb0_theta_y, &fb0_theta_z, &fb0_theta_rotate,
        &fb1_theta_mix, &fb1_theta_lumakeyvalue, &fb1_theta_lumakeythreshold, &fb1_theta_delay, &fb1_theta_hue, &fb1_theta_saturation, &fb1_theta_bright, &fb1_theta_huexmod, &fb1_theta_huexoffset, &fb1_theta_huexlfo, &fb1_theta_x, &fb1_theta_y, &fb1_theta_z, &fb1_theta_rotate,
        &fb2_theta_mix, &fb2_theta_lumakeyvalue, &fb2_theta_lumakeythreshold, &fb2_theta_delay, &fb2_theta_hue, &fb2_theta_saturation, &fb2_theta_bright, &fb2_theta_huexmod, &fb2_theta_huexoffset, &fb2_theta_huexlfo, &fb2_theta_x, &fb2_theta_y, &fb2_theta_z, &fb2_theta_rotate,
        &fb3_theta_mix, &fb3_theta_lumakeyvalue, &fb3_theta_lumakeythreshold, &fb3_theta_delay, &fb3_theta_hue, &fb3_theta_saturation, &fb3_theta_bright, &fb3_theta_huexmod, &fb3_theta_huexoffset, &fb3_theta_huexlfo, &fb3_theta_x, &fb3_theta_y, &fb3_theta_z, &fb3_theta_rotate
    };
    GuiApp::LFOSettings* fb_gui_lfos[4*14]; // Array of pointers to LFOSettings

    for(int i=0; i<4; ++i) {
        fb_gui_lfos[i*14 + 0] = &gui->feedback_settings[i].mix_lfo;
        fb_gui_lfos[i*14 + 1] = &gui->feedback_settings[i].lumakeyvalue_lfo;
        fb_gui_lfos[i*14 + 2] = &gui->feedback_settings[i].lumakeythreshold_lfo;
        fb_gui_lfos[i*14 + 3] = &gui->feedback_settings[i].delay_lfo;
        fb_gui_lfos[i*14 + 4] = &gui->feedback_settings[i].hue_lfo;
        fb_gui_lfos[i*14 + 5] = &gui->feedback_settings[i].saturation_lfo;
        fb_gui_lfos[i*14 + 6] = &gui->feedback_settings[i].bright_lfo;
        fb_gui_lfos[i*14 + 7] = &gui->feedback_settings[i].huexmod_lfo;
        fb_gui_lfos[i*14 + 8] = &gui->feedback_settings[i].huexoffset_lfo;
        fb_gui_lfos[i*14 + 9] = &gui->feedback_settings[i].huexlfo_val_lfo; // Matches struct member name
        fb_gui_lfos[i*14 + 10] = &gui->feedback_settings[i].x_lfo;
        fb_gui_lfos[i*14 + 11] = &gui->feedback_settings[i].y_lfo;
        fb_gui_lfos[i*14 + 12] = &gui->feedback_settings[i].z_lfo;
        fb_gui_lfos[i*14 + 13] = &gui->feedback_settings[i].rotate_lfo;
    }
    for(int i=0; i < 4*14; ++i) { *fb_thetas[i] += 0.01f * fb_gui_lfos[i]->theta; }

    // Global LFOs
    blur_theta_amount += 0.01f * gui->global_settings.blur_amount_lfo.theta;
    blur_theta_radius += 0.01f * gui->global_settings.blur_radius_lfo.theta;
    sharpen_theta_amount += 0.01f * gui->global_settings.sharpen_amount_lfo.theta;
    sharpen_theta_radius += 0.01f * gui->global_settings.sharpen_radius_lfo.theta;
    sharpen_theta_boost += 0.01f * gui->global_settings.sharpen_boost_lfo.theta;

    // Pixelation LFOs
    cam1_theta_pixel_scale_x += 0.01f * gui->global_settings.cam1_pixel_scale_x_lfo.theta;
    cam1_theta_pixel_scale_y += 0.01f * gui->global_settings.cam1_pixel_scale_y_lfo.theta;
    cam1_theta_pixel_mix += 0.01f * gui->global_settings.cam1_pixel_mix_lfo.theta;
    cam1_theta_pixel_brightscale += 0.01f * gui->global_settings.cam1_pixel_brightscale_lfo.theta;

    cam2_theta_pixel_scale_x += 0.01f * gui->global_settings.cam2_pixel_scale_x_lfo.theta;
    cam2_theta_pixel_scale_y += 0.01f * gui->global_settings.cam2_pixel_scale_y_lfo.theta;
    cam2_theta_pixel_mix += 0.01f * gui->global_settings.cam2_pixel_mix_lfo.theta;
    cam2_theta_pixel_brightscale += 0.01f * gui->global_settings.cam2_pixel_brightscale_lfo.theta;

    ndi_theta_pixel_scale_x += 0.01f * gui->global_settings.ndi_pixel_scale_x_lfo.theta;
    ndi_theta_pixel_scale_y += 0.01f * gui->global_settings.ndi_pixel_scale_y_lfo.theta;
    ndi_theta_pixel_mix += 0.01f * gui->global_settings.ndi_pixel_mix_lfo.theta;
    ndi_theta_pixel_brightscale += 0.01f * gui->global_settings.ndi_pixel_brightscale_lfo.theta;

    float* fb_px_thetas[] = {
        &fb0_theta_pixel_scale_x, &fb0_theta_pixel_scale_y, &fb0_theta_pixel_mix, &fb0_theta_pixel_brightscale,
        &fb1_theta_pixel_scale_x, &fb1_theta_pixel_scale_y, &fb1_theta_pixel_mix, &fb1_theta_pixel_brightscale,
        &fb2_theta_pixel_scale_x, &fb2_theta_pixel_scale_y, &fb2_theta_pixel_mix, &fb2_theta_pixel_brightscale,
        &fb3_theta_pixel_scale_x, &fb3_theta_pixel_scale_y, &fb3_theta_pixel_mix, &fb3_theta_pixel_brightscale
    };
    GuiApp::LFOSettings* fb_px_gui_lfos[4*4];
     for(int i=0; i<4; ++i) {
        fb_px_gui_lfos[i*4 + 0] = &gui->feedback_settings[i].pixel_scale_x_lfo;
        fb_px_gui_lfos[i*4 + 1] = &gui->feedback_settings[i].pixel_scale_y_lfo;
        fb_px_gui_lfos[i*4 + 2] = &gui->feedback_settings[i].pixel_mix_lfo;
        fb_px_gui_lfos[i*4 + 3] = &gui->feedback_settings[i].pixel_brightscale_lfo;
    }
    for(int i=0; i < 4*4; ++i) { *fb_px_thetas[i] += 0.01f * fb_px_gui_lfos[i]->theta; }
}

//--------------------------
void ofApp::hypercube_draw(){
    int limit=3;
    for(int i=0;i<limit;i++){
        hypercube_theta+=.1*gui->global_settings.hypercube_theta_rate;
        hypercube_phi+=.1*gui->global_settings.hypercube_phi_rate;

        hypercube_r=ofGetWidth()/16*(1);
        float xr=hypercube_r*(1);
        hypercube_x[0]=xr*(cos(hypercube_theta)-sin(hypercube_theta))*(1-.5*(cos(hypercube_phi)));
        hypercube_x[1]=xr*(cos(hypercube_theta)+sin(hypercube_theta))*(1-.5*(cos(PI/4+hypercube_phi)));
        hypercube_x[2]=xr*(-cos(hypercube_theta)+sin(hypercube_theta))*(1-.5*(cos(PI/2+hypercube_phi)));
        hypercube_x[3]=xr*(-cos(hypercube_theta)-sin(hypercube_theta))*(1-.5*(cos(3*PI/4+hypercube_phi)));
        hypercube_x[4]=xr*(cos(hypercube_theta)-sin(hypercube_theta))*(1-.5*(cos(PI+hypercube_phi)));
        hypercube_x[5]=xr*(cos(hypercube_theta)+sin(hypercube_theta))*(1-.5*(cos(5*PI/4+hypercube_phi)));
        hypercube_x[6]=xr*(-cos(hypercube_theta)+sin(hypercube_theta))*(1-.5*(cos(3*PI/2+hypercube_phi)));
        hypercube_x[7]=xr*(-cos(hypercube_theta)-sin(hypercube_theta))*(1-.5*(cos(7*PI/4+hypercube_phi)));

        float yr=hypercube_r*(1);
        hypercube_y[0]=yr*(sin(hypercube_theta)+cos(hypercube_theta))*(1-.5*(cos(hypercube_phi)));
        hypercube_y[1]=yr*(sin(hypercube_theta)-cos(hypercube_theta))*(1-.5*(cos(PI/4+hypercube_phi)));
        hypercube_y[2]=yr*(-sin(hypercube_theta)-cos(hypercube_theta))*(1-.5*(cos(PI/2+hypercube_phi)));
        hypercube_y[3]=yr*(-sin(hypercube_theta)+cos(hypercube_theta))*(1-.5*(cos(3*PI/4+hypercube_phi)));
        hypercube_y[4]=yr*(sin(hypercube_theta)+cos(hypercube_theta))*(1-.5*(cos(PI+hypercube_phi)));
        hypercube_y[5]=yr*(sin(hypercube_theta)-cos(hypercube_theta))*(1-.5*(cos(5*PI/4+hypercube_phi)));
        hypercube_y[6]=yr*(-sin(hypercube_theta)-cos(hypercube_theta))*(1-.5*(cos(3*PI/2+hypercube_phi)));
        hypercube_y[7]=yr*(-sin(hypercube_theta)+cos(hypercube_theta))*(1-.5*(cos(7*PI/4+hypercube_phi)));

        float zr=hypercube_r*(1);
        hypercube_z[0]=-zr/2*cos(hypercube_phi)+hypercube_r;
        hypercube_z[1]=-zr/2*cos(PI/4+hypercube_phi)+hypercube_r;
        hypercube_z[2]=-zr/2*cos(PI/2+hypercube_phi)+hypercube_r;
        hypercube_z[3]=-zr/2*cos(3*PI/4+hypercube_phi)+hypercube_r;
        hypercube_z[4]=-zr/2*cos(PI+hypercube_phi)+hypercube_r;
        hypercube_z[5]=-zr/2*cos(5*PI/4+hypercube_phi)+hypercube_r;
        hypercube_z[6]=-zr/2*cos(3*PI/2+hypercube_phi)+hypercube_r;
        hypercube_z[7]=-zr/2*cos(7*PI/8+hypercube_phi)+hypercube_r;

        color_theta+=.01;
        ofSetColor(127+127*sin(color_theta),0+192*abs(cos(color_theta*.2)),127+127*cos(color_theta/3.0f));
        ofNoFill();
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofRotateYRad(-PI/2);
        ofRotateZRad(hypercube_phi);
        ofRotateXRad(hypercube_theta/3);
        ofRotateXRad(hypercube_theta/5);

        ofDrawLine(hypercube_x[0],hypercube_y[0],hypercube_z[0],hypercube_x[1],hypercube_y[1],hypercube_z[1]);
        ofDrawLine(hypercube_x[1],hypercube_y[1],hypercube_z[1],hypercube_x[2],hypercube_y[2],hypercube_z[2]);
        ofDrawLine(hypercube_x[2],hypercube_y[2],hypercube_z[2],hypercube_x[3],hypercube_y[3],hypercube_z[3]);
        ofDrawLine(hypercube_x[3],hypercube_y[3],hypercube_z[3],hypercube_x[4],hypercube_y[4],hypercube_z[4]);
        ofDrawLine(hypercube_x[4],hypercube_y[4],hypercube_z[4],hypercube_x[5],hypercube_y[5],hypercube_z[5]);
        ofDrawLine(hypercube_x[5],hypercube_y[5],hypercube_z[5],hypercube_x[6],hypercube_y[6],hypercube_z[6]);
        ofDrawLine(hypercube_x[6],hypercube_y[6],hypercube_z[6],hypercube_x[7],hypercube_y[7],hypercube_z[7]);
        ofDrawLine(hypercube_x[7],hypercube_y[7],hypercube_z[7],hypercube_x[0],hypercube_y[0],hypercube_z[0]);

        ofDrawLine(hypercube_x[0],hypercube_y[0],-hypercube_z[0],hypercube_x[1],hypercube_y[1],-hypercube_z[1]);
        ofDrawLine(hypercube_x[1],hypercube_y[1],-hypercube_z[1],hypercube_x[2],hypercube_y[2],-hypercube_z[2]);
        ofDrawLine(hypercube_x[2],hypercube_y[2],-hypercube_z[2],hypercube_x[3],hypercube_y[3],-hypercube_z[3]);
        ofDrawLine(hypercube_x[3],hypercube_y[3],-hypercube_z[3],hypercube_x[4],hypercube_y[4],-hypercube_z[4]);
        ofDrawLine(hypercube_x[4],hypercube_y[4],-hypercube_z[4],hypercube_x[5],hypercube_y[5],-hypercube_z[5]);
        ofDrawLine(hypercube_x[5],hypercube_y[5],-hypercube_z[5],hypercube_x[6],hypercube_y[6],-hypercube_z[6]);
        ofDrawLine(hypercube_x[6],hypercube_y[6],-hypercube_z[6],hypercube_x[7],hypercube_y[7],-hypercube_z[7]);
        ofDrawLine(hypercube_x[7],hypercube_y[7],-hypercube_z[7],hypercube_x[0],hypercube_y[0],-hypercube_z[0]);

        ofDrawLine(hypercube_x[0],hypercube_y[0],hypercube_z[0],hypercube_x[0],hypercube_y[0],-hypercube_z[0]);
        ofDrawLine(hypercube_x[1],hypercube_y[1],hypercube_z[1],hypercube_x[1],hypercube_y[1],-hypercube_z[1]);
        ofDrawLine(hypercube_x[2],hypercube_y[2],hypercube_z[2],hypercube_x[2],hypercube_y[2],-hypercube_z[2]);
        ofDrawLine(hypercube_x[3],hypercube_y[3],hypercube_z[3],hypercube_x[3],hypercube_y[3],-hypercube_z[3]);
        ofDrawLine(hypercube_x[4],hypercube_y[4],hypercube_z[4],hypercube_x[4],hypercube_y[4],-hypercube_z[4]);
        ofDrawLine(hypercube_x[5],hypercube_y[5],hypercube_z[5],hypercube_x[5],hypercube_y[5],-hypercube_z[5]);
        ofDrawLine(hypercube_x[6],hypercube_y[6],hypercube_z[6],hypercube_x[6],hypercube_y[6],-hypercube_z[6]);
        ofDrawLine(hypercube_x[7],hypercube_y[7],hypercube_z[7],hypercube_x[7],hypercube_y[7],-hypercube_z[7]);

        ofDrawLine(hypercube_x[0],hypercube_y[0],-hypercube_z[0],hypercube_x[4],hypercube_y[4],-hypercube_z[4]);
        ofDrawLine(hypercube_x[1],hypercube_y[1],-hypercube_z[1],hypercube_x[5],hypercube_y[5],-hypercube_z[5]);
        ofDrawLine(hypercube_x[2],hypercube_y[2],-hypercube_z[2],hypercube_x[6],hypercube_y[6],-hypercube_z[6]);
        ofDrawLine(hypercube_x[3],hypercube_y[3],-hypercube_z[3],hypercube_x[7],hypercube_y[7],-hypercube_z[7]);

        ofDrawLine(hypercube_x[0],hypercube_y[0],hypercube_z[0],hypercube_x[4],hypercube_y[4],hypercube_z[4]);
        ofDrawLine(hypercube_x[1],hypercube_y[1],hypercube_z[1],hypercube_x[5],hypercube_y[5],hypercube_z[5]);
        ofDrawLine(hypercube_x[2],hypercube_y[2],hypercube_z[2],hypercube_x[6],hypercube_y[6],hypercube_z[6]);
        ofDrawLine(hypercube_x[3],hypercube_y[3],hypercube_z[3],hypercube_x[7],hypercube_y[7],hypercube_z[7]);

        ofPopMatrix();
    }
}

void ofApp::tetrahedron_setup(){
    ofVec3f tri1;
    ofVec3f tri2;
    ofVec3f tri3;
    ofVec3f tri4;
    tri1.set(1,1,1);
    tri2.set(-1,-1,1);
    tri3.set(-1,1,-1);
    tri4.set(1,-1,-1);
    float shapeScale=ofGetWidth()/8;

    tri1=tri1*shapeScale;
    tri2=tri2*shapeScale;
    tri3=tri3*shapeScale;
    tri4=tri4*shapeScale;

    tetrahedron.lineTo(tri1);
    tetrahedron.lineTo(tri2);
    tetrahedron.lineTo(tri4);
    tetrahedron.lineTo(tri1);
    tetrahedron.lineTo(tri3);
    tetrahedron.lineTo(tri2);
    tetrahedron.lineTo(tri3);
    tetrahedron.lineTo(tri4);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='3'){ aa=ss=dd=ff=gg=hh=0; }
    if(key=='1'){
        for(int i=0;i<fbob_const;i++){
            pastFrames[i].begin(); ofClear(0,0,0,255); pastFrames[i].end();
        }
        fbo_draw.begin(); ofClear(0,0,0,255); fbo_draw.end();
    }
    if(key=='a'){aa+=0.0001;} if(key=='z'){aa-=0.0001;}
    if(key=='s'){ss+=0.0001;} if(key=='x'){ss-=0.0001;}
    if(key=='d'){dd+=0.0001;} if(key=='c'){dd-=0.0001;}
    if(key=='f'){ff+=0.0001;} if(key=='v'){ff-=0.0001;}
    if(key=='g'){gg+=0.0001;} if(key=='b'){gg-=0.0001;}
    if(key=='h'){hh+=0.01;} if(key=='n'){hh-=0.01;}
    if(key=='j'){jj+=0.1;} if(key=='m'){jj-=0.1;}
    if(key=='k'){kk+=0.1;} if(key==','){kk-=0.1;}
    if(gui && key=='l'){gui->global_settings.cam1_scale+=0.01;}
    if(gui && key=='.'){gui->global_settings.cam1_scale-=0.01;}
    if(key==';'){scale1+=0.01;} if(key=='/'){scale1-=0.01;}
    if(key=='['){scale2+=0.01;} if(key==']'){scale2-=0.01;}
    if(key=='q'){qq+=.01;cout << "qq"<<qq<< endl;}
    if(key=='w'){qq-=.01;cout << "qq"<<qq<< endl;}
    if(key=='e'){ee+=.1;cout << "ee"<<ee<< endl;}
    if(key=='r'){ee-=.1;cout << "ee"<<ee<< endl;}
    if(key=='u'){ii+=1;} if(key=='i'){ii-=1;}
    if(key=='o'){oo+=.1;} if(key=='p'){oo-=.1;}
    if(key=='t'){tt+=.01;} if(key=='y'){tt-=.01;}
    if(key=='2'){amp+=.001;} if(key=='3'){amp-=.001;}
}

void ofApp::exit() {
    #ifdef RUN_APP_TESTS
    // Ensure iostream is available for std::cout, typically via ofMain.h or test_utils.h
    // and test_utils.h is included in ofApp.h
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "        RUNNING APPLICATION TESTS        " << std::endl;
    std::cout << "========================================" << std::endl;
    auto test_results = TestUtils::runAllTests();
    TestUtils::printTestResults(test_results);
    std::cout << "========================================" << std::endl;
    std::cout << "          TESTS COMPLETE             " << std::endl;
    std::cout << "========================================" << std::endl;
    #endif // RUN_APP_TESTS

	// Original exit code:
	midiIn.closePort();
	midiIn.removeListener(this);
}

void ofApp::midibiz(){
    for(unsigned int i = 0; i < midiMessages.size(); ++i) {
        ofxMidiMessage &message = midiMessages[i];
        if(message.status < MIDI_SYSEX) {
            if(message.status == MIDI_CONTROL_CHANGE) {
                 cout << "message.control"<< message.control<< endl;
                 cout << "message.value"<< message.value<< endl;
                if(message.control==20){c1=(message.value-63.0f)/63.0f;}
                if(message.control==21){c2=(message.value-63.0f)/63.0f;}
                if(message.control==22){c3=(message.value-63.0f)/63.0f;}
                if(message.control==25){c4=(message.value)/127.0f;}
                if(message.control==24){c5=(message.value)/127.0f;}
                if(message.control==28){c6=(message.value)/127.0f;}
                if(message.control==29){c7=(message.value-63.0f)/63.0f;}
                if(message.control==30){c8=(message.value)/127.0f;}
                if(message.control==4){c9=(message.value-63.0f)/63.0f;}
                if(message.control==3){c10=(message.value-63.0f)/63.0f;}
                if(message.control==12){c11=(message.value-63.0f)/63.0f;}
                if(message.control==11){c12=(message.value-63.0f)/63.0f;}
                if(message.control==5){c13=(message.value-63.0f)/63.0f;}
                if(message.control==2){c14=(message.value-63.0f)/63.0f;}
                if(message.control==13){c15=(message.value-63.0f)/63.0f;}
                if(message.control==16){c16=(message.value)/127.0f;}
                if(message.control==10){c17=(message.value-63.0f)/63.0f;}
                if(message.control==17){c18=(message.value-63.0f)/63.0f;}
                if(message.control==31){c19=(message.value)/127.0f;}
                if(message.control==27){c20=(message.value-63.0f)/63.0f;}
                if(message.control==26){c21=(message.value)/127.0f;}
                if(message.control==6){c22=(message.value-63.0f)/63.0f;}
                if(message.control==1){c23=(message.value-63.0f)/63.0f;}
                if(message.control==14){c24=(message.value-63.0f)/63.0f;}
                if(message.control==9){c25=(message.value-63.0f)/63.0f;}
                if(message.control==7){c26=(message.value-63.0f)/63.0f;}
                if(message.control==0){c27=(message.value-63.0f)/63.0f;}
                if(message.control==15){c28=(message.value-63.0f)/63.0f;}
                if(message.control==18){c29=(message.value)/127.0f;}
                if(message.control==8){c30=(message.value-63.0f)/63.0f;}
                if(message.control==19){c31=(message.value-63.0f)/63.0f;}
            }
        }
    }
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	midiMessages.push_back(msg);
	while(midiMessages.size() > maxMessages) {
		midiMessages.erase(midiMessages.begin());
	}
}

void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button) {}
void ofApp::mouseReleased() {}
```
