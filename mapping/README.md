# SHAP Mapping 

This directory contains a Python program that visualizes SHAP values
on a geographic map using a LightGBM regression model and Folium.

The program computes SHAP values for a trained LightGBM model and maps
the spatial distribution of SHAP values for a selected terrain-related
feature (e.g., ravine or pit) on an interactive map.

---

## 1. Requirements

- OS: macOS / Linux / Windows
- Python: **3.10.9 or later**
- Required Python packages:
  - pandas
  - shap
  - lightgbm
  - scikit-learn
  - folium

### Installation

```bash
pip install pandas shap lightgbm scikit-learn folium
```

## 2. Input data

- Input format: CSV
- Input file name: `nogami_200_2011-2022_onehot_geo.csv`
- The CSV file must include the following columns:
  - `Latitude`
  - `Longitude`
  - Target variable column
  - Terrain-related feature columns (e.g., `ravine`, `pit`)
- All explanatory variables must be numeric
- Please download the data files from the ‘Data File’ section into the same directory as ‘mapping.py’.


### Target variable

- `crtR(2011:2022)`

## 3. Feature selection for mapping

- The user can select **one terrain-related feature** to visualize on the map.
- Available options:
  - `ravine`
  - `pit`
- Only the selected feature is used to extract and map the corresponding
  SHAP values.

---

## 4. How to use

### 4.1 Program settings

- Open the Python script.
- Modify **only** the following user settings.

#### Feature selection

```python
# === User settings ========================
selected_feature = 'ravine'  # Choose either 'ravine' or 'pit'
# ==========================================
The current program is set to (ravine).
Please modify it as needed.

# === User settings ========================
output_path = 'Please enter the destination path.'
# ==========================================
```

### 4.2 Execution Method

- In the terminal, execute the following command:

```bash
python mapping.py
```
## 5. Processing overview

- Load the input CSV file
- Train a LightGBM regression model using a subset of the data
- Compute SHAP values for the trained model
- Extract SHAP values corresponding to the selected feature
- Filter locations where the selected feature is present
- Visualize SHAP values on an interactive Folium map
- Add topographic contour tiles (OpenTopoMap)
- Save the map as an HTML file

---

## 6. Output

- An interactive map is generated and saved as an HTML file.
- The map includes:
  - Colored circle markers representing SHAP values
  - Marker size proportional to the absolute SHAP value
  - Red markers: negative SHAP values
  - Yellow markers: positive SHAP values
- The output file is saved to the path specified by `output_path`.

---

## 7. Notes

- This program is intended for analysis and visualization purposes.
- SHAP values are computed using a sampled subset of the data to reduce
  computational cost.
- The interpretation of SHAP values depends on the trained model and
  selected feature.

- Ravine sample points on the map

![Ravine sample points in the national forest in the Nogami area (ravine.png)](ravine.html)

- Pit sample points on the map

![Pit sample points in the national forest in the Nogami area (pit.png)](pit.html)

```python

```
