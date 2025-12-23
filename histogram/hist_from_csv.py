# [Usage]
# 
# python hist_from_csv.py data.csv \
#     --column-name value \
#     --bins 50 \
#     --range 0 100 \
#     --density or --percentage \
#     --xlabel "Value" \
#     --ylabel "Probability density" \
#     --title "Value distribution" \
#     --output hist_value.png
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import sys
from typing import Optional, Tuple

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Script to create histograms from CSV files"
    )

    parser.add_argument("csv_path", help="Input CSV file")

    parser.add_argument("--column-name", help="Specify by colume name")
    parser.add_argument("--column-index", type=int, help="row number (Start with 0)")

    parser.add_argument("--bins", type=int, default=30, help="Number of bins")
    parser.add_argument(
        "--range", nargs=2, type=float, metavar=("MIN", "MAX"),
        help="Range of the target value"
    )

    parser.add_argument("--density", action="store_true",
                        help="Probability density (area = 1)")
    parser.add_argument("--percentage", action="store_true",
                        help="Percentage (Total = 100)")

    parser.add_argument("--xlabel", default=None)
    parser.add_argument("--ylabel", default=None)
    parser.add_argument("--title", default=None)

    parser.add_argument("--output", default=None)
    parser.add_argument("--dpi", type=int, default=300,
                        help="Output image resolution (dpi)")

    return parser.parse_args()


def choose_column(df, column_name, column_index):
    if column_name and column_index is not None:
        print("cannot specify both column-name and column-index at the same time.", file=sys.stderr)
        sys.exit(1)

    if column_name:
        if column_name not in df.columns:
            print(f"Column name {column_name!r} not found", file=sys.stderr)
            sys.exit(1)
        return df[column_name]

    if column_index is not None:
        if not (0 <= column_index < df.shape[1]):
            print("column-index is the out of range", file=sys.stderr)
            sys.exit(1)
        return df.iloc[:, column_index]

    print("The first column is used as no column name is specified", file=sys.stderr)
    return df.iloc[:, 0]


def main():
    args = parse_args()

    # Mutual exclusive check
    if args.density and args.percentage:
        print("cannot specify both --density and --percentage", file=sys.stderr)
        sys.exit(1)

    try:
        df = pd.read_csv(args.csv_path)
    except Exception as e:
        print(f"CSV Errors in loading CSV files: {e}", file=sys.stderr)
        sys.exit(1)

    data = choose_column(df, args.column_name, args.column_index)
    data = pd.to_numeric(data, errors="coerce").dropna()

    hist_range = tuple(args.range) if args.range else None

    # ---- Setting weights ----
    weights = None
    ylabel = args.ylabel

    if args.percentage:
        weights = np.ones(len(data)) * (100.0 / len(data))
        ylabel = ylabel or "Percentage (%)"
    elif args.density:
        ylabel = ylabel or "Probability density"
    else:
        ylabel = ylabel or "Frequency"

    fig, ax = plt.subplots()

    ax.hist(
        data,
        bins=args.bins,
        range=hist_range,
        density=args.density,
        weights=weights,
        edgecolor="black"
    )

    ax.set_xlabel(args.xlabel or "Value")
    ax.set_ylabel(ylabel)
    ax.set_title(args.title or "Histogram")

    fig.tight_layout()

    if args.output:
        fig.savefig(args.output, dpi=args.dpi)
        print(f"Saved in:  {args.output} (dpi={args.dpi})")
    else:
        plt.show()


if __name__ == "__main__":
    main()
