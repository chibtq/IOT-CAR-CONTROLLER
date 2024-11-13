from sklearn.model_selection import train_test_split
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.naive_bayes import MultinomialNB
from sklearn.metrics import accuracy_score, classification_report
import pandas
import joblib

df = pandas.read_csv('labels.csv')
# print(df)

# Chuẩn bị dữ liệu
text_data = df.label
labels = df.value

# Chia tập dữ liệu
# X_train, X_test, y_train, y_test = train_test_split(text_data, labels, test_size=0.1)

# Vector hóa văn bản
vectorizer = CountVectorizer()
X_train_vectorized = vectorizer.fit_transform(text_data)
# X_test_vectorized = vectorizer.transform(X_test)

# Xây dựng và huấn luyện mô hình
model = MultinomialNB()
model.fit(X_train_vectorized, labels)

# Dự đoán và đánh giá mô hình
# y_pred = model.predict(X_test_vectorized)
# accuracy = accuracy_score(y_test, y_pred)
# report = classification_report(y_test, y_pred)

joblib.dump(model,'action_classification.joblib')
# Lưu vectorizer
joblib.dump(vectorizer, 'text_vectorizer.joblib')



# print(y_pred)
# print(y_test)
# print(f'Accuracy: {accuracy}')
# print(f'Classification Report:\n{report}')
