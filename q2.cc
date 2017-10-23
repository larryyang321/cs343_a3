#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "q2.h"
using namespace std;


template <typename T>
void Quicksort<T>::main(){
	sort((int)low, (int)high);

}
template <typename T>
void Quicksort<T>::sort( int l,  int h){
	//cout<<l<<" "<<h<<endl;
	if(l < h){
		int position;
		position = partition(l, h);
		//cout<<position<<endl;
		verify();
		if(depth >0){
			depth--;
			//sort(l, position-1);
			Quicksort<T> right(value,(unsigned int)(position +1),h,depth);
			sort(l,position-1);
			//sort(position +1, h);
		}else{
			sort(l,position -1);
			sort(position+1, h);
		}
	}

}

template <typename T>
int Quicksort<T>::partition( int l,  int h){
	T pivot = value[l + (h - l)/2];
	//cout<<pivot<<endl;
	value [l + (h - l)/2] = value[h];
	value [h] = pivot;
	//cout<<pivot<<endl;
	int i = l -1;
	for(int j = l; j <(int)h; j++){
		if(value[j] <= pivot){
			i++;
			T tem = value[i];
			value[i] = value[j];
			value[j] = tem;
			
		}
	}
	T tem = value[h];
	value[h] = value[i+1];
	value[i+1] = tem;
	return i+1;

//	return 0;		

	

}

template <typename T>
Quicksort<T>::Quicksort( T values[], unsigned int low, unsigned int high, unsigned int depth ):value(values),low(low),high(high),depth(depth){



}

void usage(  ) {
    cerr << "Usage: quicksort -s input-file [output-file]"  << endl;
    cerr << "Usage: quicksort -t size [depth]" <<endl;
    exit( EXIT_FAILURE );				// TERMINATE
} // usage

void uMain::main(){

	if(argc <2){
		usage();
		
	}
	if(strcmp(argv[1], "-s") ==0){
		istream *infile = &cin;				// default value
    		ostream *outfile = &cout;			// default value
		switch (argc) {
            case 4:
                try {
                    outfile = new ofstream(argv[3]);
                } catch (uFile::Failure) {
                    cerr << "Could not open output file " << argv[3]<<endl;
                    
                    exit(-1);
                }
            case 3:
                try {
                    infile = new ifstream(argv[2]);
                } catch (uFile::Failure) {
                    cerr << "Could not open input file " << argv[2]<<endl;
                    exit(-1);
                }

                break;
            default:
                usage();
               
        }

	//start input


		for(;;){
			unsigned int count;
			*infile >>count;
			if(!*infile){
	//	cout<<"fault"<<endl;		
				break;
			}
			//cout<<count<<endl;

			TYPE *value = new TYPE[count];
			int i =0;
			while( i < (int)count){
			//	TYPE tem;
				*infile >> value[i];
				if(!*infile){
					cerr<<"not enough input"<<endl;
					exit(-1);
				}
				*outfile <<value[i];
				if((i+1) %22 == 0){
					*outfile << endl<<" ";
					
				}
				if(i<((int)count-1))*outfile <<" ";		
				i++;

			}
		*outfile<<endl;

			{
				Quicksort<TYPE> first(value,0,count-1,0);
			
			}
			i = 0;
			while(i < (int)count){
				*outfile << value[i];
				if((i+1) %22 == 0){
                                        *outfile << endl<<" ";

                                }
				if(i<((int)count -1))*outfile << " ";
				i++;

			}
			*outfile<<endl;
			*outfile<<endl;
			delete[] value;


		}
			if ( infile != &cin ) delete infile;		// close file, do not delete cin!
    			if ( outfile != &cout ) delete outfile;		// close file, do not delete cout!
			
		

	}else if(strcmp(argv[1], "-t") ==0){
		int depth =0;
		int size =0;
		switch(argc){
			case 4:
				try{
					depth = stoi(argv[3]);
				}catch(invalid_argument &e){
					exit(-1);
				}
			case 3:
				try{
                                        size = stoi(argv[2]);
                                }catch(invalid_argument &e){
                                        exit(-1);
                                }
				break;
			default:
				usage();


		}

		TYPE *value = new TYPE[size];
			

		int i =0;
		while(i < size){
			value[i] = size - i;
		//	cout << value[i]<<" ";
			i++;

		}
		//cout<<endl;
		{
			uProcessor p[(1 << depth) - 1] __attribute__(( unused ));
			Quicksort<TYPE> first(value, 0, (unsigned int)(size-1),depth);

		}
		
		delete[] value;

	}else{
		//cout<<argv[1]<<endl;
		usage();
	}



}

//uProcessor p[ (1 << depth) - 1 ] __attribute__(( unused )); // 2^depth-1 kernel threads
