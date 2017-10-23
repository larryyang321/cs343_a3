#ifndef QUICKSORT_H
#define QUICKSORT_H

template<typename T> _Task Quicksort {
	T *value;

	unsigned int low;
	unsigned int high;
	unsigned int depth;
//	T *value;
	void main();
	void sort( int l,  int h);
	int partition( int l,  int h);


  public:
    Quicksort( T values[], unsigned int low, unsigned int high, unsigned int depth );
};

unsigned int uDefaultStackSize() {
    return 512 * 1000;        // set task stack-size to 512K
}

//uProcessor p[ (1 << depth) - 1 ] __attribute__(( unused )); // 2^depth-1 kernel threads


#endif
