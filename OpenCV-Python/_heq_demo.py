import cv2 as cv
import numpy as npy
from matplotlib import pyplot as plt

# Filepath to image
file:str = "./exports/dicom-data/frame25.png"

# Read image
src:cv.Mat = cv.imread(file, cv.IMREAD_GRAYSCALE)

# Equalize histogram of image
dst = cv.equalizeHist(src)

# Compute source image's histogram
src_hist = cv.calcHist([src], [0], None, [256], [0,256])

# Compute histogram equalized image's histogram
dst_hist = cv.calcHist([dst], [0], None, [256], [0,256])

plt.figure(figsize=(22, 9))

plt.bar(npy.arange(256), src_hist[:,0], color='black', width=1)
plt.title("Histogram of Source Image @ Frame 25", fontsize=25)
plt.xlabel('Pixel Intensity', fontsize=20)
plt.ylabel('Frequency', fontsize=20)
plt.xticks(fontsize=18)
plt.yticks(fontsize=18)
plt.xlim([0, 256])
plt.ylim([0, npy.max(src_hist)*1.1])
plt.show()


plt.bar(npy.arange(256), dst_hist[:,0], color='black', width=1)
plt.title("Histogram of HEQ Image @ Frame 25", fontsize=25)
plt.xlabel('Pixel Intensity', fontsize=20)
plt.ylabel('Frequency', fontsize=20)
plt.xticks(fontsize=18)
plt.yticks(fontsize=18)
plt.xlim([0, 256])
plt.ylim([0, npy.max(dst_hist)*1.1])

plt.show()

# cv.imshow("Source - Frame 25", src)
# plt.title("Histogram of Source Image @ Frame 25")
# plt.xlabel('Pixel Intensity')
# plt.ylabel('Frequency')
# plt.plot(src_hist, color='black')
# plt.xlim([0,256])
# plt.show()
# cv.waitKey(0)

# # Compute histogram equalized image's histogram
# dst_hist = cv.calcHist([dst], [0], None, [256], [0,256])

# cv.imshow("HEQ - Frame 25", dst)

# plt.title("Histogram of HEQ Image @ Frame 25")
# plt.xlabel('Pixel Intensity')
# plt.ylabel('Frequency')
# plt.plot(dst_hist, color='black')
# plt.xlim([0,256])
# plt.show()
# cv.waitKey(0)