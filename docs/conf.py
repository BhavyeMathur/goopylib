"""
Configuration file for the Sphinx documentation builder.
"""

from __future__ import annotations

import sys
import os
import re

sys.path.insert(0, os.path.abspath(".."))
sys.path.insert(0, os.path.abspath("."))

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = "goopylib"
copyright = "2022, Bhavye Mathur"
author = "Bhavye Mathur"

version = "2.0.0"
release = "2.0.0.dev5"

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

master_doc = "index"
language = "en"

extensions = [
    "sphinx.ext.napoleon",
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "sphinx.ext.autosectionlabel",
    "sphinx_autodoc_typehints",
    "sphinx.ext.todo",
    "sphinxcontrib.katex"
]

add_module_names = False
todo_include_todos = True

autodoc_member_order = "bysource"
autodoc_typehints = "signature"
# autodoc_typehints_format = "short"
# autodoc_typehints_description_target = "all"

autoclass_content = "init"

typehints_document_rtype = True
typehints_use_rtype = False
typehints_defaults = "comma"
typehints_use_signature = True
typehints_use_signature_return = True

always_document_param_types = False


def autodoc_process_docstring(app, what, name, obj, options, lines: list[str]):
    print(f"\n{name}")

    # No documentation for -in and -out easing functions
    # because the documentation for the in-out function is present
    if name.startswith("goopylib.maths.easing") and (name.endswith("_in") or name.endswith("_out")):
        lines.clear()
        return

    for i, line in enumerate(lines):
        if "|" in line\
                or "only applies to resizable windows" in line\
                or ":raises RuntimeError: window has been destroyed" in line:
            lines.pop(i)
            continue


        if not line.startswith("    "):  # make sure line isn't inside a code-block
            line = line.replace("<goopylib.", "<goopylib.core.")

            # add `` around attribute names
            line = re.sub(r" ([a-zA-Z]\w+_\w*)(\W|$)", r" ``\1``\2", line)

            # style None, int, and tuples
            line = re.sub(r" (None|int|\([\w\d, \"']*\))(\s|$)", r" :py:class:`\1`\2", line)

            if name.startswith("goopylib.maths.easing"):
                line = re.sub(r" callable(\s|$)", r" :py:class:`callable`\1", line)


        lines[i] = line
        print("\t", line)


def setup(app):
    app.connect("autodoc-process-docstring", autodoc_process_docstring)

templates_path = ["templates"]
exclude_patterns = ["output", "sphinx-autodoc-typehints/*", "dev/*", "api_reference/keyboard.rst"]

# -- Options for HTML output ----------------------------------------------

html_theme = "sphinx_rtd_theme"
html_css_files = ["css/style.css"]

html_logo = "../branding/logo/goopylib_primary_logo.svg"
html_favicon = "../branding/logo/goopylib_logomark.ico"
html_static_path = ["static"]

# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
    # The paper size ("letterpaper" or "a4paper").
    # "papersize": "letterpaper",

    # The font size ("10pt", "11pt" or "12pt").
    # "pointsize": "10pt",

    # Additional stuff for the LaTeX preamble.
    # "preamble": "",

    # Latex figure (float) alignment
    # "figure_align": "htbp",
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    # (master_doc, "PoSDBoS.tex", u"PoSDBoS Documentation", u"Paul Pasler", "manual"),
]

# The name of an image file (relative to this directory) to place at the top of
# the title page.
# latex_logo = None

# For "manual" documents, if this is true, then toplevel headings are parts,
# not chapters.
# latex_use_parts = False

# If true, show page references after internal links.
# latex_show_pagerefs = False

# If true, show URL addresses after external links.
# latex_show_urls = False

# Documents to append as an appendix to all manuals.
# latex_appendices = []

# If false, no module index is generated.
# latex_domain_indices = True

# -- Options for manual page output ------------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [(master_doc, "goopylib", "goopylib Documentation", [author], 1)]

# If true, show URL addresses after external links.
# man_show_urls = False


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, "PoSDBoS", u"PoSDBoS Documentation",
     author, "PoSDBoS", "One line description of project.",
     "Miscellaneous"),
]

# Documents to append as an appendix to all manuals.
# texinfo_appendices = []

# If false, no module index is generated.
# texinfo_domain_indices = True

# How to display URL addresses: "footnote", "no", or "inline".
# texinfo_show_urls = "footnote"

# If true, do not generate a @detailmenu in the "Top" node"s menu.
# texinfo_no_detailmenu = False
