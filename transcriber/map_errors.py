class MapFileError(Exception):
    """ Base exception for script. """
    pass

class DataFileError(MapFileError):
    pass

class LineError(MapFileError):
    """Exception for errors related to lines in map files
    eg - wrong number of quantas in a line"""

    def __init__(self, line):
        self.line_num = line.line_num
        self.trigger = line.trigger
        self.file_name = ''

        
    

