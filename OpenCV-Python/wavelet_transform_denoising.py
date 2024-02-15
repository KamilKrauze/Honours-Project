import cv2 as cv
import pywt
import numpy as npy
from scipy.stats import median_abs_deviation

# Source - https://medium.com/@koushikc2000/2d-discrete-wavelet-transformation-and-its-applications-in-digital-image-processing-using-matlab-1f5c68672de3 - 15/02/2024

# Import image
img:cv.Mat = cv.imread("./exports/opencv/adaptive-histogram-eq/cl2_frame25.png", cv.IMREAD_COLOR)
img_gray:cv.Mat
# Convert to grayscale
img_gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

# Apply Gaussian noise
mean = 0
std_dev = 0.2
noise = npy.random.normal(mean, std_dev, [512, 512])

img_noise:cv.Mat = npy.clip(img_gray + noise, 0, 255).astype(npy.uint8)

# Thresholding type
type = 'hard'

# Apply 3-level DWT2D (for 2D images)
coeffs = pywt.dwt2(img_noise, 'db1')  # level - 1
cA1, (cH1, cV1, cD1) = coeffs

coeffs2 = pywt.dwt2(cA1, 'db1')  # level - 2
cA2, (cH2, cV2, cD2) = coeffs2

coeffs3 = pywt.dwt2(cA2, 'db1')  # level - 3
cA3, (cH3, cV3, cD3) = coeffs3

# Level - 3
# Find threshold on detail components
T_cH3 = npy.median(npy.abs(cH3)) / 0.6745  # Adjusted median absolute deviation
T_cV3 = npy.median(npy.abs(cV3)) / 0.6745
T_cD3 = npy.median(npy.abs(cD3)) / 0.6745
# Apply threshold, these matrices are denoised before image reconstruction
Y_cH3 = pywt.threshold(cH3, T_cH3, type)
Y_cV3 = pywt.threshold(cV3, T_cV3, type)
Y_cD3 = pywt.threshold(cD3, T_cD3, type)

# Level - 2
# Find threshold on detail components
T_cH2 = npy.median(npy.abs(cH2)) / 0.6745
T_cV2 = npy.median(npy.abs(cV2)) / 0.6745
T_cD2 = npy.median(npy.abs(cD2)) / 0.6745
# Apply threshold, these matrices are denoised before image reconstruction
Y_cH2 = pywt.threshold(cH2, T_cH2, type)
Y_cV2 = pywt.threshold(cV2, T_cV2, type)
Y_cD2 = pywt.threshold(cD2, T_cD2, type)

# Level - 1
# Find threshold on detail components
T_cH1 = npy.median(npy.abs(cH1)) / 0.6745
T_cV1 = npy.median(npy.abs(cV1)) / 0.6745
T_cD1 = npy.median(npy.abs(cD1)) / 0.6745
# Apply threshold, these matrices are denoised before image reconstruction
Y_cH1 = pywt.threshold(cH1, T_cH1, type)
Y_cV1 = pywt.threshold(cV1, T_cV1, type)
Y_cD1 = pywt.threshold(cD1, T_cD1, type)

# Apply inverse discrete wavelet transform on all levels
Y_cA2 = pywt.idwt2((cA3, (Y_cH3, Y_cV3, Y_cD3)), 'db1')
Y_cA1 = pywt.idwt2((cA2, (Y_cH2, Y_cV2, Y_cD2)), 'db1')
Y_cA = pywt.idwt2((cA1, (Y_cH1, Y_cV1, Y_cD1)), 'db1')

cv.imshow("Original Image", img)
cv.imshow("Grayscale Image", img_gray)
cv.imshow("Noise Image", img_noise)
cv.imshow("Denoised", Y_cA.astype('uint8'))

cv.waitKey(0)
cv.destroyAllWindows()