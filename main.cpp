/**************************************************************************************************
 **************************************************************************************************
 
     BSD 3-Clause License (https://www.tldrlegal.com/l/bsd3)
     
     Copyright (c) 2016 Andrés Solís Montero <http://www.solism.ca>, All rights reserved.
     
     
     Redistribution and use in source and binary forms, with or without modification,
     are permitted provided that the following conditions are met:
     
     1. Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the documentation
        and/or other materials provided with the distribution.
     3. Neither the name of the copyright holder nor the names of its contributors
        may be used to endorse or promote products derived from this software
        without specific prior written permission.
     
     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
     AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
     IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
     ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
     DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
     OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.
 
 **************************************************************************************************
 **************************************************************************************************/

#include "viva.h"
#include "factories.h"
#include <sstream>
using namespace viva;


int main(int argc, const char * argv[])
{
    const String keys =
        "{help h            |           | print this message}"
        "{@sequence         |           | url, file, folder, vot_sequence, sequence}"
        "{m method          |skcf       | tracking method: kcf, kcf2, skcf, ncc, tld, struck, ...}"
        "{p pause           |           | start sequence paused}"
        "{o output          |           | output video filename / folder for images output}"
    ;
    
    CommandLineParser parser(argc, argv, keys);

    if (parser.has("h"))
            parser.printMessage();

    string sequence = parser.get<string>(0);
    string method   = parser.get<string>("m");
    string ofilename   = parser.get<string>("o");
    
    Ptr<Input> input     = TrackerFactory::createInput(sequence);
    Ptr<Tracker> tracker = TrackerFactory::createTracker(method , argc, argv);
    Ptr<Output> output   = TrackerFactory::createOutput(ofilename);
    
    if (parser.has("h") || input.empty() || tracker.empty())
        return 0;
    
    vector<vector<Point2f> > groundTruth;
    TrackerFactory::loadGroundTruth(sequence, groundTruth);
    
    Ptr<ProcessFrame> process = new TrackingProcess(tracker, groundTruth);
    
    Processor processor;
    if (parser.has("p"))
         processor.startPaused();
   
    if (output && parser.has("o"))
        processor.setOutput(output);
    
    processor.setInput(input);
    processor.setProcess(process);
    processor.listenToKeyboardEvents();
    processor.listenToMouseEvents();
    processor.run();

    
    return 0;
}