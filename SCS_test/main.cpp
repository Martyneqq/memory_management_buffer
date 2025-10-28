#include <iostream>
#include <list>
#include <queue>

#define MAX_QUEUES 64
#define MEMORY_SIZE 2048
#define PAGE_SIZE 8

struct SubQueue {
	std::uint16_t start_id;
	std::uint16_t size;
};

typedef std::uint16_t Q;

const size_t METADATA_POOL_SIZE = MAX_QUEUES * (sizeof(SubQueue) / sizeof(unsigned char));
const size_t DATA_POOL_START = METADATA_POOL_SIZE;

unsigned char data[MEMORY_SIZE];

static std::uint16_t queue_counter = 0;

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
	size_t sq_byte_index = (queue_counter * (sizeof(SubQueue)/sizeof(unsigned char)));

	if (sq_byte_index >= DATA_POOL_START) {
		on_out_of_memory();
		return nullptr;
	}

	SubQueue* sq_ptr = reinterpret_cast<SubQueue*>(&data[sq_byte_index]);
	sq_ptr->start_id = DATA_POOL_START + (queue_counter * PAGE_SIZE);
	sq_ptr->size = 0;

	if (sq_ptr->start_id + PAGE_SIZE > MEMORY_SIZE) {
		on_out_of_memory();
		queue_counter--;
		return nullptr;
	}

	std::cout << "Sub queue created at: " << sq_ptr << std::endl;

	queue_counter++;

	return reinterpret_cast<Q*>(sq_ptr);
}
// Destroy an earlier created byte queue.
void destroy_queue(Q* q) {
	SubQueue* sq_ptr = reinterpret_cast<SubQueue*>(q);

	sq_ptr->start_id = 0;
	sq_ptr->size = 0;
}
// Adds a new byte to a queue.
void enqueue_byte(Q* q, unsigned char b) {
	SubQueue* sq_ptr = reinterpret_cast<SubQueue*>(q);

	if (sq_ptr->size >= PAGE_SIZE) {
		on_out_of_memory();
	}
	if (sq_ptr->start_id >= MEMORY_SIZE) {
		on_out_of_memory();
	}

	std::uint16_t relative_offset = sq_ptr->size % PAGE_SIZE;
	std::uint16_t absolute_index = sq_ptr->start_id + relative_offset;

	if (absolute_index >= MEMORY_SIZE) {
		on_out_of_memory();
		return;
	}

	data[absolute_index] = b;

	sq_ptr->size++;

	std::cout << "Enqueued '" << (int)b << "' into queue at " << absolute_index << ". New size: " << sq_ptr->size << std::endl;
}
// Pops the next byte off the FIFO queue.
unsigned char dequeue_byte(Q* q) {
	SubQueue* sq_ptr = reinterpret_cast<SubQueue*>(q);

	if (sq_ptr->size == 0) {
		on_illegal_operation();
		return 0;
	}

	unsigned char dequeued_byte = data[sq_ptr->start_id];

	sq_ptr->start_id++;
	sq_ptr->size--;

	return dequeued_byte;
}

int main()
{	
	

	for (size_t i = 0; i < 63; i++)
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
	}

	/*Q* q0 = create_queue();
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
