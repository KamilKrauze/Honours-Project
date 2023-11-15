from typing import List

import cv2 as cv
import numpy as npy

def entropy(probabilities: List[npy.float64]) -> npy.float64:
    """ Computes the entropy of a given list of values"""
    
    entropy = npy.float64(0.0)
    for p in probabilities:
        if p > 0:
            entropy -= p * npy.log2(p)
    
    return entropy

def coefficientsOfLocation(image):
    """ Returns a list of coefficients of 3x3 subregions of the image"""

    max_vals = list()
    min_vals = list()
    coefficients = []
    
    sub_region: npy.ndarray[any][any]    
    for y in range(510):
        for x in range(510):
            sub_region = image[y:y+3, x:x+3]

            max_vals.append(npy.amax(sub_region))
            min_vals.append(npy.amin(sub_region))

    
    if len(max_vals) == len(min_vals):
        for i in range(0, len(max_vals)):
            if ( max_vals[i] + min_vals[i] ) == 0: # Skip if denominator is 0
                i +=1
                i += 1
            else:
                a = ( max_vals[i] - min_vals[i] ) / ( max_vals[i] + min_vals[i] ) # (max - min) / (max + min)
                coefficients.append(a)
    return coefficients

def CII(sourceImg, enhancedImg) -> npy.float64:
    """Computers the Constrast Improvement Index - Higher the better"""
    
    Cloc_src = coefficientsOfLocation(sourceImg)
    Cloc_hist = coefficientsOfLocation(enhancedImg)

    entropy_src = entropy(Cloc_src)
    entropy_hist = entropy(Cloc_hist)

    CII = entropy_hist / entropy_src
    return CII
    
# fp_src:str = ".\\exports\\dicom-data\\frame00.png"
# fp_hist:str = ".\\exports\\opencv\\histogram-eq\\frame00.png"
# src = cv.imread(fp_src)
# hist = cv.imread(fp_hist)

# print(CII(src, hist))

