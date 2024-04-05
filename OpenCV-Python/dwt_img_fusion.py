import cv2 as cv
import pywt
import numpy as npy

from rich.progress import track as prog_bar

# Loosley based-off the MATLAB example, ported to Python myself
# https://www.youtube.com/watch?v=cDFJzrlm_U0
# 21/02/2024

def fuse_images(img1:cv.Mat, img2:cv.Mat, wt:str) -> cv.Mat:
    
    # Extract coefficients of both input images
    # LL - Approximate coeffs   (low-pass)
    # LH - Horizontal coeffs    (high-pass)
    # HL - Vertical coeffs      (high-pass)
    # HH - Diagonal coeffs      (high-pass)
    
    COEFFS_1 = pywt.dwt2(img1[:,:], wt) # Image 1 Coefficients
    LL1,(LH1, HL1, HH1) = COEFFS_1
    
    COEFFS_2 = pywt.dwt2(img2[:,:], wt) # Image 2 Coefficients
    LL2,(LH2, HL2, HH2) = COEFFS_2
    
    # Create empty coeffs for fusion.
    LL = npy.zeros((len(LL1), len(LL1)), dtype=npy.float64)
    LH = npy.zeros((len(LH1), len(LH1)), dtype=npy.float64)
    HL = npy.zeros((len(HL1), len(HL1)), dtype=npy.float64)
    HH = npy.zeros((len(HH1), len(HH1)), dtype=npy.float64)
    
    for i in range(len(LL)):
        for j in range(len(LL)):
            LL[i,j] = npy.max([LL1[i,j], LL2[i,j]])
            LH[i,j] = npy.max([LH1[i,j], LH2[i,j]])
            HL[i,j] = npy.max([HL1[i,j], HL2[i,j]])
            # HH[i,j] = npy.max([HH1[i,j], HH2[i,j]])

    reconstructed = pywt.idwt2( (LL, (LH, HL, None)), wavelet=wt)
    
    return cv.convertScaleAbs(reconstructed)

if __name__ == "__main__":  
    # img1:cv.Mat = cv.imread("./exports/opencv/adaptive-histogram-eq/cl2_frame25.png", cv.IMREAD_GRAYSCALE)
    # img2:cv.Mat = cv.imread("./exports/opencv/adaptive-histogram-eq/cl5_frame25.png", cv.IMREAD_GRAYSCALE)
    # fused_img = fuse_images(img1, img2, wt='db4')
    
    fused:cv.Mat = list()
    
    file_ext:str = ".png"
    fp1:str = "./exports/opencv/adaptive-histogram-eq/cl2_frame"
    fp2:str = "./exports/opencv/adaptive-histogram-eq/cl4_frame"
    
    export_fp = "./exports/dwt/cl2_cl10_fusion_db4/"
    export_name = "cl2_cl10_frame"
    
    for i in prog_bar(range(61), description="Computing DWT Image Fusion..."):
        number:str = str(i)
        if (i < 10):
            number = "0" + str(i)
        
        img1:cv.Mat = cv.imread((fp1+number+file_ext), cv.IMREAD_GRAYSCALE)
        img2:cv.Mat = cv.imread((fp2+number+file_ext), cv.IMREAD_GRAYSCALE)
        
        fused_image:cv.Mat = fuse_images(img1, img2, wt='db4')
        fused.append(fused_image)
        
    for i in prog_bar(range(61), description="Exporting..."):
        number:str = str(i)
        if (i < 10):
            number = "0" + str(i)
        path:str=export_fp + export_name + number + file_ext
        cv.imwrite(path, fused[i])