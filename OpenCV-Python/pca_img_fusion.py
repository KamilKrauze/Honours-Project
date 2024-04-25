import cv2 as cv
import pywt
import numpy as npy
import scipy as spy

from rich.progress import track as prog_bar


def doPCA_fusion(img1:cv.Mat, img2:cv.Mat) -> cv.Mat:

    # Reshape as a column matrix
    I11 = img1.reshape(-1, 1)
    I22 = img2.reshape(-1, 1)

    # Remove the mean of each column
    Z = npy.hstack((I11, I22))
    Me = npy.mean(Z, axis=0)
    X = Z - Me

    # # Calculate the covariance matrix
    C = npy.cov(X, rowvar=False)
    vals, vecs = spy.linalg.eigh(C) # Eigen Valeus, Eigen Vectors
    vals = vals.T.reshape(-1, 1)
    
    # Compute the weights
    # Equation 4.1 - https://tigerprints.clemson.edu/cgi/viewcontent.cgi?article=1615&context=all_dissertations - 16/02/2024
    weight1:npy.float64 = 0.0
    weight2:npy.float64 = 0.0
    
    for i in range(len(vecs)):
        weight1 += vecs[i][0] / (vecs[i][0] + vecs[i][1])
        weight2 += vecs[i][1] / (vecs[i][0] + vecs[i][1])

    # Low pass coefficients by weighted average
    # Equation 4.2 - https://tigerprints.clemson.edu/cgi/viewcontent.cgi?article=1615&context=all_dissertations - 16/02/2024
    fused = npy.zeros((len(img1), len(img1)), dtype=npy.float64)
    
    for i in range(len(img1)):
        for j in range(len(img1)):
            fused[i,j] = (weight1 * img1[i,j]) + (weight2 * img2[i,j])

    # Normalize the fused image
    fused = (fused - npy.min(fused)) / (npy.max(fused) - npy.min(fused)) * 255
    fused = fused.astype(npy.uint8)

    # Convert back to original size
    fused = fused.reshape(img1.shape)
    
    # Fused image 
    return fused

if "__main__" == __name__:
    # img1:cv.Mat = cv.imread("./exports/opencv/adaptive-histogram-eq/cl2_frame25.png", cv.IMREAD_GRAYSCALE)
    # img2:cv.Mat = cv.imread("./exports/opencv/adaptive-histogram-eq/cl10_frame25.png", cv.IMREAD_GRAYSCALE)
    
    # fused_image:cv.Mat = doPCA_fusion(img1, img2)
    # cv.imshow("Fused", fused_image)
    # cv.waitKey(0)
    # exit()
    
    fused:cv.Mat = list()
    
    file_ext:str = ".png"
                 
    fp1:str = "./exports/opencv/adaptive-histogram-eq/cl2_frame"
    fp2:str = "./exports/opencv/adaptive-histogram-eq/cl10_frame"
    
    export_fp = "./exports/weighted_pca_fusion/cl2_cl10/"
    export_name = "cl2_cl10_frame"
    
    for i in prog_bar(range(61), description="Computing Weighted PCA Image Fusion..."):
        number:str = str(i)
        if (i < 10):
            number = "0" + str(i)
        
        img1:cv.Mat = cv.imread((fp1+number+file_ext), cv.IMREAD_GRAYSCALE)
        img2:cv.Mat = cv.imread((fp2+number+file_ext), cv.IMREAD_GRAYSCALE)
        
        fused_image:cv.Mat = doPCA_fusion(img1, img2)
        fused.append(fused_image)
        
    for i in prog_bar(range(61), description="Exporting..."):
        number:str = str(i)
        if (i < 10):
            number = "0" + str(i)
        path:str=export_fp + export_name + number + file_ext
        cv.imwrite(path, fused[i])
