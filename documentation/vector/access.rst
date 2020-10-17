Element Access and Lookup
=========================

**Implicit Interface**:

.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_tail()`
     - Return a pointer to the last element in the *vector*

**Explicit Interface**:

.. list-table::
   :widths: auto
   :width: 100%

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

.. c:function:: void *vector_at(vector_t vector, size_t i, size_t z)

   Return a pointer to the element in the *vector* at index *i*

   .. note::

      Though this operation doesn't have the ``_z`` suffix, it **is** a part of
      the explicit interface and takes the element size of the *vector* as *z*.
      This operation is redundant if the element type of the *vector* is known
      at compile time as it's identical to ``vector + i``.

   .. note::
      This operation is implemented as a macro but documented as a function to
      clarify its intended usage.

   If *i* is the length of the *vector* then this will return a pointer to just
   past the last element of the *vector*.

   This operation is ``const`` qualified on the *vector*. That is if the element
   type of the *vector* is ``const`` qualified (*vector* is compatible with a
   `vector_c`), this will return a ``const void *``. Otherwise this will return
   a ``void *``.

   This is the inverse of `vector_index()` such that::

      vector_index(vector, vector_at(i)) == i
      vector_at(vector, vector_index(elmt)) == elmt

   If *i* is neither an index in the *vector* or its length then the behavior is
   undefined.

   .. seealso::

      `vector_index()` for the inverse operation to get the index of an element
      in a vector


.. c:function:: void *vector_tail(vector_t vector)

   Return a pointer to the last element in the *vector*

   .. note::
      This operation is implemented as a macro but documented as a function to
      clarify its intended usage.

   This operation is ``const`` qualified on the *vector*. That is if the element
   type of the *vector* is ``const`` qualified (*vector* is compatible with a
   `vector_c`) then this will return a ``const void *``. Otherwise this will
   return a ``void *``.

   If no last element is in the *vector* (the *vector*'s `length
   <vector_length>` is zero) then the behavior is undefined.

   .. hint::
      No ``vector_head()`` is available; a pointer to the first element in some
      vector is just ``vector``.

   .. seealso:: `vector_tail_z()` for the explicit interface analogue

.. c:function:: void *vector_tail_z(vector_t vector, size_t z)

   Return a pointer to the last element in the *vector*

   .. note::
      This operation is implemented as a macro but documented as a function to
      clarify its intended usage.

   This operation is ``const`` qualified on the *vector*. That is if the element
   type of the *vector* is ``const`` qualified (*vector* is compatible with a
   `vector_c`) then this will return a ``const void *``. Otherwise this will
   return a ``void *``.

   If no last element is in the *vector* (the *vector*'s `length
   <vector_length>` is zero) then the behavior is undefined.

   .. hint::
      No ``vector_head_z()`` is available; a pointer to the first element in
      some vector is just ``vector``.

   .. seealso:: `vector_tail()` for the implicit interface analogue

.. c:function:: size_t vector_index(vector_c vector, const void *elmt, size_t z)

   Return the index of the element at *elmt* in the *vector*

   .. note::

      Though this operation doesn't have the ``_z`` suffix, it **is** a part of
      the explicit interface and takes the element size of the *vector* as *z*.
      This operation is redundant if the element type of the *vector* is known
      at compile time as it's identical to ``elmt - vector``.

   This doesn't inspect the data at *elmt* or the elements in the *vector*;
   *elmt* must already be a pointer to an element in the *vector*.

   This is the inverse of `vector_at()` such that::

      vector_at(vector, vector_index(elmt)) == elmt
      vector_index(vector, vector_at(i)) == i

   If *elmt* is ``NULL`` or isn't a pointer to an element in the *vector* then
   the behavior is undefined. If *elmt* is a pointer to an offset in an element
   in the *vector* then the behavior is undefined.

   :param vector_c vector: the vector to operate on
   :param elmt: the element
   :type elmt: :emphasis:`const void *`
   :param size_t z: the element size of the *vector*
   :return: the index of *elmt* in the *vector*
   :rtype: size_t

   .. seealso::

      `vector_at()` for the inverse operation to get a pointer to an element in
      a vector

.. function:: void vector_get(vector_c vector, size_t i, void *elmt, size_t z)

   Copy the element at index *i* in the *vector* into *elmt*

   .. note::

      Though this operation doesn't have the ``_z`` suffix, it **is** a part of
      the explicit interface and takes the element size of the *vector* as *z*.
      This operation is redundant if the element type of the *vector* is known
      at compile time as it's identical to ``*elmt = vector[i]``.

   If *i* isn't an index in the *vector* or *elmt* is ``NULL`` then the behavior
   is undefined.

   :param vector_c vector: the vector to operate on
   :param size_t i: the index of the element in the *vector* to copy from
   :param elmt: the location to copy the element to
   :type elmt: :emphasis:`void *`
   :param size_t z: the element size of the *vector*

   .. seealso::

      `vector_set()` for the inverse operation to copy data into a vector

.. function:: void vector_set(vector_t vector, size_t i, const void *elmt, size_t z)

   Copy the object at *elmt* into the *vector* at index *i*

   .. note::

      Though this operation doesn't have the ``_z`` suffix, it **is** a part of
      the explicit interface and takes the element size of the *vector* as *z*.
      This operation is redundant if the element type of the *vector* is known
      at compile time as it's identical to ``vector[i] = *elmt``.

   If *i* isn't an index in the *vector*, *elmt* is ``NULL``, or the type of the
   object at *elmt* is incompatible with the element type of the *vector*, then
   the behavior is undefined.
   
   :param vector_t vector: the vector to operate on
   :param size_t i: the index of the element in the *vector* to copy to
   :param elmt: the location to copy the element from
   :type elmt: :emphasis:`const void *`
   :param size_t z: the element size of the *vector*
   
   .. seealso::

      `vector_get()` for the inverse operation to copy data from a vector
