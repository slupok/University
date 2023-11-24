import os
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
from tensorflow.keras.preprocessing.image import ImageDataGenerator
import tensorflow_datasets as tfds

# данная функция отрисует изображения в сетке размером 1х5
def plotImages(images_arr):
  fig, axes = plt.subplots(1, 10, figsize=(20, 20))
  axes = axes.flatten()
  for img, ax in zip(images_arr, axes):
    ax.imshow(img)
  plt.tight_layout()
  plt.show()

#horse_train, horse_train_info = tfds.load('cycle_gan/horse2zebra', split='trainA', shuffle_files=True, with_info=True)
#zebra_train, zebra_train_info = tfds.load('cycle_gan/horse2zebra', split='trainB', shuffle_files=True, with_info=True)
#horse_test, horse_test_info = tfds.load('cycle_gan/horse2zebra', split='testA', shuffle_files=True, with_info=True)
#zebra_test, zebra_test_info = tfds.load('cycle_gan/horse2zebra', split='testB', shuffle_files=True, with_info=True)

print(tf.config.list_physical_devices('GPU'))


horse_train_images, horse_train_labels = tfds.as_numpy(tfds.load('cycle_gan/horse2zebra', split='trainA', shuffle_files=True, batch_size=-1, as_supervised=True))
zebra_train_images, zebra_train_labels = tfds.as_numpy(tfds.load('cycle_gan/horse2zebra', split='trainB', shuffle_files=True, batch_size=-1, as_supervised=True))
horse_test_images, horse_test_labels = tfds.as_numpy(tfds.load('cycle_gan/horse2zebra', split='testA', shuffle_files=True, batch_size=-1, as_supervised=True))
zebra_test_images, zebra_test_labels = tfds.as_numpy(tfds.load('cycle_gan/horse2zebra', split='testB', shuffle_files=True, batch_size=-1, as_supervised=True))

train_images = np.concatenate((zebra_train_images, horse_train_images), axis=0)
test_images = np.concatenate((zebra_test_images, horse_test_images), axis=0)
train_labels = np.concatenate((horse_train_labels, zebra_train_labels), axis=0)
test_labels = np.concatenate((horse_test_labels, zebra_test_labels), axis=0)

print('Лошадей в обучающем наборе данных: ', len(horse_train_images))
print('Зебр в обучающем наборе данных: ', len(zebra_train_images))

print('Лошадей в тестовом наборе данных: ', len(horse_test_images))
print('Зебр в тестовом наборе данных: ', len(zebra_test_images))

print('Всего в обучающем наборе данных: ', len(train_images))
print('Всего в тестовом наборе данных: ', len(test_images))
print('--')

#fig = tfds.show_examples(zebra_test.take(1), zebra_test_info)

BATCH_SIZE = 100 # количество тренировочных изображений для обработки перед обновлением параметров модели
img_shape = np.shape(zebra_test_images)[1:3]
channelsCount = np.shape(zebra_test_images)[3]
train_image_generator = ImageDataGenerator(rescale=1./255)
test_image_generator = ImageDataGenerator(rescale=1./255)

train_data_gen = train_image_generator.flow(x = train_images,
                                            y = train_labels,
                                            batch_size=BATCH_SIZE,
                                            shuffle=True)

test_data_gen = test_image_generator.flow(x = test_images,
                                          y = test_labels,
                                          batch_size=BATCH_SIZE,
                                          shuffle=True)
#print(train_data_gen[1][0])
sample_training_images, labels = next(train_data_gen)
print(labels)
#plotImages(test_data_gen[:10])

model = tf.keras.models.Sequential([
    tf.keras.layers.Conv2D(32, (3,3), activation='relu', input_shape=(img_shape[0], img_shape[1], channelsCount)),
    tf.keras.layers.MaxPooling2D(2, 2),

    tf.keras.layers.Conv2D(64, (3, 3), activation='relu'),
    tf.keras.layers.MaxPooling2D(2, 2),

    tf.keras.layers.Conv2D(128, (3, 3), activation='relu'),
    tf.keras.layers.MaxPooling2D(2, 2),

    tf.keras.layers.Conv2D(128, (3, 3), activation='relu'),
    tf.keras.layers.MaxPooling2D(2, 2),

    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(512, activation='relu'),

    tf.keras.layers.Dense(1, activation='sigmoid')
])

model.compile(optimizer='adam',
              loss='binary_crossentropy',
              metrics=['accuracy'])

model.summary()

EPOCHS = 30
history = model.fit(
    train_data_gen,
    steps_per_epoch = int(np.ceil(len(train_images) / float(BATCH_SIZE))),
    epochs = EPOCHS,
    validation_data = test_data_gen,
    validation_steps = int(np.ceil(len(test_images) / float(BATCH_SIZE)))
)

#Теперь мы визуализируем результаты тренировки нашей модели:
acc = history.history['accuracy']
val_acc = history.history['val_accuracy']

loss = history.history['loss']
val_loss = history.history['val_loss']

epochs_range = range(EPOCHS)

plt.figure(figsize=(8,4))
plt.subplot(1, 2, 1)
plt.plot(epochs_range, acc, label='Точность на обучении')
plt.plot(epochs_range, val_acc, label='Точность на валидации')
plt.legend(loc='lower right')
plt.title('Точность на обучающих и валидационных данных')

plt.subplot(1, 2, 2)
plt.plot(epochs_range, loss, label='Потери на обучении')
plt.plot(epochs_range, val_loss, label='Потери на валидации')
plt.legend(loc='upper right')
plt.title('Потери на обучающих и валидационных данных')
plt.savefig('./foo.png')
plt.show()

