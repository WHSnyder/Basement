import copy
import numpy


objects = {}
id_max = 0

#Possibly worth switching to __slots__ for performance gains,
#for now this basic class will suffice.
class GameObject:

	def __int__(self, name):
		self.name = name 
		objects[name] = self

	def clone(self):
		return copy.deepcopy(self)

	def print_obj(self):
		print(vars(self))




















