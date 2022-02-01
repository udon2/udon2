# coding: utf8
# Adapted from spaCy's built in visualization suite for dependencies and named entities.
# https://github.com/explosion/spaCy/tree/master/spacy/displacy

from __future__ import unicode_literals


# Setting explicit height and max-width: none on the SVG is required for
# Jupyter to render it properly in a cell

TPL_DEP_SVG = """
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" id="{id}" width="{width}" height="{height}" direction="{dir}" style="max-width: none; height: {height}px; color: {color}; background: {bg}; font-family: {font}; direction: {dir}">{content}</svg>
"""


TPL_DEP_WORDS = """
<text class="token" fill="currentColor" text-anchor="middle" y="{y}">
    <tspan class="word" fill="currentColor" x="{x}" font-size="2em">{id}{text}</tspan>
    <tspan class="tag" dy="2em" fill="currentColor" x="{x}" font-size="2em">{upos}</tspan>
    <tspan class="tag" dy="2em" fill="currentColor" x="{x}" font-size="2em">{xpos}</tspan>
    {feats}
</text>
"""

TPL_DEP_FEATS = """
<tspan class="tag" dy="{dy}" fill="currentColor" x="{x}" font-size="1.75em">{feats}</tspan>
"""


TPL_DEP_ARCS = """
<g class="arrow">
    <path class="arc" id="arrow-{id}-{i}" stroke-width="{stroke}px" d="{arc}" fill="none" stroke="currentColor"/>
    <text dy="1.25em" style="font-size: 1.75em; letter-spacing: 1px">
        <textPath xlink:href="#arrow-{id}-{i}" class="label" startOffset="50%" side="{label_side}" fill="currentColor" text-anchor="middle">{label}</textPath>
    </text>
    <path class="arrowhead" d="{head}" fill="currentColor"/>
</g>
"""


TPL_FIGURE = """
<figure style="margin-bottom: 6rem">{content}</figure>
"""

TPL_TITLE = """
<h2 style="margin: 0">{title}</h2>
"""

TPL_PAGE = """
<!DOCTYPE html>
<html>
    <head>
        <title>displaCy</title>
    </head>

    <body style="font-size: 16px; font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Helvetica, Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'; padding: 4rem 2rem; direction: {dir}">{content}</body>
</html>
"""
