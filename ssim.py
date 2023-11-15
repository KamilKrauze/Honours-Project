import cv2 as cv
import numpy as npy
import modules.imgUtilities as utils

def imageMean(image: cv.Mat) -> npy.float64:
    """Computes the average pixel intensity of the whole image.

    Args:
        image (cv.Mat): Input image

    Returns:
        npy.float64: The pixel intensity average.
    """
        
    img = utils.imageDimensions(image)
    
    # Total sum of pixel intensities of the input image
    pixelIntensitySum = npy.float64(0)
    for y in range(img.height):
        for x in range(img.width):
            pixelIntensitySum += image[y][x][0]
    
    mean = pixelIntensitySum / img.total_pixels
    return mean

def imageVariance(image: cv.Mat, mean: npy.float64) -> npy.float64:
    img = utils.imageDimensions(image)
    
    summation = npy.float64(0)
    for y in range(img.height):
        for x in range(img.width):
            summation += pow( (image[y][x][0] - mean), 2)
            
    variance = summation / img.total_pixels
    return variance

def imageCovariance(originalImage: cv.Mat, enhancedImage: cv.Mat) -> npy.float64:
    org_img = utils.imageDimensions(originalImage)
    enh_img = utils.imageDimensions(enhancedImage)
    
    org_mean = imageMean(originalImage)
    enh_mean = imageMean(enhancedImage)
    
    if ( (org_img.width != enh_img.width) or (org_img.height != enh_img.height) ):
        print("Image dimensions are not identical!")
        return -1
    totalPixels = (enh_img.total_pixels) - 1 # Could be original image, but there is no difference
    
    summation = npy.float64(0)
    for y in range(enh_img.height):
        for x in range(enh_img.width):
            summation += (originalImage[y][x][0] - org_mean) * (enhancedImage[y][x][0] - enh_mean)
            
    return summation / totalPixels

def SSIM(enhancedImage: cv.Mat , originalImage: cv.Mat) -> npy.float64:
    """Computes the structural similarity index measure.

    Args:
        enhancedImage (cv.Mat): The enhanced image.
        originalImage (cv.Mat): The original image prior to enhancement.

    Returns:
        npy.float64: The Structural Similarity Index Measure value.
    """
    originalMean = imageMean(originalImage)
    enhancedMean = imageMean(enhancedImage)
    
    originalVariance = imageVariance(originalImage, originalMean)
    enhancedVariance = imageVariance(enhancedImage, enhancedMean)
    
    covariance = npy.sqrt( imageCovariance(originalImage, enhancedImage) )
    k1 = 0.01
    L1 = npy.amax(originalImage)
    c1 = npy.power((k1 * L1), 2)
    
    k2 = 0.03
    L2 = npy.amax(enhancedImage)
    c2 = npy.power((k2 * L2), 2)
    
    numerator = (2 * originalMean * enhancedMean + c1) * (2 * covariance + c2)
    denominator = (npy.power(originalMean, 2) + npy.power(enhancedMean, 2) + c1) * (npy.power(originalVariance, 2) + npy.power(enhancedVariance, 2) + c2)
    
    return numerator / denominator

# fp_src:str = ".\\exports\\dicom-data\\frame00.png"
# fp_hist:str = ".\\exports\\opencv\\adaptive-histogram-eq\\cl3_frame00.png"
# src = cv.imread(fp_src)
# hist = cv.imread(fp_hist)

# ssim = SSIM(hist, src)
# print(ssim)