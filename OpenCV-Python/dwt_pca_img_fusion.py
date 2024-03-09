import cv2 as cv
import pywt
import numpy as npy

class DWT_Coeffs:
    LL: npy.ndarray # Approximate coeffs   (low-pass)
    LH: npy.ndarray # Horizontal coeffs    (high-pass)
    HL: npy.ndarray # Vertical coeffs      (high-pass)
    HH: npy.ndarray # Diagonal coeffs      (high-pass)

    def __init__(self, _LL, _LH , _HL , _HH):
        self.LL = _LL 
        self.LH = _LH
        self.HL = _HL
        self.HH = _HH
        return
# ! class DWT_Coeffs

def get_dwt_coefficients(img: cv.Mat, wt:str) -> DWT_Coeffs:
    
    # Extract coefficients of image
    # LL - Approximate coeffs   (low-pass)
    # LH - Horizontal coeffs    (high-pass)
    # HL - Vertical coeffs      (high-pass)
    # HH - Diagonal coeffs      (high-pass)

    LL, (LH, HL, HH) = pywt.dwt2(img[:,:], wt) # Image coefficients
    return DWT_Coeffs(LL, LH, HL, HH)

def fuse_coefficients(cf1:npy.ndarray, cf2:npy.ndarray) -> npy.ndarray:

    # Reshape as a column matrix
    I11 = cf1.reshape(-1, 1)
    I22 = cf2.reshape(-1, 1)

    # Remove the mean of each column
    Z = npy.hstack((I11, I22))
    Me = npy.mean(Z, axis=0)
    X = Z - Me

    # # Calculate the covariance matrix
    C = npy.cov(X, rowvar=False)
    # D, V = npy.linalg.eigh(C) # D - Eigen Valeus, V - Eigen Vectors
    # D = D.T.reshape(-1, 1)

    vals, vecs = cv.eigenNonSymmetric(C)
    # print("Vals -> {0}".format(vals))
    # print("Vecs -> {0}".format(vecs))
        
    if vals[0] >= vals[1]:
        P = vecs[0] / npy.sum(vecs[:,0])
    else:
        P = vecs[1] / npy.sum(vecs[:,1])

    print(P)

    # Fuse coefficients based on weights
    fused_cf = (P[0] * cf1) + (P[1] * cf2)

    # Convert back to original size
    fused_cf = fused_cf.reshape(cf1.shape)

    return fused_cf

def PCA_onDWT(img1: cv.Mat, img2: cv.Mat) -> cv.Mat:
    
    coeff1 = get_dwt_coefficients(img1, wt='db4')
    coeff2 = get_dwt_coefficients(img2, wt='db4')

    # Fuse coefficients
    LL = fuse_coefficients(coeff1.LL, coeff2.LL) # Approximation
    LH = fuse_coefficients(coeff1.LH, coeff2.LH) # Horizontal
    HL = fuse_coefficients(coeff1.HL, coeff2.HL) # Vertical
    HH = fuse_coefficients(coeff1.HH, coeff2.HH)

    # Perform Inverse DWT 2D and convert to OpenCV Matrix
    return cv.convertScaleAbs(pywt.idwt2( (LL, (LH, HL, None)), wavelet='db4'))

if "__main__" == __name__:
    img1 = cv.imread('./exports/opencv/adaptive-histogram-eq/cl2_frame25.png', cv.IMREAD_GRAYSCALE)
    img2 = cv.imread('./exports/opencv/adaptive-histogram-eq/cl10_frame25.png', cv.IMREAD_GRAYSCALE)
    
    fused = PCA_onDWT(img1, img2)

    cv.imshow("Img 1", img1)
    cv.imshow("Img 2", img2)
    cv.imshow("Fused", fused)

    cv.waitKey(0)
    cv.destroyAllWindows()
