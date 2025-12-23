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
        description="CSV ファイルからヒストグラムを作成するスクリプト"
    )

    parser.add_argument("csv_path", help="入力 CSV ファイル")

    parser.add_argument("--column-name", help="列名で指定")
    parser.add_argument("--column-index", type=int, help="列番号 (0 始まり)")

    parser.add_argument("--bins", type=int, default=30, help="ビンの個数")
    parser.add_argument(
        "--range", nargs=2, type=float, metavar=("MIN", "MAX"),
        help="値の範囲"
    )

    parser.add_argument("--density", action="store_true",
                        help="確率密度（面積=1）で表示")
    parser.add_argument("--percentage", action="store_true",
                        help="パーセント表示（合計=100）")

    parser.add_argument("--xlabel", default=None)
    parser.add_argument("--ylabel", default=None)
    parser.add_argument("--title", default=None)

    parser.add_argument("--output", default=None)
    parser.add_argument("--dpi", type=int, default=300,
                        help="出力画像の解像度 (dpi)")

    return parser.parse_args()


def choose_column(df, column_name, column_index):
    if column_name and column_index is not None:
        print("column-name と column-index は同時に指定できません", file=sys.stderr)
        sys.exit(1)

    if column_name:
        if column_name not in df.columns:
            print(f"列名 {column_name!r} が見つかりません", file=sys.stderr)
            sys.exit(1)
        return df[column_name]

    if column_index is not None:
        if not (0 <= column_index < df.shape[1]):
            print("column-index が範囲外です", file=sys.stderr)
            sys.exit(1)
        return df.iloc[:, column_index]

    print("列指定がないため先頭列を使用します", file=sys.stderr)
    return df.iloc[:, 0]


def main():
    args = parse_args()

    # 排他チェック
    if args.density and args.percentage:
        print("--density と --percentage は同時に指定できません", file=sys.stderr)
        sys.exit(1)

    try:
        df = pd.read_csv(args.csv_path)
    except Exception as e:
        print(f"CSV 読み込みエラー: {e}", file=sys.stderr)
        sys.exit(1)

    data = choose_column(df, args.column_name, args.column_index)
    data = pd.to_numeric(data, errors="coerce").dropna()

    hist_range = tuple(args.range) if args.range else None

    # ---- weights 設定 ----
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
        print(f"保存しました: {args.output} (dpi={args.dpi})")
    else:
        plt.show()


if __name__ == "__main__":
    main()
