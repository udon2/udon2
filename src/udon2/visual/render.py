# coding: utf8

# Modified by Dmytro Kalpakchi, 2020
# Adapted from spaCy's built in visualization suite for dependencies and named entities.
# https://github.com/explosion/spaCy/tree/master/spacy/displacy

from __future__ import unicode_literals

import uuid

from .templates import TPL_DEP_SVG, TPL_DEP_WORDS, TPL_DEP_ARCS, TPL_DEP_FEATS
from .templates import TPL_FIGURE, TPL_TITLE, TPL_PAGE
from .utils import escape_html, minify_html

DEFAULT_DIR = "ltr"


class DependencyRenderer(object):
    """Render dependency parses as SVGs."""

    def __init__(self, options={}):
        """Initialise dependency renderer.

        options (dict): Visualiser-specific options (compact, word_spacing,
            arrow_spacing, arrow_width, arrow_stroke, distance, offset_x,
            color, bg, font)
        """
        self.compact = options.get("compact", False)
        self.word_spacing = options.get("word_spacing", 45)
        self.arrow_spacing = options.get("arrow_spacing", 12 if self.compact else 20)
        self.arrow_width = options.get("arrow_width", 6 if self.compact else 10)
        self.arrow_stroke = options.get("arrow_stroke", 2)
        self.distance = options.get("distance", 150 if self.compact else 175)
        self.offset_x = options.get("offset_x", 50)
        self.color = options.get("color", "#000000")
        self.bg = options.get("bg", "#ffffff")
        self.font = options.get("font", "Arial")
        self.has_upos = options.get('include_upos', True)
        self.has_xpos = options.get('include_xpos', False)
        self.direction = DEFAULT_DIR

    def render(self, parsed, page=False, minify=False):
        """Render complete markup.

        parsed (list): Dependency parses to render.
        page (bool): Render parses wrapped as full HTML page.
        minify (bool): Minify HTML markup.
        RETURNS (unicode): Rendered SVG or HTML markup.
        """
        # Create a random ID prefix to make sure parses don't receive the
        # same ID, even if they're identical
        id_prefix = uuid.uuid4().hex
        rendered = []
        for i, p in enumerate(parsed):
            if i == 0:
                settings = p.get("settings", {})
                self.direction = settings.get("direction", DEFAULT_DIR)
            render_id = "{}-{}".format(id_prefix, i)
            svg = self.render_svg(render_id, p["words"], p["arcs"])
            rendered.append(svg)
        if page:
            content = "".join([TPL_FIGURE.format(content=svg) for svg in rendered])
            markup = TPL_PAGE.format(
                content=content, dir=self.direction
            )
        else:
            markup = "".join(rendered)
        if minify:
            return minify_html(markup)
        return markup

    def render_svg(self, render_id, words, arcs):
        """Render SVG.

        render_id (int): Unique ID, typically index of document.
        words (list): Individual words and their tags.
        arcs (list): Individual arcs and their start, end, direction and label.
        RETURNS (unicode): Rendered SVG markup.
        """
        self.levels = self.get_levels(arcs)
        self.highest_level = len(self.levels)
        self.offset_y = self.distance / 2 * self.highest_level + self.arrow_stroke
        self.width = self.offset_x + len(words) * self.distance
        self.max_feats = max([len(w['feats']) for w in words])
        self.height = self.offset_y + (1.1 * self.max_feats + 2 * (self.has_upos + self.has_xpos)) * self.word_spacing
        self.id = render_id
        words = [self.render_word(w["text"], w["upos"], w["xpos"], w["feats"], w['id'], i) for i, w in enumerate(words)]
        arcs = [
            self.render_arrow(a["label"], a["start"], a["end"], a["dir"], i)
            for i, a in enumerate(arcs)
        ]
        content = "".join(words) + "".join(arcs)
        return TPL_DEP_SVG.format(
            id=self.id,
            width=self.width,
            height=self.height,
            color=self.color,
            bg=self.bg,
            font=self.font,
            content=content,
            dir=self.direction,
        )

    def render_word(self, text, upos, xpos, feats, idx, i):
        """Render individual word.

        text (unicode): Word text.
        tag (unicode): Part-of-speech tag.
        i (int): Unique ID, typically word index.
        RETURNS (unicode): Rendered SVG markup.
        """
        y = self.offset_y + self.word_spacing
        x = self.offset_x + i * self.distance
        if self.direction == "rtl":
            x = self.width - x
        svg_text = escape_html(text)
        svg_feats = "\n".join([
            TPL_DEP_FEATS.format(dy="2em", feats=m, x=x)
            for i, m in enumerate(feats)
        ])
        return TPL_DEP_WORDS.format(text=svg_text, id=idx, upos=upos, xpos=xpos, feats=svg_feats, x=x, y=y)

    def render_arrow(self, label, start, end, direction, i):
        """Render individual arrow.

        label (unicode): Dependency label.
        start (int): Index of start word.
        end (int): Index of end word.
        direction (unicode): Arrow direction, 'left' or 'right'.
        i (int): Unique ID, typically arrow index.
        RETURNS (unicode): Rendered SVG markup.
        """
        if start < 0 or end < 0:
            error_args = dict(start=start, end=end, label=label, dir=direction)
            raise ValueError(Errors.E157.format(**error_args))
        level = self.levels.index(end - start) + 1
        x_start = self.offset_x + start * self.distance + self.arrow_spacing
        if self.direction == "rtl":
            x_start = self.width - x_start
        y = self.offset_y
        x_end = (
            self.offset_x
            + (end - start) * self.distance
            + start * self.distance
            - self.arrow_spacing * (self.highest_level - level) / 4
        )
        if self.direction == "rtl":
            x_end = self.width - x_end
        y_curve = self.offset_y - level * self.distance / 2
        if self.compact:
            y_curve = self.offset_y - level * self.distance / 6
        if y_curve == 0 and len(self.levels) > 5:
            y_curve = -self.distance
        arrowhead = self.get_arrowhead(direction, x_start, y, x_end)
        arc = self.get_arc(x_start, y, y_curve, x_end)
        label_side = "right" if self.direction == "rtl" else "left"
        return TPL_DEP_ARCS.format(
            id=self.id,
            i=i,
            stroke=self.arrow_stroke,
            head=arrowhead,
            label=label,
            label_side=label_side,
            arc=arc,
        )

    def get_arc(self, x_start, y, y_curve, x_end):
        """Render individual arc.

        x_start (int): X-coordinate of arrow start point.
        y (int): Y-coordinate of arrow start and end point.
        y_curve (int): Y-corrdinate of Cubic Bézier y_curve point.
        x_end (int): X-coordinate of arrow end point.
        RETURNS (unicode): Definition of the arc path ('d' attribute).
        """
        template = "M{x},{y} C{x},{c} {e},{c} {e},{y}"
        if self.compact:
            template = "M{x},{y} {x},{c} {e},{c} {e},{y}"
        return template.format(x=x_start, y=y, c=y_curve, e=x_end)

    def get_arrowhead(self, direction, x, y, end):
        """Render individual arrow head.

        direction (unicode): Arrow direction, 'left' or 'right'.
        x (int): X-coordinate of arrow start point.
        y (int): Y-coordinate of arrow start and end point.
        end (int): X-coordinate of arrow end point.
        RETURNS (unicode): Definition of the arrow head path ('d' attribute).
        """
        if direction == "left":
            pos1, pos2, pos3 = (x, x - self.arrow_width + 2, x + self.arrow_width - 2)
        else:
            pos1, pos2, pos3 = (
                end,
                end + self.arrow_width - 2,
                end - self.arrow_width + 2,
            )
        arrowhead = (
            pos1,
            y + 2,
            pos2,
            y - self.arrow_width,
            pos3,
            y - self.arrow_width,
        )
        return "M{},{} L{},{} {},{}".format(*arrowhead)

    def get_levels(self, arcs):
        """Calculate available arc height "levels".
        Used to calculate arrow heights dynamically and without wasting space.

        args (list): Individual arcs and their start, end, direction and label.
        RETURNS (list): Arc levels sorted from lowest to highest.
        """
        levels = set(map(lambda arc: arc["end"] - arc["start"], arcs))
        return sorted(list(levels))
