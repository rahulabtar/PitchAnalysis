
import numpy as np
import csv

#Finds the Autocorrelation for a given lag 
def ACF(buffer, lag):
    R = 0 
    if lag == 0:
        R = np.sum(buffer * buffer) 
    else:
        R = np.sum(buffer[:-lag] * buffer[lag:])
    return R

#find the local maxima by looking to the right and left of a sample, makes a list of all observed maxima and returns it
def findLocalMaxima(corrs):
    size = len(corrs)
    maxima = []
    for lag in range(1, size - 1):
        if (corrs[lag] >= corrs[lag - 1] and corrs[lag] >= corrs[lag + 1]): 
            maxima.append(lag + 1)
    return maxima

#utilizes finding local maxima to calculate the distance between 
def getFreqRahul(buffer,fs):
    maxima = findLocalMaxima(buffer)
    if (len(maxima) < 2): return 0
    return fs / (maxima[1] - maxima[0])
    
#find the correlation values for the entire buffer, return a list of corrs corresponding to different lag values
def getCorr(samps):
    corrs = []
    # for lag in range(round(fs / fHigh) - 1, round(fs / fLow)):
    for lag in range(0, len(samps)):
         corrs.append((ACF(samps, lag))) 
    return corrs

#calculates the frequency detected by find the lag that corresponds to distance between two ACF peaks
def getFreq(corrs, fs):
    corrs = corrs[1:] #get rid of first sample because lag of zero will lead to highest AC value
    freq = fs / (corrs.index(max(corrs)) + 1) #maybe should be plus 2
    return freq

#scales the data between -1 and 1
def maxAbsoluteScaling(data):
    data = [abs(element) for element in data]
    xMax = max(data)
    data = [element / xMax for element in data]
    return data

#Generates a buffer of pure Sin wave given a frequency and sampling frequency and a number of samples
def genSin(f, fs, numSamp):
    n = np.arange(0, numSamp)
    samps = np.sin(2 * np.pi * (f / fs) * n)
    return samps

#Creates a loop that increases the sampling frequency and lists key values into csv 
with open('80Hz.csv', 'w', newline = '') as csvfile:
    csvwriter = csv.writer(csvfile)
    # Write the header row
    csvwriter.writerow(['Sampling Frequency', 'True Frequency', 'Frequency Calculated', 'ACF Vals:'])

    fn = 82 #about the freq of low e string guitar fundamental
    for fs in range(48000, 48010):
        numSamps = round(fs / fn * 3) #generates 3 cycles of signal
        samps = genSin(fn, fs, numSamps)
        corrs = getCorr(samps)
        #corrBestLag = corrs[1:].index(max(corrs[1:]))
        #idealLag = fs / fn
        #bestCorr = round(max(corrs[1:]),3)
        freqCalc = getFreqRahul(corrs, fs)
        roundedcorrs = [round(corr, 3) for corr in corrs]
        #csvwriter.writerow([fs, corrBestLag, idealLag, bestCorr, freqCalc] + roundedcorrs)
        csvwriter.writerow([fs, fn, freqCalc] + roundedcorrs)






