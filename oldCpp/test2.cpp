#include <aubio/aubio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>

//compiler clang++ -o test2  test2.cpp -I/opt/homebrew/include  -laubio -lm -lfftw3 -lsndfile 

void convertToNote(float freq, std::string *Note, int *Octave) {
    if ((int)freq == 0){
        *Note = "Silence";
        *Octave = 0;
        return; 
    } 

    int stepsAway = std::round(12 * log2f(freq / 261.626)); // 261.626 Hz is the frequency for middle C
    *Octave = stepsAway / 12 + 4;  // Calculate the octave
    int steps = stepsAway % 12;    // Calculate the number of semitones away from C

    // Assign the note as a string
    if (steps == 0) *Note = "C";
    else if (steps == 1) *Note = "C#";
    else if (steps == 2) *Note = "D";
    else if (steps == 3) *Note = "D#";
    else if (steps == 4) *Note = "E";
    else if (steps == 5) *Note = "F";
    else if (steps == 6) *Note = "F#";
    else if (steps == 7) *Note = "G";
    else if (steps == 8) *Note = "G#";
    else if (steps == 9) *Note = "A";
    else if (steps == 10) *Note = "A#";
    else if (steps == 11) *Note = "B";
    else if (steps < 0) *Note = "C"; //in rare ocasions if the freq is close enough, a negative value can be outp
    else *Note = std::to_string(steps);  // Default empty string for invalid cases
}


int main() {
    auto totalStartTime = std::chrono::high_resolution_clock::now(); //start processing timer

    const char *filename = "Happ192k.wav";  // Input audio file

    std::ofstream csvFile("output.csv");

    // Write CSV header (optional, but good practice)
    csvFile << "Buffnum,Timestamp,Buffer Size, Detected Freq (Hz), Interpol Note, Commputation Time (uS)" << std::endl;

    if (!csvFile.is_open()) {
        std::cerr << "Error: Could not open the file for writing!" << std::endl;
        return -1;
    }

    // Parameters
    uint_t hop_size = 512;      // Hop size (number of samples per frame)
    uint_t buf_size = 2048;     // Buffer size
    uint_t samplerate = 0;      // Set to 0 to detect samplerate from file

    // Create a source object to read the audio file
    aubio_source_t *source = new_aubio_source(filename, samplerate, hop_size);
    if (!source) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return -1;
    }

    // Get the actual samplerate of the file
    samplerate = aubio_source_get_samplerate(source);
    std::cout<< "Sampling Rate read from File "<< samplerate <<std::endl; 

    // Create a pitch detection object
    aubio_pitch_t *pitch_detector = new_aubio_pitch("yinfast", buf_size, hop_size, samplerate);

    // Set optional parameters for pitch detection
    aubio_pitch_set_unit(pitch_detector, "Hz");  // Pitch output in Hz
    aubio_pitch_set_silence(pitch_detector, -40); // Silence threshold in dB

    // Buffers for audio processing
    fvec_t *frame = new_fvec(hop_size);  // Input audio frame buffer
    fvec_t *pitch_output = new_fvec(1);  // Pitch output buffer

    // Number of frames read from the audio file
    uint_t frames_read = 0;

    //Get Sample Rate for Time calculations
    samplerate = aubio_source_get_samplerate(source);

    uint i = 0;
    // Process the audio file and detect pitch
    std::string Note;
    int Octave;

    while (true) {
        i++;
        auto startTime = std::chrono::high_resolution_clock::now(); //start processing timer

        // Read the next block of audio
        aubio_source_do(source, frame, &frames_read);

        // Check if we've reached the end of the file
        if (frames_read < hop_size) {
            break;  // End of file reached
        }

        // Perform pitch detection on the audio frame
        aubio_pitch_do(pitch_detector, frame, pitch_output);

        // Get the detected pitch in Hz
        float detected_freq = pitch_output->data[0];

        //Convert Frequency to Nearest Note
        convertToNote(detected_freq, &Note, &Octave);

        //stop timer and calculate total time to calculate
        auto stopTime = std::chrono::high_resolution_clock::now();
        auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count();

        // Print the detected pitch
        csvFile << i << "," <<(float)hop_size * (float)i * (1/((double)samplerate)) << "," << buf_size << "," << detected_freq << ',' << Note << Octave << ',' << totalTime << std::endl;
    }

    // Clean up resources
    del_aubio_source(source);
    del_aubio_pitch(pitch_detector);
    del_fvec(frame);
    del_fvec(pitch_output);

    auto totalstopTime = std::chrono::high_resolution_clock::now();
    auto totaltotalTime = std::chrono::duration_cast<std::chrono::milliseconds>(totalstopTime - totalStartTime).count();

    std::cout << "Operation took " << totaltotalTime << " ms" << std::endl; 

    csvFile.close();
    std::cout<<"Outputted To File"<<std::endl; 

    return 0;
}
