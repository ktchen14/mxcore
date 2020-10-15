Common
======

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

.. c:type:: void * vector_t

   Used to indicate a vector with an indeterminate element type

   This is intended to be used strictly when the element type of a vector is
   indeterminate (such as when implementing this file) and shouldn't be used
   otherwise. Instead a normal C pointer should be used. For example a vector
   with element type ``size_t`` should be declared and created with::
 
      size_t *v_size = vector_create();

.. c:type:: void const * vector_c

   A `vector_t` with a ``const`` element type

.. c:function:: size_t vector_length(vector_c vector)

   Return the length of (the number of elements in) the *vector*

.. c:function:: size_t vector_volume(vector_c vector)

  Return the volume of the *vector*

  A vector's volume is the number of elements that it can hold without a
  reallocation and should always be greater than or equal to the vector's
  `length <vector_length>`.
