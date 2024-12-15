
#include "Audioer.h"

// Constructor
Audioer::Audioer(const char* FileNameIn) : FileName(FileNameIn) {
    file = sf_open(FileName, SFM_READ, &sfinfo);
    std::cout << "Object created!" << std::endl;
    if (!file) {
        std::cerr << "Error opening file: " << sf_strerror(file) << std::endl;
        abort(); // Stops program if file cannot open
    }
}

// Destructor
Audioer::~Audioer() {
    sf_close(file);
    std::cout << "Object destroyed!" << std::endl;
}

// Prints audio file information
void Audioer::printFileInfo() {
    std::cout << "Sample Rate: " << sfinfo.samplerate << std::endl;
    std::cout << "Channels: " << sfinfo.channels << std::endl;
    std::cout << "Frames: " << sfinfo.frames << std::endl;
}

// Returns audio samples
std::vector<float> Audioer::getSamples() {
    int num_frames = sfinfo.frames;
    int num_channels = sfinfo.channels;
    std::vector<float> audio_data(num_frames * num_channels);

    sf_readf_float(file, audio_data.data(), num_frames);
    return audio_data;
}
