Welcome to Vector's documentation!
==================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   vector/common
   vector/create
   vector/delete
   vector/access
   vector/debug
   vector/resize
   vector/insert
   vector/remove
   vector/shift
   vector/move
   vector/sort
   vector/comparison

.. rubric:: Common Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_t`
     - Used to indicate a vector with an indeterminate element type
   * - `vector_c`
     - A `vector_t` with a ``const`` element type
   * - `vector_length()`
     - Return the length of the *vector*
   * - `vector_volume()`
     - Return the volume of the *vector*

   * - `vector_create()`
     - Allocate and initialize a zero length vector

   * - `vector_delete()`
     - Deallocate the *vector* and return ``NULL``

.. rubric:: Implicit Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_import()`
     - Allocate and initialize a vector from *length* elements of *data*
   * - `vector_define`
     - Allocate and initialize a vector from the argument list

   * - `vector_tail()`
     - Return a pointer to the last element in the *vector*

   * - `vector_debug()`
     - Print debugging information about the *vector* to ``stderr``

   * - `vector_resize()`
     - Resize the `volume <vector_volume>` of the *vector* to *volume*
   * - `vector_ensure()`
     - Ensure that the `volume <vector_volume>` of the *vector* is no less than *length*
   * - `vector_shrink()`
     - Reduce the volume of the *vector* to its length

   * - `vector_insert()`
     - Insert the data at *elmt* into the *vector* at index *i*
   * - `vector_inject()`
     - Insert *n* elements from *elmt* into the *vector* starting at index *i*
   * - `vector_append()`
     - Insert the data at *elmt* as the last element in the *vector*
   * - `vector_extend()`
     - Append *n* elements from *elmt* to the tail of the *vector*

   * - `vector_remove()`
     - Remove the element at index *i* from the *vector*
   * - `vector_excise()`
     - Remove *n* elements at index *i* from the *vector*
   * - `vector_truncate()`
     - Reduce the `length <vector_length>` of the *vector* to *length*

   * - `vector_push()`
     - Insert the data at *elmt* as the last element in the *vector*
   * - `vector_pull()`
     - Copy the last element in the *vector* to *elmt* and remove it
   * - `vector_shift()`
     - Copy the first element in the *vector* to *elmt* and remove it

   * - `vector_swap()`
     - Swap the element at index *i* with the element at index *j* in the *vector*
   * - `vector_move()`
     - Move the element at index *source* to index *target* in the *vector*

   * - `vector_sort()`
     - Sort the *vector* in ascending order on a comparator
   * - `vector_sort_with()`
     - Sort the *vector* in ascending order on a contextual comparator

.. rubric:: Explicit Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_import_z()`
     - Allocate and initialize a vector from *length* elements of *data*

   * - `vector_at()`
     - Return a pointer to the element in the *vector* at index *i*
   * - `vector_tail_z()`
     - Return a pointer to the last element in the *vector*
   * - `vector_index()`
     - Return the index of the element at *elmt* in the *vector*
   * - `vector_get()`
     - Copy the element at index *i* in the *vector* into *elmt*
   * - `vector_set()`
     - Copy the object at *elmt* into the *vector* at index *i*

   * - `vector_debug_z()`
     - Print debugging information about the *vector* to ``stderr``

   * - `vector_resize_z()`
     - Resize the `volume <vector_volume>` of the *vector* to *volume*
   * - `vector_ensure_z()`
     - Ensure that the `volume <vector_volume>` of the *vector* is no less than *length*
   * - `vector_shrink_z()`
     - Reduce the volume of the *vector* to its length

   * - `vector_insert_z()`
     - Insert the data at *elmt* into the *vector* at index *i*
   * - `vector_inject_z()`
     - Insert *n* elements from *elmt* into the *vector* starting at index *i*
   * - `vector_append_z()`
     - Insert the data at *elmt* as the last element in the *vector*
   * - `vector_extend_z()`
     - Append *n* elements from *elmt* to the tail of the *vector*

   * - `vector_remove_z()`
     - Remove the element at index *i* from the *vector*
   * - `vector_excise_z()`
     - Remove *n* elements at index *i* from the *vector*
   * - `vector_truncate_z()`
     - Reduce the `length <vector_length>` of the *vector* to *length*

   * - `vector_push()`
     - Insert the data at *elmt* as the last element in the *vector*
   * - `vector_pull()`
     - Copy the last element in the *vector* to *elmt* and remove it
   * - `vector_shift()`
     - Copy the first element in the *vector* to *elmt* and remove it

   * - `vector_swap_z()`
     - Swap the element at index *i* with the element at index *j* in the *vector*
   * - `vector_move_z()`
     - Move the element at index *source* to index *target* in the *vector*

   * - `vector_sort_z()`
     - Sort the *vector* in ascending order on a comparator
   * - `vector_sort_with_z()`
     - Sort the *vector* in ascending order on a contextual comparator

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
