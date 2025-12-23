This program package is prepared to produce histogram images
from the data samples using Python.

Histogram images can be generated using the following commands:

> python3 hist_from_csv.py date_2011-2017_landuse_mapping.csv --column-index 0 --bins 16 --range -0.6 1.0 --percentage --xlabel "R value (from 2011 to 2017)" --ylabel "Probability (in percent)" --title "R value histogram (Fukushima&Date)" --output date_2011-2017_landuse_mapping.png --dpi 300

![Histogram of the dose rate reduction ratio in the sample area of Fukushima City and Date City](date_2011-2017_landuse_mapping.png)

> python3 hist_from_csv.py nogami_200_continuous_mapping.csv --column-index 0 --bins 16 --range -0.6 1.0 --percentage --xlabel "R value (from 2011 to 2022)" --ylabel "Probability (in percent)" --title "R value histogram (Nogami)" --output nogami_2011-2022_continuous_mapping.png --dpi 300

![Histogram of the dose rate reduction ratio in the national forest around the National Route 288 in Nogami area](nogami_2011-2022_continuous_mapping.png)
