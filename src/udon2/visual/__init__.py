# coding: utf8

# Modified by Dmytro Kalpakchi, 2020
# Adapted from spaCy's built in visualization suite for dependencies and named entities.
# https://github.com/explosion/spaCy/tree/master/spacy/displacy

from __future__ import unicode_literals

import re

from udon2 import Node
import svgling

from .render import DependencyRenderer
from .utils import is_in_jupyter


_html = {}
RENDER_WRAPPER = None

def render_dep_tree(node, fname, img_format='svg', page=False, options={}, minify=False):
    if (img_format == 'svg'):
        with open(fname, 'w') as f:
            f.write(render(node, page=page, minify=minify, options=options))


def tree2list(node):
    if not node.children:
        return node.form
    lst = [node.form]
    for x in node.children:
        res = tree2list(x)
        if res:
            lst.append(res)
    return lst


def render_tree(node, fname, img_format='svg'):
    base_pixl = 16
    options = svgling.core.TreeOptions()
    with open(fname, 'w') as f:
        svg_string = svgling.core.TreeLayout(tree2list(node), options=options)._repr_svg_()
        ems = set(re.findall(r"\d+.?\d*em", svg_string))
        for em in ems:
            px = float(em[:-2]) * base_pixl
            svg_string = svg_string.replace(em, "{}px".format(px))
        f.write(svg_string)


def render(
    node, page=False, minify=False, jupyter=None, options={}, manual=False
):
    """Render displaCy visualisation.

    docs (list or Doc): Document(s) to visualise.
    style (unicode): Visualisation style, 'dep' or 'ent'.
    page (bool): Render markup as full HTML page.
    minify (bool): Minify HTML markup.
    jupyter (bool): Override Jupyter auto-detection.
    options (dict): Visualiser-specific options, e.g. colors.
    manual (bool): Don't parse `Doc` and instead expect a dict/list of dicts.
    RETURNS (unicode): Rendered HTML markup.

    DOCS: https://spacy.io/api/top-level#displacy.render
    USAGE: https://spacy.io/usage/visualizers
    """
    if isinstance(node, Node):
        docs = [node]
    if not all(isinstance(obj, Node) for obj in docs):
        raise ValueError("Invalid object passed to displaymaster: can work only with udon2.Node")
    renderer = DependencyRenderer(options=options)
    parsed = [parse_deps(doc, options) for doc in docs] if not manual else docs
    _html["parsed"] = renderer.render(parsed, page=page, minify=minify).strip()
    html = _html["parsed"]
    if RENDER_WRAPPER is not None:
        html = RENDER_WRAPPER(html)
    if jupyter or (jupyter is None and is_in_jupyter()):
        # return HTML rendered by IPython display()
        # See #4840 for details on span wrapper to disable mathjax
        from IPython.core.display import display, HTML

        return display(HTML('<span class="tex2jax_ignore">{}</span>'.format(html)))
    return html


def parse_deps(orig_doc, options={}):
    """Generate dependency parse in {'words': [], 'arcs': []} format.

    doc (Doc): Document do parse.
    RETURNS (dict): Generated dependency parse keyed by words and arcs.
    """
    doc = orig_doc # might reparse it later
    words = [{
        "id": "{}: ".format(int(w.id)) if options.get('include_ids', False) else '',
        "text": w.form,
        "upos": w.upos if options.get('include_upos', True) else "",
        "xpos": w.xpos if options.get('include_xpos', False) else "",
        "feats": str(w.feats).split("|") if options.get('include_feats', True) else ''
    } for w in doc.linear()]
    id2index = {w.id : i for i, w in enumerate(doc.linear())}
    arcs = []
    for word in doc.linear():
        if word.parent:
            i, governor = id2index[word.id], id2index[word.parent.id]
            if i < governor:
                arcs.append({
                    "start": i,
                    "end": governor,
                    "label": word.deprel,
                    "dir": "left"
                })
            elif i > governor:
                arcs.append({
                    "start": governor,
                    "end": i,
                    "label": word.deprel,
                    "dir": "right",
                })
    return {"words": words, "arcs": arcs, "settings": get_doc_settings(orig_doc)}


def set_render_wrapper(func):
    """Set an optional wrapper function that is called around the generated
    HTML markup on displacy.render. This can be used to allow integration into
    other platforms, similar to Jupyter Notebooks that require functions to be
    called around the HTML. It can also be used to implement custom callbacks
    on render, or to embed the visualization in a custom page.

    func (callable): Function to call around markup before rendering it. Needs
        to take one argument, the HTML markup, and should return the desired
        output of displacy.render.
    """
    global RENDER_WRAPPER
    if not hasattr(func, "__call__"):
        raise ValueError(Errors.E110.format(obj=type(func)))
    RENDER_WRAPPER = func


def get_doc_settings(doc):
    return {
        "direction": "ltr" # just set this as the only available option
    }
