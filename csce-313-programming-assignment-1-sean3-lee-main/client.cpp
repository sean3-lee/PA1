/*
	Original author of the starter code
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date: 2/8/20
	
	Please include your Name, UIN, and the date below
	Name: Sean Lee
	UIN: 430008692
	Date: 09/22/2023
*/
#include "common.h"
#include "FIFORequestChannel.h"
#include <chrono>
#include <sys/wait.h>

using namespace std;
using namespace chrono;


int main (int argc, char *argv[]) {
	int opt;
	int p = 0;
	double t = -1.0;
	int e = 1;

	
	int s = MAX_MESSAGE;
	string filename = "";

	bool getNewPt = false;
	bool chanreq = false;
	bool file = false;
	bool data = false;
	
	while ((opt = getopt(argc, argv, "p:t:e:m:f:c")) != -1) {
		switch (opt) {
			case 'p':
				p = atoi (optarg);
				break;
			case 't':
				t = atof (optarg);
				break;
			case 'e':
				e = atoi (optarg);
				break;
			case 'm':
				s = atoi (optarg);
				break;
			case 'f':
				file = true;
				filename = optarg;
				break;
			case 'c':
				chanreq = true;
				break;
		}
	}


	if(fork() == 0){
		cout<<"child process running"<<endl;
		char *arg[] = {"./server","-m", (char *) to_string(s).c_str(), NULL}
		if( execvp (args[0], args) < 0){
			exit(0);
		}

    FIFORequestChannel* chan = new FIFORequestChannel ("control", FIFORequestChannel::CLIENT_SIDE);
	FIFORequestChannel* cchan = chan;

	auto tstart = high_resolution_clock::now()
	if(getnewPt){
		if(!data){
			datamsg d(p, t, e);
			double value;
			
			currentChan->cwrite (&d, sizeof (d));
			currentChan->cread (&value, sizeof(double));

			cout << "Patient " << p << " at time of " << t << " has a value of "<< value << endl;
		}
		else{
			datamsg d (p, 0.0, e);
			double value;
			for (int i=0; i<1000; i++){
				currentChan->cwrite (&d, sizeof (d));
				currentChan->cread (&value, sizeof (double));
				
				d.seconds += 0.004;
				cout << "Get value " << value << endl;
			}
		}
	}
	else if(file){
		filemsg f (0,0); 
		fileSize = sizeof (filemsg) + filename.size() + 1;
		
		char* buffer = new char [fileSize];
		memcpy (buffer, &f, sizeof(filemsg));
		strcpy (buffer + sizeof(filemsg), filename.c_str());
		cchan->cwrite (buffer, fileSize);
		
		__int64_t fileLen;
		
		cchan->cread (&fileLen, sizeof(__int64_t));
		filemsg* recevFile = (filemsg*) buffer;
		char* recvBuf = new char [s]; 

	
		string outfilepath = string("received/") + filename;
		FILE* outfile = fopen (outfilepath.c_str(), "wb");
		recevFile->offset = 0;
		__int64_t remBytes = fileLen;
		
		while (remBytes>0){
			// to fit in all size of 
			bufferrecevFile->length = (int)min(remBytes, (__int64_t)s);
			cchan->cwrite (buffer, fileSize);
			cchan->cread (recvBuf, MAX_MESSAGE);
			remBytes -= recevFile->length;
			fwrite (recvBuf, 1, recevFile->length, outfile);
			recevFile->offset += recevFile->length;
		}
		// close the file and delete the pointers
		fclose (outfile);
		delete recvBuf;
		delete buffer;
		cout << "The file " << filename << " has been transfered to ~/received/" << endl;
	}

	else if (chanreq){
		MESSAGE_TYPE m = NEWCHANNEL_MSG;
		char newChanBuf[MAX_MESSAGE];
		chan->cwrite (&m, sizeof (m));
		chan->cread (newChanBuf, sizeof(newChanBuf));
		
		// create a new channel and assign as the current using channel
		// the old channel is also kept in the memory
		cchan = new FIFORequestChannel (newChanBuf,FIFORequestChannel::CLIENT_SIDE);
		cout << "The new channel is now created. " << endl;
	}



	auto timeEnd = high_resolution_clock::now();
		
	// calculate the time for the process
	auto processDuration = duration_cast<microseconds>(timeEnd -timeStart).count();
	cout << "The process took " << (double)processDuration / 1000 << " ms." <<endl;
		
	// send out a quit message
	MESSAGE_TYPE q = QUIT_MSG;
	cchan->cwrite (&q, sizeof (MESSAGE_TYPE));
	
	// close the new channel if created
	if (cchan != chan){
		cout << "New channel closed" << endl;
		chan->cwrite (&q, sizeof (MESSAGE_TYPE));
	}
	wait (0);
	// example data point request
    /*char buf[MAX_MESSAGE]; // 256
    datamsg x(1, 0.0, 1);
	
	memcpy(buf, &x, sizeof(datamsg));
	chan.cwrite(buf, sizeof(datamsg)); // question
	double reply;
	chan.cread(&reply, sizeof(double)); //answer
	cout << "For person " << p << ", at time " << t << ", the value of ecg " << e << " is " << reply << endl;*/
	
    // sending a non-sense message, you need to change this
	filemsg fm(0, 0);
	string fname = "teslkansdlkjflasjdf.dat";
	
	int len = sizeof(filemsg) + (fname.size() + 1);
	char* buf2 = new char[len];
	memcpy(buf2, &fm, sizeof(filemsg));
	strcpy(buf2 + sizeof(filemsg), fname.c_str());
	chan.cwrite(buf2, len);  // I want the file length;

	delete[] buf2;
	
    // closing the channel    
    MESSAGE_TYPE m = QUIT_MSG;
    chan.cwrite(&m, sizeof(MESSAGE_TYPE));
}
