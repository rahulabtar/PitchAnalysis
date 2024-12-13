#include <iostream>
#include <vector> 
#include <sndfile.h>
using namespace std; 

class Audioer {
public:
    // Constructor
    Audioer(const char* FileNameIn) : FileName(FileNameIn){
        file = sf_open(FileName, SFM_READ, &sfinfo);
        std::cout << "Object created!" << std::endl;
        if (!file) {
            std::cerr << "Error opening file: " << sf_strerror(file) << std::endl;
            abort(); //stops program if file cannot open
        }
    }

    // Destructor
    ~Audioer() {
        // Clean up
        sf_close(file);
        std::cout << "Object destroyed!" << std::endl;
    }

private:
    // Private member variable
    const char * FileName; 
    SF_INFO sfinfo;
    SNDFILE *file; 

public:

    void printFileInfo() {
        std::cout << "Sample Rate: " << sfinfo.samplerate << std::endl;
        std::cout << "Channels: " << sfinfo.channels << std::endl;
        std::cout << "Frames: " << sfinfo.frames << std::endl;
    } 

     vector<float> getSamples() {
        // Allocate memory to store the audio data
        int num_frames = sfinfo.frames;
        int num_channels = sfinfo.channels;
        vector<float> audio_data(num_frames * num_channels);

        // Read the audio data into the buffer
        sf_readf_float(file, audio_data.data(), num_frames);
        return audio_data; 
    }
};


int main() {
    Audioer a("C3.wav");
    vector<float> audio_data = a.getSamples(); 
    for (int i = 0; i < 20; ++i) {
        std::cout << audio_data[i] << std::endl;
    }
}
