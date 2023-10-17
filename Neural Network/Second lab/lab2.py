import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os

from sklearn.model_selection import train_test_split
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense

#https://www.kaggle.com/datasets/fedesoriano/stroke-prediction-dataset
os.chdir('c:/University/Neural Network/Second lab')
data = X = pd.read_csv("healthcare-dataset-stroke-data.csv")

#Кодируем нечисловые данные
data['gender'] = pd.factorize(data['gender'])[0]
data['ever_married'] = pd.factorize(data['ever_married'])[0]
data['work_type'] = pd.factorize(data['work_type'])[0]
data['Residence_type'] = pd.factorize(data['Residence_type'])[0]
data['avg_glucose_level'] = pd.factorize(data['avg_glucose_level'])[0]
data['smoking_status'] = pd.factorize(data['smoking_status'])[0]
print(data.head())


data = data.drop(['bmi'], axis=1)

y = data['stroke']
X = data.drop(['stroke'], axis=1)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=21)

model = Sequential()
model.add(Dense(25, input_dim=10, activation="linear"))
model.add(Dense(25, activation="relu"))
model.add(Dense(20, activation="linear"))
model.add(Dense(20, activation="relu"))
model.add(Dense(1, activation="linear"))
model.add(Dense(1, activation="sigmoid"))

model.compile(loss="binary_crossentropy", optimizer="adam", metrics=["accuracy"])
history = model.fit(X_train, y_train, epochs=15, validation_data=(X_test, y_test))

print(f"Точность: {model.evaluate(X_test, y_test)[1]}")
print(f"Предсказание: {model.predict(X_test).round().T[0]}")