# Import / Bult-in packages and modules.
import os
import cv2 as cv
import numpy as npy

# Custom self-written modules.
from modules import ioUtilties as IOUtils
from modules import imgUtilities as ImgUtils
from modules.imgUtilities import ImageProperties as IMG

def threshold(image: cv.Mat, threshold: npy.float64, clampBelow:bool=True, clampAbove:bool=True) -> cv.Mat:    
    """Thresholds the pixel values to specified value.

        Anything beyond the threshold point is set to white,
        anything below the threshold point is set to black.

    Args:
        image (cv.Mat): The input image to threshold.
        threshold (npy.float64): The threshold point (Min=0, Max=1.0).
        clampBelow (bool, optional): Should it set the pixel colour values below the threshold to black?Defaults to True.
        clampAbove (bool, optional): Should it set the pixel colour values above the threshold to white? Defaults to True.

    Returns:
        cv.Mat: The thresholded image.
    """


    imageCopy = image.copy() # Image copy to not override the source image

    properties:IMG = ImgUtils.imageDimensions(image)
    threeChanThreshold: npy.ndarray = ImgUtils.LinearToRGB3f(threshold)


    for y in range(properties.height):
        for x in range(properties.width):
            if(imageCopy[y][x][0] > threeChanThreshold[0] and 
               imageCopy[y][x][1] > threeChanThreshold[1] and 
               imageCopy[y][x][2] > threeChanThreshold[2] and 
               clampAbove):
                
                imageCopy[y][x][0] = 255 # Red channel
                imageCopy[y][x][1] = 255 # Blue channel
                imageCopy[y][x][2] = 255 # Green channel

            elif (imageCopy[y][x][0] <= threeChanThreshold[0] and 
                  imageCopy[y][x][1] <= threeChanThreshold[1] and 
                  imageCopy[y][x][2] <= threeChanThreshold[2] and 
                  clampBelow):
                
                imageCopy[y][x][0] = 0 # Red channel
                imageCopy[y][x][1] = 0 # Blue channel
                imageCopy[y][x][2] = 0 # Green channel
            
    return imageCopy







# Run this code section 
if __name__ == "__main__":
    FILEPATH:str = f"./exports/opencv/adaptive-histogram-eq/cl3_frame45.png"
    if(not IOUtils.checkIfDirectoryExists(FILEPATH)):
        print("FILEPATH is invalid!")
        exit(0)

    img = cv.imread(FILEPATH) 
    dst = threshold(img, 0.475, True, True)

    cv.imshow("HE - Frame 0" , img)
    cv.imshow("HE - Frame 0 - Thresholded", dst)
    
    cv.imwrite("./exports/opencv/binarisation/cl3/frame45.png", dst)
    cv.waitKey()