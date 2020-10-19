Element Removal
===============

.. rubric:: Implicit Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_remove()`
     - Remove the element at index *i* from the *vector*
   * - `vector_excise()`
     - Remove *n* elements at index *i* from the *vector*
   * - `vector_truncate()`
     - Reduce the `length <vector_length>` of the *vector* to *length*

.. rubric:: Explicit Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_remove_z()`
     - Remove the element at index *i* from the *vector*
   * - `vector_excise_z()`
     - Remove *n* elements at index *i* from the *vector*
   * - `vector_truncate_z()`
     - Reduce the `length <vector_length>` of the *vector* to *length*

.. autoaeratefunction:: vector_remove
.. autoaeratefunction:: vector_remove_z
.. autoaeratefunction:: vector_excise
.. autoaeratefunction:: vector_excise_z
.. autoaeratefunction:: vector_truncate
.. autoaeratefunction:: vector_truncate_z
