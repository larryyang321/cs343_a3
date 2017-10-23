template<typename T> _Task Quicksort {
  public:
    Quicksort( T values[], unsigned int low, unsigned int high, unsigned int depth );
};

unsigned int uDefaultStackSize() {
    return 512 * 1000;        // set task stack-size to 512K
}

uProcessor p[ (1 << depth) - 1 ] __attribute__(( unused )); // 2^depth-1 kernel threads
