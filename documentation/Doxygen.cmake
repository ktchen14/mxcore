# The DOXYGEN_VERBATIM_VARS variable can be used to specify a list of Doxygen
# variables (including the leading DOXYGEN_ prefix) which should not be quoted.
# The project is then responsible for ensuring that those variables’ values make
# sense when placed directly in the Doxygen input file. In the case of list
# variables, list items are still separated by spaces, it is only the automatic
# quoting that is skipped.

set(DOXYGEN_VERBATIM_VARS DOXYGEN_ALIASES)

#---------------------------------------------------------------------------
# Project related configuration options
#---------------------------------------------------------------------------

# Set the OPTIMIZE_OUTPUT_FOR_C tag to YES if your project consists of C sources
# only. Doxygen will then generate output that is more tailored for C. For
# instance, some of the names that are used will be different. The list of all
# members will be omitted, etc.

set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)

# When the INLINE_SIMPLE_STRUCTS tag is set to YES, structs, classes, and unions
# with only public data fields or simple typedef fields will be shown inline in
# the documentation of the scope in which they are defined (i.e. file,
# namespace, or group documentation), provided this scope is documented. If set
# to NO, structs, classes, and unions are shown on a separate page (for HTML and
# Man pages) or section (for LaTeX and RTF).

set(DOXYGEN_INLINE_SIMPLE_STRUCTS YES)

# When TYPEDEF_HIDES_STRUCT tag is enabled, a typedef of a struct, union, or
# enum is documented as struct, union, or enum with the name of the typedef. So
# typedef struct TypeS {} TypeT, will appear in the documentation as a struct
# with name TypeT. When disabled the typedef will appear as a member of a file,
# namespace, or class. And the struct will be named TypeS. This can typically be
# useful for C code in case the coding convention dictates that all compound
# types are typedef'ed and only the typedef is referenced, never the tag name.

set(DOXYGEN_TYPEDEF_HIDES_STRUCT YES)

# This tag can be used to specify a number of aliases that act as commands in
# the documentation. You can put \n's in the value part of an alias to insert
# newlines (in the resulting output). You can put ^^ in the value part of an
# alias to insert a newline as if a physical newline was in the original file.
# When you need a literal { or } or , in the value part of an alias you have to
# escape them by means of a backslash (\), this can lead to conflicts with the
# commands \{ and \} for these it is advised to use the version @{ and @} or use
# a double escape (\\{ and \\})

set(DOXYGEN_ALIASES
  length="@ref vector_length \\\"length\\\""
  volume="@ref vector_volume \\\"volume\\\""
)

#---------------------------------------------------------------------------
# Configuration options related to warning and progress messages
#---------------------------------------------------------------------------

# The QUIET tag can be used to turn on/off the messages that are generated to
# standard output by doxygen. If QUIET is set to YES this implies that the
# messages are off.

set(DOXYGEN_QUIET YES)

#---------------------------------------------------------------------------
# Configuration options related to the input files
#---------------------------------------------------------------------------

# The INPUT_FILTER tag can be used to specify a program that doxygen should
# invoke to filter for each input file. Doxygen will invoke the filter program
# by executing (via popen()) the command:
#
# <filter> <input-file>
#
# where <filter> is the value of the INPUT_FILTER tag, and <input-file> is the
# name of an input file. Doxygen will then use the output that the filter
# program writes to standard output. If FILTER_PATTERNS is specified, this tag
# will be ignored.
#
# Note that the filter must not add or remove lines; it is applied before the
# code is scanned, but not when the output code is generated. If lines are added
# or removed, the anchors will not be placed correctly.
#
# Note that for custom extensions or not directly supported extensions you also
# need to set EXTENSION_MAPPING for the extension otherwise the files are not
# properly processed by doxygen.

set(DOXYGEN_INPUT_FILTER "${PROJECT_SOURCE_DIR}/tool/documentation-filter")

#---------------------------------------------------------------------------
# Configuration options related to source browsing
#---------------------------------------------------------------------------

# If the SOURCE_BROWSER tag is set to YES then a list of source files will be
# generated. Documented entities will be cross-referenced with these sources.

set(DOXYGEN_SOURCE_BROWSER YES)

# Setting the STRIP_CODE_COMMENTS tag to YES will instruct doxygen to hide any
# special comment blocks from generated source code fragments. Normal C, C++ and
# Fortran comments will always remain visible.

set(DOXYGEN_STRIP_CODE_COMMENTS NO)

# If the REFERENCED_BY_RELATION tag is set to YES then for each documented
# entity all documented functions referencing it will be listed.

set(DOXYGEN_REFERENCED_BY_RELATION YES)

# If the REFERENCES_RELATION tag is set to YES then for each documented function
# all documented entities called/used by that function will be listed.

set(DOXYGEN_REFERENCES_RELATION YES)

# If the REFERENCES_LINK_SOURCE tag is set to YES and SOURCE_BROWSER tag is set
# to YES then the hyperlinks from functions in REFERENCES_RELATION and
# REFERENCED_BY_RELATION lists will link to the source code. Otherwise they will
# link to the documentation.

set(DOXYGEN_REFERENCES_LINK_SOURCE YES)

# If SOURCE_TOOLTIPS is enabled (the default) then hovering a hyperlink in the
# source code will show a tooltip with additional information such as prototype,
# brief description and links to the definition and documentation. Since this
# will make the HTML file larger and loading of large files a bit slower, you
# can opt to disable this feature.
# This tag requires that the tag SOURCE_BROWSER is set to YES.

set(DOXYGEN_SOURCE_TOOLTIPS YES)

#---------------------------------------------------------------------------
# Configuration options related to the HTML output
#---------------------------------------------------------------------------

# If the HTML_DYNAMIC_SECTIONS tag is set to YES then the generated HTML
# documentation will contain sections that can be hidden and shown after the
# page has loaded.
# This tag requires that the tag GENERATE_HTML is set to YES.

set(DOXYGEN_HTML_DYNAMIC_SECTIONS NO)

# If you want full control over the layout of the generated HTML pages it might
# be necessary to disable the index and replace it with your own. The
# DISABLE_INDEX tag can be used to turn on/off the condensed index (tabs) at top
# of each HTML page. A value of NO enables the index and the value YES disables
# it. Since the tabs in the index contain the same information as the navigation
# tree, you can set this option to YES if you also set GENERATE_TREEVIEW to YES.
# This tag requires that the tag GENERATE_HTML is set to YES.

set(DOXYGEN_DISABLE_INDEX NO)

# The GENERATE_TREEVIEW tag is used to specify whether a tree-like index
# structure should be generated to display hierarchical information. If the tag
# value is set to YES, a side panel will be generated containing a tree-like
# index structure (just like the one that is generated for HTML Help). For this
# to work a browser that supports JavaScript, DHTML, CSS and frames is required
# (i.e. any modern browser). Windows users are probably better off using the
# HTML help feature. Via custom style sheets (see HTML_EXTRA_STYLESHEET) one can
# further fine-tune the look of the index. As an example, the default style
# sheet generated by doxygen has an example that shows how to put an image at
# the root of the tree instead of the PROJECT_NAME. Since the tree basically has
# the same information as the tab index, you could consider setting
# DISABLE_INDEX to YES when enabling this option.
# This tag requires that the tag GENERATE_HTML is set to YES.

set(DOXYGEN_GENERATE_TREEVIEW YES)

# Use this tag to change the font size of LaTeX formulas included as images in
# the HTML documentation. When you change the font size after a successful
# doxygen run you need to manually remove any form_*.png images from the HTML
# output directory to force them to be regenerated.
# This tag requires that the tag GENERATE_HTML is set to YES.

set(DOXYGEN_FORMULA_FONTSIZE 12)

# Enable the USE_MATHJAX option to render LaTeX formulas using MathJax (see
# https://www.mathjax.org) which uses client side Javascript for the rendering
# instead of using pre-rendered bitmaps. Use this if you do not have LaTeX
# installed or if you want to formulas look prettier in the HTML output. When
# enabled you may also need to install MathJax separately and configure the path
# to it using the MATHJAX_RELPATH option.
# This tag requires that the tag GENERATE_HTML is set to YES.

set(DOXYGEN_USE_MATHJAX YES)

#---------------------------------------------------------------------------
# Configuration options related to the preprocessor
#---------------------------------------------------------------------------

# If the ENABLE_PREPROCESSING tag is set to YES, doxygen will evaluate all
# C-preprocessor directives found in the sources and include files.

set(DOXYGEN_ENABLE_PREPROCESSING YES)

# If the MACRO_EXPANSION tag is set to YES, doxygen will expand all macro names
# in the source code. If set to NO, only conditional compilation will be
# performed. Macro expansion can be done in a controlled way by setting
# EXPAND_ONLY_PREDEF to YES.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

set(DOXYGEN_MACRO_EXPANSION YES)

# If the EXPAND_ONLY_PREDEF and MACRO_EXPANSION tags are both set to YES then
# the macro expansion is limited to the macros specified with the PREDEFINED and
# EXPAND_AS_DEFINED tags.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

set(DOXYGEN_EXPAND_ONLY_PREDEF YES)

# The PREDEFINED tag can be used to specify one or more macro names that are
# defined before the preprocessor is started (similar to the -D option of e.g.
# gcc). The argument of the tag is a list of macros of the form: name or
# name=definition (no spaces). If the definition and the "=" are omitted, "=1"
# is assumed. To prevent a macro definition from being undefined via #undef or
# recursively expanded use the := operator instead of the = operator.
# This tag requires that the tag ENABLE_PREPROCESSING is set to YES.

set(DOXYGEN_PREDEFINED "__attribute__(x)=" "DOXYGEN")

set(DOXYGEN_GENERATE_XML YES)
