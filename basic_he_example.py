import os
import cv2 as cv

src = cv.imread("./kamil.png")

if src is None:
    print('Image could not be opened at:\n./kamil.png')
    exit(0)

src = cv.cvtColor(src, cv.COLOR_RGB2GRAY)

dst = cv.equalizeHist(src)

cv.imshow('Source Image', src)
cv.imshow('HE Image', dst)

cv.waitKey()