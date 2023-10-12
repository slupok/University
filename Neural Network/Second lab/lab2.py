import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
# импортируем import MLPClassifier
from sklearn.neural_network import MLPClassifier

#https://www.kaggle.com/datasets/fedesoriano/stroke-prediction-dataset

# звгружаем набор данных
os.chdir('c:/University/Neural Network/Second lab')
cancer = X = pd.read_csv("healthcare-dataset-stroke-data.csv")

#кодируем бинарные и непорядковые категориальные признаки методом факторизации
cancer['gender'] = pd.factorize(cancer['gender'])[0]
cancer['ever_married'] = pd.factorize(cancer['ever_married'])[0]
cancer['work_type'] = pd.factorize(cancer['work_type'])[0]
cancer['Residence_type'] = pd.factorize(cancer['Residence_type'])[0]
cancer['avg_glucose_level'] = pd.factorize(cancer['avg_glucose_level'])[0]
cancer['smoking_status'] = pd.factorize(cancer['smoking_status'])[0]
print(cancer.head())

#bmi have NaN 
cancer = cancer.drop(['bmi'], axis=1)

#указываем входы и выход сети
y = cancer['stroke']
X = cancer.drop(['stroke'], axis=1)

#импортируем библиотеку для формирования тестовой выборки
from sklearn.model_selection import train_test_split
#делим данные на тестовую и обучающую выборку
X_train, X_test, y_train, y_test = train_test_split(X, y,test_size=0.2, random_state=21)

#импортируем библиотеку tensorflow.keras
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense

# Создаем последовательную модель
model = Sequential()
model.add(Dense(25, input_dim=10, activation="linear"))
model.add(Dense(25, activation="relu"))
model.add(Dense(20, activation="linear"))
model.add(Dense(20, activation="relu"))
model.add(Dense(1, activation="linear"))
model.add(Dense(1, activation="sigmoid"))

# ЗАДАЕМ ОБУЧАЮЩИЕ ПАРАМЕТРЫ
model.compile(loss="binary_crossentropy", optimizer="adam", metrics=["accuracy"])

#print(model.summary())

#обучаем сеть на обучающей выборке и сразу тестируем на тестовой
history = model.fit(X_train, y_train, epochs=15, validation_data=(X_test, y_test))

#выведем итоговую точность на тестовой  выборке
print("точность на тестовой выборке: ")
print(model.evaluate(X_test, y_test)[1])

#делаем предсказания для всей тестовой выборки
print("Предсказание: ")
print(model.predict(X_test).round().T[0])