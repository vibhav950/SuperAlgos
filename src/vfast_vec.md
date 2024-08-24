# vfast_vec usage guide

| Operation                            | Description                                                                                    | Syntax                      |
| ------------------------------------ | ---------------------------------------------------------------------------------------------- | --------------------------- |
| `vec(type)`                        | Defines a vector type for the specified type.                                                  | `vec(int) v1, v2;`        |
| `vec_capacity(vector)`             | Returns the capacity of the vector as a `size_t`.                                            | `vec_capacity(v1);`       |
| `vec_length(vector)`               | Returns the number of elements in the vector as a `size_t`.                                  | `vec_length(v1);`         |
| `vec_empty(vector)`                | Returns 1 if the vector is empty, 0 otherwise.                                                 | `vec_empty(v1);`          |
| `vec_full(vector)`                 | Returns 1 if the vector is full, 0 otherwise.                                                  | `vec_full(v1);`           |
| `vec_type(vector)`                 | Returns the type of elements stored in the vector.                                             | `vec_type(v1);`           |
| `vec_init(type, vector, capacity)` | Initializes the vector with the given capacity (or default capacity if 0).                     | `vec_init(int, v1, 100);` |
| `vec_free(vector)`                 | Frees the memory allocated for the vector.                                                     | `vec_free(v1);`           |
| `vec_resize(vector, capacity)`     | Resizes the vector to the specified capacity.                                                  | `vec_resize(v1, 200);`    |
| `vec_shrink(vector)`               | Shrinks the vector's capacity to match its length.                                             | `vec_shrink(v1);`         |
| `vec_push(vector, value)`          | Pushes a value to the end of the vector.                                                       | `vec_push(v1, 10);`       |
| `vec_insert(vector, value, pos)`   | Inserts a value at the specified position in the vector.                                       | `vec_insert(v1, 10, 2);`  |
| `vec_pop(vector)`                  | Removes the element at the end of the vector (does not change capacity).                       | `vec_pop(v1);`            |
| `vec_delete_last(vector)`          | Deletes the element at the end of the vector (reduces capacity by 1 at most).                  | `vec_delete_last(v1);`    |
| `vec_remove(vector, pos)`          | Removes the element from the specified position in the vector (does not change capacity).      | `vec_remove(v1, 2);`      |
| `vec_delete(vector, pos)`          | Deletes the element from the specified position in the vector (reduces capacity by 1 at most). | `vec_delete(v1, 2);`      |
| `vec_replace(vector, value, pos)`  | Replaces the element at the specified position in the vector with the specified value.         | `vec_replace(v1, 0, 2);`  |
| `vec_copy(dst, src)`               | Copies the elements of one vector to another.                                                  | `vec_copy(v2, v1);`       |
