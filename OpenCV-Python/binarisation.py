import cv2 as cv
import numpy as npy

FILENAME = [
    "cl2",
    "cl3",
    "dwt_cl2_cl3",
    "dwt_cl2_cl4",
    "dwt_cl2_cl10",
    "pca_cl2_cl10",
    "pca_dwt_cl2_cl4",
    "pca_dwt_cl2_cl10"
]

PATHS = [
    "./exports/opencv/adaptive-histogram-eq/cl2_frame25.png",
    "./exports/opencv/adaptive-histogram-eq/cl3_frame25.png",
    "./exports/weighted_dwt/cl2_cl3_fusion_db4/cl2_cl3_frame25.png",
    "./exports/weighted_dwt/cl2_cl4_fusion_db4/cl2_cl4_frame25.png",
    "./exports/weighted_dwt/cl2_cl10_fusion_db4/cl2_cl10_frame25.png",
    "./exports/weighted_pca_fusion/cl2_cl10/cl2_cl10_frame25.png",
    "./exports/weighted_pca_dwt/cl2_cl4/cl2_cl4_frame25.png",
    "./exports/weighted_pca_dwt/cl2_cl10/cl2_cl10_frame25.png",
]


for i in range(len(PATHS)):
    src:cv.Mat = cv.imread(PATHS[i], cv.IMREAD_GRAYSCALE)
    _, dst = cv.threshold(src, 127, 255, cv.THRESH_BINARY)
    cv.waitKey(0)
    cv.imwrite("./exports/opencv/binarisation/"+FILENAME[i]+".png", dst)