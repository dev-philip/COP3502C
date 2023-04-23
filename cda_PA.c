#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CACHE_SIZE 32
#define NUM_SETS 8
#define NUM_WAYS 4
#define BLOCK_SIZE 4

// Block in 2-way mapping cache
typedef struct
{
    int valid;
    int tag;
} block;

// Cache entry in 4-way mapping cache
struct cache_entry
{
    bool valid;
    unsigned int tag;
};

// Global variables to track cache hits and total accesses
int cache_hits = 0;
int total_accesses = 0;
char fileName[10] = "traces.txt";

block cache_two_map[NUM_SETS][NUM_WAYS];
block cache[CACHE_SIZE / 2][2];

// 2 way mapping
int get_block_index(int address)
{
    return (address / BLOCK_SIZE) % (CACHE_SIZE / BLOCK_SIZE);
}

int get_set_index(int address)
{
    return ((address / BLOCK_SIZE) / (CACHE_SIZE / BLOCK_SIZE)) % 2;
}

int get_tag_2(int address)
{
    return address / (CACHE_SIZE / 2);
}

void cache_access(int address)
{
    int set_index = get_set_index(address);
    int block_index = get_block_index(address);
    int tag = get_tag_2(address);

    // Check if the block is already in the cache
    if (cache[block_index][set_index].valid == 1 && cache[block_index][set_index].tag == tag)
    {
        cache_hits++;
    }
    else
    {
        // If not, replace the block with the new one
        cache[block_index][set_index].valid = 1;
        cache[block_index][set_index].tag = tag;
    }

    total_accesses++;
}
// end of 2 way mapping

// start 4way
int get_set(int address)
{
    return (address >> 2) % NUM_SETS;
}

int get_tag(int address)
{
    return address >> 5;
}

int find_block(int address)
{
    int set = get_set(address);
    int tag = get_tag(address);
    for (int i = 0; i < NUM_WAYS; i++)
    {
        if (cache_two_map[set][i].valid && cache_two_map[set][i].tag == tag)
        {
            return i;
        }
    }
    return -1;
}

int evict_block(int set)
{
    int min_index = 0;
    for (int i = 1; i < NUM_WAYS; i++)
    {
        if (cache_two_map[set][i].valid && cache_two_map[set][i].tag < cache_two_map[set][min_index].tag)
        {
            min_index = i;
        }
    }
    cache_two_map[set][min_index].valid = 0;
    return min_index;
}

// end 4 way

void direct_mapping()
{
    int cache[CACHE_SIZE]; // initialize the cache
    int hits = 0;          // initialize number of hits to 0
    int accesses = 0;      // initialize number of total accesses to 0

    // read the input file
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        EXIT_FAILURE;
    }

    // process each address in the file
    unsigned int address;
    while (fscanf(file, "%x", &address) != EOF)
    {
        int index = address % CACHE_SIZE; // calculate the index in the cache
        if (cache[index] == address)
        {           // check if the address is in the cache
            hits++; // increment the number of hits
        }
        else
        {
            cache[index] = address; // add the address to the cache
        }
        accesses++; // increment the number of total accesses
    }

    fclose(file); // close the file

    // print the results
    printf("Direct Mapping - Number of hits: %d\n", hits);
    printf("Direct Mapping - Number of total accesses: %d\n", accesses);
    printf("Direct Mapping - Hit rate: %.2f%%\n", (float)hits / (float)accesses * 100.0);
}

void two_way_mapping()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        EXIT_FAILURE;
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        int address = strtol(line, NULL, 16);
        cache_access(address);
    }

    printf("\n2 Way Mapping - Number of hits: %d\n", cache_hits);
    printf("2 Way Mapping - Number of total accesses: %d\n", total_accesses);
    printf("2 Way Mapping - Hit rate: %.2f%%\n", ((float)cache_hits / total_accesses) * 100);

    fclose(fp);
    if (line)
    {
        free(line);
    }
}

void four_way_mapping()
{
    int hits = 0;
    int accesses = 0;
    FILE *file = fopen(fileName, "r");
    char hex[9];
    while (fscanf(file, "%s", hex) != EOF)
    {
        int address = (int)strtol(hex, NULL, 16);
        int set = get_set(address);
        int tag = get_tag(address);
        int block_index = find_block(address);
        if (block_index == -1)
        {
            int empty_index = -1;
            for (int i = 0; i < NUM_WAYS; i++)
            {
                if (!cache_two_map[set][i].valid)
                {
                    empty_index = i;
                    break;
                }
            }
            if (empty_index == -1)
            {
                empty_index = evict_block(set);
            }
            cache_two_map[set][empty_index].tag = tag;
            cache_two_map[set][empty_index].valid = 1;
        }
        else
        {
            hits++;
        }
        accesses++;
    }
    fclose(file);

    double hit_rate = (double)hits / (double)accesses;
    printf("\n4 Way Mapping - Hits: %d\n", hits);
    printf("4 Way Mapping - Total accesses: %d\n", accesses);
    printf("4 Way Mapping - Hit rate: %f\n", hit_rate);
}

void fully_associative_mapping()
{
    struct cache_entry cache[CACHE_SIZE];
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        cache[i].valid = false;
    }

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("Failed to open input file.\n");
        EXIT_FAILURE;
    }

    unsigned int address;
    int total_accesses = 0;
    int hits = 0;
    while (fscanf(fp, "%x", &address) != EOF)
    {
        total_accesses++;
        bool hit = false;
        unsigned int tag = address / CACHE_SIZE;
        for (int i = 0; i < CACHE_SIZE; i++)
        {
            if (cache[i].valid && cache[i].tag == tag)
            {
                hit = true;
                hits++;
                break;
            }
        }
        if (!hit)
        {
            int oldest_entry = 0;
            for (int i = 1; i < CACHE_SIZE; i++)
            {
                if (cache[i].valid && cache[i].tag < cache[oldest_entry].tag)
                {
                    oldest_entry = i;
                }
            }
            cache[oldest_entry].valid = true;
            cache[oldest_entry].tag = tag;
        }
    }

    printf("\nFully Assocaitive Mapping - Number of hits: %d\n", hits);
    printf("Fully Assocaitive Mapping - Number of total accesses: %d\n", total_accesses);
    printf("Fully Assocaitive Mapping - Hit rate: %.2f%%\n", (float)hits / total_accesses * 100);

    fclose(fp);
}

int main()
{
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening or reading input file\n");
        return EXIT_FAILURE;
    }
    fclose(file);
    direct_mapping();
    two_way_mapping();
    four_way_mapping();
    fully_associative_mapping();
    return 0;
}
