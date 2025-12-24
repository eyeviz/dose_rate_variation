https://github.com/eyeviz/dose_rate_variation

This repository contains program packages for the work entitled

Ambient Dose Rate Variation in the Fukushima Region
Visualized Using Explainable AI Techniques

authored by 

Ryu Yoshida, Hiroshi Kurikami, Fumiya Nagao,
Shigeo Takahashi, and Yukihisa Sanada.

This repository contains two directories as follows:

mapview/

This directory contains C++ program codes for visualizing
the ambient air dose rate and its reduction ratio on the map
in Fukushima Prefecture. The program loads the integrated
ambient dose rate map obtained through minotoring from 2011
to 2022, and compute the relevant dose rate reduction ratios
in terms of several important factors such as elevation,
distance from Fukushima Daiichi Nuclear Power Plant (FDNPP),
land use types, and terrain surface curvatures.

See mapview/README.md for more details

![System snapshot for analyzing Fukushima City and Date City](images/system_view_for_date.png)

??????/

See ??????/README.md for more details


SHAP/

This directory contains Python program codes for generating
SHAP summmary plots of the dose rate reduction ratio in
specific sample areas (Figures 4(b), 6(a), 6(c), and 8 in
the manuscript).

See SHAP/README.md for more details

![SHAP summary plot of the dose rate reduction ratio in terms of land use tapes in in Fukushima City and Date City](images/date_2011-2017_landuse_shap.svg)


histogram/

This directory contains Python program codes for generating
histograms of the dose rate reduction ratio in specific
sample areas (Figures 5(b) and 7(c) in the manuscript).

See histogram/README.md for more details

![Histogram of the dose rate reduction ratios in Fukushima City and Date City](images/date_2011-2017_landuse_histogram.png)
