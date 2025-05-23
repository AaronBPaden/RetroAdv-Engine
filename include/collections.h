#ifndef _COLLECTIONS_H_
#define _COLLECTIONS_H_

#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * A generic collection interface.
 */
typedef struct collection {
    /*
     * Append an item to the end of the collection.
     */
    void (*add)(struct collection* self, void* item);

    /*
     * Execute the provided function for each element in the collection,
     * return true if any_func returns true for any element.
     */
    bool (*any)(struct collection*, bool (*any_func)(void *item));

    /*
     * Filter a collection. Returns a new collection with a copy of the data.
     */
    struct collection* (*filter)(struct collection*, bool (*filter_func)(void *item));

    /*
     * Call the provided function for each element in the collection.
     */
    void (*for_each)(struct collection*, void (*for_each_func)(void *next));

    /*
     * Map elements from a collection into a new collection.
     * The provided size is the size of the elements in the new collection.
     */
    struct collection* (*map)(struct collection*, void* (*map_func)(void *item), size_t size);

    /*
     * Reduce the collection to a single item.
     */
    void* (*reduce)(struct collection*, void* initial, void* (*reduce_func)(void* next, void* current), size_t size);

    /*
     * Clean up the collection. If element_cleanup is not NULL, this function
     * will be called to deconstruct each individual element.
     */
    void (*cleanup)(struct collection **self);

    /*
     * Return the number of elements in the collection.
     */
    size_t (*count)(struct collection *self);

    void (*set)(struct collection* self, void* value, size_t i);

    void* (*at)(struct collection* self, size_t i);

    size_t size;

    void *data;
} collection_t;

/*
 * new_array_list: construct a new array list.
 * size: The size of a list element.
 * cleanup: optional cleanup function for individual elements.
 */
collection_t* new_array_list(size_t size, void (*cleanup)(void* item));

/*
 * Hashable interface.
 */
typedef struct hashable {
    /*
     * Convert the hashable item into a 64-bit hash.
     */
    uint64_t (*hash)(struct hashable* self);

    /*
     * The actual data to be hashed.
     */
    void* data;

    /* size (in bytes) of the data*/
    size_t len;

    uint64_t precomputed_hash;

    /*
     * Free the hashable item.
     */
    void (*destroy)(struct hashable** self);
} hashable_t;

/*
 * Create a hashable
 */
hashable_t *hashable_init(void* data, size_t len);

/*
 * A hash map provides quick access to items by providing a key.
 */
typedef void* hash_map_t;

/*
 * Initialize a new hashmap.
 */
hash_map_t* hash_map_init(size_t value_size);

/*
 * Retrieve a value from the given key, or NULL if the key does not exist.
 */
void* hash_map_get(hash_map_t* self, hashable_t *key);

/*
 * Set the value for a given key.
 * This will change the value if the key already exists.
 */
void hash_map_set(hash_map_t* self, hashable_t* key, void* value);

/*
 * Retrieve the collection of hashable_t items used as keys.
 */
collection_t* hash_map_keys(hash_map_t* self);

/*
 * Free the hash map.
 */
void hash_map_destroy(hash_map_t* self);

#endif
