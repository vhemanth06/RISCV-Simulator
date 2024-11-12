#include "functions.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#define MAX_ADDRESS 0x50001
typedef struct
{
    uint64_t address;
    uint8_t value;
} MemEntry;

typedef struct
{
    bool valid;
    bool dirty;
    unsigned int tag;
    int *data;
    int lru_counter;
} cacheblock;
typedef struct
{
    cacheblock *blocks;
    int *fifo_queue;
} cachesets;
typedef struct
{
    cachesets *sets;
    int numsets;
    int associativity;
    int block_size;
    int hits;
    int misses;
    char *replacement_policy;
    char *wb_policy;
} cache;

void lru_counter_update(cache *cache, int index, int block_index, int n)
{
    if (cache->associativity != 0)
    {
        for (int i = 0; i < cache->associativity; i++)
        {
            if (i != block_index && cache->sets[index].blocks[i].valid == 1)
            {
                cache->sets[index].blocks[i].lru_counter++;
            }
        }
        cache->sets[index].blocks[block_index].lru_counter = 0;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (i != block_index && cache->sets[index].blocks[i].valid == 1)
            {
                cache->sets[index].blocks[i].lru_counter++;
            }
        }
        cache->sets[index].blocks[block_index].lru_counter = 0;
    }
}

void fifo(cache *cache, unsigned int ptag, int index, int loadnumber, MemEntry *mem, uint64_t address, int *t, int n, int index_bits, int byte_offset_bits)
{
    int block_to_replace = -1;
    int empty_block = -1;
    if (cache->associativity != 0)
    {
        for (int i = 0; i < cache->associativity; i++)
        {
            if (cache->sets[index].blocks[i].valid == 0)
            {
                empty_block = i;
                break;
            }
        }
        if (empty_block != -1)
        {
            block_to_replace = empty_block;
        }
        else
        {
            block_to_replace = cache->sets[index].fifo_queue[0];
            if (strcmp(cache->wb_policy, "WB") == 0)
            {
                if (cache->sets[index].blocks[block_to_replace].dirty == 1)
                {
                    unsigned int evicted_address = (cache->sets[index].blocks[block_to_replace].tag << (index_bits + byte_offset_bits)) | (index << byte_offset_bits);
                    for (int k = 0; k < cache->block_size; k++)
                    {
                        mem[evicted_address + k].value = cache->sets[index].blocks[block_to_replace].data[k];
                    }
                    cache->sets[index].blocks[block_to_replace].dirty = 0;
                }
            }
        }
        for (int i = 0; i < cache->associativity - 1; i++)
        {
            cache->sets[index].fifo_queue[i] = cache->sets[index].fifo_queue[i + 1];
        }
        cache->sets[index].fifo_queue[cache->associativity - 1] = block_to_replace;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (cache->sets[index].blocks[i].valid == 0)
            {
                empty_block = i;
                break;
            }
        }
        if (empty_block != -1)
        {
            block_to_replace = empty_block;
        }
        else
        {

            block_to_replace = cache->sets[index].fifo_queue[0];

            if (strcmp(cache->wb_policy, "WB") == 0)
            {
                if (cache->sets[index].blocks[block_to_replace].dirty == 1)
                {
                    unsigned int evicted_address = (cache->sets[index].blocks[block_to_replace].tag << (byte_offset_bits));
                    for (int k = 0; k < cache->block_size; k++)
                    {
                        mem[evicted_address + k].value = cache->sets[index].blocks[block_to_replace].data[k];
                    }
                    cache->sets[index].blocks[block_to_replace].dirty = 0;
                }
            }
        }
        for (int i = 0; i < n - 1; i++)
        {
            cache->sets[index].fifo_queue[i] = cache->sets[index].fifo_queue[i + 1];
        }
        cache->sets[index].fifo_queue[n - 1] = block_to_replace;
    }
    cache->sets[index].blocks[block_to_replace].tag = ptag;
    cache->sets[index].blocks[block_to_replace].valid = 1;
    uint64_t address2 = address >> (byte_offset_bits);
    address2 = address2 << (byte_offset_bits);
    for (int k = 0; k < cache->block_size; k++)
    {
        cache->sets[index].blocks[block_to_replace].data[k] = mem[address2 + k].value;
    }

    *t = block_to_replace;
}

void lru(cache *cache, unsigned int ptag, int index, int loadnumber, MemEntry *mem, uint64_t address, int *t, int n, int index_bits, int byte_offset_bits)
{
    int lru_index = -1;
    int max_counter = -1;

    if (cache->associativity != 0)
    {
        for (int i = 0; i < cache->associativity; i++)
        {
            if (cache->sets[index].blocks[i].valid == 0)
            {
                lru_index = i;
                break;
            }
            else if (cache->sets[index].blocks[i].lru_counter > max_counter)
            {
                max_counter = cache->sets[index].blocks[i].lru_counter;
                lru_index = i;
            }
        }
        if (strcmp(cache->wb_policy, "WB") == 0)
        {
            if (cache->sets[index].blocks[lru_index].dirty == 1 && cache->sets[index].blocks[lru_index].valid == 1)
            {
                unsigned int evicted_address = (cache->sets[index].blocks[lru_index].tag << (index_bits + byte_offset_bits)) | (index << byte_offset_bits);
                for (int k = 0; k < cache->block_size; k++)
                {
                    mem[evicted_address + k].value = cache->sets[index].blocks[lru_index].data[k];
                }
                cache->sets[index].blocks[lru_index].dirty = 0;
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (cache->sets[index].blocks[i].valid == 0)
            {
                lru_index = i;
                break;
            }
            else if (cache->sets[index].blocks[i].lru_counter > max_counter)
            {
                max_counter = cache->sets[index].blocks[i].lru_counter;
                lru_index = i;
            }
        }
        if (strcmp(cache->wb_policy, "WB") == 0)
        {
            if (cache->sets[index].blocks[lru_index].dirty == 1 && cache->sets[index].blocks[lru_index].valid == 1)
            {
                unsigned int evicted_address = (cache->sets[index].blocks[lru_index].tag << (byte_offset_bits));
                for (int k = 0; k < cache->block_size; k++)
                {
                    mem[evicted_address + k].value = cache->sets[index].blocks[lru_index].data[k];
                }
                cache->sets[index].blocks[lru_index].dirty = 0;
            }
        }
    }
    cache->sets[index].blocks[lru_index].tag = ptag;
    cache->sets[index].blocks[lru_index].valid = 1;
    cache->sets[index].blocks[lru_index].lru_counter = 0;
    uint64_t address2 = address >> (byte_offset_bits);
    address2 = address2 << (byte_offset_bits);
    for (int k = 0; k < cache->block_size; k++)
    {
        cache->sets[index].blocks[lru_index].data[k] = mem[address2 + k].value;
    }

    *t = lru_index;
}
void initialize_random()
{
    static int initialized = 0;
    if (!initialized)
    {
        srand(time(NULL));
        initialized = 1;
    }
}
void random_policy(cache *cache, unsigned int ptag, int index, int loadnumber, MemEntry *mem, uint64_t address, int *t, int n, int index_bits, int byte_offset_bits)
{
    initialize_random();
    int random_block = -1;
    int empty_block = -1;
    if (cache->associativity != 0)
    {
        for (int i = 0; i < cache->associativity; i++)
        {
            if (cache->sets[index].blocks[i].valid == 0)
            {
                empty_block = i;
                break;
            }
        }
        if (empty_block != -1)
        {
            random_block = empty_block;
        }
        else
        {
            random_block = rand() % cache->associativity;
            if (strcmp(cache->wb_policy, "WB") == 0)
            {
                if (cache->sets[index].blocks[random_block].dirty == 1)
                {
                    unsigned int evicted_address = (cache->sets[index].blocks[random_block].tag << (index_bits + byte_offset_bits)) | (index << byte_offset_bits);
                    for (int k = 0; k < cache->block_size; k++)
                    {
                        mem[evicted_address + k].value = cache->sets[index].blocks[random_block].data[k];
                    }
                    cache->sets[index].blocks[random_block].dirty = 0;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (cache->sets[index].blocks[i].valid == 0)
            {
                empty_block = i;
                break;
            }
        }
        if (empty_block != -1)
        {
            random_block = empty_block;
        }
        else
        {
            random_block = rand() % n;
            if (strcmp(cache->wb_policy, "WB") == 0)
            {
                if (cache->sets[index].blocks[random_block].dirty == 1)
                {
                    unsigned int evicted_address = (cache->sets[index].blocks[random_block].tag << (byte_offset_bits));
                    for (int k = 0; k < cache->block_size; k++)
                    {
                        mem[evicted_address + k].value = cache->sets[index].blocks[random_block].data[k];
                    }
                    cache->sets[index].blocks[random_block].dirty = 0;
                }
            }
        }
    }
    cache->sets[index].blocks[random_block].tag = ptag;
    cache->sets[index].blocks[random_block].valid = 1;
    uint64_t address2 = address >> (byte_offset_bits);
    address2 = address2 << (byte_offset_bits);
    for (int k = 0; k < cache->block_size; k++)
    {
        cache->sets[index].blocks[random_block].data[k] = mem[address2 + k].value;
    }

    *t = random_block;
}
void cache_read(cache *cache, unsigned int ptag, int index, void *result, int load_number, MemEntry *mem_entries, uint64_t address, FILE *cache_out, unsigned int offset, int n, int result_size, int index_bits, int byte_offset_bits)
{
    if (cache->associativity != 0)
    {
        for (int i = 0; i < cache->associativity; i++)
        {
            if (cache->sets[index].blocks[i].tag == ptag && cache->sets[index].blocks[i].valid == 1)
            {
                for (int k = 0; k < load_number; k++)
                {
                    if (result_size == sizeof(int64_t))
                    {
                        *(int64_t *)result |= ((int64_t)cache->sets[index].blocks[i].data[k + offset] << (k * 8));
                    }
                    else if (result_size == sizeof(int32_t))
                    {
                        *(int32_t *)result |= ((int64_t)cache->sets[index].blocks[i].data[k + offset] << (k * 8));
                    }
                    else if (result_size == sizeof(int16_t))
                    {
                        *(int16_t *)result |= ((int64_t)cache->sets[index].blocks[i].data[k + offset] << (k * 8));
                    }
                    else if (result_size == sizeof(int8_t))
                    {
                        *(int8_t *)result |= ((int64_t)cache->sets[index].blocks[i].data[k + offset] << (k * 8));
                    }
                }
                fprintf(cache_out, "R: Address: 0x%x, Set: 0x%x, Hit, Tag: 0x%x, ", address, index, ptag);
                if (cache->sets[index].blocks[i].dirty == 0)
                {
                    fprintf(cache_out, "Clean\n");
                }
                else
                {
                    fprintf(cache_out, "Dirty\n");
                }
                lru_counter_update(cache, index, i, n);
                ;
                cache->hits++;
                return;
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            unsigned int tagcheck = cache->sets[index].blocks[i].tag;
            if (cache->sets[index].blocks[i].tag == ptag && cache->sets[index].blocks[i].valid == 1)
            {
                for (int k = 0; k < load_number; k++)
                {
                    if (result_size == sizeof(int64_t))
                    {
                        *(int64_t *)result |= ((int64_t)cache->sets[index].blocks[i].data[k + offset] << (k * 8));
                    }
                    else if (result_size == sizeof(int32_t))
                    {
                        *(int32_t *)result |= ((int64_t)cache->sets[index].blocks[i].data[k + offset] << (k * 8));
                    }
                    else if (result_size == sizeof(int16_t))
                    {
                        *(int16_t *)result |= ((int64_t)cache->sets[index].blocks[i].data[k + offset] << (k * 8));
                    }
                    else if (result_size == sizeof(int8_t))
                    {
                        *(int8_t *)result |= ((int64_t)cache->sets[index].blocks[i].data[k + offset] << (k * 8));
                    }
                }
                fprintf(cache_out, "R: Address: 0x%x, Set: 0x%x, Hit, Tag: 0x%x, ", address, index, ptag);
                if (cache->sets[index].blocks[i].dirty == 0)
                {
                    fprintf(cache_out, "Clean\n");
                }
                else
                {
                    fprintf(cache_out, "Dirty\n");
                }
                cache->hits++;
                lru_counter_update(cache, index, i, n);
                return;
            }
        }
    }
    cache->misses++;

    int t;
    if (cache->associativity != 1)
    {
        if (strcmp(cache->replacement_policy, "LRU") == 0)
        {
            lru(cache, ptag, index, load_number, mem_entries, address, &t, n, index_bits, byte_offset_bits);
        }
        else if (strcmp(cache->replacement_policy, "FIFO") == 0)
        {
            fifo(cache, ptag, index, load_number, mem_entries, address, &t, n, index_bits, byte_offset_bits);
        }
        else if (strcmp(cache->replacement_policy, "RANDOM") == 0)
        {
            random_policy(cache, ptag, index, load_number, mem_entries, address, &t, n, index_bits, byte_offset_bits);
        }
        for (int k = 0; k < load_number; k++)
        {
            if (result_size == sizeof(int64_t))
            {
                *(int64_t *)result |= ((int64_t)mem_entries[address + k].value << (k * 8));
            }
            else if (result_size == sizeof(int32_t))
            {
                *(int32_t *)result |= ((int64_t)mem_entries[address + k].value << (k * 8));
            }
            else if (result_size == sizeof(int16_t))
            {
                *(int16_t *)result |= ((int64_t)mem_entries[address + k].value << (k * 8));
            }
            else if (result_size == sizeof(int8_t))
            {
                *(int8_t *)result |= ((int64_t)mem_entries[address + k].value << (k * 8));
            }
        }
        lru_counter_update(cache, index, t, n);
        fprintf(cache_out, "R: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, ", address, index, ptag);
        if (cache->sets[index].blocks[t].dirty == 0)
        {
            fprintf(cache_out, "Clean\n");
        }
        else
        {
            fprintf(cache_out, "Dirty\n");
        }
    }
    else
    {
        cache->sets[index].blocks[0].tag = ptag;
        cache->sets[index].blocks[0].valid = 1;
        for (int k = 0; k < load_number; k++)
        {

            if (result_size == sizeof(int64_t))
            {
                *(int64_t *)result |= ((int64_t)mem_entries[address + k].value << (k * 8));
            }
            else if (result_size == sizeof(int32_t))
            {
                *(int32_t *)result |= ((int64_t)mem_entries[address + k].value << (k * 8));
            }
            else if (result_size == sizeof(int16_t))
            {
                *(int16_t *)result |= ((int64_t)mem_entries[address + k].value << (k * 8));
            }
            else if (result_size == sizeof(int8_t))
            {
                *(int8_t *)result |= ((int64_t)mem_entries[address + k].value << (k * 8));
            }
        }
        uint64_t address2 = address >> (byte_offset_bits);
        address2 = address2 << (byte_offset_bits);
        for (int k = 0; k < cache->block_size; k++)
        {
            cache->sets[index].blocks[0].data[k] = mem_entries[address2 + k].value;
        }
        lru_counter_update(cache, index, 0, n);
        fprintf(cache_out, "R: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, ", address, index, ptag);
        if (cache->sets[index].blocks[0].dirty == 0)
        {
            fprintf(cache_out, "Clean\n");
        }
        else
        {
            fprintf(cache_out, "Dirty\n");
        }
    }
}

void cache_write(cache *cache, unsigned int ptag, int index, int load_number, MemEntry *mem_entries, uint64_t address, FILE *cache_out, unsigned int offset, int n, long int store_value, int index_bits, int byte_offset_bits)
{
    if (cache->associativity != 0)
    {
        for (int i = 0; i < cache->associativity; i++)
        {
            if (cache->sets[index].blocks[i].tag == ptag && cache->sets[index].blocks[i].valid == 1)
            {
                if (strcmp(cache->wb_policy, "WT") == 0)
                {
                    fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Hit, Tag: 0x%x, Clean\n", address, index, ptag);
                    uint64_t address2 = address >> (byte_offset_bits);
                    address2 = address2 << (byte_offset_bits);
                    for (int j = 0; j < load_number; j++)
                    {
                        mem_entries[address + j].value = (store_value >> (j * 8)) & 0xFF;
                        cache->sets[index].blocks[i].data[j + offset] = (store_value >> (j * 8)) & 0xFF;
                    }
                }
                else if (strcmp(cache->wb_policy, "WB") == 0)
                {
                    fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Hit, Tag: 0x%x, ", address, index, ptag);
                    if (cache->sets[index].blocks[i].dirty == 0)
                    {
                        fprintf(cache_out, "Clean\n");
                    }
                    else
                    {
                        fprintf(cache_out, "Dirty\n");
                    }
                    for (int j = 0; j < load_number; j++)
                    {
                        cache->sets[index].blocks[i].data[j + offset] = (store_value >> (j * 8)) & 0xFF;
                        cache->sets[index].blocks[i].dirty = 1;
                    }
                }
                lru_counter_update(cache, index, i, n);
                ;
                cache->hits++;
                return;
            }
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (cache->sets[index].blocks[i].tag == ptag && cache->sets[index].blocks[i].valid == 1)
            {
                if (strcmp(cache->wb_policy, "WT") == 0)
                {
                    fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Hit, Tag: 0x%x, Clean\n", address, index, ptag);
                    for (int j = 0; j < load_number; j++)
                    {
                        mem_entries[address + j].value = (store_value >> (j * 8)) & 0xFF;
                        cache->sets[index].blocks[i].data[j + offset] = (store_value >> (j * 8)) & 0xFF;
                    }
                }
                else if (strcmp(cache->wb_policy, "WB") == 0)
                {
                    fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Hit, Tag: 0x%x, ", address, index, ptag);
                    if (cache->sets[index].blocks[i].dirty == 0)
                    {
                        fprintf(cache_out, "Clean\n");
                    }
                    else
                    {
                        fprintf(cache_out, "Dirty\n");
                    }
                    for (int j = 0; j < load_number; j++)
                    {
                        cache->sets[index].blocks[i].data[j + offset] = (store_value >> (j * 8)) & 0xFF;
                        cache->sets[index].blocks[i].dirty = 1;
                    }
                }
                cache->hits++;
                lru_counter_update(cache, index, i, n);
                return;
            }
        }
    }
    cache->misses++;

    int t;
    if (cache->associativity != 1)
    {
        if (strcmp(cache->replacement_policy, "LRU") == 0)
        {
            if (strcmp(cache->wb_policy, "WT") == 0)
            {
                for (int j = 0; j < load_number; j++)
                {
                    mem_entries[address + j].value = (store_value >> (j * 8)) & 0xFF;
                }
                fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, Clean\n", address, index, ptag);
            }
            else if (strcmp(cache->wb_policy, "WB") == 0)
            {
                lru(cache, ptag, index, load_number, mem_entries, address, &t, n, index_bits, byte_offset_bits);
                lru_counter_update(cache, index, t, n);
                fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, ", address, index, ptag);
                if (cache->sets[index].blocks[t].dirty == 0)
                {
                    fprintf(cache_out, "Clean\n");
                }
                else
                {
                    fprintf(cache_out, "Dirty\n");
                }
                for (int j = 0; j < load_number; j++)
                {
                    cache->sets[index].blocks[t].data[j + offset] = (store_value >> (j * 8)) & 0xFF;
                    cache->sets[index].blocks[t].dirty = 1;
                }
            }
        }
        else if (strcmp(cache->replacement_policy, "FIFO") == 0)
        {
            if (strcmp(cache->wb_policy, "WT") == 0)
            {
                for (int j = 0; j < load_number; j++)
                {
                    mem_entries[address + j].value = (store_value >> (j * 8)) & 0xFF;
                }
                fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, Clean\n", address, index, ptag);
            }
            else if (strcmp(cache->wb_policy, "WB") == 0)
            {
                fifo(cache, ptag, index, load_number, mem_entries, address, &t, n, index_bits, byte_offset_bits);
                fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, ", address, index, ptag);
                if (cache->sets[index].blocks[t].dirty == 0)
                {
                    fprintf(cache_out, "Clean\n");
                }
                else
                {
                    fprintf(cache_out, "Dirty\n");
                }
                for (int j = 0; j < load_number; j++)
                {
                    cache->sets[index].blocks[t].data[j + offset] = (store_value >> (j * 8)) & 0xFF;
                    cache->sets[index].blocks[t].dirty = 1;
                }
            }
        }
        else if (strcmp(cache->replacement_policy, "RANDOM") == 0)
        {
            if (strcmp(cache->wb_policy, "WT") == 0)
            {
                for (int j = 0; j < load_number; j++)
                {
                    mem_entries[address + j].value = (store_value >> (j * 8)) & 0xFF;
                }
                fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, Clean\n", address, index, ptag);
            }
            else if (strcmp(cache->wb_policy, "WB") == 0)
            {
                random_policy(cache, ptag, index, load_number, mem_entries, address, &t, n, index_bits, byte_offset_bits);
                fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, ", address, index, ptag);
                if (cache->sets[index].blocks[t].dirty == 0)
                {
                    fprintf(cache_out, "Clean\n");
                }
                else
                {
                    fprintf(cache_out, "Dirty\n");
                }
                for (int j = 0; j < load_number; j++)
                {
                    cache->sets[index].blocks[t].data[j + offset] = (store_value >> (j * 8)) & 0xFF;
                    cache->sets[index].blocks[t].dirty = 1;
                }
            }
        }
    }
    else
    {
        int h = 0;
        if (strcmp(cache->wb_policy, "WT") == 0)
        {
            for (int k = 0; k < load_number; k++)
            {
                mem_entries[address + k].value = (store_value >> (k * 8)) & 0xFF;
            }
            fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, Clean\n", address, index, ptag);
            h = -1;
        }
        else if (strcmp(cache->wb_policy, "WB") == 0)
        {
            cache->sets[index].blocks[0].tag = ptag;
            cache->sets[index].blocks[0].valid = 1;
            lru_counter_update(cache, index, 0, n);
            fprintf(cache_out, "W: Address: 0x%x, Set: 0x%x, Miss, Tag: 0x%x, ", address, index, ptag);
            if (cache->sets[index].blocks[0].dirty == 0)
            {
                fprintf(cache_out, "Clean\n");
            }
            else
            {
                fprintf(cache_out, "Dirty\n");
            }
            for (int j = 0; j < load_number; j++)
            {
                cache->sets[index].blocks[0].data[j + offset] = (store_value >> (j * 8)) & 0xFF;
                cache->sets[index].blocks[0].dirty = 1;
            }
        }
    }
}
void run_instruction(char *line, char **tokens, long int register_value[], MemEntry *mem_entries, int *pc_counter, char **label_names, int label_line_numbers[], int *counter_ptr, int label_position_iter, Stack *call_stack, bool cache_in, cache *cache, int cachesize, FILE *cache_out)
{
    if (strcmp(tokens[0], "add") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] + register_value[rs2];
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "sub") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] - register_value[rs2];
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "and") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] & register_value[rs2];
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "or") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] | register_value[rs2];
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "xor") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);

        register_value[rd] = register_value[rs1] ^ register_value[rs2];
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "sll") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);
        register_value[rd] = register_value[rs1] << register_value[rs2];
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "srl") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);
        register_value[rd] = (unsigned int)register_value[rs1] >> register_value[rs2];
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "sra") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        int rs2 = register_finder(tokens[3]);
        register_value[rd] = register_value[rs1] >> register_value[rs2];
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "addi") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char *endpointer;
        long int num = strtol(tokens[3], &endpointer, 0);
        register_value[rd] = register_value[rs1] + num;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "andi") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char *endpointer;
        long int num = strtol(tokens[3], &endpointer, 0);
        register_value[rd] = register_value[rs1] & num;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "ori") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char *endpointer;
        long int num = strtol(tokens[3], &endpointer, 0);
        register_value[rd] = register_value[rs1] | num;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "xori") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char *endpointer;
        long int num = strtol(tokens[3], &endpointer, 0);
        register_value[rd] = register_value[rs1] ^ num;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "slli") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char *endpointer;
        long int num = strtol(tokens[3], &endpointer, 0);
        register_value[rd] = register_value[rs1] << num;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "srli") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char *endpointer;
        long int num = strtol(tokens[3], &endpointer, 0);
        register_value[rd] = (uint64_t)register_value[rs1] >> num;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "srai") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[2]);
        char *endpointer;
        long int num = strtol(tokens[3], &endpointer, 0);
        register_value[rd] = (int64_t)register_value[rs1] >> num;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "lui") == 0)
    {
        int rd = register_finder(tokens[1]);
        char *endptr;
        int num = strtol(tokens[2], &endptr, 0) & 0xfffff;

        register_value[rd] = (long int)(num << 12);
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "ld") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int64_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int k = 0; k < 8; k++)
            {
                result |= ((int64_t)mem_entries[mem + k].value << (k * 8));
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;

                cache_read(cache, c_m, 0, &result, 8, mem_entries, mem, cache_out, byte_offset, n, sizeof(int64_t), 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_read(cache, tag, index, &result, 8, mem_entries, mem, cache_out, byte_offset, n, sizeof(int64_t), index_bits, byte_offset_bits);
            }
        }

        register_value[rd] = result;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "lw") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int32_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int k = 0; k < 4; k++)
            {
                result |= ((int64_t)mem_entries[mem + k].value << (k * 8));
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_read(cache, c_m, 0, &result, 4, mem_entries, mem, cache_out, byte_offset, n, sizeof(int32_t), 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_read(cache, tag, index, &result, 4, mem_entries, mem, cache_out, byte_offset, n, sizeof(int32_t), index_bits, byte_offset_bits);
            }
        }
        register_value[rd] = result;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "lh") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int16_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int k = 0; k < 2; k++)
            {
                result |= ((int64_t)mem_entries[mem + k].value << (k * 8));
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_read(cache, c_m, 0, &result, 2, mem_entries, mem, cache_out, byte_offset, n, sizeof(int16_t), 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_read(cache, tag, index, &result, 2, mem_entries, mem, cache_out, byte_offset, n, sizeof(int16_t), index_bits, byte_offset_bits);
            }
        }
        register_value[rd] = result;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "lb") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        int8_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int k = 0; k < 1; k++)
            {
                result |= ((int64_t)mem_entries[mem + k].value << (k * 8));
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_read(cache, c_m, 0, &result, 1, mem_entries, mem, cache_out, byte_offset, n, sizeof(int8_t), 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_read(cache, tag, index, &result, 1, mem_entries, mem, cache_out, byte_offset, n, sizeof(int8_t), index_bits, byte_offset_bits);
            }
        }
        register_value[rd] = result;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "lwu") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint32_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int k = 0; k < 4; k++)
            {
                result |= ((int64_t)mem_entries[mem + k].value << (k * 8));
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_read(cache, c_m, 0, &result, 4, mem_entries, mem, cache_out, byte_offset, n, sizeof(int32_t), 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_read(cache, tag, index, &result, 4, mem_entries, mem, cache_out, byte_offset, n, sizeof(int32_t), index_bits, byte_offset_bits);
            }
        }
        register_value[rd] = result;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "lhu") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint16_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int k = 0; k < 2; k++)
            {
                result |= ((int64_t)mem_entries[mem + k].value << (k * 8));
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_read(cache, c_m, 0, &result, 2, mem_entries, mem, cache_out, byte_offset, n, sizeof(int16_t), 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_read(cache, tag, index, &result, 2, mem_entries, mem, cache_out, byte_offset, n, sizeof(int16_t), index_bits, byte_offset_bits);
            }
        }
        register_value[rd] = result;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "lbu") == 0)
    {
        int rd = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint8_t result = 0;
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int k = 0; k < 1; k++)
            {
                result |= ((int64_t)mem_entries[mem + k].value << (k * 8));
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_read(cache, c_m, 0, &result, 1, mem_entries, mem, cache_out, byte_offset, n, sizeof(int8_t), 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_read(cache, tag, index, &result, 1, mem_entries, mem, cache_out, byte_offset, n, sizeof(int8_t), index_bits, byte_offset_bits);
            }
        }
        register_value[rd] = result;
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "sd") == 0)
    {
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                mem_entries[mem + i].value = (register_value[rs2] >> (i * 8)) & 0xFF;
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_write(cache, c_m, 0, 8, mem_entries, mem, cache_out, byte_offset, n, register_value[rs2], 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_write(cache, tag, index, 8, mem_entries, mem, cache_out, byte_offset, n, register_value[rs2], index_bits, byte_offset_bits);
            }
        }

        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "sw") == 0)
    {
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int i = 0; i < 4; i++)
            {
                mem_entries[mem + i].value = (register_value[rs2] >> (i * 8)) & 0xFF;
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_write(cache, c_m, 0, 4, mem_entries, mem, cache_out, byte_offset, n, register_value[rs2], 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_write(cache, tag, index, 4, mem_entries, mem, cache_out, byte_offset, n, register_value[rs2], index_bits, byte_offset_bits);
            }
        }

        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "sh") == 0)
    {
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int i = 0; i < 2; i++)
            {
                mem_entries[mem + i].value = (register_value[rs2] >> (i * 8)) & 0xFF;
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_write(cache, c_m, 0, 2, mem_entries, mem, cache_out, byte_offset, n, register_value[rs2], 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_write(cache, tag, index, 2, mem_entries, mem, cache_out, byte_offset, n, register_value[rs2], index_bits, byte_offset_bits);
            }
        }
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "sb") == 0)
    {
        int rs2 = register_finder(tokens[1]);
        int rs1 = register_finder(tokens[3]);
        int num = atoi(tokens[2]);
        uint64_t mem = (uint64_t)(register_value[rs1] + num);
        if (cache_in == 0)
        {
            for (int i = 0; i < 1; i++)
            {
                mem_entries[mem + i].value = (register_value[rs2] >> (i * 8)) & 0xFF;
            }
        }
        else
        {
            int n = cachesize / cache->block_size;
            if (cache->associativity == 0)
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                cache_write(cache, c_m, 0, 1, mem_entries, mem, cache_out, byte_offset, n, register_value[rs2], 0, byte_offset_bits);
            }
            else
            {
                int byte_offset_bits = (int)(log((double)cache->block_size) / log(2));
                int index_bits = (int)(log((double)(cachesize / (cache->block_size * cache->associativity))) / log(2));
                uint32_t c_m = mem >> (byte_offset_bits);
                uint32_t x = c_m << byte_offset_bits;
                uint32_t byte_offset = mem - x;
                unsigned int tag = c_m >> (index_bits);
                unsigned int tag2 = tag << (index_bits);
                unsigned int index = c_m - tag2;
                cache_write(cache, tag, index, 1, mem_entries, mem, cache_out, byte_offset, n, register_value[rs2], index_bits, byte_offset_bits);
            }
        }
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        *pc_counter = *pc_counter + 4;
    }
    else if (strcmp(tokens[0], "beq") == 0)
    {
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index = -1;
        int imm;

        for (int k = 0; k < label_position_iter; k++)
        {
            if (strcmp(tokens[3], label_names[k]) == 0)
            {
                index = label_line_numbers[k];
                break;
            }
        }
        if (index != -1)
        {
            imm = ((index) - (*counter_ptr)) * 4;
        }
        if (index == -1 && non_int_char_finder(tokens[3]) == 0)
        {
            imm = atoi(tokens[3]);
        }
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        if (register_value[rs1] == register_value[rs2])
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm / 4);
            (*counter_ptr)--;
        }
        else
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    }
    else if (strcmp(tokens[0], "bne") == 0)
    {
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index = -1;
        int imm;

        for (int k = 0; k < label_position_iter; k++)
        {
            if (strcmp(tokens[3], label_names[k]) == 0)
            {
                index = label_line_numbers[k];
                break;
            }
        }
        if (index != -1)
        {
            imm = ((index) - (*counter_ptr)) * 4;
        }
        if (index == -1 && non_int_char_finder(tokens[3]) == 0)
        {
            imm = atoi(tokens[3]);
        }
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        if (register_value[rs1] != register_value[rs2])
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm / 4);
            (*counter_ptr)--;
        }
        else
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    }
    else if (strcmp(tokens[0], "blt") == 0)
    {

        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index = -1;
        int imm;

        for (int k = 0; k < label_position_iter; k++)
        {
            if (strcmp(tokens[3], label_names[k]) == 0)
            {
                index = label_line_numbers[k];
                break;
            }
        }
        if (index != -1)
        {
            imm = ((index) - (*counter_ptr)) * 4;
        }
        if (index == -1 && non_int_char_finder(tokens[3]) == 0)
        {
            imm = atoi(tokens[3]);
        }
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        if (register_value[rs1] < register_value[rs2])
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm / 4);
            (*counter_ptr)--;
        }
        else
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    }
    else if (strcmp(tokens[0], "bge") == 0)
    {
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index = -1;
        int imm;

        for (int k = 0; k < label_position_iter; k++)
        {
            if (strcmp(tokens[3], label_names[k]) == 0)
            {
                index = label_line_numbers[k];
                break;
            }
        }
        if (index != -1)
        {
            imm = ((index) - (*counter_ptr)) * 4;
        }
        if (index == -1 && non_int_char_finder(tokens[3]) == 0)
        {
            imm = atoi(tokens[3]);
        }
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        if (register_value[rs1] >= register_value[rs2])
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm / 4);
            (*counter_ptr)--;
        }
        else
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    }
    else if (strcmp(tokens[0], "bltu") == 0)
    {
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index = -1;
        int imm;

        for (int k = 0; k < label_position_iter; k++)
        {
            if (strcmp(tokens[3], label_names[k]) == 0)
            {
                index = label_line_numbers[k];
                break;
            }
        }
        if (index != -1)
        {
            imm = ((index) - (*counter_ptr)) * 4;
        }
        if (index == -1 && non_int_char_finder(tokens[3]) == 0)
        {
            imm = atoi(tokens[3]);
        }
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        if ((unsigned int)register_value[rs1] < (unsigned int)register_value[rs2])
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm / 4);
            (*counter_ptr)--;
        }
        else
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    }
    else if (strcmp(tokens[0], "bgeu") == 0)
    {
        int rs1 = register_finder(tokens[1]);
        int rs2 = register_finder(tokens[2]);
        int index = -1;
        int imm;

        for (int k = 0; k < label_position_iter; k++)
        {
            if (strcmp(tokens[3], label_names[k]) == 0)
            {
                index = label_line_numbers[k];
                break;
            }
        }
        if (index != -1)
        {
            imm = ((index) - (*counter_ptr)) * 4;
        }
        if (index == -1 && non_int_char_finder(tokens[3]) == 0)
        {
            imm = atoi(tokens[3]);
        }
        call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
        if ((unsigned int)register_value[rs1] >= (unsigned int)register_value[rs2])
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + imm;
            *counter_ptr = *counter_ptr + (imm / 4);
            (*counter_ptr)--;
        }
        else
        {
            printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
            *pc_counter = *pc_counter + 4;
        }
    }
    else if (strcmp(tokens[0], "jal") == 0)
    {
        int rs1 = register_finder(tokens[1]);
        int index = -1;
        int imm;
        for (int k = 0; k < label_position_iter; k++)
        {
            if (strcmp(tokens[2], label_names[k]) == 0)
            {
                index = label_line_numbers[k];
                call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
                push(call_stack, tokens[2]);
                call_stack->line_num[call_stack->top_index] = *counter_ptr + 1;
                break;
            }
        }
        if (index != -1)
        {
            imm = ((index) - (*counter_ptr)) * 4;
        }
        if (index == -1 && non_int_char_finder(tokens[3]) == 0)
        {
            imm = atoi(tokens[3]);
        }
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        register_value[rs1] = *pc_counter + 4;
        *pc_counter = *pc_counter + imm;
        *counter_ptr = *counter_ptr + (imm / 4);
        (*counter_ptr)--;
    }
    else if (strcmp(tokens[0], "jalr") == 0)
    {
        int rs2 = register_finder(tokens[3]);
        int rs1 = register_finder(tokens[1]);
        int imm = atoi(tokens[1]);
        int pc_1 = *pc_counter;
        printf("Executed %s; PC=0x%08x\n", line, *pc_counter);
        register_value[rs1] = *pc_counter + 4;
        *pc_counter = register_value[rs2] + imm;
        int diff;
        diff = *pc_counter - pc_1;
        pop(call_stack);
        *counter_ptr = *counter_ptr + (diff / 4);
        (*counter_ptr)--;
    }
}
