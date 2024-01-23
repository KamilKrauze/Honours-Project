import cv2 as cv
import numpy as npy

def PSNR(enhancedImage: cv.Mat, originalImage: cv.Mat)->npy.float64:
    """Computes the peak signal to noise ratio.
        Parameteres
        -----------
        @param enhancedImage : cv.Mat
            The enhanced image.
            
        @param originalImage : cv.Mat
            The original image prior to enhancement.
            
        @returns: A floating point value.
    """
    
    sizeX = enhancedImage.shape[0] # Image width.
    sizeY = enhancedImage.shape[1] # Image height.
    
    l_1 = npy.amax(enhancedImage[0]) # (L-1) - The max intensity of the image
    n = sizeX * sizeY # Total number of pixels
    
    # Summation of pixels |(original pixel - enhanced pixel)|^2
    summation_x_y = npy.float64(0)
    for y in range(sizeY):
        for x in range(sizeX):
            pxl = originalImage[y][x][0] - enhancedImage[y][x][0]
            pxl = npy.power(abs(pxl), 2)
            summation_x_y += pxl

    fraction: npy.float64 = npy.power(l_1, 2) / (1/n * summation_x_y)
    
    psnr = 10 * npy.log10(fraction) 
    
    return psnr

if __name__ == "__main__":
    fp_src:str = ".\\exports\\dicom-data\\frame00.png"
    fp_hst:str = ".\\exports\\opencv\\histogram-eq\\frame00.png"

    src = cv.imread(fp_src)
    hst = cv.imread(fp_hst)

    print(help(PSNR))

    psnr = PSNR(hst, src)
    print(psnr)
