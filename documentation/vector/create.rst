Vector Allocation and Creation
==============================

.. rubric:: Common Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_create()`
     - Allocate and initialize a zero length vector

.. rubric:: Implicit Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_import()`
     - Allocate and initialize a vector from *length* elements of *data*
   * - `vector_define`
     - Allocate and initialize a vector from the argument list

.. rubric:: Explicit Interface
.. list-table::
   :widths: auto
   :width: 100%

   * - `vector_import_z()`
     - Allocate and initialize a vector from *length* elements of *data*

.. autoaeratefunction:: vector_create
.. autoaeratefunction:: vector_import
.. autoaeratefunction:: vector_import_z
.. autoaeratemacro:: vector_define
