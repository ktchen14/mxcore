# Configuration file for the Sphinx documentation builder

import os
import sys
sys.path.insert(0, os.path.abspath('../../aerate'))

if os.environ.get("READTHEDOCS"):
    import subprocess
    subprocess.check_call("./before-rtd")

# -- Project information -----------------------------------------------------

project = 'Vector'
copyright = '2020, Kaiting Chen'
author = 'Kaiting Chen'

# The full version, including alpha/beta/rc tags
with open("../VERSION") as file:
    release = file.read().strip()


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "aerate",
]

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['output', 'Thumbs.db', '.DS_Store', '.venv']

# The name of the default domain. Can also be None to disable a default domain.
# The default is 'py'. Those objects in other domains (whether the domain name
# is given explicitly, or selected by a default-domain directive) will have the
# domain name explicitly prepended when named (e.g., when the default domain is
# C, Python functions will be named “Python function”, not just “function”).
primary_domain = 'c'

# The name of a reST role (builtin or Sphinx extension) to use as the default
# role, that is, for text marked up `like this`. This can be set to 'py:obj' to
# make `filter` a cross-reference to the Python function “filter”. The default
# is None, which doesn’t reassign the default role.
default_role = 'any'


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for a
# list of builtin themes.
import sphinx_readable_theme
html_theme_path = [sphinx_readable_theme.get_html_theme_path()]
html_theme = 'readable'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ["static"]
html_css_files = ["custom.css"]

# The default language to highlight source code in.

highlight_language = 'c'
