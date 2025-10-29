#include <iostream>
#include <list>
#include <queue>

#define MAX_QUEUES 64
#define MEMORY_SIZE 2048
#define CHUNK_SIZE 8


struct Queue {
	std::uint16_t head;
	std::uint16_t tail;
	std::uint16_t size;
};

struct DataChunk {
	unsigned char data[6];
	std::uint16_t next;
};

static std::uint16_t free_list_head = 0;

typedef std::uint16_t Q;

const size_t METADATA_POOL_SIZE = MAX_QUEUES * (sizeof(Queue) / sizeof(unsigned char));
const size_t DATA_POOL_START = METADATA_POOL_SIZE;

unsigned char data[MEMORY_SIZE];

static std::uint16_t queue_counter = 0;

void init_chunks() {
	const size_t TOTAL_CHUNKS = (MEMORY_SIZE - DATA_POOL_START) / CHUNK_SIZE;
	free_list_head = DATA_POOL_START;

	for (size_t i = 0; i < TOTAL_CHUNKS; ++i)
	{
		size_t current_chunk_index = DATA_POOL_START + (i * CHUNK_SIZE);

		DataChunk* current_chunk = reinterpret_cast<DataChunk*>(&data[current_chunk_index]);
		
		size_t next_chunk_index = DATA_POOL_START + ((i+1) * CHUNK_SIZE);

		if (i < TOTAL_CHUNKS - 1)
		{
			current_chunk->next = next_chunk_index;
		}
		else
		{
			current_chunk->next = NULL;
		}

		std::cout << "Chunk: " << i << " created. It points to: " << current_chunk->next << std::endl;
	}

	std::cout << "Memory allocated. " << "Free list head: " << free_list_head << std::endl;
}

// Handle out of memory exceptions
void on_out_of_memory() {
	std::cout << "Out of memory\n";
}
// Handle illegal requests
void on_illegal_operation() {
	std::cout << "Illegal operation\n";
}

// Creates a FIFO byte queue, returning a handle to it.
Q* create_queue() {
	size_t index = free_list_head;
	Queue* sq_ptr = reinterpret_cast<Queue*>(&data[index]);
	DataChunk* current_chunk = reinterpret_cast<DataChunk*>(&data[index]);

	sq_ptr->head = index;
	sq_ptr->tail = sq_ptr->head;
	sq_ptr->size = 0;

	if (current_chunk->next == NULL) {
		on_out_of_memory();
		return nullptr;
	}

	std::cout << "Queue created at: " << sq_ptr << ". Head at: " << sq_ptr->head << ". Tail at: " << sq_ptr->tail << ". Size: " << sq_ptr->size << std::endl;

	free_list_head = current_chunk->next;

	return reinterpret_cast<Q*>(sq_ptr);
}
// Destroy an earlier created byte queue.
/*void destroy_queue(Q* q) {
	Queue* sq_ptr = reinterpret_cast<Queue*>(q);

	sq_ptr->start_id = 0;
	sq_ptr->size = 0;
}*/
// Adds a new byte to a queue.
void enqueue_byte(Q* q, unsigned char b) {
	Queue* sq_ptr = reinterpret_cast<Queue*>(q);

	std::uint16_t sq_byte_count = sq_ptr->size % CHUNK_SIZE;

	DataChunk* current_chunk = reinterpret_cast<DataChunk*>(&data[sq_ptr->tail]);

	if (sq_byte_count == 0 && sq_ptr->size > 0) {

		size_t index = free_list_head;
		//free_list_head = ;
	}

	current_chunk->data[sq_byte_count] = b;

	sq_ptr->size++;

	//std::cout << "Enqueued '" << (int)b << "' into queue at " << absolute_index << ". New size: " << sq_ptr->size << std::endl;
}
// Pops the next byte off the FIFO queue.
/*unsigned char dequeue_byte(Q* q) {
	Queue* sq_ptr = reinterpret_cast<Queue*>(q);

	if (sq_ptr->size == 0) {
		on_illegal_operation();
		return 0;
	}

	unsigned char dequeued_byte = data[sq_ptr->start_id];

	sq_ptr->start_id++;
	sq_ptr->size--;

	return dequeued_byte;
}*/

int main()
{	
	init_chunks();


	/*for (size_t i = 0; i < 63; i++)
	{
		Q* q = create_queue();
		for (size_t j = 0; j < 12; j++)
		{
			enqueue_byte(q, j);
		}
		for (size_t j = 0; j < 12; j++)
		{
			printf("%d", dequeue_byte(q));
			printf("\n");
		}
		if (i == 12) {
			destroy_queue(q);
		}
	}*/

	Q* q0 = create_queue();
	Q* q1 = create_queue();

	std::cout << "Free list head: " << free_list_head << std::endl;

	enqueue_byte(q0, 0);
	enqueue_byte(q0, 1);

	/*
	Q* q0 = create_queue();
	enqueue_byte(q0, 0);
	enqueue_byte(q0, 1);
	Q* q1 = create_queue();
	enqueue_byte(q1, 3);
	enqueue_byte(q0, 2);
	enqueue_byte(q1, 4);
	printf("%d", dequeue_byte(q0));
	printf("%d\n", dequeue_byte(q0));
	enqueue_byte(q0, 5);
	enqueue_byte(q1, 6);
	printf("%d", dequeue_byte(q0));
	printf("%d\n", dequeue_byte(q0));
	destroy_queue(q0);
	printf("%d", dequeue_byte(q1));
	printf("%d", dequeue_byte(q1));
	printf("%d\n", dequeue_byte(q1));
	destroy_queue(q1);*/
}
