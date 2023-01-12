import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import time
import secret
df = pd.read_excel('Occupancy.xlsx')

df[['Date','Time']] = df.date.str.split(expand=True)
df.drop(['date'], axis=1, inplace=True)

dolu = df[df['Occupancy'] == 1]
bos = df[df['Occupancy'] == 0]

print(df.head())

'''
plt.scatter(dolu.Date,dolu.Time,color='red', label='Dolu', alpha=0.3)
plt.scatter(bos.Date,bos.Time, color='green', label='Bos', alpha=0.3)
plt.legend()
plt.show()
'''

x_data = df.drop('Occupancy', axis=1)
y = df.Occupancy.values

x_data["Date"] = pd.to_datetime(x_data["Date"])
x_data["Date"] = [dt.timestamp() for dt in x_data["Date"]]

x_data["Time"] = pd.to_datetime(x_data["Time"])
x_data["Time"] = [dt.timestamp() for dt in x_data["Time"]]
print(x_data.head())
x_data = x_data.drop(['Date'], axis=1)
x_data = x_data.drop(['Time'], axis=1)
x_data = x_data.drop(['Light'], axis=1)
x_data = x_data.drop(['CO2'], axis=1)
x_data = x_data.drop(['HumidityRatio'], axis=1)
print(x_data)

#normalization

x = (x_data - np.min(x_data))/(np.max(x_data)-np.min(x_data))




#train test split
from sklearn.model_selection import train_test_split
x_train, x_test, y_train, y_test = train_test_split(x,y,test_size=0.99,random_state=1)

from sklearn.tree import DecisionTreeClassifier
dt = DecisionTreeClassifier()
dt.fit(x_train,y_train)

#predict
y_pred = dt.predict(x_test)

#score
print("score: ",dt.score(x_test,y_test))
print(x_test)
print(y_test)

#confusion matrix
from sklearn.metrics import confusion_matrix
cm = confusion_matrix(y_test,y_pred)
print(cm)

from sklearn.metrics import classification_report
print(classification_report(y_test,y_pred))



from firebase import firebase
DATABASE_URL = secret.DATABASE_URL
fb_app = firebase.FirebaseApplication(DATABASE_URL, None)


while True:
    firebase_humadity = fb_app.get('/Humadity', None)
    firebase_temperature = fb_app.get('/Temperature', None)
    
    sensorDf = [[firebase_temperature,firebase_humadity]]
    sensorDf = pd.DataFrame (sensorDf, columns = ['Temperature','Humidity'])   
    print(type(sensorDf))
    #normalization
    sensorDf = (sensorDf - np.min(x_data))/(np.max(x_data)-np.min(x_data))
    print(sensorDf)
    if firebase_humadity != None and firebase_temperature != None:
        sonuc = dt.predict(sensorDf)
        print("SONUC:" + str(sonuc))
        result = fb_app.put("/","Occupancy",int(sonuc[0]))
    else:
        print("Firebase Error")
        time.sleep(1)


