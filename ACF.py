import math

#"lag = sampling frequency / target frequency"
#looks correct...
def ACF(buffer, lag):
    R = 0 
    for n in range(0, len(buffer) - lag - 1):
        R += (buffer[n] * buffer[n - lag])
    R /= len(buffer)
    return R

def getCorr(samps, fLow, fHigh, fs):
    corrs = []
    # for lag in range(round(fs / fHigh) - 1, round(fs / fLow)):
    for lag in range(0, len(samps)):
         corrs.append((ACF(samps, lag))) 
    return corrs

# fs / "best lag" = suspected freq
# best lag = suspected freq * fs
def getFreq(corrs, fs, fHigh):
    corrs = corrs[1:] #get rid of first sample because that will be best 
    # freq = fs / (corrs.index(max(corrs)) + (fs / fHigh) + 1) 
    freq = fs / (corrs.index(max(corrs)) + 1) 
    return freq

#looks correct 
# fs / f = samples per cycle of freq of interest
# above times * number of desired cycles of freq of interest = numSamp

def genSin(f, fs, numSamp):
    samps = []
    for n in range(0,numSamp):
        samps.append(math.sin(math.pi * 2 * (f / fs) * n))
    return samps 

fLow = 1 #lower range of freq guess
fHigh = 20 #high range of freq guess

for i in range(20, 100):
    fn = 10
    fs = i
    numSamps = 30 #round(fs / fn * 3) #num of samples in sample buffer
    samps = genSin(fn, fs, numSamps)
    corrs = getCorr(samps, fLow , fHigh, fs)[1:]
    print("fs = " + str(i) + " ||  Corr best lag: " + str(corrs[1:].index(max(corrs[1:])) + 1) + " || " + "ideal lag " + str(fs / fn) + " || " + "freq calculated " + str(getFreq(corrs, fs, fHigh)))

fn = 1 #freq of genSin
fs = 9 #samp frequency to sample fn at 
numSamps = round(fs / fn * 3) #num of samples in sample buffer






