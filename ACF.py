import struct
import math

#may not be correct...
def readWavVals():

    # Open the .wav file in binary mode
    with open("YINPlay/test.wav", "rb") as file:
        # Skip the first 44 bytes (header) to get to the waveform data
        file.seek(44)
        
        # Read the rest of the file (audio data)
        audio_data = file.read()

    # Define sample size and number of channels
    sample_size = 2  # 16 bits = 2 bytes
    num_channels = 2  # Change to 2 for stereo audio

    # Iterate through the audio data in chunks of 2 bytes (16 bits)
    samples = []
    for i in range(0, len(audio_data), sample_size * num_channels):
        # Extract 16-bit sample(s) depending on the number of channels
        if num_channels == 1:  # Mono audio
            sample = int.from_bytes(audio_data[i:i+sample_size], byteorder="little", signed=True)
            samples.append(sample)
        elif num_channels == 2:  # Stereo audio
            left_sample = int.from_bytes(audio_data[i:i+sample_size], byteorder="little", signed=True)
            right_sample = int.from_bytes(audio_data[i+sample_size:i+2*sample_size], byteorder="little", signed=True)
            #samples.append((left_sample, right_sample))
            samples.append(left_sample)

    return samples 

    # Output the samples (one line per sample)
    # Output every 100th sample
    # for index, sample in enumerate(samples):
    #     if 1000 < index < 1100 :  # Check if the index is divisible by 100
    #         print(f"Sample {index}: {sample}")

def readWavHeader():
    
    # Open the .wav file in binary mode
    with open("YINPlay/test.wav", "rb") as file:
        # Read the first 44 bytes (standard size of a .wav file header)
        header = file.read(44)

    # Display the header in hexadecimal and ASCII formats
    print("Header in Hex:")
    print(" ".join([format(byte, "02X") for byte in header]))

    print("\nHeader in ASCII:")
    print("".join([chr(byte) if 32 <= byte <= 126 else "." for byte in header]))

    # Interpret specific header fields (example: RIFF and WAVE identifiers)
    riff = header[0:4].decode("ascii")
    wave = header[8:12].decode("ascii")
    print(f"\nChunk ID: {riff}")
    print(f"Format: {wave}")

    # Extract metadata (assuming little-endian byte order)
    file_size = int.from_bytes(header[4:8], byteorder="little")
    audio_format = int.from_bytes(header[20:22], byteorder="little")
    num_channels = int.from_bytes(header[22:24], byteorder="little")
    sample_rate = int.from_bytes(header[24:28], byteorder="little")
    bit_depth = int.from_bytes(header[34:36], byteorder="little")

    print(f"File Size: {file_size} bytes")
    print(f"Audio Format: {audio_format} (1 = PCM)")
    print(f"Number of Channels: {num_channels}")
    print(f"Sample Rate: {sample_rate} Hz")
    print(f"Bit Depth: {bit_depth} bits")


#"lag = sampling frequency / target frequency"
def ACF(buffer, lag):
    R = 0 
    for n in range(0, len(buffer) - lag - 1):
        R += (buffer[n] * buffer[n - lag])
    R *= 1/len(buffer)
    return R

# fs / "best lag" = suspected freq
def getCorr(samps, fLow, fHigh, fs):
    corrs = []
    for lag in range(round(fs / fHigh) - 1, round(fs / fLow)):
        corrs.append(abs(ACF(samps, lag))) #notsure about abs
    return corrs

def getFreq(corrs, fs, fHigh):
    print("meow: " + str(corrs.index(max(corrs))))
    freq = fs / (corrs.index(max(corrs)) + (fs / fHigh) + 1) 
    return freq

#looks correct 
# fs / f = samples per cycle of freq of interest
# above times * number of desired cycles of freq of interest = numSamp

def genSin(f, fs, numSamp):
    samps = []
    for n in range(0,numSamp):
        samps.append(round(math.sin(math.pi * 2 * (f / fs) * n), 5))
    return samps 

fn = 200 #freq of genSin
fs = 22000 #samp frequency to sample fn at 
numSamps = round(fs / fn * 3) #num of samples in sample buffer
fLow = 1 #lower range of freq guess
fHigh = 400 #high range of freq guess


samps = genSin(fn, fs, numSamps)
#print(samps)
corrs = getCorr(samps, fLow , fHigh, fs)
#print(corrs)
print(getFreq(corrs, fs, fHigh))



