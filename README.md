# Installation Guide
1. git clone https://github.com/Martyneqq/memory_management_buffer.git
2. cd memory_management_buffer/SCS_test
4. mkdir build
5. cd build
6. cmake ..
7. make

# Documentation
In this project, I implemented a memory managing structure, that does not use heap memory for its allocation. Everything is stored in one array with size of 2048 bytes. The main focus is on efficiently combining speed and memory. All of the operations on the queue are O(1). However, the struct has to be initialized with O(n) time at the beginning, which is a small price to pay in comparison with the enqueue/dequeue speed (O(1)). Most of the data types are std::uint16_t to save as much memory as possible. In total, with configuration of chunk size 12 bytes, 40% of the array is used as a metadata/pointer storage. Now I am working on further memory efficiency.
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
  - initialize all of the chunks in the data segment, connect them with their pointers. At the position of the first chunk points free_list_head static variable. Runs only once at the beginning of the program.
  - time complexity: O(n)
  <img width="2192" height="504" alt="2" src="https://github.com/user-attachments/assets/27be06a9-79f5-47d2-b752-79f384dad405" />

+ create_queue
  - in the metadata section, allocate space for a queue at position 0 in data array (next will be at position 8 because queue is 8 bytes long)
  - now, first chunk from the free chunk chain will be given to this queue to save its data. The chunk will receive position at the start of the free list. free_list_head's pointer is adjusted to point to the next chunk. This chunk will point to 0 to indicate the end of the queue.
  - time complexity: O(1)
  <img width="2192" height="504" alt="3" src="https://github.com/user-attachments/assets/0621a86d-1671-4990-af61-84993a61f970" />

+ destroy_queue
  - reattach current chunk to the free chain and adjust free_list_head to this chunk
  - time complexity: O(1)
  <img width="2192" height="504" alt="4" src="https://github.com/user-attachments/assets/a8c2939b-d84f-4314-a2cf-19864db7139e" />

+ enqueue_byte
  - time complexity: O(1)
  <img width="2192" height="966" alt="5" src="https://github.com/user-attachments/assets/10c2c13d-8299-4681-92a2-ed5f5e779657" />

  
+ dequeue_byte
  - time complexity: O(1)
  <img width="2192" height="966" alt="6" src="https://github.com/user-attachments/assets/d0c377f9-1391-42dd-9e2e-6c4d0ce0bd82" />
