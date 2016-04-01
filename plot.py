#!/usr/bin/python
# /**************************************************************************************************
#  **************************************************************************************************
#  
#      BSD 3-Clause License (https://www.tldrlegal.com/l/bsd3)
#      
#      Copyright (c) 2016 Andres Solis Montero <http://www.solism.ca>, All rights reserved.
#      
#      
#      Redistribution and use in source and binary forms, with or without modification,
#      are permitted provided that the following conditions are met:
#      
#      1. Redistributions of source code must retain the above copyright notice,
#         this list of conditions and the following disclaimer.
#      2. Redistributions in binary form must reproduce the above copyright notice,
#         this list of conditions and the following disclaimer in the documentation
#         and/or other materials provided with the distribution.
#      3. Neither the name of the copyright holder nor the names of its contributors
#         may be used to endorse or promote products derived from this software
#         without specific prior written permission.
#      
#      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#      AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#      IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#      ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#      LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#      DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#      LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
#      OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
#      OF THE POSSIBILITY OF SUCH DAMAGE.
#  
#  **************************************************************************************************
#  **************************************************************************************************/


from shapely.geometry import Polygon
import argparse
import os.path as op
import csv
import numpy as np
import matplotlib.pyplot as plt

#
# Parses csv values into list of polygons and centroids
#
def parseCSV(filename):
	csv_data = {}
	csv_data["size"] = 0;
	csv_data["polygons"] = []
	csv_data["centroids"] = []
	with open(filename,'r') as csvfile:
		csvreader = csv.reader(csvfile)
		for row in csvreader:
			#Convert x,y, width, height to
			# clock-wise ordered
			# x1, y1, x2, y2, x3, y3, x4, y4
			row = [ float(x) for x in row]
			if len(row) == 4: 
				row = [row[0], \
					   row[1], \
					   row[0] + row[2], \
					   row[1], \
					   row[0] + row[2],\
					   row[1] + row[3],\
					   row[0],\
					   row[1] + row[3]];
			# Assume format of 8 coordinates
			# clock-wise ordered
			# x1, y1, x2, y2, x3, y3, x4, y4 
			# to create a polygon and compute its centroid
			if len(row) == 8:
				polygon = Polygon([(row[0], row[1]),\
								   (row[2], row[3]),\
								   (row[4], row[5]),\
								   (row[6], row[7])])
				csv_data["polygons"].append(polygon)
				csv_data["centroids"].append(polygon.centroid)
				
	csv_data["size"] = len(csv_data["polygons"])
	return csv_data

#
# Computes delta and accuracy between 
# ground-truth and execution over the same sequence
# there is no checking, assuming data is correct
#
def compute(data1, data2):
	result = {}
	result["name"] = data2["name"]
	result["delta"] = []
	result["accuracy"] = []
	for idx in range (0, data1["data"]["size"]):
		centroid1 = data1["data"]["centroids"][idx]
		centroid2 = data2["data"]["centroids"][idx]
		polygon1  = data1["data"]["polygons"][idx]
		polygon2  = data2["data"]["polygons"][idx]
		interArea = polygon1.intersection(polygon2).area
		unionArea = polygon1.union(polygon2).area
		result["delta"].append(centroid1.distance(centroid2))
		result["accuracy"].append( interArea / unionArea)
	return result

#
# Accuracy as defined in the VOT Challenge
#
def accuracyPlot(results, show ):
	filename = 'accuracy.png'
	plt.ylabel("Accuracy")
	plt.xlabel("Frame n")
	plt.title("Accuracy per Frame")
	plt.axis([0, len(results[0]["delta"]), 0, 1])
	plt.grid()
	for result in results:
		name = '{0} [{1:.2f}]'.format(result["name"], \
			sum(result["accuracy"])/len(result["accuracy"]))
		plt.plot(result["accuracy"], label=name)
	plt.legend()
	if (show):
		plt.show()
	else:
		plt.savefig(filename)
		print filename, ': saved.'
	
#
# Precision Plot as defined in the Tracking Benchmark
#
def precisionPlot(results, show):
	thrMax = 50
	filename = 'precision.png'
	plt.ylabel("Precision")
	plt.xlabel("Location error threshold")
	plt.title("Overall Precision")
	plt.axis([0, thrMax, 0, 1])
	plt.grid()
	for result in results:
		delta = result["delta"]
		data   =[]
		datalen= len(delta)
		for x in range(0, thrMax + 1):
			data.append(float(sum( 1  for idx in delta if idx <= x))/datalen )
		name = '{0} [{1:.2f}]'.format(result["name"], \
			sum(data)/len(data))
		plt.plot(data, label=name)
	plt.legend()
	if (show):
		plt.show()
	else:
		plt.savefig(filename)
		print filename, ': saved.'

#
# Success Plot as defined in the Tracking Benchmark
#
def successPlot(results, show):
	step = 0.1
	filename = 'success.png'
	plt.ylabel("Success")
	plt.xlabel("Overlap threshold")
	plt.title("Overall Success")
	plt.axis([0, 1, 0, 1])
	plt.grid()
	for result in results:
		accuracy = result["accuracy"]
		data   =[]
		datalen= len(accuracy)
		for x in np.arange(0,1.1,0.1):
			data.append(float(sum( 1  for idx in accuracy if idx >= x))/datalen )
		name = '{0} [{1:.2f}]'.format(result["name"], \
			sum(data)/len(data))
		plt.plot(data, np.arange(0,1.1,0.1), label=name)
	plt.legend()
	if (show):
		plt.show()
	else:
		plt.savefig(filename)
		print filename, ': saved.'

def getSequencePath(sequence):
	with open('sequences.txt', 'r') as myfile:
		folder = myfile.read().replace('\n', '')
	return op.join(folder, sequence, 'groundtruth.txt')
#
# Returns true if the following sequence has a groundtruth file 
def sequenceExists(sequence):
	path = getSequencePath(sequence)
	return (op.isfile(path),path)

#
#  Command line arguments
#
parser = argparse.ArgumentParser(description='Plot vivaTracker ground-truth/output files')
parser.add_argument('files', metavar='N', type=str, nargs='+',
                   help='filenames with ground-truth/output per frame number in comma \
                   separated value format. First filename must be the ground-truth.')
parser.add_argument('--plot', dest='method', choices=['accuracy', 'precision', 'success'],\
					help="plot the selected graph")
parser.add_argument('--save', dest='save', action='store_true', default=False)
#
#  Arguments
args  = parser.parse_args()

# Check file
files = [args.files[idx] for idx in range(1, len(args.files))  \
									if op.isfile(args.files[idx])]

# Check ground-truth
groundtruth =  sequenceExists(args.files[0])
if groundtruth[0]:
	files = [groundtruth[1]] + files
elif op.isfile(groundtruth[1]):
	files = [args.files[0]] + files

# Parse ground-truth data 
data  =  [ {"name": op.splitext(op.basename(file))[0], \
		    "data": parseCSV(file)} \
		    for file in files]
#
# Compute:
# Delta  (i.e., euclidean distance) between centroids for each frame 
# Accuracy (i.e., A & B / A | B) for each frame
results = [compute(data[0], data[idx]) for idx in range(1, len(data))]

if (args.method == 'accuracy'):
	accuracyPlot(results, not(args.save))
elif (args.method == 'precision'):
	precisionPlot(results, not(args.save))
else:
	successPlot(results, not(args.save))
	


