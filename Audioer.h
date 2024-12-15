// Audioer.h
#ifndef AUDIOER_H
#define AUDIOER_H

#include <iostream>
#include <vector>
#include <sndfile.h>

// The Audioer class
class Audioer {
public:
    Audioer(const char* FileNameIn); // Constructor
    ~Audioer();                      // Destructor

    void printFileInfo();            // Prints audio file info
    std::vector<float> getSamples(); // Returns audio samples

private:
    const char* FileName;            // File name
    SF_INFO sfinfo;                  // Sound file information
    SNDFILE* file;                   // Sound file handle
};

#endif // AUDIOER_H
