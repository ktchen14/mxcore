Resize
======

**Implicit Interface**:

.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_resize()`
     - Resize the `volume <vector_volume>` of the *vector* to *volume*

**Explicit Interface**:

.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_resize_z()`
     - Resize the `volume <vector_volume>` of the *vector* to *volume*

.. function:: vector_t vector_resize_za(vector_t vector, size_t volume, size_t z)
   :aerate_id: group__vector__module.vector_resize

   Reference to :func:`target <id=group__vector__module.vector_resize>`.

   Resize the `volume <vector_volume>` of the *vector* to *volume*

   If *volume* is less than the `length <vector_length>` of the *vector* then
   the *vector* will be truncated and have its length reduced to *volume*.

   This will fail if the underlying ``realloc()`` returns ``NULL``. The C
   standard **doesn't** guarantee success of a ``realloc()`` to a smaller size,
   so this can fail even if *volume* is less than the volume of the *vector*. In
   either case if the ``realloc()`` fails then the *vector* will be unmodified
   and the value of ``errno`` set by ``realloc()`` will be retained.

   :param vector_t vector: the vector to operate on
   :param size_t volume: the volume to resize the *vector* to
   :param size_t z: the element size of the *vector*
   :return: the resized vector on success; otherwise ``NULL``
   :rtype: `vector_t`

   .. seealso:: `vector_resize()` for the implicit interface analogue

.. autoaeratefunction:: vector_resize
.. autoaeratefunction:: vector_resize_z
