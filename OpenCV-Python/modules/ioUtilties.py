from typing import List 
import os

def readFileToList(fp:str) -> List[str]:
    """Reads contents from file and appends to list. Also removes "\n" characters if found.

    Args:
        fp (str): Filepath

    Returns:
        List[str]: Contents of file as list.
    """
    if not checkIfDirectoryExists(fp):
        return list()
    
    
    io = open(fp, "r")
    lines = io.readlines()
    
    data = list()
    
    for line in lines:
        data.append(line.rstrip('\n'))
    
    return data

def writeListToFile(list: List, fp:str):
    """Writes list data to file where each element is on a new line.

    Args:
        list (List): The list to write to file.
        fp (str): The filepath, if it does not exist, it will be created.
    """
    io = open(fp, "w")
    
    for i in range(len(list)):
        if (i != (len(list)-1)):
            io.write( str(list[i]) +"\n")
        else:
            io.write( str(list[i]) )    
    return

def checkIfDirectoryExists(dir: str) -> bool:
    """Check if specified directory exists.

    Args:
        dir (str): The directory.

    Returns:
        bool:\nTrue - The directory exists.
        \nFalse - Directory does not exist 
    """
    return os.path.exists(dir)

def getListofImgs(dir: str, suffix: str) -> List[str]:
    """Gets a list of images from directory specified.

    Santizes list to only files that have '.png' format and by suffix
    
    Args:
        dir (str): Directory
        suffix (str): Specific file suffix 

    Returns:
        List[str]: List of PNG images.
    """      
    if (not checkIfDirectoryExists(dir)):
        raise IOError("ERROR: File not found!")    
    
    files = os.listdir(dir)
    images = list()
    
    searchForSuffix: bool = (suffix != "")
    
    for i in range(len(files)):
        temp_str:str = ""
        if(searchForSuffix):
            if files[i].startswith(suffix): 
                temp_str = files[i]
            else:
                continue
        else:
            temp_str = files[i]  
            
        if(temp_str.endswith(".png")):
            images.append(temp_str)    
    return images