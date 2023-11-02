from __future__ import print_function
import cv2 as cv
import pydicom as dicom
import numpy

ds = dicom.read_file("./P_1_SR3")

DIMS = (ds.Rows, ds.Columns)
SPACING = float(ds.PixelSpacing[0], ds.PixelSpacing[1] , ds.SliceThickness )

print (SPACING)
# cv.imshow("DICOM pixel data", ds.PixelData)
# cv.imshow('HE Image', dst)