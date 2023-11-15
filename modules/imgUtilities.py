from typing import Tuple

import cv2 as cv

class ImageProperties:
    """Simple data structure to hold image properties."""
    def __init__(self, width: int, height: int, totalPixels: int):
        self.width = width
        self.height = height
        self.total_pixels = totalPixels
    
    width: int
    height: int
    total_pixels: int

def imageDimensions(image: cv.Mat) -> ImageProperties:
    """Returns the image dimensions and total pixel count.

    Args:
        image (cv.Mat): Input image

    Returns:
        ImageProperties: -> Image properties
    """
    width = image.shape[0]
    height = image.shape[1]
    
    return ImageProperties(height, width, (height * width) )
