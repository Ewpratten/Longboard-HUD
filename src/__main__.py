import time

def printToDisplay(text):
	print("Display: " + str(text))

class Sensor(object):
	def __init__(self, id):
		self.id = id
		self.enabled = False
		self.saftey = True	# Must disable to use, just a saftey feature
		self.lasttime = time.clock()
	
	def enable(self):
		self.saftey = False
	
	def disable(self):
		self.saftey = True
	
	def read(self):
		value = True
		
		self.enabled = value
		return self.enabled
	
	def getDiff(self):
		if self.read():
			diff = time.clock() - self.lasttime
			self.lasttime = time.clock()
		else:
			diff = None
		return diff

hall1 = Sensor(1)
hall1.enable()

# set start times to show display is working
t1 = 999

while True:
	td1 = hall1.getDiff()
	if td1 != None:
		t1 = td1
	
	printToDisplay(t1)
	