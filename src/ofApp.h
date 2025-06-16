/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "GuiApp.h"
#include "ofxNDISender.h"
#include "ofxNDISendStream.h"
#include "ofVideoPlayer.h"
#include "test_utils.h" // Added for running tests

class ofApp : public ofBaseApp, public ofxMidiListener {

public:

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();

    void midibiz();

    void NDI_reciever_setup(string reciever_name);
    void NDI_reciever_update();

    ofxNDIReceiver ndi_receiver_;
    ofxNDIRecvVideoFrameSync ndi_video_;
    ofPixels ndi_pixels;

    ofFbo ndi_fbo;

    void NDI_sender_setup(string app_name);
    void NDI_sender_update();

    void tetrahedron_setup();

    static float lfo(float amp, float rate,int shape); // Changed to static

    void lfo_update();

    void hypercube_draw();

    float hypercube_theta=0;
    float hypercube_phi=0;
    float hypercube_r=0.0;

    float hypercube_x[8];
    float hypercube_y[8];
    float hypercube_z[8];

    //-----------guibiz
    shared_ptr<GuiApp> gui;

	void newMidiMessage(ofxMidiMessage& eventArgs);

	ofxMidiIn midiIn;
	std::vector<ofxMidiMessage> midiMessages;
	std::size_t maxMessages = 10; //< max number of messages to keep track of

    ofShader shader_mixer;
    ofShader shader_blur;
    ofShader shader_sharpen;

    ofFbo fbo_draw;
    ofFbo fbo_feedback;
    ofFbo fbo_blur;

    ofFbo syphonTexture;

    ofVideoGrabber cam1;
    ofVideoGrabber cam2;

    // Video Player Members
    ofVideoPlayer videoPlayer;
    bool videoLoaded = false;
    bool videoPlaying = false;
    string loadedVideoPathString = "No video loaded";

    // Particle System
    struct Particle {
        ofVec2f pos;
        ofVec2f vel;
        ofColor color;
        float life; // Remaining lifespan in seconds
        float initialLife;
        float size;
    };
    std::vector<Particle> particles;
    ofFbo fbo_particles;
    ofPixels inputSourcePixels; // To store pixels from the selected input source for particles

    // Slit-Scan System
    ofFbo fbo_slitscan;
    ofFbo fbo_slitscan_source_input; // To prepare the source texture (e.g., apply delay)
    std::deque<ofPixels> slitscan_delay_buffer_pixels; // For frame delay using ofPixels
    ofTexture slitscan_delayed_source_tex; // Temp texture to load delayed pixels for drawing
    int currentSlitScanWritePos {0}; // Current column/row in fbo_slitscan to write to

    void setupParticleSystem();
    void updateParticleSystem(const GuiApp::ParticleFeedbackSettings& settings);
    void spawnParticles(const GuiApp::ParticleFeedbackSettings& settings);
    void drawParticlesToFbo(const GuiApp::ParticleFeedbackSettings& settings);
    void getInputSourcePixels(int source_id, const GuiApp::ParticleFeedbackSettings& settings); // This is for particles, might need a separate one for slitscan or make it generic

    void setupSlitScan();
    void updateSlitScan(const GuiApp::SlitScanSettings& settings);
    void prepareSlitScanSourceTexture(const GuiApp::SlitScanSettings& settings); // Prepares fbo_slitscan_source_input with current or delayed frame
    void drawSlitScanToFbo(const GuiApp::SlitScanSettings& settings);

    // Noise Generator System
    ofFbo fbo_noise;
    ofShader noiseShader;

    void setupNoiseGenerator();
    void updateNoiseGenerator(); // Will read settings from gui and call drawNoiseToFbo if needed
    void drawNoiseToFbo();     // Renders noise into fbo_noise using the shader

    // Pixel Sorting System
    ofFbo fbo_pixel_sort_output;
    ofShader pixelSortShader;
    // Optional: ofFbo fbo_pixel_sort_input_buffer; // If complex source handling/delay is needed later

    void setupPixelSort();
    void updatePixelSort(); // Placeholder for now, might not be needed if all settings are shader uniforms
    void drawPixelSortToFbo(ofTexture& sourceTexture, const GuiApp::PixelSortSettings& settings);

    // Reaction-Diffusion System
    ofFbo fbo_rd[2]; // Ping-pong FBOs
    ofFbo fbo_rd_display; // For final colored output before mixing
    ofShader rdShader;
    ofShader rdDisplayShader;
    int currentRdFbo {0};
    ofVec2f lastMousePressPos {-1,-1}; // For mouse seeding

    void setupReactionDiffusion();
    void updateReactionDiffusion(); // Reads settings from gui
    void seedRDCanvas(const GuiApp::ReactionDiffusionSettings& settings);
    void drawRDToDisplayFbo(const GuiApp::ReactionDiffusionSettings& settings);

    void setupVideoPlayer();
    void handleVideoEvents();
    void loadVideoFromFile();
    void playVideo();
    void pauseVideo();
    void stopVideo();
    void toggleLoop();
    void updateVideoPlayerData();
};
