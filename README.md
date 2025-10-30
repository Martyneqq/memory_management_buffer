# Installation Guide
1. git clone https://github.com/Martyneqq/memory_management_buffer.git
2. cd memory_management_buffer/SCS_test
4. mkdir build
5. cd build
6. cmake ..
7. make

# Documentation
In this project, I implemented a memory managing structure, that does not use heap memory for its allocation. Everything is stored in one array with size of 2048 bytes. The main focus is on efficiently combining speed and memory. All of the operations on the queue are O(1). However, the struct has to be initialized with O(n) time at the beginning, which is a small price to pay in comparison with the enqueue/dequeue speed (O(1)). Most of the data types are std::uint16_t to save as much memory as possible.
<img width="2192" height="504" alt="1" src="https://github.com/user-attachments/assets/9a86a6bd-a6a1-47ef-a0c4-364d522301a7" />

## Structs
+ Q (Queue)
  - tail = beginning of queue
  - head = end of queue
  - index_position = size/last index of queue
  - read_offset = marks the dequeued byte offset from the beginning of queue in the beginning of the queue
+ DataChunk
  - data[DATA_SIZE] = size of data segment in the chunk
  - next = pointer to the next chunk

## Functions
+ init_chunks
  - 
  <img width="2192" height="504" alt="2" src="https://github.com/user-attachments/assets/9faf14ca-a4f0-49b3-b754-180529c19ac8" />
+ create_queue
+ destroy_queue
+ enqueue_byte
+ dequeue_byte
+ on_out_of_memory
+ on_illegal_operation

