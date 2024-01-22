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
    
    # Create blank white image of the same size as input.
    output:cv.Mat = npy.zeros((img1.shape[0], img1.shape[1], 3), npy.uint8)
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

def simple_weighted_avg( img1:cv.Mat, img2:cv.Mat, weight: float):
    
    if (img1 is None or img2 is None):
        raise SystemExit("ERROR: One or both images are NULL")
    
    if (img1.shape != img2.shape):
        raise  SystemExit("ERROR: Dimensions aren't the same")
            
    fused_image:cv.Mat = cv.addWeighted(img1, weight, img2, 1-weight, 0)

    # Convert to uint8 if necessary
    fused_image = npy.clip(fused_image, 0, 255).astype(npy.uint8)    
    return fused_image

if __name__ == "__main__":
    src = cv.imread("./exports/opencv/adaptive-histogram-eq/cl2_frame25.png", cv.IMREAD_GRAYSCALE)
    enh = cv.imread("./exports/opencv/adaptive-histogram-eq/cl3_frame25.png", cv.IMREAD_GRAYSCALE)

    src = cv.GaussianBlur(src, (5,5), .9, None, .9)
    enh = cv.GaussianBlur(enh, (5,5), 0)
    
    (ret, src) = cv.threshold(src, 90, 255, cv.THRESH_BINARY)
    (ret, enh) = cv.threshold(enh, 70, 255, cv.THRESH_TOZERO)

    
    # thresh1 = cv.adaptiveThreshold(src, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 15, 0.05)
    # thresh2 = cv.adaptiveThreshold(enh, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 9, 0.05)
    
    # fused = select_max(thresh1, thresh2)
    
    fused = simple_weighted_avg(src, enh, 0.5)

    
    # Side by side image concatenation - https://www.geeksforgeeks.org/how-to-display-multiple-images-in-one-window-using-opencv-python/ - Date Accessed: 14/12/2023
    cv.imshow("Source Images", npy.concatenate((src, enh), axis=1))
    cv.imshow("Binarized Images", npy.concatenate((src, enh), axis=1))
    
    cv.imshow("Fused image", fused)
    
    cv.waitKey()
    