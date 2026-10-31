# -*- coding: utf-8 -*-
"""Erzeugt die zwei Diagramme fuer die Seite "Betriebsarten" der ETS.

  Baggages/kwlmodeselect.png   Welche Betriebsart gilt? Die Ebenen 7...11 der
                               Vorfahrt (StageArbiter), oberste aktive gewinnt.
  Baggages/kwlstageframe.png   Grund- und Maximalstufe als Rahmen der Automatik,
                               an drei Beispielen mit den Vorgabewerten.

Farben, Schrift und Ueberabtastung wie make_ramp.py.
Aufruf aus dem OFM-Wurzelverzeichnis:  python graphics/make_modes.py
"""
import os
from PIL import Image, ImageDraw, ImageFont

OUT = os.path.join('src', 'Baggages')

INK = (32, 38, 46)
MUTED = (120, 130, 142)
GRID = (205, 212, 220)
SUPPLY = (52, 120, 196)
BAND = (214, 230, 247)
RAMP = (236, 170, 54)
DEAD = (70, 150, 100)
BOX = (243, 246, 249)
PAPER = (255, 255, 255)

# 512 px: die ETS zeichnet mit der Windows-Skalierung (125 % = 640 px) in eine
# Spalte von etwa 665 px; 560 px werden rechts abgeschnitten.
W = 512
S = 2


def font(size, bold=False):
    for name in (('seguisb.ttf', 'segoeuib.ttf', 'arialbd.ttf') if bold
                 else ('segoeui.ttf', 'arial.ttf')):
        try:
            return ImageFont.truetype(os.path.join('C:\\Windows\\Fonts', name), size * S)
        except Exception:
            pass
    return ImageFont.load_default()


def text(d, x, y, s, f, fill, anchor='mm'):
    d.text((x * S, y * S), s, font=f, fill=fill, anchor=anchor)


def line(d, pts, fill, width=2):
    d.line([(x * S, y * S) for x, y in pts], fill=fill, width=width * S, joint='curve')


def rect(d, x0, y0, x1, y1, fill, outline=None, r=6):
    d.rounded_rectangle((x0 * S, y0 * S, x1 * S, y1 * S), radius=r * S, fill=fill,
                        outline=outline, width=S if outline else 0)


def dashed(d, x0, x1, y, fill, dash=5, gap=4, width=1):
    x = x0
    while x < x1:
        line(d, [(x, y), (min(x + dash, x1), y)], fill, width)
        x += dash + gap


def arrow_down(d, x, y0, y1, fill):
    line(d, [(x, y0), (x, y1)], fill, 2)
    line(d, [(x - 5, y1 - 7), (x, y1), (x + 5, y1 - 7)], fill, 2)


def arrow_right(d, x0, x1, y, fill):
    line(d, [(x0, y), (x1, y)], fill, 2)
    line(d, [(x1 - 7, y - 5), (x1, y), (x1 - 7, y + 5)], fill, 2)


def save(im, name, h):
    im = im.resize((W, h), Image.LANCZOS)
    path = os.path.join(OUT, name)
    im.save(path, optimize=True)
    print('geschrieben:', path, im.size)


def mode_select():
    """Ebenen 7...11 (StageArbiter): die oberste aktive bestimmt den Parametersatz."""
    H = 300
    im = Image.new('RGB', (W * S, H * S), PAPER)
    d = ImageDraw.Draw(im)
    f_title, f, f_b, f_s = font(15, True), font(12), font(12, True), font(11)

    text(d, W / 2, 16, 'Welche Betriebsart gilt?', f_title, INK)

    levels = [
        ('Zwangsobjekt 1 / 2 / 3', '1 Bit · Betriebsart per Parameter · Laufzeit'),
        ('Nacht', '1 Bit · z. B. Uhr oder Rollladen'),
        ('Zwangsbetriebsart', '1 Byte · mit Laufzeit · 0 beendet'),
        ('Betriebsart', '1 Byte · dauerhaft · 0 = Standard'),
        ('Standard-Betriebsart', 'Parameter · wenn nichts anderes anliegt'),
    ]
    x0, x1 = 50, 296
    y, h, gap = 40, 36, 8
    for i, (name, note) in enumerate(levels):
        rect(d, x0, y, x1, y + h, BOX, GRID)
        text(d, x0 + 12, y + 12, name, f_b, INK, anchor='lm')
        text(d, x0 + 12, y + 26, note, f_s, MUTED, anchor='lm')
        y += h + gap
    y_end = y - gap

    # Vorrang: von oben nach unten
    arrow_down(d, 38, 44, y_end - 4, SUPPLY)
    # Senkrechte Beschriftung: PIL kann das nur ueber ein gedrehtes Bild.
    lab = Image.new('RGBA', (90 * S, 16 * S), (255, 255, 255, 0))
    ImageDraw.Draw(lab).text((45 * S, 8 * S), 'Vorrang', font=f_s, fill=SUPPLY, anchor='mm')
    lab = lab.rotate(90, expand=True)
    im.paste(lab, (14 * S, int((40 + y_end) / 2 * S - lab.size[1] / 2)), lab)

    # Ergebnis: Parametersatz
    mid = (40 + y_end) / 2
    arrow_right(d, x1 + 8, 340, mid, INK)
    rect(d, 348, mid - 62, W - 12, mid + 62, BAND, SUPPLY)
    text(d, (348 + W - 12) / 2, mid - 44, 'Parametersatz', f_b, INK)
    text(d, (348 + W - 12) / 2, mid - 30, 'der Betriebsart', f_b, INK)
    for k, s in enumerate(('Grundstufe', 'Maximalstufe', 'Führungen', 'Zyklusregel, Nachlauf')):
        text(d, (348 + W - 12) / 2, mid - 8 + k * 16, s, f, INK)

    text(d, W / 2, H - 28, 'Die oberste Ebene, die gerade anliegt, gilt.', f_s, INK)
    text(d, W / 2, H - 12,
         'Eine Bedienung weiter unten setzt die Ebenen darüber zurück.', f_s, MUTED)
    save(im, 'kwlmodeselect.png', H)


def stage_frame():
    """Rahmen der Automatik: max(Grundstufe, Fuehrung), begrenzt auf die Maximalstufe."""
    H = 290
    im = Image.new('RGB', (W * S, H * S), PAPER)
    d = ImageDraw.Draw(im)
    f_title, f, f_b, f_s = font(15, True), font(12), font(12, True), font(11)

    text(d, W / 2, 16, 'Grund- und Maximalstufe', f_title, INK)

    x_axis, y0, step = 60, 214, 34       # y0 = Stufe 0
    ys = lambda st: y0 - st * step
    for st in range(5):
        text(d, x_axis - 10, ys(st), str(st), f_s, MUTED, anchor='rm')
        dashed(d, x_axis, W - 16, ys(st), GRID)
    text(d, x_axis - 10, ys(4) - 20, 'Stufe', f_s, MUTED, anchor='rm')

    # (Titel, Grund, Max, Wunsch der Fuehrungen oder None, Erklaerung)
    cases = [
        ('Komfort', 1, 4, 3, 'CO₂ will 3 → 3'),
        ('Nacht', 1, 1, 3, 'CO₂ will 3 → 1'),
        ('Standby', 1, 2, None, 'keine Anforderung → 1'),
    ]
    cw = (W - 16 - x_axis) / 3
    for i, (name, base, top, wish, note) in enumerate(cases):
        cx = x_axis + cw * i + cw / 2
        bx0, bx1 = cx - 34, cx + 34
        # Rahmen: von Grund- bis Maximalstufe
        rect(d, bx0, ys(top) - 6, bx1, ys(base) + 6, BAND, SUPPLY, r=5)
        text(d, cx, y0 + 22, name, f_b, INK)
        text(d, cx, y0 + 38, f'Grund {base} · Max {top}', f_s, MUTED)
        if wish is not None:
            line(d, [(cx - 52, ys(wish)), (cx - 40, ys(wish))], RAMP, 3)
            dashed(d, cx - 40, cx + 52, ys(wish), RAMP, 4, 3, 2)
        stage = min(max(base, wish or 0), top)
        r = 7
        d.ellipse(((cx - r) * S, (ys(stage) - r) * S, (cx + r) * S, (ys(stage) + r) * S), fill=INK)
        if wish is not None and wish > top:
            line(d, [(cx, ys(wish) + 6), (cx, ys(top) - 10)], RAMP, 2)
            line(d, [(cx - 5, ys(top) - 16), (cx, ys(top) - 9), (cx + 5, ys(top) - 16)], RAMP, 2)
        text(d, cx, y0 + 54, note, f_s, INK)

    # Legende
    ly = 40
    rect(d, x_axis + 4, ly - 6, x_axis + 22, ly + 6, BAND, SUPPLY, r=3)
    text(d, x_axis + 28, ly, 'Rahmen der Betriebsart', f_s, INK, anchor='lm')
    dashed(d, x_axis + 186, x_axis + 210, ly, RAMP, 4, 3, 2)
    text(d, x_axis + 216, ly, 'Wunsch der Führungen', f_s, INK, anchor='lm')
    d.ellipse(((x_axis + 360 - 5) * S, (ly - 5) * S, (x_axis + 360 + 5) * S, (ly + 5) * S), fill=INK)
    text(d, x_axis + 370, ly, 'Stufe', f_s, INK, anchor='lm')

    save(im, 'kwlstageframe.png', H)


if __name__ == '__main__':
    mode_select()
    stage_frame()
