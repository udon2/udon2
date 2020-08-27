# coding: utf8
"""
Adapted from spaCy's built in visualization suite for dependencies and named entities.

DOCS: https://spacy.io/api/top-level#displacy
USAGE: https://spacy.io/usage/visualizers
"""
from __future__ import unicode_literals

from udon2 import Node
import langdetect as lang

from .render import DependencyRenderer, EntityRenderer
from .utils import is_in_jupyter


_html = {}
RENDER_WRAPPER = None

def render_as_svg(docs, fname, style='dep', page=False, minify=False):
    with open(fname, 'w') as f:
        f.write(render(docs, style=style, page=page, minify=minify))


def render(
    docs, style="dep", page=False, minify=False, jupyter=None, options={}, manual=False
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
    factories = {
        "dep": (DependencyRenderer, parse_deps),
        "ent": (EntityRenderer, parse_ents),
    }
    if style not in factories:
        raise ValueError("Unknown style {style}".format(style=style))
    if isinstance(docs, Node):
        docs = [docs]
    if not all(isinstance(obj, Node) for obj in docs):
        raise ValueError("Invalid object passed to displaymaster: can work only with udon2.Node")
    renderer, converter = factories[style]
    renderer = renderer(options=options)
    parsed = [converter(doc, options) for doc in docs] if not manual else docs
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


def serve(
    docs,
    style="dep",
    page=True,
    minify=False,
    options={},
    manual=False,
    port=5000,
    host="0.0.0.0",
):
    """Serve displaCy visualisation.

    docs (list or Doc): Document(s) to visualise.
    style (unicode): Visualisation style, 'dep' or 'ent'.
    page (bool): Render markup as full HTML page.
    minify (bool): Minify HTML markup.
    options (dict): Visualiser-specific options, e.g. colors.
    manual (bool): Don't parse `Doc` and instead expect a dict/list of dicts.
    port (int): Port to serve visualisation.
    host (unicode): Host to serve visualisation.

    DOCS: https://spacy.io/api/top-level#displacy.serve
    USAGE: https://spacy.io/usage/visualizers
    """
    from wsgiref import simple_server

    if is_in_jupyter():
        user_warning(Warnings.W011)

    render(docs, style=style, page=page, minify=minify, options=options, manual=manual)
    httpd = simple_server.make_server(host, port, app)
    print("\nUsing the '{}' visualizer".format(style))
    print("Serving on http://{}:{} ...\n".format(host, port))
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("Shutting down server on port {}.".format(port))
    finally:
        httpd.server_close()


def app(environ, start_response):
    # Headers and status need to be bytes in Python 2, see #1227
    headers = [(b_to_str(b"Content-type"), b_to_str(b"text/html; charset=utf-8"))]
    start_response(b_to_str(b"200 OK"), headers)
    res = _html["parsed"].encode(encoding="utf-8")
    return [res]


def parse_deps(orig_doc, options={}):
    """Generate dependency parse in {'words': [], 'arcs': []} format.

    doc (Doc): Document do parse.
    RETURNS (dict): Generated dependency parse keyed by words and arcs.
    """
    doc = orig_doc.get_children()[0] if orig_doc.is_root() else orig_doc # might reparse it later
    words = [{'text': 'root', 'tag': 'root', "morph": ["root"]}]
    words.extend([{"text": w.get_text(), "tag": w.get_pos(), "morph": w.get_morph().split("|")} for w in doc.linear()])
    arcs = []
    for word in doc.linear():
        i, governor = int(word.get_id()), int(word.get_parent().get_id())
        if i < governor:
            arcs.append({
                "start": i,
                "end": governor,
                "label": word.get_rel(),
                "dir": "left"
            })
        elif i > governor:
            arcs.append({
                "start": governor,
                "end": i,
                "label": word.get_rel(),
                "dir": "right",
            })
    return {"words": words, "arcs": arcs, "settings": get_doc_settings(orig_doc)}


def parse_ents(doc, options={}):
    """Generate named entities in [{start: i, end: i, label: 'label'}] format.

    doc (Doc): Document do parse.
    RETURNS (dict): Generated entities keyed by text (original text) and ents.
    """
    ents = [
        {"start": ent.start_char, "end": ent.end_char, "label": ent.label_}
        for ent in doc.ents
    ]
    if not ents:
        user_warning(Warnings.W006)
    title = doc.user_data.get("title", None) if hasattr(doc, "user_data") else None
    settings = get_doc_settings(doc)
    return {"text": doc.text, "ents": ents, "title": title, "settings": settings}


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
