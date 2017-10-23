#include<iostream>
#include "MPRNG.h"
#include "q3.h"
using namespace std;
MPRNG *mprng;
bool signal_flag=false;


template<typename T>
BoundedBuffer<T>::BoundedBuffer( const unsigned int size  ):size(size){
	list =new T[size];

	/*while(i < size){
		list[i] = -1;
		i++;
	}*/
	head =0;
	tail = 0;
	number_so_far =0;

}
template<typename T>
BoundedBuffer<T>::~BoundedBuffer(){
	delete[] list;
}

template<typename T>
bool BoundedBuffer <T>::isempty(){
	return (number_so_far == 0);
}
template<typename T>
bool BoundedBuffer <T>::isfull(){
        return (number_so_far == size);
}


template<typename T>   
 void BoundedBuffer<T>::insert( T elem ){

		#ifdef BUSY
			mutx.acquire();
			
			while(isfull()){
			//	con_lock.signal();
				prod_lock.wait(mutx);
				
			}

		#endif

		#ifdef NOBUSY
			mutx.acquire();
			if(signal_flag){
				//prod_lock.signal();
			//	cout<<"here"<<endl;
				barge.wait(mutx);
			}
			if(isfull()){
				signal_flag = true;
				if(!barge.empty()){
					barge.signal();
				}else if(!con_lock.empty()){
					con_lock.signal();
				}else{
					signal_flag = false;
				}
				prod_lock.wait(mutx);

			}


		#endif


		assert(!isfull());
		list[tail] = elem;
		tail = ((tail+1)%size);
		number_so_far ++;
		
		#ifdef BUSY
		//	mutx.release();
			if(!con_lock.empty())con_lock.signal();
			mutx.release();
		#endif

		#ifdef NOBUSY
			/*if(!con_lock.empty()){
				signal_flag =true;
				con_lock.signal();

			}else*/ 
			if(!barge.empty()){
				signal_flag = true;
				barge.signal();
			}else if(!con_lock.empty()){
                                signal_flag =true;
                                con_lock.signal();

                        }else{
				signal_flag = false;
			}
			mutx.release();
		#endif
			
	}
template<typename T>
T BoundedBuffer<T>::remove(){

	#ifdef NOBUSY
                        mutx.acquire();
                        if(signal_flag){
				//con_lock.signal();
			//	cout<<"con"<<endl;	
				barge.wait(mutx);
				
			}
                        if(isempty()){
				signal_flag = true;
				if(!barge.empty()){
					barge.signal();
				}else if(!prod_lock.empty()){

					prod_lock.signal();
				}else {
					signal_flag = false;
				}
				con_lock.wait(mutx);
			}


                #endif
	#ifdef BUSY
		mutx.acquire();

		while(isempty()){
			//prod_lock.signal();
			con_lock.wait(mutx);
			
		}

	#endif
		T tem;
		assert(!isempty());

		number_so_far--;
		tem =list[head];	
		head = ((head+1)%size);
	#ifdef BUSY
		if(!prod_lock.empty())prod_lock.signal();
		mutx.release();
	#endif	
	#ifdef NOBUSY
                        /*if(!prod_lock.empty()){
				signal_flag = true;
                                prod_lock.signal();

                        }else*/
			 if (!barge.empty()){
				signal_flag = true;
                                barge.signal();
                        }else if(!prod_lock.empty()){
                                signal_flag = true;
                                prod_lock.signal();

                        }else{
				signal_flag = false;
			}
                        mutx.release();
                #endif

	return tem;


}




	void Producer::main(){
		int i =0;
		while(i < P){
			yield((*mprng)(D-1));
			b.insert(i+1);
			i++;
		}
	}
   Producer:: Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay ):b(buffer), P(Produce) ,D(Delay){}

    void Consumer::main(){
		int i =0;
		while(true){
			yield((*mprng)(D-1));
		
			i = b.remove();
			if(i == sen){
				break;
			}
			sum_protect.acquire();
			s += i;
			sum_protect.release();
		}
	}
    Consumer::Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum ):b(buffer),D(Delay),sen(Sentinel),s(sum){}

void usage(){
	cerr<<"Usage: ./bufferNOBUSY.txt [ Cons (> 0) [ Prods (> 0) [ Produce (> 0) [ BufferSize (> 0) [ Delay (> 0) ] ] ] ] ]"<<endl;
	exit(-1);
}
void uMain::main(){
	int cons = 5;
	int prods = 3;
	int produce = 10;
	int buffersize =10;
	int delays = -1;
	//all default

	switch(argc){
		case 6:
			try{
				delays = stoi(argv[5]); 		
			}catch(invalid_argument& e){
				exit(-1);
			}
			if(delays <=0)usage();
		case 5:
			try{
                                buffersize = stoi(argv[4]);
                        }catch(invalid_argument& e){
                                exit(-1);
                        }
			if(buffersize <=0)usage();
		case 4:
			try{
                                produce = stoi(argv[3]);
                        }catch(invalid_argument& e){
                                exit(-1);
                        }
			if(produce <=0)usage();
		case 3:
                        try{
                                prods = stoi(argv[2]);
                        }catch(invalid_argument& e){
                                exit(-1);
                        }
                        if(prods <=0)usage();
		case 2:
                        try{
                                cons = stoi(argv[1]);
                        }catch(invalid_argument& e){
                                exit(-1);
                        }
                        if(cons <=0)usage();
		default:
			break;
		
	}
	if(delays == -1)delays = prods+cons;
	cout<<cons<< " "<<prods<< " "<<produce<< " "<<buffersize<< " "<<delays<<endl;
	#ifdef __U_MULTI__
		uProcessor p[3] __attribute__(( unused )); // create 3 kernel thread for a total of 4
	#endif // U MULTI
	mprng = new MPRNG();
	int sum = 0;
	BoundedBuffer <int> buf(buffersize);
	Consumer *con[cons];
	Producer * pro[prods];
	int i = 0;
	while(i < cons){
		con[i] = new Consumer(buf,delays,SENTINEL, sum);
		i++;
	}
	i = 0;
	while (i < prods){
		pro[i] = new Producer(buf, produce, delays);
		i++;
	}
	
	
	i =0;
	while(i < prods){
		delete pro[i];
		i++;
	}
	 i =0;
	while(i < cons){
		buf.insert(SENTINEL);
		i++;
	}
	i = 0;
	cout<<"total: "<<sum<<endl;
	while(i < cons){
		delete con[i];
		i++;
	}

	
	delete mprng;

}
