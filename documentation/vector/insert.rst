Element Insertion
=================

.. rubric:: Implicit Interface
.. list-table::
   :widths: auto
   :width: 100%

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

   * - `vector_insert_z()`
     - Insert the data at *elmt* into the *vector* at index *i*
   * - `vector_inject_z()`
     - Insert *n* elements from *elmt* into the *vector* starting at index *i*
   * - `vector_append_z()`
     - Insert the data at *elmt* as the last element in the *vector*
   * - `vector_extend_z()`
     - Append *n* elements from *elmt* to the tail of the *vector*

.. autoaeratefunction:: vector_insert
.. autoaeratefunction:: vector_insert_z
.. autoaeratefunction:: vector_inject
.. autoaeratefunction:: vector_inject_z
.. autoaeratefunction:: vector_append
.. autoaeratefunction:: vector_append_z
.. autoaeratefunction:: vector_extend
.. autoaeratefunction:: vector_extend_z
