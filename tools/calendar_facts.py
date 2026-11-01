#!/usr/bin/env python3
"""Every date the story leans on, computed. Old Style dates are Julian; between 1700 and 1800 a
Julian date plus 11 days is its Gregorian natural day. tests in story_check.py read FACTS."""
import datetime as dt
import sys

OFFSET = dt.timedelta(days=11)
WEEKDAYS = ("Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday")

# (description, Old-Style-or-New-Style flag, year as reckoned from 1 January, month, day, weekday)
FACTS = [
    ("the Society is founded", "OS", 1722, 9, 10, "Monday"),
    ("Mary Ashby dies in Rotterdam, written 10 Feb 1745 there", "NS", 1745, 2, 10, "Wednesday"),
    ("the same natural day in England", "OS", 1745, 1, 30, "Wednesday"),
    ("the Fleet wedding, written 3 Feb 1744", "OS", 1745, 2, 3, "Sunday"),
    ("Hannah dies", "OS", 1748, 3, 12, "Saturday"),
    ("the note as dated, 4 March 1750/1", "OS", 1751, 3, 4, "Monday"),
    ("the note as written, 4 March 1752", "OS", 1752, 3, 4, "Wednesday"),
    ("the trust is sealed", "OS", 1751, 10, 14, "Monday"),
    ("the Rotterdam burial paper, read as Old Style", "OS", 1745, 2, 10, "Sunday"),
    ("Hale writes to Lyle", "OS", 1752, 6, 15, "Monday"),
    ("Thomas 'dies'", "OS", 1752, 1, 20, "Monday"),
    ("Lady Day 1752", "OS", 1752, 3, 25, "Wednesday"),
    ("Griggs dies", "OS", 1752, 6, 8, "Monday"),
    ("Chapter 1 opens", "OS", 1752, 8, 1, "Saturday"),
    ("Chapter 1 report is due", "OS", 1752, 8, 4, "Tuesday"),
    ("Jonas Reed dies", "OS", 1752, 8, 9, "Sunday"),
    ("Will Perrin is attacked", "OS", 1752, 8, 11, "Tuesday"),
    ("Margery Bowell dies", "OS", 1752, 8, 13, "Thursday"),
    ("Bartholomew Fair is proclaimed", "OS", 1752, 8, 22, "Saturday"),
    ("Kitty Sharpe is attacked, the storm", "OS", 1752, 8, 25, "Tuesday"),
    ("Pereira sails from Harwich", "OS", 1752, 8, 29, "Saturday"),
    ("the last Old Style day", "OS", 1752, 9, 2, "Wednesday"),
    ("Rotterdam on the English 2 September", "NS", 1752, 9, 13, "Wednesday"),
    ("the first New Style day", "NS", 1752, 9, 14, "Thursday"),
    ("the term day, 10 September Old Style", "NS", 1752, 9, 21, "Thursday"),
    ("Pereira would hang, if he hangs", "NS", 1752, 11, 1, "Wednesday"),
    ("Lyle is sentenced", "NS", 1752, 12, 8, "Friday"),
    ("Lyle hangs", "NS", 1752, 12, 11, "Monday"),
    ("the General Bill closes", "NS", 1752, 12, 12, "Tuesday"),
    ("Old Christmas", "NS", 1753, 1, 5, "Friday"),
    ("Sloane dies", "NS", 1753, 1, 11, "Thursday"),
]


def weekday(style, year, month, day):
    date = dt.date(year, month, day)
    if style == "OS":
        date += OFFSET
    return WEEKDAYS[date.weekday()]


FUND = 31137


def fund(lucy, will, kitty, ned, lyle_guilty=False, pereira_guilty=False):
    """Pounds Ruth's trust shares are worth. The shares of living, verified Lives split the fund."""
    ruth = 1 + int(lucy) + int(will)
    others = (0 if lyle_guilty else 1) + (0 if pereira_guilty else int(kitty) + int(ned))
    return ruth, ruth + others, FUND * ruth // (ruth + others)


def main():
    bad = 0
    for text, style, year, month, day, expected in FACTS:
        got = weekday(style, year, month, day)
        if got != expected:
            print(f"WRONG {text}: {day}/{month}/{year} {style} is {got}, not {expected}")
            bad += 1
    fund = 24 * 400 * 1.04 ** 30
    if round(fund) != 31137:
        print("WRONG fund", fund)
        bad += 1
    if dt.date(1752, 9, 13) - dt.date(1752, 9, 2) != OFFSET:
        print("WRONG offset")
        bad += 1
    print(f"calendar facts: {len(FACTS)} dates checked, {bad} wrong")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
