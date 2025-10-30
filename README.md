# Installation Guide
1. git clone https://github.com/Martyneqq/memory_management_buffer.git
2. cd memory_management_buffer/SCS_test
4. mkdir build
5. cd build
6. cmake ..
7. make

# Documentation
In this project, I implemented a memory managing structure, that does not use heap memory for its allocation. Everything is stored in one array with size of 2048 bytes. The main focus is on efficiently combining speed and memory. All of the operations on the queue are O(1). However, the struct has to be initialized with O(n) time at the beginning, which is a small price to pay in comparison with the enqueue/dequeue speed (O(1)). Most of the data types are std::uint16_t to save as much memory as possible.
## Structs
+ Q (Queue)
  - tail = marks the beginning of the queue
  - head = marks the end of the queue
  - index_position = marks the size/last index of the queue
  - read_offset = marks the missing bytes dequeued in the beginning of the queue
+ DataChunk
  - data[DATA_SIZE] = 
  - next = 

The core functionality is:
+ Take the first index in the array where the data segment starts
+ Initialize CHUNK sized objects in the whole data segment. The DataChunk structure includes array and pointer (2 bytes) segment, so there would be always 2 bytes per chunk consumed by the pointer. This approach is necessary to keep O(1) adding/removing speed.
+ Each DataChunk is in this phase connected by pointers. This approach gives us information where the data will continue if we overflow our chunk with data.
+ Static variable free_list_head is set to point at this chain of DataChunks. Every chunk in this chain is empty. If we now need a new chunk in the case of chunk overflow, we can take one from here and connect it to the end of the current chunk.
+ Now, when user creates a Q, new chunk is created at the position of this free_list_head. Inside it, there is a queue that starts at this first chunk. At the same time, the information about this queue is kept in the first data segment (metadata, starting at 0).
+ The free_list_head will then point to the next chunk, and the taken chunk will be disconnected from the chain of free chunks. Now the chunk is ready to accept data when enqueueing.
+ 
