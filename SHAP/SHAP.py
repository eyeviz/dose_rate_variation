#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import shap
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.model_selection import train_test_split
from lightgbm import LGBMRegressor


file_path = 'filename.csv'
target_variable = 'crtR(2011:2022)'

data = pd.read_csv(file_path)


X = data.drop(target_variable, axis=1)
y = data[target_variable]


X_sample, _, y_sample, _ = train_test_split(X, y, test_size=0.2, random_state=42)


model = LGBMRegressor(n_estimators=100, max_depth=100, random_state=42, n_jobs=-1)
model.fit(X_sample, y_sample)

explainer = shap.Explainer(model) 
shap_values = explainer(X_sample)


plt.figure(figsize=(20, 20))
shap.summary_plot(shap_values, X_sample, show=False)


plt.title('SHAP Summary Plot (LightGBM)', fontsize=12)
plt.xlabel('SHAP Value (impact on model output)', fontsize=12)
plt.ylabel('Features', fontsize=12)
plt.xticks(fontsize=12)
plt.yticks(fontsize=18)
plt.tight_layout()
plt.savefig(
    "filename.svg",
    format="svg",
    bbox_inches="tight"
)
plt.show()

