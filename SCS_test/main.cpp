#include <iostream>
#include <list>
#include <queue>

#define MAX_QUEUES 64
#define MEMORY_SIZE 2048
#define CHUNK_SIZE 8


struct Q {
	std::uint16_t head;
	std::uint16_t tail;
	std::uint16_t index_position;
	std::uint16_t read_offset;
};

const uint16_t DATA_SIZE = CHUNK_SIZE - sizeof(std::uint16_t);

struct DataChunk {
	unsigned char data[DATA_SIZE];
	std::uint16_t next;
};

static std::uint16_t free_list_head = 0;

const uint16_t METADATA_POOL_SIZE = MAX_QUEUES * (sizeof(Q) / sizeof(unsigned char));
const uint16_t DATA_POOL_START = METADATA_POOL_SIZE;

unsigned char data[MEMORY_SIZE];

static std::uint16_t queue_counter = 0;

void init_chunks() {
	const uint16_t TOTAL_CHUNKS = (MEMORY_SIZE - DATA_POOL_START) / CHUNK_SIZE;
	free_list_head = DATA_POOL_START;

	for (uint16_t i = 0; i < TOTAL_CHUNKS; ++i)
	{
		std::uint16_t current_chunk_index = DATA_POOL_START + (i * CHUNK_SIZE);

		DataChunk* current_chunk = reinterpret_cast<DataChunk*>(&data[current_chunk_index]);
		
		std::uint16_t next_chunk_index = DATA_POOL_START + ((i+1) * CHUNK_SIZE);

		if (i < TOTAL_CHUNKS - 1)
		{
			current_chunk->next = next_chunk_index;
		}
		else
		{
			current_chunk->next = 0;
		}

		// std::cout << "Chunk: " << i << " created. It points to: " << current_chunk->next << std::endl;
	}

	// std::cout << "Memory allocated. " << "Free list head: " << free_list_head << std::endl;
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
	std::uint16_t metadata_id = queue_counter;

	uint16_t sq_byte_index = metadata_id * (sizeof(Q) / sizeof(unsigned char));
	Q* q_ptr = reinterpret_cast<Q*>(&data[sq_byte_index]);

	if (free_list_head == 0) {
		on_out_of_memory();
	}

	uint16_t chunk_index = free_list_head;

	DataChunk* current_chunk = reinterpret_cast<DataChunk*>(&data[chunk_index]);

	free_list_head = current_chunk->next;

	current_chunk->next = 0;

	q_ptr->head = chunk_index;
	q_ptr->tail = chunk_index;
	q_ptr->index_position = 0;

	// std::cout << "Queue created at: " << q_ptr << ". Head at: " << q_ptr->head << ". Tail at: " << q_ptr->tail << ". Size: " << q_ptr->index_position << std::endl;

	queue_counter++;
	return q_ptr;
}
// Destroy an earlier created byte queue.
void destroy_queue(Q* q) {
	Q* q_ptr = reinterpret_cast<Q*>(q);

	std::uint16_t C_first_index = q_ptr->head;
	std::uint16_t C_size = q_ptr->tail;

	DataChunk* tail_chunk = reinterpret_cast<DataChunk*>(&data[C_size]);

	if (q_ptr->index_position == 0 || q_ptr->head == 0) {
		q_ptr->head = 0;
		q_ptr->tail = 0;
		q_ptr->index_position = 0;
		// std::cout << "Queue " << q_ptr << " successfully destroyed" << std::endl;
		return;
	}

	DataChunk* C_last_ptr = reinterpret_cast<DataChunk*>(&data[C_size]);

	C_last_ptr->next = free_list_head;

	free_list_head = C_first_index;

	q_ptr->head = 0;
	q_ptr->tail = 0;
	q_ptr->index_position = 0;
	q_ptr->read_offset = 0;

	// std::cout << "Queue " << q_ptr << " successfully destroyed" << std::endl;
}
// Adds a new byte to a queue.
void enqueue_byte(Q* q, unsigned char b) {
	Q* q_ptr = reinterpret_cast<Q*>(q);
	DataChunk* current_chunk = reinterpret_cast<DataChunk*>(&data[q_ptr->tail]);

	std::uint16_t byte_offset = q_ptr->index_position % DATA_SIZE;

	if (byte_offset == 0 && q_ptr->index_position > 0) {
		std::uint16_t new_chunk_index = free_list_head;

		if (new_chunk_index == 0) {
			on_out_of_memory();
			return;
		}

		DataChunk* new_chunk = reinterpret_cast<DataChunk*>(&data[new_chunk_index]);

		free_list_head = new_chunk->next;
		new_chunk->next = 0;

		current_chunk->next = new_chunk_index;

		q_ptr->tail = new_chunk_index;

		current_chunk = new_chunk;
	}

	current_chunk->data[byte_offset] = b;
	q_ptr->index_position++;

	// std::cout << "Enqueued '" << (int)b << "'. Tail at " << q_ptr->tail << ". Local offset " << byte_offset << ". New size: " << q_ptr->index_position << std::endl;
}
// Pops the next byte off the FIFO queue.
unsigned char dequeue_byte(Q* q) {
	Q* q_ptr = reinterpret_cast<Q*>(q);

	if (q_ptr->index_position == 0) {
		on_illegal_operation();
		return 0;
	}

	DataChunk* head_chunk = reinterpret_cast<DataChunk*>(&data[q_ptr->head]);

	unsigned char dequeued_byte = head_chunk->data[q_ptr->read_offset];

	q_ptr->read_offset++;

	if (q_ptr->read_offset >= DATA_SIZE) {

		std::uint16_t old_chunk_index = q_ptr->head;

		q_ptr->head = head_chunk->next;

		head_chunk->next = free_list_head;
		free_list_head = old_chunk_index;

		q_ptr->read_offset = 0;
	}

	return dequeued_byte;
}

int main()
{	
	init_chunks();

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
	destroy_queue(q1);
}
