import cv2 as cv
import numpy as npy
from matplotlib import pyplot as plt

# File path to source image
file:str = "./exports/dicom-data/frame25.png"

# Read source image as grayscale
src:cv.Mat = cv.imread(file, cv.IMREAD_GRAYSCALE)

# Create the CLAHE object with clipLimit of 2, and kernel size of 8x8
clahe:cv.CLAHE = cv.createCLAHE(clipLimit=2, tileGridSize=(8,8))

# Apply CLAHE to source matrix
dst:cv.Mat = clahe.apply(src)

src_hist = cv.calcHist([src], [0], None, [256], [0,256])

dst_hist = cv.calcHist([dst], [0], None, [256], [0,256])
cv.imshow("Source - Frame 25", src)
plt.title("Histogram of Source Image @ Frame 25", fontsize=25)
plt.xlabel('Pixel Intensity', fontsize=20)
plt.ylabel('Frequency', fontsize=20)
plt.bar(npy.arange(256), src_hist[:,0], color='black', width=1)
plt.xticks(fontsize=18)
plt.yticks(fontsize=18)
plt.xlim([0,256])
plt.show()
cv.waitKey(0)

cv.imshow("CLAHE - Frame 25", dst)
plt.title("Histogram of CLAHE Image @ Frame 25", fontsize=25)
plt.xlabel('Pixel Intensity', fontsize=20)
plt.ylabel('Frequency', fontsize=20)
plt.bar(npy.arange(256), dst_hist[:,0], color='black', width=1)
plt.xticks(fontsize=18)
plt.yticks(fontsize=18)
plt.xlim([0,256])
plt.show()