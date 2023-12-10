from typing import Tuple

import cv2 as cv
import numpy as npy

class ImageProperties:
    """Simple data structure to hold image properties."""
    def __init__(self, width: int, height: int, totalPixels: int):
        self.width = width
        self.height = height
        self.total_pixels = totalPixels
    
    width: int
    height: int
    total_pixels: int


def Linear1fToRGB( float1: npy.float64 ) -> int:
    """Converts linear scale values to RGB integer scale

    Args:
        float1 (npy.float64): Min=0.0, Max=1.0

    Returns:
        int: RGB integer scale
    """
    float1 = max(min(0, float1), 1)
    return float1 * 255

def Linear2fToRGB( float1: npy.float64, float2: npy.float64 ) -> npy.ndarray:
    """Converts linear scale values to RGB integer scale, 2 channel colour component

    Args:
        float1 (npy.float64): Min=0.0, Max=1.0
        float2 (npy.float64): Min=0.0, Max=1.0

    Returns:
        npy.ndarray[int, int]: 2 channel colour
    """
    
    float1 = max(min(0, float1), 1)
    float2 = max(min(0, float2), 1)
    
    return npy.ndarray[ float1*255, float2*255 ]

def Linear3fToRGB( float1: npy.ndarray, float2: npy.ndarray, float3: npy.ndarray ):
    """Converts linear scale values to RGB integer scale, 3 channel colour component

    Args:
        float1 (npy.ndarray): Min=0.0, Max=1.0
        float2 (npy.ndarray): Min=0.0, Max=1.0
        float3 (npy.ndarray): Min=0.0, Max=1.0

    Returns:
        npy.ndarray[int, int, int]:  2 channel colour
    """
    
    float1 = max(min(0, float1), 1)
    float2 = max(min(0, float2), 1)
    float3 = max(min(0, float3), 1)

    return npy.ndarray[ float1*255, float2*255, float3*255 ]

def LinearToRGB3f( float1: npy.float64 ) -> npy.ndarray:
    floatClamped:npy.float64 = max(min(1, float1), 0)

    return npy.array([int(floatClamped*255), int(floatClamped*255), int(floatClamped*255)])


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