import cv2 as cv
import pywt
import numpy as npy
from scipy.stats import median_abs_deviation

# Source - https://medium.com/@koushikc2000/2d-discrete-wavelet-transformation-and-its-applications-in-digital-image-processing-using-matlab-1f5c68672de3 - 15/02/2024

# Import image
img:cv.Mat = cv.imread("../exports/opencv/adaptive-histogram-eq/cl2_frame25.png", cv.IMREAD_COLOR)
img_gray:cv.Mat

# Retrieve approximation and detail coefficients for each channel R, G, B
coeffs_R = pywt.dwt2(img[:,:,0], 'haar')
coeffs_G = pywt.dwt2(img[:,:,1], 'haar')
coeffs_B = pywt.dwt2(img[:,:,2], 'haar')

xaR, (xhR, xvR, xdR) = coeffs_R
xaG, (xhG, xvG, xdG) = coeffs_G
xaB, (xhB, xvB, xdB) = coeffs_B

xa = npy.stack([xaR, xaG, xaB], axis=-1)
xh = npy.stack([xhR, xhG, xhB], axis=-1)
xv = npy.stack([xvR, xvG, xvB], axis=-1)
xd = npy.stack([xdR, xdG, xdB], axis=-1)

xA = xa / 255.0

cv.imshow('xA', xA * 0.3)
cv.imshow('xh', npy.log10(xh) * 0.3)
cv.imshow('xv', npy.log10(xv) * 0.3)

cv.imshow('xd', npy.log10(xd) * 0.3)

# Apply DWT on each of the partial components above
coeffs_aaR = pywt.dwt2(xa[:,:,0], 'haar')
coeffs_aaG = pywt.dwt2(xa[:,:,1], 'haar')
coeffs_aaB = pywt.dwt2(xa[:,:,2], 'haar')

xaaR, (xhhR, xvvR, xddR) = coeffs_aaR
xaaG, (xhhG, xvvG, xddG) = coeffs_aaG
xaaB, (xhhB, xvvB, xddB) = coeffs_aaB

xaa = npy.stack([xaaR, xaaG, xaaB], axis=-1)
xhh = npy.stack([xhhR, xhhG, xhhB], axis=-1)
xvv = npy.stack([xvvR, xvvG, xvvB], axis=-1)
xdd = npy.stack([xddR, xddG, xddB], axis=-1)

xAA = xaa / 255.0

print(xAA.shape[0])

cv.imshow("Average Component ",(xAA * 0.3))
cv.imshow("Horizontal Component", (npy.log10(xhh) * 0.5))
cv.imshow("Vertical Component", (npy.log10(xvv) * 0.5))
cv.imshow("Diagonal Component", (npy.log10(xdd) * 0.5))

cv.waitKey(0)
cv.destroyAllWindows()
