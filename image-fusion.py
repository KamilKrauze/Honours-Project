from typing import List
import cv2 as cv
import numpy as npy

def SR_method(img1: cv.Mat, img2: cv.Mat, levels:int) -> cv.Mat:
    # Create image copies and convert to floating point
    image1 = npy.float64(img1.copy())
    image2 = npy.float64(img2.copy())

    # Apply Laplacian pyramid decomposition on both images
    gp1: List[npy.float64] = [image1.copy()]
    gp2: List[npy.float64] = [image2.copy()]
    
    for i in range(levels):
        # Gaussian pyarmid - https://docs.opencv.org/4.x/d4/d1f/tutorial_pyramids.html - 14/12/2023
        image1 = cv.pyrDown(image1)
        image2 = cv.pyrDown(image2)
        gp1.append(image1.copy())
        gp2.append(image2.copy())

    # Generate the laplacian pyramid for the images
    lp1: List[npy.float64] = [gp1[levels - 1]]
    lp2: List[npy.float64] = [gp2[levels - 1]]
    
    for i in range(levels - 1, 0, -1):
        # Expand the smaller image
        image1 = cv.pyrUp(gp1[i])
        image2 = cv.pyrUp(gp2[i])

        # Resize the larger image to match the size of the smaller one
        temp1 = cv.resize(gp1[i - 1], (image1.shape[1], image1.shape[0]))
        temp2 = cv.resize(gp2[i - 1], (image2.shape[1], image2.shape[0]))

        # Add Laplacian
        new_image1 = cv.subtract(temp1, image1)
        new_image2 = cv.subtract(temp2, image2)

        lp1.append(new_image1)
        lp2.append(new_image2)
        
    # Combine the Laplacian pyramid of two images
    fused_pyramid = []
    for l1, l2 in zip(lp1, lp2):
        fused_pyramid.append((l1 + l2) / 2)
    
    print(len(fused_pyramid))
    
    # Reconstruct the fused image from the fused pyramid
    fused_image = fused_pyramid[0]
    for i in range(1, levels):
        fused_image = cv.pyrUp(fused_image)
        fused_image = cv.add(fused_pyramid[i], fused_image)
        
    fused_image = npy.uint8(fused_image)
    return fused_image

if __name__ == "__main__":
    src = cv.imread("./exports/opencv/adaptive-histogram-eq/cl2_frame25.png")
    enh = cv.imread("./exports/opencv/adaptive-histogram-eq/cl8_5_frame25.png")
    fused = SR_method(src, enh, 1)
    
    # Side by side image concatenation - https://www.geeksforgeeks.org/how-to-display-multiple-images-in-one-window-using-opencv-python/ - Date Accessed: 14/12/2023
    cv.imshow("Source Images", npy.concatenate((src,enh), axis=1))
    
    cv.imshow("Fused image", fused)
    
    cv.waitKey()
    