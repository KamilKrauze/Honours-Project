import cv2 as cv
import numpy as npy

def select_max(img1: cv.Mat, img2: cv.Mat) -> cv.Mat:
    """Selects the greater pixel values and assigns that as the pixel to be used in the output image.

    Args:
        img1 (cv.Mat): Input image 1
        img2 (cv.Mat): Input image 2

    Raises:
        SystemExit: Crash the program if one or both images are NULL.
        SystemExit: If either image dimensions are not identical then crash the program.

    Returns:
        cv.Mat: The fused output image
    """
    
    if (img1 is None or img2 is None):
        raise SystemExit("ERROR: One or both images are NULL")
    
    if (img1.shape != img2.shape):
        raise  SystemExit("ERROR: Dimensions aren't the same")
    
    output:cv.Mat = npy.zeros((512, 512, 3), npy.uint8)
    output[:,:,0] = 0xFF # Blue channel
    output[:,:,1] = 0xFF # Green channel
    output[:,:,2] = 0xFF # Red channel
    
    for row in range(img1.shape[0]):
        for col in range(img1.shape[1]):
            
            # Select maximum pixel intensity
            if (img1[row][col] >= img2[row][col]):
                output[row][col] = img1[row][col]
            else:
                output[row][col] = img2[row][col]
    
    return output



if __name__ == "__main__":
    src = cv.imread("./exports/opencv/adaptive-histogram-eq/cl3_frame25.png", cv.IMREAD_GRAYSCALE)
    enh = cv.imread("./exports/opencv/adaptive-histogram-eq/cl85_frame25.png", cv.IMREAD_GRAYSCALE)
    fused = select_max(src, enh)
    
    # Side by side image concatenation - https://www.geeksforgeeks.org/how-to-display-multiple-images-in-one-window-using-opencv-python/ - Date Accessed: 14/12/2023
    cv.imshow("Source Images", npy.concatenate((src,enh), axis=1))
    
    cv.imshow("Fused image", fused)
    
    cv.waitKey()
    