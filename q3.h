#ifndef Q3_H
#define Q3_H
template<typename T> class BoundedBuffer {
	const unsigned int size;
	T *list;
	int head;
	int tail;
	bool isempty();
	bool isfull();
	unsigned int number_so_far;
	public:
    BoundedBuffer( const unsigned int size =10  );
    void insert( T elem );
    T remove();
	~BoundedBuffer();
};

#ifdef BUSY                            // busy waiting implementation
// implementation
uOwnerLock mutx;
uCondLock prod_lock;
uCondLock con_lock;
#endif
uOwnerLock sum_protect;

 // BUSY

#ifdef NOBUSY                          // no busy waiting implementation
uOwnerLock mutx;
uCondLock prod_lock;
uCondLock con_lock;
uCondLock barge;
//uCondLock barge_con;

// implementation
#endif // NOBUSY

_Task Producer {
	BoundedBuffer<int> &b;
	const int P;
	const int D;
    void main();
  public:
    Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay );
};

_Task Consumer {
	BoundedBuffer<int> &b;
	const int D;
	const int sen;
	int &s;
    void main();
  public:
    Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum );
};
#endif
