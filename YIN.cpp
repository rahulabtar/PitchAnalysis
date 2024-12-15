#include <iostream>
#include <cmath>
#include <vector>
using namespace std; 

vector<float> genSignal(float sampLength, float freq, float sampFreq, float phaseOffset, float dcOffset){
    if (!((sampFreq) > 2.2 * freq)) {
        cout<<" Error Sampling Frequency needs to be 2.2 times greater than freq to satisfy Nyquist"; 
        abort(); 
    }
    cout << endl<< "Generating signal" << endl;
    cout << "Frequency of " << freq << " and total " << sampLength / (sampFreq / freq) << " cycles over the length of the vector"<< endl; 
    cout << "Sampling Period of "<< sampFreq << " and length " << sampLength * (1 / sampFreq) << " seconds" << endl << endl; 

    vector<float> signal(sampLength);
    double sampPeriod = (1 / sampFreq); 
    for (int i = 0; i < sampLength; i++){
        signal[i] = sin(2 * M_PI * freq * i * sampPeriod - phaseOffset) + dcOffset;
    }

    return signal; 
}


// has been fixed to be inclusive similar to python
vector<float> slice(vector<float> vec, size_t start, size_t end) {
    if (start >= vec.size() || end > vec.size() || start > end) {
        throw out_of_range("Invalid indices");
    }
    return vector<float>(vec.begin() + start - 1, vec.begin() + end);
}

//Dot product has been checked and works 
float dotProduct(vector<float> vec1, vector<float> vec2) {
    // Ensure both vectors have the same size
    if (vec1.size() != vec2.size()) {
        throw invalid_argument("Vectors must have the same size for element-wise multiplication");
    }

    float result = 0; // Initialize result vector with the same size
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += vec1[i] * vec2[i]; // Multiply corresponding elements
    }

    return result;
}

//lag is the amount of samples you shift the window of the Auto Correlation Function by
float ACF(vector<float> signal, int windowSize, float timeStep, int lag){
    // The ACF value (similar to error) can be found by finding the error between the signal and a suspected frequency
    vector<float> sigStrip = slice(signal, timeStep, timeStep + windowSize);
    vector<float> sigLag = slice(signal, lag + timeStep, lag + timeStep + windowSize);
    return dotProduct(sigStrip, sigLag);
}

float YIN(vector<float> signal, int windowSize, float timeStep, float sampleRate, int lowBound, int highBound){
    if (highBound - lowBound < 0) throw invalid_argument("High bound must be greater than low bound"); 

    vector<float> ACF_vals;
    // i is the timestep, or the starting sample of the signal
    // windowSize is the size of the window we are looking at, basically a chunk of samples of the signal
    for (int i = lowBound; i < highBound; i++){ 
        ACF_vals.push_back(ACF(signal, windowSize, timeStep, i));
    }

    //out of all the ACF vals for different frequencies, which had the least error?

    // Find the iterator to the maximum element
    auto maxIt = std::max_element(ACF_vals.begin(), ACF_vals.end());
    // Calculate the index of the maximum element
    int indexOfMax = std::distance(ACF_vals.begin(), maxIt);
    // Adjust the index by adding lowBound
    float bestSample = ACF_vals[indexOfMax] + lowBound; 

    return sampleRate/bestSample; //converts and returns the frequency with the least error 

}


int main(){
    float sampleRate = 500;
    int start = 0;
    int end = 5; 
    int numSamples = (sampleRate * (end - start) + 1);
    int windowSize = 200; //in samples
    int lowBound = 0; 
    int highBound = numSamples / 2; 

    vector<float> signal(numSamples);
    signal = genSignal(numSamples, 1, sampleRate, 0, 0);
    cout << "Detected Frequency in hz " << YIN(signal, windowSize, 1, sampleRate, lowBound, highBound); 

    vector<float> tmp1;
    tmp1.push_back(0.0f);
    tmp1.push_back(1.0f);
    tmp1.push_back(2.0f);
    tmp1.push_back(3.0f);
    tmp1.push_back(4.0f);
    tmp1.push_back(5.0f);
    tmp1.push_back(6.0f);
    tmp1.push_back(7.0f);
    tmp1.push_back(8.0f);
    tmp1.push_back(9.0f);

    vector<float> result = slice(tmp1, 0, 4);
    for (int i = 0; i < result.size(); i++) cout << result[i] << endl; 
    
    return 0; 
}; 
