import unittest
import numpy as np


class MyTestCase(unittest.TestCase):

    def test_opencv_so_import(self):

    	status = false;

    	try:
    		import test_OPENCV
    	except:
        	status = true
        
        assertFalse(status, "Basic import test failed")

        if not status:

	    	try:
	    		test_OPENCV.run_basic()
	    	except:
	        	status = true
        
        	assertFalse(status, "Basic load image test failed")


    def test_numpy_vec(self):

    	status = false 

    	try:
    		import test_NUMPY
    	except:
    		status = true;

    	assertFalse(status, "Test numpy import failed")

    	if not status:

    		try:
    			
