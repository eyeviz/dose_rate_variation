#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import numpy as np
import pandas as pd
from sklearn.model_selection import GroupKFold
from sklearn.metrics import mean_squared_error, mean_absolute_error, r2_score
import lightgbm as lgb
import folium


file_path = 'date_2011_2017_土地利用_mapping.csv'
target_variable = 'crtR(2011:2017)'

RANDOM_STATE = 42
N_SPLITS = 5  #K-Fold


LON_COL = 'Longitude'
LAT_COL = 'Latitude'


GRID_SIZE = 0.01

df = pd.read_csv(file_path)


df['block_x'] = (df[LON_COL] / GRID_SIZE).astype(int)
df['block_y'] = (df[LAT_COL] / GRID_SIZE).astype(int)
df['spatial_block'] = df['block_x'].astype(str) + '_' + df['block_y'].astype(str)

print("ユニーク spatial_block 数:", df['spatial_block'].nunique())


groups = df['spatial_block'].values

id_columns = [LON_COL, LAT_COL, 'block_x', 'block_y', 'spatial_block']

y = df[target_variable]
X = df.drop(columns=[target_variable] + id_columns)

print("データ形状:", X.shape)
print("目的変数の概要:")
print(y.describe())

y_min = y.min()
y_max = y.max()
y_range = y_max - y_min
print(f"\n目的変数の変動域 (max - min): {y_range:.4f}")

categorical_cols = X.select_dtypes(include=['object', 'category']).columns.tolist()
print("カテゴリ列:", categorical_cols)

lgb_params = {
    'objective': 'regression',   
    'metric': 'rmse',            
    'boosting_type': 'gbdt',
    'learning_rate': 0.05,
    'num_leaves': 31,
    'max_depth': -1,
    'min_data_in_leaf': 20,
    'feature_fraction': 0.8,
    'bagging_fraction': 0.8,
    'bagging_freq': 1,
    'n_estimators': 1000,        
    'n_jobs': -1,
    'random_state': RANDOM_STATE
}

gkf = GroupKFold(n_splits=N_SPLITS)


oof_pred = np.zeros(len(X))


block_to_fold = {}


for fold, (train_idx, valid_idx) in enumerate(gkf.split(X, y, groups), 1):
    print(f"\n========== Spatial Fold {fold} / {N_SPLITS} ==========")

    X_train, X_valid = X.iloc[train_idx], X.iloc[valid_idx]
    y_train, y_valid = y.iloc[train_idx], y.iloc[valid_idx]

    valid_blocks = df.iloc[valid_idx]['spatial_block'].unique()
    for b in valid_blocks:
        block_to_fold[b] = fold

    model = lgb.LGBMRegressor(**lgb_params)

    model.fit(
        X_train,
        y_train,
        eval_set=[(X_valid, y_valid)],
        eval_metric='rmse',
        callbacks=[
            lgb.early_stopping(stopping_rounds=50, verbose=True)
        ],
        categorical_feature=categorical_cols
    )

    valid_pred = model.predict(X_valid, num_iteration=model.best_iteration_)
    oof_pred[valid_idx] = valid_pred

    fold_rmse = mean_squared_error(y_valid, valid_pred, squared=False)
    fold_mae = mean_absolute_error(y_valid, valid_pred)
    fold_r2  = r2_score(y_valid, valid_pred)

    print(f"Fold {fold} RMSE: {fold_rmse:.4f}")
    print(f"Fold {fold} MAE : {fold_mae:.4f}")
    print(f"Fold {fold} R2  : {fold_r2:.4f}")

cv_rmse = mean_squared_error(y, oof_pred, squared=False)
cv_mae  = mean_absolute_error(y, oof_pred)
cv_r2   = r2_score(y, oof_pred)


if y_range > 0:
    rel_mae  = cv_mae  / y_range * 100.0
    rel_rmse = cv_rmse / y_range * 100.0
else:
    rel_mae = np.nan
    rel_rmse = np.nan

print("\n========== Spatial CV 結果（OOF） ==========")
print(f"Spatial CV RMSE: {cv_rmse:.4f}")
print(f"Spatial CV MAE : {cv_mae:.4f}")
print(f"Spatial CV R2  : {cv_r2:.4f}")
print(f"相対誤差 MAE  : {rel_mae:.2f} %")
print(f"相対誤差 RMSE : {rel_rmse:.2f} %")




blocks = df[['block_x', 'block_y', 'spatial_block']].drop_duplicates().reset_index(drop=True)

blocks['fold'] = blocks['spatial_block'].map(block_to_fold)

print("\nFold 割当済みブロック数:", len(blocks))

center_lat = df[LAT_COL].mean()
center_lon = df[LON_COL].mean()
m = folium.Map(location=[center_lat, center_lon], zoom_start=10)


fold_colors = {
    1: "#e41a1c",  # red
    2: "#377eb8",  # blue
    3: "#4daf4a",  # green
    4: "#984ea3",  # purple
    5: "#ff7f00",  # orange
}


for _, row in blocks.iterrows():
    bx = row['block_x']
    by = row['block_y']
    fold = row['fold']


    if pd.isna(fold):
        continue

    lon_min = bx * GRID_SIZE
    lon_max = lon_min + GRID_SIZE
    lat_min = by * GRID_SIZE
    lat_max = lat_min + GRID_SIZE

    folium.Rectangle(
        bounds=[(lat_min, lon_min), (lat_max, lon_max)],
        color=fold_colors.get(int(fold), "#000000"),
        weight=1,
        fill=True,
        fill_opacity=0.3,
        popup=f"Block: {row['spatial_block']}, Fold: {int(fold)}"
    ).add_to(m)


legend_html = """
<div style="
    position: fixed;
    bottom: 50px;
    left: 50px;
    width: 180px;
    z-index:9999;
    font-size:14px;
    ">
    <b>Spatial CV Folds</b><br>
    <i style="background:#e41a1c;opacity:0.7;">&nbsp;&nbsp;&nbsp;&nbsp;</i> Fold 1<br>
    <i style="background:#377eb8;opacity:0.7;">&nbsp;&nbsp;&nbsp;&nbsp;</i> Fold 2<br>
    <i style="background:#4daf4a;opacity:0.7;">&nbsp;&nbsp;&nbsp;&nbsp;</i> Fold 3<br>
    <i style="background:#984ea3;opacity:0.7;">&nbsp;&nbsp;&nbsp;&nbsp;</i> Fold 4<br>
    <i style="background:#ff7f00;opacity:0.7;">&nbsp;&nbsp;&nbsp;&nbsp;</i> Fold 5<br>
</div>
"""
m.get_root().html.add_child(folium.Element(legend_html))

# Jupyter / Colab なら最後に m を評価すると地図が表示される
m
# 必要なら HTML として保存
# m.save("spatial_cv_folds_map.html")

