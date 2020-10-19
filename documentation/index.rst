Welcome to Vector's documentation!
==================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   vector/common
   vector/create
   vector/delete
   vector/access
   vector/resize
   vector/insert

.. toctree::
   :hidden:

   source/index

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

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
