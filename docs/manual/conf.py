# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
curdir = os.getcwd()

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Embroidermodder'
copyright = '2011-2025, The Embroidermodder Team'
author = 'The Embroidermodder Team'
release = '2.0.0-alpha'
description = "A suite of free and open source software for machine embroidery."

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.doctest',
    'sphinx.ext.mathjax',
    'sphinx.ext.viewcode',
    'sphinx.ext.imgmath',
    'sphinx.ext.todo'
]

templates_path = [
    '_templates'
]
exclude_patterns = [
    '_build',
    'Thumbs.db',
    '.DS_Store',
    'embroidermodder/*.md',
    'libembroidery/*.md',
    'embroiderymobile/*.md'
]

html_title = "Embroidermodder " + release

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = "furo"
html_static_path = ['_static']
html_copy_source = True
html_show_sourcelink = True
html_theme_options = {
    "source_repository": "https://github.com/embroidermodder/website",
    "source_branch": "main",
    "source_directory": "emrm/"
}

# -- Options for LaTeX output ------------------------------------------------

latex_additional_files = ['emrm.sty']
latex_elements = {
    'preamble': r'''
\usepackage{emrm}
'''
}
latex_show_urls = 'footnote'

