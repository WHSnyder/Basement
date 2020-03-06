import unittest

class MyTestCase(unittest.TestCase):

    def test_opencv_so_import(self):

    	status = false;

    	try:
    		import test_OPENCV
    	except:
        	status = true
        
        assertFalse(status, "Basic import test failed")


    def test_opencv_so_run(self):

    	status = false

    	try:
    		test_OPENCV.run_basic
    	except:
        	status = true
        
        assertFalse(status, "Basic load image test failed")