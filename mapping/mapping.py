#!/usr/bin/env python
# coding: utf-8

# In[1]:


import pandas as pd
import shap
import folium
from lightgbm import LGBMRegressor
from sklearn.model_selection import train_test_split


file_path = 'nogami_200_2011-2022_onehot_with_geo.csv'
target_variable = 'crtR(2011:2022)'

# === User settings ========================
# ------------------------------------------
# Select 'ravine' to plot ravine sample points on the map
selected_feature = 'ravine'     #Specify terrain feature type
# ------------------------------------------
# Select 'pit' to plot pit sample points on the map
# selected_feature = 'pit'     #Specify terrain feature type
# ------------------------------------------
# ==========================================

data = pd.read_csv(file_path)
latitude = data['Latitude']
longitude = data['Longitude']


X = data.drop(columns=[target_variable, 'Latitude', 'Longitude'])
y = data[target_variable]


X_sample, _, y_sample, _ = train_test_split(X, y, test_size=0.4, random_state=42)


model = LGBMRegressor(n_estimators=100, max_depth=100, random_state=42, n_jobs=-1)
model.fit(X_sample, y_sample)


explainer = shap.Explainer(model, X_sample)
shap_values = explainer(X_sample).values
feature_index = list(X_sample.columns).index(selected_feature)
shap_selected = shap_values[:, feature_index]


mask = X_sample[selected_feature] == 1
lat_masked = latitude.iloc[X_sample.index][mask]
lon_masked = longitude.iloc[X_sample.index][mask]
shap_masked = shap_selected[mask]


m = folium.Map(location=[latitude.mean(), longitude.mean()], zoom_start=10)

#Add contour lines
folium.TileLayer(
    tiles='https://{s}.tile.opentopomap.org/{z}/{x}/{y}.png',
    attr='&copy; <a href="https://opentopomap.org">OpenTopoMap</a>',
    name='OpenTopoMap',
    overlay=True,
    control=True
).add_to(m)

#mapping
for lat, lon, shap_val in zip(lat_masked, lon_masked, shap_masked):
    if shap_val <= -0.00:
        color = 'red'
    else:
        color = 'yellow'
    
    folium.CircleMarker(
        location=[lat, lon],
        radius=1 + abs(shap_val) * 1,
        color=color,
        fill=True,
        fill_color=color,
        fill_opacity=0.6
    ).add_to(m)


folium.LayerControl().add_to(m)

# === User settings ========================
# Temporary set the label of the selected feature as the output file name (in HTML).
output_path = selected_feature + ".html"
# ==========================================

m.save(output_path)
print(f"Map saved: {output_path}")


# In[ ]:




