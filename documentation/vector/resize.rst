Vector Expansion and Contraction
================================

.. rubric:: Implicit Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_resize()`
     - Resize the `volume <vector_volume>` of the *vector* to *volume*
   * - `vector_ensure()`
     - Ensure that the `volume <vector_volume>` of the *vector* is no less than *length*
   * - `vector_shrink()`
     - Reduce the volume of the *vector* to its length

.. rubric:: Explicit Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_resize_z()`
     - Resize the `volume <vector_volume>` of the *vector* to *volume*
   * - `vector_ensure_z()`
     - Ensure that the `volume <vector_volume>` of the *vector* is no less than *length*
   * - `vector_shrink_z()`
     - Reduce the volume of the *vector* to its length

.. autoaeratefunction:: vector_resize
.. autoaeratefunction:: vector_resize_z
.. autoaeratefunction:: vector_ensure
.. autoaeratefunction:: vector_ensure_z
.. autoaeratefunction:: vector_shrink
.. autoaeratefunction:: vector_shrink_z
