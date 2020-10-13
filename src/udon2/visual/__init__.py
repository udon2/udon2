# coding: utf8

# Adapted from spaCy's built in visualization suite for dependencies and named entities.
# https://github.com/explosion/spaCy/tree/master/spacy/displacy

from __future__ import unicode_literals

from udon2 import Node
import langdetect as lang

from .render import DependencyRenderer
from .utils import is_in_jupyter


_html = {}
RENDER_WRAPPER = None

def render_as_svg(docs, fname, page=False, minify=False):
    with open(fname, 'w') as f:
        f.write(render(docs, page=page, minify=minify))


def render(
    docs, page=False, minify=False, jupyter=None, options={}, manual=False
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
    if isinstance(docs, Node):
        docs = [docs]
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
    words = [{"text": w.get_form(), "tag": w.get_upos(), "morph": w.get_feats_as_string().split("|")} for w in doc.linear()]
    id2index = {w.get_id() : i for i, w in enumerate(doc.linear())}
    arcs = []
    for word in doc.linear():
        if word.get_parent():
            i, governor = id2index[word.get_id()], id2index[word.get_parent().get_id()]
            if i < governor:
                arcs.append({
                    "start": i,
                    "end": governor,
                    "label": word.get_deprel(),
                    "dir": "left"
                })
            elif i > governor:
                arcs.append({
                    "start": governor,
                    "end": i,
                    "label": word.get_deprel(),
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
        "lang": lang.detect(doc.get_subtree_text()),
        "direction": "ltr" # just set this as the only available option
    }
